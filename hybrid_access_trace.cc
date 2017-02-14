/*
 * hybrid_access_trace.cc
 */

#include "gen_trace.h"
#include <stdio.h>
#include <stddef.h>
#include "addr_util.h"

// Generates hybrid query trace in hybrid addressing, begin with a base address.
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
int GenHybTrace_1(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenHybTrace_1 is running, Select f3,f4 from table-a where f10 > x .\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";
  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};

  int stride =  kCachelineSize / kCellSize;

  // col read f10 first
  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f10_addr = addr;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
            f10_addr.channel, f10_addr.lowrow, f10_addr.column,
            f10_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f10_addr));
#endif

    // for each line in of the data in this cacheline, compare f10 first, if success, then row_read the f3,f4
    for(int j = 0; j < stride; j++)
    {
    	if((i + j) % 10 < THRESHOLD)
    	{
    		continue;
    	}
    	uint32_t temp_col = CalTupleColAddrLayout2(i + j, tuple_size, base_col_addr);
    	uint32_t curr_row_addr = ColAddr2RowAddr(temp_col);
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
      	++num_traces;
    } 
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}


// Generates hybrid query trace in hybrid addressing, begin with a base address.
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
int GenHybTrace_2(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenHybTrace_2 is running, Select * from table-a where f10 > x .\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";
  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};

  int stride =  kCachelineSize / kCellSize;

  // col read f10 first
  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f10_addr = addr;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
            f10_addr.channel, f10_addr.lowrow, f10_addr.column,
            f10_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f10_addr));
#endif

    // for each line in of the data in this cacheline, compare f10 first, if success, then row_read the f3,f4
    for(int j = 0; j < stride; j++)
    {
      if((i + j) % 10 < THRESHOLD)
      {
        continue;
      }
      uint32_t temp_col = CalTupleColAddrLayout2(i + j, tuple_size, base_col_addr);
      uint32_t curr_row_addr = ColAddr2RowAddr(temp_col);
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
        ++num_traces;
        
        curr_row_addr = curr_row_addr + kCachelineSize;
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
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}


// Generates hybrid query trace in hybrid addressing, begin with a base address.
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
int GenHybTrace_3(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenHybTrace_3 is running, update table-a set f3 = xx, f4 = xx where f10 = xx .\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";
  const char *op_R = "R";
  const char *op_W = "W";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};

  int stride =  kCachelineSize / kCellSize;

  // col read f10 first
  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f10_addr = addr;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
            f10_addr.channel, f10_addr.lowrow, f10_addr.column,
            f10_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f10_addr));
#endif

    // for each line in of the data in this cacheline, compare f10 first, if success, then row_read the f3,f4
    for(int j = 0; j < stride; j++)
    {
      if((i + j) % 10 < THRESHOLD)
      {
        continue;
      }
      uint32_t temp_col = CalTupleColAddrLayout2(i + j, tuple_size, base_col_addr);
      uint32_t curr_row_addr = ColAddr2RowAddr(temp_col);
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
        ++num_traces;
        
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_W,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_W,
              curr_row_addr);
#endif
        ++num_traces;
    } 
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}


// Generates hybrid query trace in hybrid addressing, begin with a base address.
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
int GenHybTrace_4(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {

  printf("GenHybTrace_4 is running, Select * from table-a where f10 > x | most of f10 is > x .\n");
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


// Generates hybrid query trace in hybrid addressing, begin with a base address.
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
int GenHybTrace_5(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenHybTrace_5 is running, Select f9 from table-a where f10 > x | most f10 is > x.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";

  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f9_addr = {};

  int stride =  kCachelineSize / kCellSize;

  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f10_addr = addr;
    f9_addr = addr;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
    f9_addr.column += offsetof(IMDBRow, f9) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
            f10_addr.channel, f10_addr.lowrow, f10_addr.column,
            f10_addr.intrabus);
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f9_addr), f9_addr.highrow, f9_addr.bank,
            f9_addr.channel, f9_addr.lowrow, f9_addr.column,
            f9_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f10_addr));
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f9_addr));
#endif
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}


// Generates hybrid query trace in hybrid addressing, begin with a base address.
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
int GenHybTrace_6(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenHybTrace_6 is running, update table-a set f9 = xx where f10 = xx .| few f10 = xx\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";
  const char *op_R = "R";
  const char *op_W = "W";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};

  int stride =  kCachelineSize / kCellSize;

  // col read f10 first
  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f10_addr = addr;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
            f10_addr.channel, f10_addr.lowrow, f10_addr.column,
            f10_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f10_addr));
