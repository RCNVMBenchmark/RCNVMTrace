/*
 * row_write_trace.cc
 */

#include "gen_trace.h"
#include <stdio.h>
#include "addr_util.h"

// Generates row scan write trace in row addressing, begin with a base address
// The access pattern is read a row from base address sequentially.
//
// Layout 1, Row-first:
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
int GenRowTrace_7(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_row_addr) {
  printf("GenRowTrace_7 is running\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *write_op = "W";
  const char *op = write_op;

  // Row access by cache line
  // write num_traces cache lines
  // Assume that kRowBufferSize is divisible by tuple_size, and tuple_size is
  // larger than kCachelineSize
  int num_traces = tuple_size * num_tuples / kCachelineSize;

  uint32_t curr_addr = GetCacheLineAddr(base_row_addr);
  printf("Base row address: 0x%08x\n", curr_addr);

  for (int i = 0; i < num_traces; i++) {
    // trace format needed by NVMain
    // Op_Width: 0 for row access; 1, 2, 3 ... for column access
    // Index Op Addr ThreadID Op_Width
#ifdef DEBUG
    Address addr;
    ParseRowAddr(curr_addr, addr);
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", i, op, curr_addr,
            addr.highrow, addr.bank, addr.channel, addr.lowrow, addr.column,
            addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", i, op, curr_addr);
#endif
    curr_addr += kCachelineSize; // increment by one cache line
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Layout 2, column-first:
// Wrap in a bank
//
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
int GenRowTrace_8(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_row_addr) {
  printf("GenRowTrace_8 is running\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *write_op = "W";
  const char *op = write_op;

  int num_traces = 0;

  // Count how many cache lines in one tuple
  int cacheline_in_one_tuple = (tuple_size - 1) / kCachelineSize + 1;

  base_row_addr = GetCacheLineAddr(base_row_addr);
  uint32_t curr_row_addr = base_row_addr;
  printf("Base row address: 0x%08x\n", base_row_addr);

  uint32_t base_col_addr = RowAddr2ColAddr(base_row_addr);
  uint32_t curr_col_addr = base_col_addr;
  
  for(int i = 0; i < num_tuples; ++i) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    curr_row_addr = ColAddr2RowAddr(curr_col_addr);
    for (int j = 0; j < cacheline_in_one_tuple; j++) {
#ifdef DEBUG
      Address addr;
      ParseRowAddr(curr_row_addr + j * kCachelineSize, addr);
      dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op,
              curr_row_addr + j * kCachelineSize, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
      cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op,
              curr_row_addr + j * kCachelineSize);
#endif
      ++num_traces;
    }
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}
