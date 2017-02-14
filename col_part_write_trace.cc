/*
 * column_write_trace.cc
 */

#include "gen_trace.h"
#include <stdio.h>
#include <stddef.h>
#include "addr_util.h"

// Generates column write trace in column addressing, begin with a base address.
// The access pattern is writing two columns from base address sequentially.
// In the first version, I'd like to access the f3 and f10 column shown below.
//
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 1:
// Fill as many tuples as possible in a row
//
//+=========+=========+=========+=========+
//| Tuple 1 | Tuple 2 | Tuple 3 | Tuple 4 |
//+---------+---------+---------+---------+
//| Tuple 5 | Tuple 6 | Tuple 7 |         |
//+---------+---------+---------+---------+
//|         |         |         |         |
//+---------+---------+---------+---------+
//|         |         |         | Tuple N |
//+---------+---------+---------+---------+
// Assume Tuple 1 is in Column 0
int GenColTrace_3(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenColTrace_3 is running, Sequential Column Read.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *read_op = "w";
  const char *op = read_op;

  // Count how many cache tuples in 1 2D-NVM row
  int tuple_in_one_row = (kRowBufferSize * kCellSize) / tuple_size;
  int temp = num_tuples
      / (tuple_in_one_row * kCachelineSize / kCellSize);
  int remain = num_tuples % (tuple_in_one_row * kCachelineSize / kCellSize);
  int num_traces = temp * tuple_in_one_row * 2;
  if (remain < tuple_in_one_row)
    num_traces += remain * 2;
  else
    num_traces += tuple_in_one_row * 2;

  // Column access by cache lines
  // read trace_total cache lines
  uint32_t base_curr_addr_cacheline = GetCacheLineAddr(base_col_addr);
  printf("Base column address: 0x%08x\n", base_curr_addr_cacheline);

  uint32_t curr_addr = base_curr_addr_cacheline;
  Address addr = {};
  ParseColAddr(curr_addr, addr);
  // Calculate the column address of the f3 and f10 elements in the first row
  Address f3_addr = addr;
  f3_addr.column = addr.column + offsetof(IMDBRow, f3) / kCellSize;
  Address f10_addr = addr;
  f10_addr.column = addr.column + offsetof(IMDBRow, f10) / kCellSize;

  for (int i = 0; i < num_traces; i += 2 * tuple_in_one_row) {
    f3_addr = addr;
    f10_addr = addr;
    for (int j = 0; j < tuple_in_one_row && i + 2 * j  < num_traces; j++) {
      // Calculate the column address of the f3 and f10 elements
      f3_addr.column = addr.column + offsetof(IMDBRow, f3) / kCellSize
          + j * sizeof(IMDBRow) / kCellSize;
      f10_addr.column = addr.column + offsetof(IMDBRow, f10) / kCellSize
          + j * sizeof(IMDBRow) / kCellSize;
#ifdef DEBUG
      dprintf(fp, "%d %s 0x%08x 0 1, [%d %d %d %d %d %d]\n", i + 2 * j, op,
              GenColAddr(f3_addr), f3_addr.highrow, f3_addr.bank,
              f3_addr.channel, f3_addr.lowrow, f3_addr.column,
              f3_addr.intrabus);
      dprintf(fp, "%d %s 0x%08x 0 1, [%d %d %d %d %d %d]\n", i + 2 * j + 1, op,
              GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
              f10_addr.channel, f10_addr.lowrow, f10_addr.column,
              f10_addr.intrabus);
#else
      cprintf(fp, "%d %s 0x%08x 0 1\n", i + 2 * j, op, GenColAddr(f3_addr));
      cprintf(fp, "%d %s 0x%08x 0 1\n", i + 2 * j + 1, op,
              GenColAddr(f10_addr));
#endif
    }
    // Increment rows by a cache line
    curr_addr += kCachelineSize;
    ParseColAddr(curr_addr, addr);
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Layout 2
// Wrap in a bank
//
//+=========+=========+==+=========+
//|  Row Buffer                    |
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
int GenColTrace_4(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenColTrace_4 is running\n");
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *read_op = "w";
  const char *op = read_op;

  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f3_addr = {}, f10_addr = {};

  for(int i = 0; i < num_tuples; i += kCachelineSize / kCellSize) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f3_addr = addr;
    f10_addr = addr;
    f3_addr.column += offsetof(IMDBRow, f3) / kCellSize;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 1, [%d %d %d %d %d %d]\n", num_traces++, op,
            GenColAddr(f3_addr), f3_addr.highrow, f3_addr.bank, f3_addr.channel,
            f3_addr.lowrow, f3_addr.column, f3_addr.intrabus);
    dprintf(fp, "%d %s 0x%08x 0 1, [%d %d %d %d %d %d]\n", num_traces++, op,
            GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
            f10_addr.channel, f10_addr.lowrow, f10_addr.column,
            f10_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 1\n", num_traces++, op, GenColAddr(f3_addr));
    cprintf(fp, "%d %s 0x%08x 0 1\n", num_traces++, op, GenColAddr(f10_addr));
#endif
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