#endif

    // for each line in of the data in this cacheline, compare f10 first, if success, then row_read the f3,f4
    for(int j = 0; j < stride; j++)
    {
      if((i + j) % 10 < THRESHOLD)
      {
        continue;
      }
      uint32_t temp_col = CalTupleColAddrLayout2(i + j, tuple_size, base_col_addr);
      uint32_t curr_row_addr = ColAddr2RowAddr(temp_col);
      curr_row_addr = GetCacheLineAddr(curr_row_addr);
      curr_row_addr = curr_row_addr + kCachelineSize;
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
#ifdef DEBUG
        ParseRowAddr(curr_row_addr, addr);
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces, op_W,
              curr_row_addr, addr.highrow, addr.bank,
              addr.channel, addr.lowrow, addr.column, addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces, op_W,
              curr_row_addr);
#endif
        ++num_traces;
    } 
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates hybrid query trace in hybrid addressing, begin with a base address.
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
int GenHybTrace_7(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr_a, uint32_t base_col_addr_b) {
  printf("GenHybTrace_7 is running, Select table-a.f3, table-b.f4 from table-a, table-b where table-a.f1 > table-b.f1 table-a.f9 = table-b.f9.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";
  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr_a = GetCacheLineAddr(base_col_addr_a);
  base_col_addr_b = GetCacheLineAddr(base_col_addr_b);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address for table-a: 0x%08x\n", base_col_addr_a);
  printf("Base column address for table-b: 0x%08x\n", base_col_addr_b);
  uint32_t curr_col_addr_a = base_col_addr_a;
  uint32_t curr_col_addr_b = base_col_addr_b;
  Address addr_a = {};
  Address addr_b = {};
  Address f1_addr_a = {};
  Address f1_addr_b = {};
  Address f9_addr_a = {};
  Address f9_addr_b = {};

  int stride =  kCachelineSize / kCellSize;

  // col read f1, f9 first
  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr_a = CalTupleColAddrLayout2(i, tuple_size, base_col_addr_a);
    ParseColAddr(curr_col_addr_a, addr_a);
    // Calculate the column address of the table-a.f9 and table-b.f9
    f1_addr_a = addr_a;
    f1_addr_a.column += offsetof(IMDBRow, f1) / kCellSize;
    f9_addr_a = addr_a;
    f9_addr_a.column += offsetof(IMDBRow, f9) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f1_addr_a), f1_addr_a.highrow, f1_addr_a.bank,
            f1_addr_a.channel, f1_addr_a.lowrow, f1_addr_a.column,
            f1_addr_a.intrabus);
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f9_addr_a), f9_addr_a.highrow, f9_addr_a.bank,
            f9_addr_a.channel, f9_addr_a.lowrow, f9_addr_a.column,
            f9_addr_a.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f1_addr_a));
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f9_addr_a));
#endif
  }
  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr_b = CalTupleColAddrLayout2(i, tuple_size, base_col_addr_b);
    ParseColAddr(curr_col_addr_b, addr_b);
    // Calculate the column address of the table-a.f9 and table-b.f9
    f1_addr_b = addr_b;
    f1_addr_b.column += offsetof(IMDBRow, f1) / kCellSize;
    f9_addr_b = addr_b;
    f9_addr_b.column += offsetof(IMDBRow, f9) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f1_addr_b), f1_addr_b.highrow, f1_addr_b.bank,
            f1_addr_b.channel, f1_addr_b.lowrow, f1_addr_b.column,
            f1_addr_b.intrabus);
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f9_addr_b), f9_addr_b.highrow, f9_addr_b.bank,
            f9_addr_b.channel, f9_addr_b.lowrow, f9_addr_b.column,
            f9_addr_b.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f1_addr_b));
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f9_addr_b));
#endif
  }

  // compare first; if success, then row_read the table-a.f3, and table-b.f4
  for(int i = 0; i < num_tuples; i++)
  {
    if(i % 10 < THRESHOLD)
    {
      continue;
    }
    uint32_t temp_col_a = CalTupleColAddrLayout2(i, tuple_size, base_col_addr_a);
    uint32_t curr_row_addr_a = ColAddr2RowAddr(temp_col_a);
    curr_row_addr_a = GetCacheLineAddr(curr_row_addr_a);
    uint32_t temp_col_b = CalTupleColAddrLayout2(i, tuple_size, base_col_addr_b);
    uint32_t curr_row_addr_b = ColAddr2RowAddr(temp_col_b);
    curr_row_addr_b = GetCacheLineAddr(curr_row_addr_b);
#ifdef DEBUG
    //Address addr;
    ParseRowAddr(curr_row_addr_a, addr_a);
    ParseRowAddr(curr_row_addr_b, addr_b);
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_R,
           curr_row_addr_a, addr_a.highrow, addr_a.bank,
           addr_a.channel, addr_a.lowrow, addr_a.column, addr_a.intrabus);
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_R,
           curr_row_addr_b, addr_b.highrow, addr_b.bank,
           addr_b.channel, addr_b.lowrow, addr_b.column, addr_b.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_R,
           curr_row_addr_a);
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_R,
           curr_row_addr_b);
