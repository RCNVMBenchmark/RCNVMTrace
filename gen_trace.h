/*
 * gen_trace.h
 */

#ifndef GEN_TRACE_H_
#define GEN_TRACE_H_

#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

// IMDB row (tuple) format with wide column:
// The length of the tuple is 128 bytes, or 16 columns.
//+-------+-------+-------+-------+--------+-------+--------+--------+--------+
//| f1(8) | f2(8) | f3(8) | f4(8) | f5(16) | f6(8) | f7(16) | f8(32) | f9(24) |
//+-------+-------+-------+-------+--------+-------+--------+--------+--------+
// However, if we consider the column splitting of wide-column, then the length
// will be 8*(9+4+2+1), 9, 4, 2, and 1 columns, respectively.
struct IMDBWideColumnRow{
  uint8_t f1[8];
  uint8_t f2[8];
  uint8_t f3[8];
  uint8_t f4[8];
  uint8_t f5[16];
  uint8_t f6[8];
  uint8_t f7[16];
  uint8_t f8[32];
  uint8_t f9[24];
} __attribute__((packed));


// IMDB row (tuple) format with wide column:
// The length of the tuple is 128 bytes, or 16 columns.
//+--------+-------+-------+-------+--------+-------+--------+--------+--------+
//| f1(64) | f2(8) | f3(8) | f4(8) | f5(8)  | f6(8) | f7(8)  | f8(8)  | f9(8)  |
//+--------+-------+-------+-------+--------+-------+--------+--------+--------+
// However, if we consider the column splitting of wide-column, then the length
// will be 8*(9+4+2+1), 9, 4, 2, and 1 columns, respectively.
struct IMDB15{
  uint8_t f1[48];
  uint8_t f2[8];
  uint8_t f3[8];
  uint8_t f4[8];
  uint8_t f5[8];
  uint8_t f6[8];
  uint8_t f7[8];
  uint8_t f8[8];
  uint8_t f9[8];
  uint8_t f10[16];
} __attribute__((packed));

// IMDB row (tuple) format with wide column:
// The length of the tuple is 128 bytes, or 16 columns.
//+-------+--------+--------+--------+-------+
//| f1(8) | f2(40) | f3(32) | f4(40) | f5(8) |
//+-------+--------+--------+--------+-------+
// However, if we consider the column splitting of wide-column, then the length
// will be 8*(1+5+4+5+1), 1, 5, 4, 5, and 1 columns, respectively.
// The f3 element stretches over a cache line border.
struct IMDBOverCLRow{
  uint8_t f1[8];
  uint8_t f2[40];
  uint8_t f3[32];
  uint8_t f4[40];
  uint8_t f5[8];
} __attribute__((packed));

// IMDB row (tuple) format:
// The length of the tuple is 512 bytes, or 64 columns.
// There are 64 fields in one tuple, and each field is 8 bytes.
struct IMDB_Large{
  uint8_t f1[8];
  uint8_t f2[8];
  uint8_t f3[8];
  uint8_t f4[8];
  uint8_t f5[8];
  uint8_t f6[8];
  uint8_t f7[8];
  uint8_t f8[8];
  uint8_t f9[8];
  uint8_t f10[8];
  uint8_t f11[8];
  uint8_t f12[8];
  uint8_t f13[8];
  uint8_t f14[8];
  uint8_t f15[8];
  uint8_t f16[8];
  uint8_t f17[8];
  uint8_t f18[8];
  uint8_t f19[8];
  uint8_t f20[8];
  uint8_t f21[8];
  uint8_t f22[8];
  uint8_t f23[8];
  uint8_t f24[8];
  uint8_t f25[8];
  uint8_t f26[8];
  uint8_t f27[8];
  uint8_t f28[8];
  uint8_t f29[8];
  uint8_t f30[8];
  uint8_t f31[8];
  uint8_t f32[8];
  uint8_t f33[8];
  uint8_t f34[8];
  uint8_t f35[8];
  uint8_t f36[8];
  uint8_t f37[8];
  uint8_t f38[8];
  uint8_t f39[8];
  uint8_t f40[8];
  uint8_t f41[8];
  uint8_t f42[8];
  uint8_t f43[8];
  uint8_t f44[8];
  uint8_t f45[8];
  uint8_t f46[8];
  uint8_t f47[8];
  uint8_t f48[8];
  uint8_t f49[8];
  uint8_t f50[8];
  uint8_t f51[8];
  uint8_t f52[8];
  uint8_t f53[8];
  uint8_t f54[8];
  uint8_t f55[8];
  uint8_t f56[8];
  uint8_t f57[8];
  uint8_t f58[8];
  uint8_t f59[8];
  uint8_t f60[8];
  uint8_t f61[8];
  uint8_t f62[8];
  uint8_t f63[8];
  uint8_t f64[8];
} __attribute__((packed));
// IMDB row (tuple) format:
// The length of the tuple is 128 bytes, or 16 columns.
// There are 16 fields in one tuple, and each field is 8 bytes.
struct IMDBRow{
  uint8_t f1[8];
  uint8_t f2[8];
  uint8_t f3[8];
  uint8_t f4[8];
  uint8_t f5[8];
  uint8_t f6[8];
  uint8_t f7[8];
  uint8_t f8[8];
  uint8_t f9[8];
  uint8_t f10[8];
  uint8_t f11[8];
  uint8_t f12[8];
  uint8_t f13[8];
  uint8_t f14[8];
  uint8_t f15[8];
  uint8_t f16[8];
} __attribute__((packed));


