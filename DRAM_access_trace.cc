/*
 * row_access_trace.cc
 */

#include "gen_trace.h"
#include <stdio.h>
#include <stddef.h>
#include "addr_util.h"

// Generates DRAM query trace in row addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f3,f4 from table-a where f10 > x 
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_1(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenDRAMTrace_1 is running, Select f3,f4 from table-a where f10 > x .\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f3_addr = {};

  // row read f10 first
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f3_addr = addr;
    f10_addr = addr;
    f3_addr.column += offsetof(IMDBRow, f3) / kCellSize;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f10_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    // if f10 match row read f3, f4
    if(i % 10 < THRESHOLD)
    {
      continue;
    }
    curr_row_addr = ColAddr2RowAddr(GenColAddr(f3_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in row addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select * from table-a where f10 > x 
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_2(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenDRAMTrace_2 is running, Select * from table-a where f10 > x .\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f3_addr = {};

  // row read f10 first
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f3_addr = addr;
    f10_addr = addr;
    f3_addr.column += offsetof(IMDBRow, f3) / kCellSize;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f10_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    // if f10 match row read f1, f2, f3, f4
    if(i % 10 < THRESHOLD)
    {
      continue;
    }
    curr_row_addr = ColAddr2RowAddr(GenColAddr(f3_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in row addressing, begin with a base address.
// The acces pattern of the following SQL query:
// update table-a set f3 = xx, f4 = xx where f10 = xx
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_3(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenDRAMTrace_3 is running, update table-a set f3 = xx, f4 = xx where f10 = xx.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";
  const char *op_W = "W";
  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f3_addr = {};

  // row read f10 first
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f3_addr = addr;
    f10_addr = addr;
    f3_addr.column += offsetof(IMDBRow, f3) / kCellSize;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f10_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    // if f10 match row read f1, f2, f3, f4
    if(i % 10 < THRESHOLD)
    {
      continue;
    }
    curr_row_addr = ColAddr2RowAddr(GenColAddr(f3_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_W,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_W,
              curr_row_addr);
#endif
    num_traces++;
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}



// Generates DRAM query trace in DRAM addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select * from table-a where f10 > x  | most of f10 is > x
// 
//
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_4(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {

  printf("GenDRAMTrace_4 is running, Select * from table-a where f10 > x | most of f10 is > x .\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f3_addr = {};

  // row read f10 first
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f3_addr = addr;
    f10_addr = addr;
    f3_addr.column += offsetof(IMDBRow, f3) / kCellSize;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f10_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    // if f10 match row read f1, f2, f3, f4
    if(i % 10 < THRESHOLD_LOW)
    {
      continue;
    }
    curr_row_addr = ColAddr2RowAddr(GenColAddr(f3_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in DRAM addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f9 from table-a where f10 > x  | most of f10 is > x
// 
//
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_5(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenDRAMTrace_5 is running, Select f9 from table-a where f10 > x | most f10 > x.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f3_addr = {};

  // row read f10 first
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f3_addr = addr;
    f10_addr = addr;
    f3_addr.column += offsetof(IMDBRow, f3) / kCellSize;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f10_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in DRAM addressing, begin with a base address.
// The acces pattern of the following SQL query:
// update table-a set f9 = xx where f10 = xx | few f10 = xx
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_6(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenDRAMTrace_6 is running, update table-a set f3 = xx, f4 = xx where f10 = xx. | few f10 = xx\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";
  const char *op_W = "W";
  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f3_addr = {};

  // row read f10 first
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f3_addr = addr;
    f10_addr = addr;
    f3_addr.column += offsetof(IMDBRow, f3) / kCellSize;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f10_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    // if f10 match row read f1, f2, f3, f4
    if(i % 10 < THRESHOLD)
    {
      continue;
    }
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_W,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_W,
              curr_row_addr);
#endif
    num_traces++;
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in row addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select table-a.f3, table-b.f4 from table-a, table-b where table-a.f1 > table-b.f1 and table-a.f9 = table-b.f9 
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_7(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr_a, uint32_t base_col_addr_b) {
  printf("GenDRAMTrace_7 is running, Select table-a.f3, table-b.f4 from table-a, table-b where table-a.f1 > table-b.f1 and table-a.f9 = table-b.f9.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address for table-a: 0x%08x\n", base_col_addr_a);
  printf("Base column address for table-b: 0x%08x\n", base_col_addr_b);

  // row read table-a.f1 and table-a.f9
  for(int i = 0; i < num_tuples; i++) {
    Address addr = {};
    Address f1_addr = {};
    Address f9_addr = {};
    uint32_t curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr_a);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of f1 and f9
    f1_addr = addr;
    f9_addr = addr;
    f1_addr.column += offsetof(IMDBRow, f1) / kCellSize;
    f9_addr.column += offsetof(IMDBRow, f9) / kCellSize;
    uint32_t curr_row_addr_f1 = ColAddr2RowAddr(GenColAddr(f1_addr));
    uint32_t curr_row_addr_f9 = ColAddr2RowAddr(GenColAddr(f9_addr));
    curr_row_addr_f1 = GetCacheLineAddr(curr_row_addr_f1);
    curr_row_addr_f9 = GetCacheLineAddr(curr_row_addr_f9);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr_f1, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_R,
              curr_row_addr_f1, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
        ParseRowAddr(curr_row_addr_f9, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_R,
              curr_row_addr_f9, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_R,
              curr_row_addr_f1);
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_R,
              curr_row_addr_f9);
#endif
  }
  // row read table-a.f1 and table-b.f9
  for(int i = 0; i < num_tuples; i++) {
    Address addr = {};
    Address f1_addr = {};
    Address f9_addr = {};
    uint32_t curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr_b);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of f1 and f9
    f1_addr = addr;
    f9_addr = addr;
    f1_addr.column += offsetof(IMDBRow, f1) / kCellSize;
    f9_addr.column += offsetof(IMDBRow, f9) / kCellSize;
    uint32_t curr_row_addr_f1 = ColAddr2RowAddr(GenColAddr(f1_addr));
    uint32_t curr_row_addr_f9 = ColAddr2RowAddr(GenColAddr(f9_addr));
    curr_row_addr_f1 = GetCacheLineAddr(curr_row_addr_f1);
    curr_row_addr_f9 = GetCacheLineAddr(curr_row_addr_f9);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr_f1, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_R,
              curr_row_addr_f1, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
        ParseRowAddr(curr_row_addr_f9, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_R,
              curr_row_addr_f9, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_R,
              curr_row_addr_f1);
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_R,
              curr_row_addr_f9);
#endif
  }

  // row read table-a.f3 & table-b.f4
  // They have been accessed with f1

  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in row addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select table-a.f3, table-b.f4 from table-a, table-b where table-a.f9 = table-b.f9 
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_8(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr_a, uint32_t base_col_addr_b) {
  printf("GenDRAMTrace_8 is running, Select table-a.f3, table-b.f4 from table-a, table-b where table-a.f9 = table-b.f9.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address for table-a: 0x%08x\n", base_col_addr_a);
  printf("Base column address for table-b: 0x%08x\n", base_col_addr_b);

  // row read table-a.f9
  for(int i = 0; i < num_tuples; i++) {
    Address addr = {};
    Address f9_addr = {};
    uint32_t curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr_a);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of f9
    f9_addr = addr;
    f9_addr.column += offsetof(IMDBRow, f9) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f9_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_R,
              curr_row_addr);
#endif
  }
  // row read table-b.f9
  for(int i = 0; i < num_tuples; i++) {
    Address addr = {};
    Address f9_addr = {};
    uint32_t curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr_b);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of f9
    f9_addr = addr;
    f9_addr.column += offsetof(IMDBRow, f9) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f9_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_R,
              curr_row_addr);
#endif
  }

  // row read table-a.f3 & table-b.f4
  for(int i = 0; i < num_tuples; i++) {
    // if table-a.f9 = table-b.f9 row read f3 & f4
    if(i % 10 < THRESHOLD)
    {
      continue;
    }
    // Calculate the column address of f3 & f4
    // f3 & f4 belong to the first cache line in each tuple
    Address f_addr = {};
    uint32_t curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr_a);
    ParseColAddr(curr_col_addr, f_addr);
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_R,
              curr_row_addr);
#endif
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr_b);
    ParseColAddr(curr_col_addr, f_addr);
    curr_row_addr = ColAddr2RowAddr(GenColAddr(f_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_R,
              curr_row_addr);
#endif
  }

  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in row addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f3,f4 from table-a where f1 > x and f9 < y
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_9(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenDRAMTrace_9 is running, Select f3,f4 from table-a where f1 > x and f9 < y.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f1_addr = {};
  Address f9_addr = {};

  // row read f1, f9 first
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f1 and f9 elements
    f1_addr = addr;
    f9_addr = addr;
    f1_addr.column += offsetof(IMDBRow, f1) / kCellSize;
    f9_addr.column += offsetof(IMDBRow, f9) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f1_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    curr_row_addr = ColAddr2RowAddr(GenColAddr(f9_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    // if f1 and f9 match then row read f3, f4
    // but f3 & f4 are in the same cache line with f1
    // so there is nothing to do here
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}



// Generates DRAM query trace in row addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f3,f4 from table-a where f1 > x and f2 < y
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_10(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenDRAMTrace_10 is running, Select f3,f4 from table-a where f1 > x and f2 < y.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f1_addr = {};

  // row read f1, f2 first
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of f1
    f1_addr = addr;
    f1_addr.column += offsetof(IMDBRow, f1) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f1_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    // if f1 and f2 match then row read f3, f4
    // but f3 & f4 are in the same cache line with f1 & f2
    // so there is nothing to do here
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates hybrid query trace in hybrid addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f3,f4 from table-a where f10 > x | use L3 cache to speed up
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_11(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int L3_size) {

  printf("GenDRAMTrace_11 is running, Select f3,f4 from table-a where f10 > x . | use L3 cache to speed up\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f3_addr = {};

  // row read f10 first
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f3_addr = addr;
    f10_addr = addr;
    f3_addr.column += offsetof(IMDBRow, f3) / kCellSize;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f10_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    // if f10 match row read f3, f4
    if(i % 10 < THRESHOLD)
    {
      continue;
    }
    curr_row_addr = ColAddr2RowAddr(GenColAddr(f3_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in DRAM addressing, begin with a base address.
// The acces pattern is generated from random_pattern, mixed with R, W, r, w
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_12(const char* trace_file_name, int num_query, int r_ratio, int w_ratio, int cr_ratio, const char* random_file, const char* description) {
  printf("%s%s%s", "GenDRAMTrace_12_", description, " is running, generating a random pattern.\n");
  // Generate random pattern
  FILE *data = fopen(random_file, "r");
  if (data == NULL){
    int ret = random_pattern(num_query, r_ratio, w_ratio, cr_ratio, random_file);
    if (ret != kSuccess){
      return ret;
    }
    data = fopen(random_file, "r");
  }

  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL){
    fclose(data);
    return kFailure;
  }

  int op, pos;
  int num_traces = 0;
  int stride =  kCachelineSize / kCellSize;

  while (fscanf(data, "%d %d", &op, &pos) > 0){
    Address addr = {};

    switch (op){
      case 0:
        pos = GetCacheLineAddr(pos);
	ParseRowAddr(pos, addr);
#ifdef DEBUG
      	dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, "R",
              pos, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
      	cprintf(fp, "%d %s 0x%08x 0 0\n", ++num_traces, "R", pos);
#endif
        break;
      case 1:
        pos = GetCacheLineAddr(pos);
	ParseRowAddr(pos, addr);
#ifdef DEBUG
      	dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, "W",
              pos, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
      	cprintf(fp, "%d %s 0x%08x 0 0\n", ++num_traces, "W", pos);
#endif
        break;
      case 2:
        for (int i = 0; i < stride; i ++){
    	  uint32_t temp_col = pos + i * kCellSize;
    	  uint32_t curr_row_addr = ColAddr2RowAddr(temp_col);
	  curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
      	  ParseRowAddr(curr_row_addr, addr);
      	  dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, "R",
                curr_row_addr, addr.highrow, addr.bank,
                addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
          cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, "R", curr_row_addr);
#endif
        }
        break;
      case 3:
        for (int i = 0; i < stride; i ++){
    	  uint32_t temp_col = pos + i * kCellSize;
    	  uint32_t curr_row_addr = ColAddr2RowAddr(temp_col);
	  curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
      	  ParseRowAddr(curr_row_addr, addr);
      	  dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, "W",
                curr_row_addr, addr.highrow, addr.bank,
                addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
          cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, "W", curr_row_addr);
#endif
        }
        break;
    } 
  }
  printf("Total number of traces: %d\n", num_traces);

  fclose(data);
  fclose(fp);
  return kSuccess;
}

// Generates hybrid query trace in hybrid addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f3 from table-a
// f3 here is a wide column which stretches over the border of a cache line
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_13(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int prefetch_size) {
  printf("GenDRAMTrace_13 is running, Select f3,f4 from table-a where f1 > x and f9 < y.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f3_addr = {};
  int column_num = (offsetof(IMDBOverCLRow, f4) - offsetof(IMDBOverCLRow, f3)) / kCellSize;

  // row read f3
  for(int i = 0; i < num_tuples; i++) {
    for (int j = 0; j < column_num; j += kCachelineSize){
      curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
      ParseColAddr(curr_col_addr, addr);
      // Calculate the column address of f3
      f3_addr = addr;
      f3_addr.column += offsetof(IMDBRow, f3) / kCellSize + j;
      uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f3_addr));
      curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
      ParseRowAddr(curr_row_addr, addr);
      dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_R,
            curr_row_addr, addr.highrow, addr.bank,
            addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
      cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_R,
            curr_row_addr);
#endif
    }
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in DRAM addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f3, f6, ,f10  from table-a
// This is a prefetch version
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_14(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int prefetch_size) {
  printf("GenDRAMTrace_14 is running, Select f3, f6, f10 from table-a.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};

  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    uint32_t curr_row_addr = ColAddr2RowAddr(curr_col_addr);
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    curr_row_addr += kCachelineSize;
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in DRAM addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f1, ,f9  from table-a
// where f1 is a wide col has 64B
// This is a prefetch version
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_15(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int prefetch_size) {
  printf("GenDRAMTrace_15 is running, Select f1,f9 from table-a, f9 is a 64B wide col\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};

  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    uint32_t curr_row_addr = ColAddr2RowAddr(curr_col_addr);
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    curr_row_addr += kCachelineSize;
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in DRAM addressing, begin with a base address.
// The acces pattern of the following SQL query:
// This is a prefetch version
// Select f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16  from table-a where f19 > x.
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_16(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int prefetch_size) {
  printf("GenDRAMTrace_16 is running, Select f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16  from table-a where f19 > x.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f1_addr = {};
  Address f17_addr = {};

  // row read f10 first
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f10_addr.column += offsetof(IMDB_Large, f10) / kCellSize;
    f17_addr.column += offsetof(IMDB_Large, f17) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f17_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    // if f10 match row read f3, f4
    if(i % 10 < THRESHOLD)
    {
      continue;
    }
    curr_row_addr = ColAddr2RowAddr(GenColAddr(f1_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
    curr_row_addr = ColAddr2RowAddr(GenColAddr(f10_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in DRAM addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select * from table-a where f19 > x
// This is a prefetch version
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_17(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int prefetch_size) {
  printf("GenDRAMTrace_17 is running, Select * from table-a where f19 > x\n");
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f19_addr = {};
  Address f1_addr = {};
  Address f17_addr = {};

  // row read f10 first
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f19_addr.column += offsetof(IMDB_Large, f19) / kCellSize;
    f17_addr.column += offsetof(IMDB_Large, f17) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f19_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    // if f10 match row read f3, f4
    if(i % 10 < THRESHOLD)
    {
      continue;
    }
    curr_row_addr = ColAddr2RowAddr(GenColAddr(f1_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
	for(int k = 0; k < tuple_size / kCachelineSize; k++)
	{
		if(k == 2)
		{
			curr_row_addr += kCachelineSize;
			continue;
		}
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
	curr_row_addr += kCachelineSize;
	}  
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in DRAM addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f1, f9 ,f19, f29  from table-a
// This is a prefetch version
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_18(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int prefetch_size) {
  printf("GenDRAMTrace_18 is running, Select f1,f9,f19,f29  from table-a.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
    for(int k = 0; k < 4; k++)
    {
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
  	curr_row_addr += kCachelineSize;	
    }
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in DRAM addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f1, f9 ,f19, f29, f39  from table-a
// This is a prefetch version
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_19(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int prefetch_size) {
  printf("GenDRAMTrace_19 is running, Select f1,f9,f19,f29,f39  from table-a.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
    for(int k = 0; k < 5; k++)
    {
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
  	curr_row_addr += kCachelineSize;	
    }
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}


// Generates DRAM query trace in hybrid addressing, begin with a base address.
// The acces pattern of the following SQL query:
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.
// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
// mix of col read and row read
int GenDRAMTrace_20(const char* trace_file_name, int num_tuples, int tuple_size,
		  uint32_t base_col_addr, int row_rate, int col_rate) {
  printf("GenDRAMTrace_20 is running, Mix col read with row read Row Select f1 from table a, Col Select f10 from table a. The rate is Row:Col =  %d : %d.\n",row_rate,col_rate);

  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  uint32_t curr_col_addr2 = base_col_addr;

  int row_tuple_count = 0;
  int col_tuple_count = 0;
  // col read f10 first

  for(int i = 0; i < num_tuples; i ++) {
   if((i % (row_rate + col_rate)) < col_rate)
{
	for(int k = 0; k < kCachelineSize / kCellSize; k++)
	{
    curr_col_addr = CalTupleColAddrLayout2(col_tuple_count++, tuple_size, base_col_addr);
    uint32_t curr_row_addr = ColAddr2RowAddr(curr_col_addr);
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
    curr_row_addr += kCachelineSize;	
#ifdef DEBUG
      	ParseRowAddr(curr_row_addr, addr);
      	dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
      	cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
      	++num_traces;
	}
}
else
{	// for each line in of the data in this cacheline, compare f10 first, if success, then row_read the f3,f4
	curr_col_addr2 = CalTupleColAddrLayout2(row_tuple_count++, tuple_size, base_col_addr);
    	uint32_t curr_row_addr2 = ColAddr2RowAddr(curr_col_addr2);
	curr_row_addr2 = GetCacheLineAddr(curr_row_addr2);
	
#ifdef DEBUG
      	ParseRowAddr(curr_row_addr2, addr);
      	dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr2, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
      	cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr2);
#endif
      	++num_traces;
	}
} 
printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in hybrid addressing, begin with a base address.
// The acces pattern of the following SQL query:
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.
// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
// mix of col read and row read
int GenDRAMTrace_21(const char* trace_file_name, int num_tuples, int tuple_size,
		  uint32_t base_col_addr, int row_rate, int col_rate) {
  printf("GenDRAMTrace_21 is running, Mix col read with row read Row Set f1 from table a, Col Set f10 from table a. The rate is Row:Col =  %d : %d.\n",row_rate,col_rate);

  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "W";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  uint32_t curr_col_addr2 = base_col_addr;

  int row_tuple_count = 0;
  int col_tuple_count = 0;
  // col read f10 first

  for(int i = 0; i < num_tuples; i ++) {
   if((i % (row_rate + col_rate)) < col_rate)
{
	for(int k = 0; k < kCachelineSize / kCellSize; k++)
	{
    curr_col_addr = CalTupleColAddrLayout2(col_tuple_count++, tuple_size, base_col_addr);
    uint32_t curr_row_addr = ColAddr2RowAddr(curr_col_addr);
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
    curr_row_addr += kCachelineSize;	
#ifdef DEBUG
      	ParseRowAddr(curr_row_addr, addr);
      	dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
      	cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
      	++num_traces;
	}
}
else
{	// for each line in of the data in this cacheline, compare f10 first, if success, then row_read the f3,f4
	curr_col_addr2 = CalTupleColAddrLayout2(row_tuple_count++, tuple_size, base_col_addr);
    	uint32_t curr_row_addr2 = ColAddr2RowAddr(curr_col_addr2);
	curr_row_addr2 = GetCacheLineAddr(curr_row_addr2);
	
#ifdef DEBUG
      	ParseRowAddr(curr_row_addr2, addr);
      	dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr2, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
      	cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr2);
#endif
      	++num_traces;
	}
} 
printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates DRAM query trace in DRAM addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f9 from table-a where f10 > x  | few of f10 is > x
// 
//
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_22(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenDRAMTrace_22 is running, Select f9 from table-a where f10 > x | few f10 > x.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f3_addr = {};

  // row read f10 first
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f3_addr = addr;
    f10_addr = addr;
    f3_addr.column += offsetof(IMDBRow, f3) / kCellSize;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f10_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}


// Generates DRAM query trace in row addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select AVG(f1) from table-a where f10 > x 
// 
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_23(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenDRAMTrace_23 is running, Select AVG(f1) from table-a where f10 > x .\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f1_addr = {};

  // row read f10 first
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f1 and f10 elements
    f1_addr = addr;
    f10_addr = addr;
    f1_addr.column += offsetof(IMDBRow, f1) / kCellSize;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f10_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    // if f10 match row read f1
    if(i % 10 < THRESHOLD)
    {
      continue;
    }
    curr_row_addr = ColAddr2RowAddr(GenColAddr(f1_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}


// Generates DRAM query trace in DRAM addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select AVG(f1) from table-a where f10 > x  | most of f10 is > x
// 
//
// The base_addr means the address of the first element in the left-upper corner
// of the IMDB in the form of column addressing.

// Layout 2:
// Fill as many tuples as possible in a row
//+=========+=========+==+=========+
//| Tuple 1 | Tuple 5 |  |         |
//+---------+---------+--+---------+
//| Tuple 2 | Tuple 6 |  |         |
//+---------+---------+--+---------+
//| Tuple 3 | Tuple 7 |  |         |
//+---------+---------+--+---------+
//| Tuple 4 | Tuple 8 |  | Tuple N |
//+---------+---------+--+---------+
// Assume Tuple 1 is in Column 0
int GenDRAMTrace_24(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {

  printf("GenDRAMTrace_24 is running, Select AVG(f1) from table-a where f10 > x | most of f10 is > x .\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f1_addr = {};

  // row read f10 first
  for(int i = 0; i < num_tuples; i++) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f1 and f10 elements
    f1_addr = addr;
    f10_addr = addr;
    f1_addr.column += offsetof(IMDBRow, f1) / kCellSize;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
    uint32_t curr_row_addr = ColAddr2RowAddr(GenColAddr(f10_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;

    // if f10 match row read f1
    if(i % 10 < THRESHOLD_LOW)
    {
      continue;
    }
    curr_row_addr = ColAddr2RowAddr(GenColAddr(f1_addr));
    curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_R,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_R,
              curr_row_addr);
#endif
    num_traces++;
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