#endif
  } 
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates hybrid query trace in hybrid addressing, begin with a base address.
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
int GenHybTrace_8(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr_a, uint32_t base_col_addr_b) {
  printf("GenHybTrace_8 is running, Select table-a.f3, table-b.f4 from table-a, table-b where table-a.f9 = table-b.f9.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";
  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr_a = GetCacheLineAddr(base_col_addr_a);
  base_col_addr_b = GetCacheLineAddr(base_col_addr_b);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address for table-a: 0x%08x\n", base_col_addr_a);
  printf("Base column address for table-b: 0x%08x\n", base_col_addr_b);
  uint32_t curr_col_addr_a = base_col_addr_a;
  uint32_t curr_col_addr_b = base_col_addr_b;
  Address addr_a = {};
  Address addr_b = {};
  Address f9_addr_a = {};
  Address f9_addr_b = {};

  int stride =  kCachelineSize / kCellSize;

  // col read table-a.f9, table-b.f9 first
  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr_a = CalTupleColAddrLayout2(i, tuple_size, base_col_addr_a);
    ParseColAddr(curr_col_addr_a, addr_a);
    // Calculate the column address of the table-a.f9 and table-b.f9
    f9_addr_a = addr_a;
    f9_addr_a.column += offsetof(IMDBRow, f9) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f9_addr_a), f9_addr_a.highrow, f9_addr_a.bank,
            f9_addr_a.channel, f9_addr_a.lowrow, f9_addr_a.column,
            f9_addr_a.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f9_addr_a));
#endif
  }
  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr_b = CalTupleColAddrLayout2(i, tuple_size, base_col_addr_b);
    ParseColAddr(curr_col_addr_b, addr_b);
    // Calculate the column address of the table-a.f9 and table-b.f9
    f9_addr_b = addr_b;
    f9_addr_b.column += offsetof(IMDBRow, f9) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f9_addr_b), f9_addr_b.highrow, f9_addr_b.bank,
            f9_addr_b.channel, f9_addr_b.lowrow, f9_addr_b.column,
            f9_addr_b.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f9_addr_b));
#endif
  }

  // compare table-a.f9 and table-b.f9 first, if success, then row_read the table-a.f3, and table-b.f4
  for(int i = 0; i < num_tuples; i++)
  {
    if(i % 10 < THRESHOLD)
    {
      continue;
    }
    uint32_t temp_col_a = CalTupleColAddrLayout2(i, tuple_size, base_col_addr_a);
    uint32_t curr_row_addr_a = ColAddr2RowAddr(temp_col_a);
    curr_row_addr_a = GetCacheLineAddr(curr_row_addr_a);
    uint32_t temp_col_b = CalTupleColAddrLayout2(i, tuple_size, base_col_addr_b);
    uint32_t curr_row_addr_b = ColAddr2RowAddr(temp_col_b);
    curr_row_addr_b = GetCacheLineAddr(curr_row_addr_b);
#ifdef DEBUG
    //Address addr;
    ParseRowAddr(curr_row_addr_a, addr_a);
    ParseRowAddr(curr_row_addr_b, addr_b);
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_R,
           curr_row_addr_a, addr_a.highrow, addr_a.bank,
           addr_a.channel, addr_a.lowrow, addr_a.column, addr_a.intrabus);
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_R,
           curr_row_addr_b, addr_b.highrow, addr_b.bank,
           addr_b.channel, addr_b.lowrow, addr_b.column, addr_b.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_R,
           curr_row_addr_a);
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_R,
           curr_row_addr_b);
#endif
  } 
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}


// Generates hybrid query trace in hybrid addressing, begin with a base address.
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
int GenHybTrace_9(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenHybTrace_9 is running, Select f3,f4 from table-a where f1 > x and f9 < y.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";
  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f1_addr = {};
  Address f9_addr = {};

  int stride =  kCachelineSize / kCellSize;

  // col read f1, f9 first
  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f1 and f9 elements
    f1_addr = addr;
    f9_addr = addr;
    f1_addr.column += offsetof(IMDBRow, f1) / kCellSize;
    f9_addr.column += offsetof(IMDBRow, f9) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f1_addr), f1_addr.highrow, f1_addr.bank,
            f1_addr.channel, f1_addr.lowrow, f1_addr.column,
            f1_addr.intrabus);
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f9_addr), f9_addr.highrow, f9_addr.bank,
            f9_addr.channel, f9_addr.lowrow, f9_addr.column,
            f9_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f1_addr));
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f9_addr));
#endif

    // for each line in of the data in this cacheline, compare f1 and f9 first, if success, then row_read the f3,f4
    for(int j = 0; j < stride; j++)
    {
    	if((i + j) % 10 < THRESHOLD)
    	{
    		continue;
    	}
    	uint32_t temp_col = CalTupleColAddrLayout2(i + j, tuple_size, base_col_addr);
    	uint32_t curr_row_addr = ColAddr2RowAddr(temp_col);
        curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
      	//Address addr;
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
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}