enum {
   kSuccess = 0,
   kFailure = -1,
};

// Row address format:
// +------------+---------+------+------------+------------+-------------+
// | HighRow(5) | Bank(3) | C(1) | LowRow(10) | Column(10) | IntraBus(3) |
// +------------+---------+------+------------+------------+-------------+
//
// Column address format:
// +------------+---------+------+------------+------------+-------------+
// | HighRow(5) | Bank(3) | C(1) | Column(10) | LowRow(10) | IntraBus(3) |
// +------------+---------+------+------------+------------+-------------+
struct Address {
  int highrow;
  int bank;
  int channel;
  int lowrow;
  int column;
  int intrabus;
};

const int kHighRowBit = 5;
const int kBankBit = 3;
const int kChannelBit = 1;
const int kLowRowBit = 10;
const int kColumnBit = 10;
const int kIntraBusBit = 3;

const int kCachelineSize = 64;
const int kRowBufferSize = 1024;
const int kColumnBufferSize = 1024;
const int kCellSize = 8;

// Read two columns
int GenRowTrace_1(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_row_addr);
int GenRowTrace_2(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_row_addr);
// Write two columns
int GenRowTrace_3(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_row_addr);
int GenRowTrace_4(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_row_addr);
// Read all
int GenRowTrace_5(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_row_addr);
int GenRowTrace_6(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_row_addr);
// Write trace
int GenRowTrace_7(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_row_addr);
int GenRowTrace_8(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_row_addr);

// Read two columns
int GenColTrace_1(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr);
int GenColTrace_2(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr);
// Write two columns
int GenColTrace_3(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr);
int GenColTrace_4(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr);

//int GenHybridTrace_1(const char* trace_file_name, int trace_total,
//                     uint32_t base_addr);


int GenHybTrace_1(const char* trace_file_name, int num_tuples, int tuple_size,
			                  uint32_t base_col_addr); 
int GenHybTrace_2(const char* trace_file_name, int num_tuples, int tuple_size,
                        uint32_t base_col_addr); 
int GenHybTrace_3(const char* trace_file_name, int num_tuples, int tuple_size,
                        uint32_t base_col_addr); 
int GenHybTrace_4(const char* trace_file_name, int num_tuples, int tuple_size,
                        uint32_t base_col_addr); 
int GenHybTrace_5(const char* trace_file_name, int num_tuples, int tuple_size,
                        uint32_t base_col_addr); 
int GenHybTrace_6(const char* trace_file_name, int num_tuples, int tuple_size,
                        uint32_t base_col_addr); 
int GenHybTrace_7(const char* trace_file_name, int num_tuples, int tuple_size,
                        uint32_t base_col_addr_a, uint32_t base_col_addr_b); 
int GenHybTrace_8(const char* trace_file_name, int num_tuples, int tuple_size,
                        uint32_t base_col_addr_a, uint32_t base_col_addr_b); 
int GenHybTrace_9(const char* trace_file_name, int num_tuples, int tuple_size,
			uint32_t base_col_addr); 
int GenHybTrace_10(const char* trace_file_name, int num_tuples, int tuple_size,
			uint32_t base_col_addr); 
int GenHybTrace_11(const char* trace_file_name, int num_tuples, int tuple_size,
			uint32_t base_col_addr,int L3_size); 