// Generates hybrid query trace in hybrid addressing, begin with a base address.
// The access pattern of the following SQL query:
// Select f3,f4 from table-a where f1 > x and f2 < y 
//
// This method is better for 2D NVM if the miss rate of 'where clause' is high
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
int GenHybTrace_10(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenHybTrace_10 is running, Select f3,f4 from table-a where f1 > x and f2 < y.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";
  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f1_addr = {};
  Address f2_addr = {};

  int stride =  kCachelineSize / kCellSize;

  // col read f1, f2 first
  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f1 and f2 elements
    f1_addr = addr;
    f2_addr = addr;
    f1_addr.column += offsetof(IMDBRow, f1) / kCellSize;
    f2_addr.column += offsetof(IMDBRow, f2) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f1_addr), f1_addr.highrow, f1_addr.bank,
            f1_addr.channel, f1_addr.lowrow, f1_addr.column,
            f1_addr.intrabus);
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f2_addr), f2_addr.highrow, f2_addr.bank,
            f2_addr.channel, f2_addr.lowrow, f2_addr.column,
            f2_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f1_addr));
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f2_addr));
#endif

    // for each line in of the data in this cacheline, compare f1 and f2 first, if success, then row_read the f3,f4
    for(int j = 0; j < stride; j++)
    {
    	if((i + j) % 10 < THRESHOLD)
    	{
    		continue;
    	}
    	uint32_t temp_col = CalTupleColAddrLayout2(i + j, tuple_size, base_col_addr);
    	uint32_t curr_row_addr = ColAddr2RowAddr(temp_col);
	curr_row_addr = GetCacheLineAddr(curr_row_addr);
#ifdef DEBUG
      	//Address addr;
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
int GenHybTrace_11(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int L3_size) {
  printf("GenHybTrace_11 is running, Select f3,f4 from table-a where f10 > x | use L3 cache to speed up.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  // how many cache lines can stored in L3  
  int num_cacheline = (int)( L3_size * 1024 * 1024 / kCachelineSize);

  const char *op_C = "r";
  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};

  int stride =  kCachelineSize / kCellSize;
  // adjust num_cacheline 
  num_cacheline = (int) (num_cacheline / (1 + stride * THRESHOLD / 10));

  // col read f10 first
  for(int i = 0; i < num_tuples; i += stride * num_cacheline) {
    for(int k = 0; k < num_cacheline; k++)
    {
    curr_col_addr = CalTupleColAddrLayout2(i + k * stride, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f10_addr = addr;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
            f10_addr.channel, f10_addr.lowrow, f10_addr.column,
            f10_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f10_addr));
#endif
    }
    // for each line in of the data in this cacheline, compare f10 first, if success, then row_read the f3,f4
    for(int k = 0; k < num_cacheline; k++)
    {
    	for(int j = 0; j < stride; j++)
    	{
    		if((i + j + k * stride) % 10 < THRESHOLD)
    		{	
    			continue;
    		}
    		uint32_t temp_col = CalTupleColAddrLayout2(i + k * stride + j, tuple_size, base_col_addr);
    		uint32_t curr_row_addr = ColAddr2RowAddr(temp_col);
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
      		++num_traces;
    	}
    } 
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates hybrid query trace in hybrid addressing, begin with a base address.
// The access pattern is generated from random_pattern, mixed with R, W, r, w
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
//+---------+---------+--+0
int GenHybTrace_12(const char* trace_file_name, int num_query, int r_ratio, int w_ratio, int cr_ratio, const char* random_file, const char* description) {
  printf("%s%s%s", "GenHybTrace_12_", description, " is running, generating a random pattern.\n");
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
  while (fscanf(data, "%d %d", &op, &pos) > 0){
    pos = GetCacheLineAddr(pos);
    Address addr = {};

    switch (op){
      case 0:
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
        ParseColAddr(pos, addr);
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, "r",
                GenColAddr(addr), addr.highrow, addr.bank,
                addr.channel, addr.lowrow, addr.column,
                addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, "r", GenColAddr(addr));
#endif
            break;
      case 3:
        ParseColAddr(pos, addr);
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, "w",
                GenColAddr(addr), addr.highrow, addr.bank,
                addr.channel, addr.lowrow, addr.column,
                addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, "w", GenColAddr(addr));
#endif
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
int GenHybTrace_13(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int prefetch_size) {
  printf("GenHybTrace_13 is running, Select f3 from table-a (f3 is a wide column).\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f3_addr = {};

  int stride =  kCachelineSize / kCellSize;
  int column_num = (offsetof(IMDBOverCLRow, f4) - offsetof(IMDBOverCLRow, f3)) / kCellSize;

  // col read f3a and f3b
  for(int i = 0; i < num_tuples; i += stride * prefetch_size) {
    for (int k = 0; k < column_num; k ++){
      for (int j = 0; j < prefetch_size; j ++){
        curr_col_addr = CalTupleColAddrLayout2(i + j * stride, tuple_size, base_col_addr);
        ParseColAddr(curr_col_addr, addr);
        // Calculate the column address of f3a and f3b
        f3_addr = addr;
        f3_addr.column += offsetof(IMDBOverCLRow, f3) / kCellSize + k;
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
                GenColAddr(f3_addr), f3_addr.highrow, f3_addr.bank,
                f3_addr.channel, f3_addr.lowrow, f3_addr.column,
                f3_addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f3_addr));
#endif
      }
    }
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}


// Generates hybrid query trace in hybrid addressing, begin with a base address.
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
int GenHybTrace_14(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int prefetch_size) {
  printf("GenHybTrace_14 is running, Select f3, f6, f10 from table-a.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f3_addr = {};
  Address f6_addr = {};
  Address f10_addr = {};

  int stride =  kCachelineSize / kCellSize;

  // col read f3a and f3b
  for(int i = 0; i < num_tuples; i += stride * prefetch_size) {
      for (int j = 0; j < prefetch_size; j ++){
        curr_col_addr = CalTupleColAddrLayout2(i + j * stride, tuple_size, base_col_addr);
        ParseColAddr(curr_col_addr, addr);
        // Calculate the column address of f3a and f3b
        f3_addr = addr;
        f3_addr.column += offsetof(IMDBRow, f3) / kCellSize;
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
                GenColAddr(f3_addr), f3_addr.highrow, f3_addr.bank,
                f3_addr.channel, f3_addr.lowrow, f3_addr.column,
                f3_addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f3_addr));
#endif
      }
      for (int j = 0; j < prefetch_size; j ++){
        curr_col_addr = CalTupleColAddrLayout2(i + j * stride, tuple_size, base_col_addr);
        ParseColAddr(curr_col_addr, addr);
        // Calculate the column address of f3a and f3b
        f6_addr = addr;
        f6_addr.column += offsetof(IMDBRow, f6) / kCellSize;
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
                GenColAddr(f6_addr), f6_addr.highrow, f6_addr.bank,
                f6_addr.channel, f6_addr.lowrow, f6_addr.column,
                f6_addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f6_addr));
#endif
      }
      for (int j = 0; j < prefetch_size; j ++){
        curr_col_addr = CalTupleColAddrLayout2(i + j * stride, tuple_size, base_col_addr);
        ParseColAddr(curr_col_addr, addr);
        // Calculate the column address of f3a and f3b
        f10_addr = addr;
        f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
                GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
                f10_addr.channel, f10_addr.lowrow, f10_addr.column,
                f10_addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f10_addr));
#endif
      }
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates hybrid query trace in hybrid addressing, begin with a base address.
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
int GenHybTrace_15(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int prefetch_size) {

  printf("GenHybTrace_15 is running, Select f1,f9 from table a, where f1 is a 64B wide col .\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";
  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f9_addr = {};

  int stride =  kCachelineSize / kCellSize;

  // col read f10 first
  for(int i = 0; i < num_tuples; i += stride * prefetch_size) {
    // for each line in of the data in this cacheline, compare f10 first, if success, then row_read the f3,f4
    for(int k = 0; k < prefetch_size; k++)
    {
    	for(int j = 0; j < stride; j++)
    	{
    		uint32_t temp_col = CalTupleColAddrLayout2(i + k * stride + j, tuple_size, base_col_addr);
    		uint32_t curr_row_addr = ColAddr2RowAddr(temp_col);
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
      		++num_traces;
    	}
    } 
    for(int k = 0; k < prefetch_size; k++)
    {
    curr_col_addr = CalTupleColAddrLayout2(i + k * stride, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f9_addr = addr;
    f9_addr.column += offsetof(IMDB15, f9) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f9_addr), f9_addr.highrow, f9_addr.bank,
            f9_addr.channel, f9_addr.lowrow, f9_addr.column,
            f9_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f9_addr));
#endif
    }
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}


// Generates hybrid query trace in hybrid addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16, from table-a where f19 > x 
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
// 
// More complex version for Trace 1
int GenHybTrace_16(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenHybTrace_16 is running, Select  f1,f2,f3,f4,f5,f6,f7,f8,f9f3,f10,f11,f12,f13,f14,f15,f16,  from table-a where f19 > x .\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";
  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f19_addr = {};

  int stride =  kCachelineSize / kCellSize;

  // col read f10 first
  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f19_addr = addr;
    f19_addr.column += offsetof(IMDB_Large, f19) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f19_addr), f19_addr.highrow, f19_addr.bank,
            f19_addr.channel, f19_addr.lowrow, f19_addr.column,
            f19_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f19_addr));
#endif

    // for each line in of the data in this cacheline, compare f10 first, if success, then row_read the f3,f4
    for(int j = 0; j < stride; j++)
    {
    	if((i + j) % 10 < THRESHOLD)
    	{
    		continue;
    	}
    	uint32_t temp_col = CalTupleColAddrLayout2(i + j, tuple_size, base_col_addr);
    	uint32_t curr_row_addr = ColAddr2RowAddr(temp_col);
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
      	++num_traces;
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
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates hybrid query trace in hybrid addressing, begin with a base address.
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
//
// More complex version for trace 2
int GenHybTrace_17(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenHybTrace_17 is running, Select * from table-a where f10 > x .\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";
  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};

  int stride =  kCachelineSize / kCellSize;

  // col read f10 first
  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f10_addr = addr;
    f10_addr.column += offsetof(IMDB_Large, f10) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
            f10_addr.channel, f10_addr.lowrow, f10_addr.column,
            f10_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f10_addr));