int GenHybTrace_12(const char* trace_file_name, int num_query, int r_ratio, int w_ratio, int cr_ratio, const char* random_file, const char* description);
int GenHybTrace_13(const char* trace_file_name, int num_tuples, int tuple_size,
			uint32_t base_col_addr, int prefetch_size); 
int GenHybTrace_14(const char* trace_file_name, int num_tuples, int tuple_size,
			uint32_t base_col_addr, int prefetch_size); 
int GenHybTrace_15(const char* trace_file_name, int num_tuples, int tuple_size,
			uint32_t base_col_addr, int prefetch_size); 
int GenHybTrace_16(const char* trace_file_name, int num_tuples, int tuple_size,
			uint32_t base_col_addr); 
int GenHybTrace_17(const char* trace_file_name, int num_tuples, int tuple_size,
			uint32_t base_col_addr); 
int GenHybTrace_18(const char* trace_file_name, int num_tuples, int tuple_size,
			uint32_t base_col_addr, int prefetch_size); 
int GenHybTrace_19(const char* trace_file_name, int num_tuples, int tuple_size,
			uint32_t base_col_addr, int prefetch_size); 
int GenHybTrace_20(const char* trace_file_name, int num_tuples, int tuple_size,
			uint32_t base_col_addr, int row_rate,int col_rate); 
int GenHybTrace_21(const char* trace_file_name, int num_tuples, int tuple_size,
			uint32_t base_col_addr, int row_rate,int col_rate); 
int GenHybTrace_22(const char* trace_file_name, int num_tuples, int tuple_size,
                        uint32_t base_col_addr); 
int GenHybTrace_23(const char* trace_file_name, int num_tuples, int tuple_size,
                        uint32_t base_col_addr); 
int GenHybTrace_24(const char* trace_file_name, int num_tuples, int tuple_size,
                        uint32_t base_col_addr); 

int GenDRAMTrace_1(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr); 
int GenDRAMTrace_2(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr); 
int GenDRAMTrace_3(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr); 
int GenDRAMTrace_4(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr); 
int GenDRAMTrace_5(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr); 
int GenDRAMTrace_6(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr); 
int GenDRAMTrace_7(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr_a, uint32_t base_col_addr_b); 
int GenDRAMTrace_8(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr_a, uint32_t base_col_addr_b); 
int GenDRAMTrace_9(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr); 
int GenDRAMTrace_10(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr); 
int GenDRAMTrace_11(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr, int L3_size); 
int GenDRAMTrace_12(const char* trace_file_name, int num_query, int r_ratio, int w_ratio, int cr_ratio, const char* random_file, const char* description);
int GenDRAMTrace_13(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr_a, int prefetch_size); 
int GenDRAMTrace_14(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr_a, int prefetch_size); 
int GenDRAMTrace_15(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr_a, int prefetch_size); 
int GenDRAMTrace_16(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr_a, int prefetch_size); 
int GenDRAMTrace_17(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr_a, int prefetch_size); 
int GenDRAMTrace_18(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr_a, int prefetch_size); 
int GenDRAMTrace_19(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr_a, int prefetch_size); 
int GenDRAMTrace_20(const char* trace_file_name, int num_tuples, int tuple_size,
			uint32_t base_col_addr, int row_rate,int col_rate); 
int GenDRAMTrace_21(const char* trace_file_name, int num_tuples, int tuple_size,
			uint32_t base_col_addr, int row_rate,int col_rate); 
int GenDRAMTrace_22(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr); 
int GenDRAMTrace_23(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr); 
int GenDRAMTrace_24(const char* trace_file_name, int num_tuples, int tuple_size,
                  uint32_t base_col_addr); 

// TODO: Add functions use number of tuples as an input.

__attribute_used__ static inline uint64_t gettimestamp_ns() {
  uint64_t val;
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  val = ts.tv_sec;
  val *= 1000000000;
  val += ts.tv_nsec;
  return val;
}

// Generate random pattern
int random_pattern(int num_query, int r_ratio, int w_ratio, int cr_ratio, const char* random_file);

// Cache print
void dprintf(FILE* fp, const char* format, int n, const char* op, unsigned addr,
            int highrow, int bank, int channel, int lowrow, int column,
            int intrabus);

void cprintf(FILE* fp, const char* format, int n,
		const char* op, unsigned addr);

#endif /* GEN_TRACE_H_ */