#endif

    // for each line in of the data in this cacheline, compare f10 first, if success, then row_read the f3,f4
    for(int j = 0; j < stride; j++)
    {
      if((i + j) % 10 < THRESHOLD)
      {
        continue;
      }
      uint32_t temp_col = CalTupleColAddrLayout2(i + j, tuple_size, base_col_addr);
      uint32_t curr_row_addr = ColAddr2RowAddr(temp_col);
      curr_row_addr = GetCacheLineAddr(curr_row_addr);
 	for(int k = 0; k < tuple_size / kCachelineSize; k++)
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
        ++num_traces;
        curr_row_addr = curr_row_addr + kCachelineSize;
	}
    } 
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates hybrid query trace in hybrid addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f3, f10, ,f19,f29  from table-a
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
//
// More complex version for trace 14
int GenHybTrace_18(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int prefetch_size) {
  printf("GenHybTrace_18 is running, Select f3, f10, f19, f29 from table-a.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f3_addr = {};
  Address f9_addr = {};
  Address f19_addr = {};
  Address f29_addr = {};

  int stride =  kCachelineSize / kCellSize;

  for(int i = 0; i < num_tuples; i += stride * prefetch_size) {
      for (int j = 0; j < prefetch_size; j ++){
        curr_col_addr = CalTupleColAddrLayout2(i + j * stride, tuple_size, base_col_addr);
        ParseColAddr(curr_col_addr, addr);
        f3_addr = addr;
        f3_addr.column += offsetof(IMDB_Large, f3) / kCellSize;
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
                GenColAddr(f3_addr), f3_addr.highrow, f3_addr.bank,
                f3_addr.channel, f3_addr.lowrow, f3_addr.column,
                f3_addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f3_addr));
#endif
      }
      for (int j = 0; j < prefetch_size; j ++){
        curr_col_addr = CalTupleColAddrLayout2(i + j * stride, tuple_size, base_col_addr);
        ParseColAddr(curr_col_addr, addr);
        f9_addr = addr;
        f9_addr.column += offsetof(IMDB_Large, f9) / kCellSize;
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
                GenColAddr(f9_addr), f9_addr.highrow, f9_addr.bank,
                f9_addr.channel, f9_addr.lowrow, f9_addr.column,
                f9_addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f9_addr));
#endif
      }
      for (int j = 0; j < prefetch_size; j ++){
        curr_col_addr = CalTupleColAddrLayout2(i + j * stride, tuple_size, base_col_addr);
        ParseColAddr(curr_col_addr, addr);
        f19_addr = addr;
        f19_addr.column += offsetof(IMDB_Large, f19) / kCellSize;
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
                GenColAddr(f19_addr), f19_addr.highrow, f19_addr.bank,
                f19_addr.channel, f19_addr.lowrow, f19_addr.column,
                f19_addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f19_addr));
#endif
      }
      for (int j = 0; j < prefetch_size; j ++){
        curr_col_addr = CalTupleColAddrLayout2(i + j * stride, tuple_size, base_col_addr);
        ParseColAddr(curr_col_addr, addr);
        f29_addr = addr;
        f29_addr.column += offsetof(IMDB_Large, f29) / kCellSize;
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
                GenColAddr(f29_addr), f29_addr.highrow, f29_addr.bank,
                f29_addr.channel, f29_addr.lowrow, f29_addr.column,
                f29_addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f29_addr));
#endif
      }
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates hybrid query trace in hybrid addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f1, f9, f19, f29 ,f39  from table-a
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
//
// More complex version for trace 15
int GenHybTrace_19(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int prefetch_size) {

  printf("GenHybTrace_19 is running, Select f1,f9, f19, f29, f39 from table a.\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);
  //base_row_addr = GetCacheLineAddr(base_row_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f3_addr = {};
  Address f9_addr = {};
  Address f19_addr = {};
  Address f29_addr = {};
  Address f39_addr = {};

  int stride =  kCachelineSize / kCellSize;

  for(int i = 0; i < num_tuples; i += stride * prefetch_size) {
      for (int j = 0; j < prefetch_size; j ++){
        curr_col_addr = CalTupleColAddrLayout2(i + j * stride, tuple_size, base_col_addr);
        ParseColAddr(curr_col_addr, addr);
        f3_addr = addr;
        f3_addr.column += offsetof(IMDB_Large, f3) / kCellSize;
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
                GenColAddr(f3_addr), f3_addr.highrow, f3_addr.bank,
                f3_addr.channel, f3_addr.lowrow, f3_addr.column,
                f3_addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f3_addr));
#endif
      }
      for (int j = 0; j < prefetch_size; j ++){
        curr_col_addr = CalTupleColAddrLayout2(i + j * stride, tuple_size, base_col_addr);
        ParseColAddr(curr_col_addr, addr);
        f9_addr = addr;
        f9_addr.column += offsetof(IMDB_Large, f9) / kCellSize;
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
                GenColAddr(f9_addr), f9_addr.highrow, f9_addr.bank,
                f9_addr.channel, f9_addr.lowrow, f9_addr.column,
                f9_addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f9_addr));
#endif
      }
      for (int j = 0; j < prefetch_size; j ++){
        curr_col_addr = CalTupleColAddrLayout2(i + j * stride, tuple_size, base_col_addr);
        ParseColAddr(curr_col_addr, addr);
        f19_addr = addr;
        f19_addr.column += offsetof(IMDB_Large, f19) / kCellSize;
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
                GenColAddr(f19_addr), f19_addr.highrow, f19_addr.bank,
                f19_addr.channel, f19_addr.lowrow, f19_addr.column,
                f19_addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f19_addr));
#endif
      }
      for (int j = 0; j < prefetch_size; j ++){
        curr_col_addr = CalTupleColAddrLayout2(i + j * stride, tuple_size, base_col_addr);
        ParseColAddr(curr_col_addr, addr);
        f29_addr = addr;
        f29_addr.column += offsetof(IMDB_Large, f29) / kCellSize;
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
                GenColAddr(f29_addr), f29_addr.highrow, f29_addr.bank,
                f29_addr.channel, f29_addr.lowrow, f29_addr.column,
                f29_addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f29_addr));
#endif
      }
      for (int j = 0; j < prefetch_size; j ++){
        curr_col_addr = CalTupleColAddrLayout2(i + j * stride, tuple_size, base_col_addr);
        ParseColAddr(curr_col_addr, addr);
        f39_addr = addr;
        f39_addr.column += offsetof(IMDB_Large, f39) / kCellSize;
#ifdef DEBUG
        dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
                GenColAddr(f39_addr), f39_addr.highrow, f39_addr.bank,
                f39_addr.channel, f39_addr.lowrow, f39_addr.column,
                f39_addr.intrabus);
#else
        cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f39_addr));
#endif
      }
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates hybrid query trace in hybrid addressing, begin with a base address.
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
int GenHybTrace_20(const char* trace_file_name, int num_tuples, int tuple_size,
		  uint32_t base_col_addr, int row_rate, int col_rate) {
  printf("GenHybTrace_20 is running, Mix col read with row read Row Select f1 from table a, Col Select f10 from table a. The rate is Row:Col =  %d : %d.\n",row_rate,col_rate);

  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";
  const char *op_R = "R";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  uint32_t curr_col_addr2 = base_col_addr;
  Address addr = {};
  Address f10_addr = {};

  int stride =  kCachelineSize / kCellSize;
  int row_tuple_count = 0;
  int col_tuple_count = 0;
  // col read f10 first

  for(int i = 0; i < num_tuples; i ++) {
   if((i % (row_rate + col_rate)) < col_rate)
{
    curr_col_addr = CalTupleColAddrLayout2(col_tuple_count, tuple_size, base_col_addr);
    col_tuple_count += stride;
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f10_addr = addr;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
            f10_addr.channel, f10_addr.lowrow, f10_addr.column,
            f10_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f10_addr));
#endif
}
else
{	// for each line in of the data in this cacheline, compare f10 first, if success, then row_read the f3,f4
	curr_col_addr2 = CalTupleColAddrLayout2(row_tuple_count++, tuple_size, base_col_addr);
    	uint32_t curr_row_addr = ColAddr2RowAddr(curr_col_addr2);
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
      	++num_traces;
	}
} 
printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates hybrid query trace in hybrid addressing, begin with a base address.
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
int GenHybTrace_21(const char* trace_file_name, int num_tuples, int tuple_size,
		  uint32_t base_col_addr, int row_rate, int col_rate) {
  printf("GenHybTrace_21 is running, Mix col read with row read Row Set f1 from table a, Col Set f10 from table a. The rate is Row:Col =  %d : %d.\n",row_rate,col_rate);

  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "w";
  const char *op_R = "W";

  //int tuples_in_one_col = kCachelineSize
  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  uint32_t curr_col_addr2 = base_col_addr;
  Address addr = {};
  Address f10_addr = {};

  int stride =  kCachelineSize / kCellSize;
  int row_tuple_count = 0;
  int col_tuple_count = 0;
  // col read f10 first

  for(int i = 0; i < num_tuples; i ++) {
   if((i % (row_rate + col_rate)) < col_rate)
{
    curr_col_addr = CalTupleColAddrLayout2(col_tuple_count, tuple_size, base_col_addr);
    col_tuple_count += stride;
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f10_addr = addr;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
            f10_addr.channel, f10_addr.lowrow, f10_addr.column,
            f10_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f10_addr));
#endif
}
else
{	// for each line in of the data in this cacheline, compare f10 first, if success, then row_read the f3,f4
	curr_col_addr2 = CalTupleColAddrLayout2(row_tuple_count++, tuple_size, base_col_addr);
    	uint32_t curr_row_addr = ColAddr2RowAddr(curr_col_addr2);
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
      	++num_traces;
	}
} 
printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates hybrid query trace in hybrid addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f9 from table-a where f10 > x  
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
int GenHybTrace_22(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenHybTrace_22 is running, Select f9 from table-a where f10 > x\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";

  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f9_addr = {};

  int stride =  kCachelineSize / kCellSize;

  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f10_addr = addr;
    f9_addr = addr;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
    f9_addr.column += offsetof(IMDBRow, f9) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
            f10_addr.channel, f10_addr.lowrow, f10_addr.column,
            f10_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f10_addr));
#endif

  if(i % 10 < THRESHOLD)
  {
  	continue;
  }
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f9_addr), f9_addr.highrow, f9_addr.bank,
            f9_addr.channel, f9_addr.lowrow, f9_addr.column,
            f9_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f9_addr));
#endif
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

// Generates hybrid query trace in hybrid addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f1 from table-a where f10 > x  | most of f10 is > x
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
int GenHybTrace_23(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenHybTrace_23 is running, Select f1 from table-a where f10 > x | most of f10 is > x\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";

  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f9_addr = {};

  int stride =  kCachelineSize / kCellSize;

  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f10_addr = addr;
    f9_addr = addr;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
    f9_addr.column += offsetof(IMDBRow, f1) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
            f10_addr.channel, f10_addr.lowrow, f10_addr.column,
            f10_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f10_addr));
#endif

  if(i % 10 < THRESHOLD)
  {
  	continue;
  }
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f9_addr), f9_addr.highrow, f9_addr.bank,
            f9_addr.channel, f9_addr.lowrow, f9_addr.column,
            f9_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f9_addr));
#endif
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}


// Generates hybrid query trace in hybrid addressing, begin with a base address.
// The acces pattern of the following SQL query:
// Select f1 from table-a where f10 > x  
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
int GenHybTrace_24(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr) {
  printf("GenHybTrace_24 is running, Select f1 from table-a where f10 > x\n");
  // TODO: Check the range of num_tuples
  FILE *fp = fopen(trace_file_name, "w");
  if (fp == NULL) return kFailure;

  const char *op_C = "r";

  int num_traces = 0;

  // 2 cache lines in 1 RAM row
  base_col_addr = GetCacheLineAddr(base_col_addr);

  printf("Base column address: 0x%08x\n", base_col_addr);
  uint32_t curr_col_addr = base_col_addr;
  Address addr = {};
  Address f10_addr = {};
  Address f9_addr = {};

  int stride =  kCachelineSize / kCellSize;

  for(int i = 0; i < num_tuples; i += stride) {
    curr_col_addr = CalTupleColAddrLayout2(i, tuple_size, base_col_addr);
    ParseColAddr(curr_col_addr, addr);
    // Calculate the column address of the f3 and f10 elements
    f10_addr = addr;
    f9_addr = addr;
    f10_addr.column += offsetof(IMDBRow, f10) / kCellSize;
    f9_addr.column += offsetof(IMDBRow, f1) / kCellSize;
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f10_addr), f10_addr.highrow, f10_addr.bank,
            f10_addr.channel, f10_addr.lowrow, f10_addr.column,
            f10_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f10_addr));
#endif

  if(i % 10 < THRESHOLD_LOW)
  {
  	continue;
  }
#ifdef DEBUG
    dprintf(fp, "%d %s 0x%08x 0 0, [%d %d %d %d %d %d]\n", num_traces++, op_C,
            GenColAddr(f9_addr), f9_addr.highrow, f9_addr.bank,
            f9_addr.channel, f9_addr.lowrow, f9_addr.column,
            f9_addr.intrabus);
#else
    cprintf(fp, "%d %s 0x%08x 0 0\n", num_traces++, op_C, GenColAddr(f9_addr));
#endif
  }
  printf("Total number of traces: %d\n", num_traces);
  fclose(fp);
  return kSuccess;
}

