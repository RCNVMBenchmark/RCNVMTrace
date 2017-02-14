/*
 * gen_trace.cc
 *
 * This program generates traces of row-column access of a 2D-table
 * for NVMain simulator.
 */

#include "gen_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include "addr_util.h"

// Cache print
void dprintf(FILE* fp, const char* format, int n, const char* op, unsigned addr_,
            int highrow, int bank, int channel, int lowrow, int column,
            int intrabus) {
    int i;
    Address addr;
    if (strcmp(op, "W") == 0 || strcmp(op, "R") == 0) {
      for (i = 0; i < 8; i ++) {
        ParseRowAddr(addr_ + 8*i, addr);
        fprintf(fp, format, n, op,
                addr_ + 8*i, addr.highrow, addr.bank,
                addr.channel, addr.lowrow, addr.column, addr.intrabus);
      }
    } else {
      for (i = 0; i < 8; i ++) {
        ParseColAddr(addr_ + 8*i, addr);
        fprintf(fp, format, n, op,
                addr_ + 8*i, addr.highrow, addr.bank,
                addr.channel, addr.lowrow, addr.column, addr.intrabus);
    }
  }
}
void cprintf(FILE* fp, const char* format, int n,
		const char* op, unsigned addr) {
  int i = 0;
  for (i = 0; i < 8; i ++){
    fprintf(fp, format, n*8+i, op, addr + 8*i);
  }
}


int main(int argc, char *argv[]) {
  // Generates row trace, column trace, or hybrid trace
  // according to the command line options.
  if (argc == 1) {
    fprintf(stderr, "No arguments!\n");
    exit(EXIT_FAILURE);
  }

  printf("Column offset: f1 = %zu, f2 = %zu, f3 = %zu, f4 = %zu\n"
         "               f5 = %zu, f6 = %zu, f7 = %zu, f8 = %zu\n"
         "               f9 = %zu, f10 = %zu, f11 = %zu, f12 = %zu\n"
         "               f13 = %zu, f14 = %zu, f15 = %zu, f16 = %zu\n",
         offsetof(IMDBRow, f1), offsetof(IMDBRow, f2), offsetof(IMDBRow, f3),
         offsetof(IMDBRow, f4), offsetof(IMDBRow, f5), offsetof(IMDBRow, f6),
         offsetof(IMDBRow, f7), offsetof(IMDBRow, f8), offsetof(IMDBRow, f9),
         offsetof(IMDBRow, f10), offsetof(IMDBRow, f11), offsetof(IMDBRow, f12),
         offsetof(IMDBRow, f13), offsetof(IMDBRow, f14), offsetof(IMDBRow, f15),
         offsetof(IMDBRow, f16));
  printf("Length of an IMDB row: %zu\n", sizeof(IMDBRow));

  int num_tuples = 500000;

  if (argv[1][0] == 'r') { // generate row trace
    printf("Generate row sequential read trace\n");
    GenRowTrace_1("row-part-read-layout1.trace", num_tuples, sizeof(IMDBRow), 0x0u);
    GenRowTrace_2("row-part-read-layout2.trace", num_tuples, sizeof(IMDBRow), 0x0u);
    GenRowTrace_3("row-part-write-layout1.trace", num_tuples, sizeof(IMDBRow), 0x0u);
    GenRowTrace_4("row-part-write-layout2.trace", num_tuples, sizeof(IMDBRow), 0x0u);
    GenRowTrace_5("row-seq-read-layout1.trace", num_tuples, sizeof(IMDBRow), 0x0u);
    GenRowTrace_6("row-seq-read-layout2.trace", num_tuples, sizeof(IMDBRow), 0x0u);
    GenRowTrace_7("row-seq-write-layout1.trace", num_tuples, sizeof(IMDBRow), 0x0u);
    GenRowTrace_8("row-seq-write-layout2.trace", num_tuples, sizeof(IMDBRow), 0x0u);
  } else if (argv[1][0] == 'c') { // generate column trace
    printf("Generate column sequential trace for f3 and f10 columns:\n");
    GenColTrace_1("col-part-read-layout1.trace", num_tuples, sizeof(IMDBRow), 0x0u);
    GenColTrace_2("col-part-read-layout2.trace", num_tuples, sizeof(IMDBRow), 0x0u);
    GenColTrace_3("col-part-write-layout1.trace", num_tuples, sizeof(IMDBRow), 0x0u);
    GenColTrace_4("col-part-write-layout2.trace", num_tuples, sizeof(IMDBRow), 0x0u);
  } else if (argv[1][0] == 'h') { // generate hybrid trace
    printf("Generate hybrid query trace\n");
    GenHybTrace_1("hybrid-qurey1-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenHybTrace_2("hybrid-qurey2-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenHybTrace_3("hybrid-qurey12-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenHybTrace_4("hybrid-qurey3-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenHybTrace_5("hybrid-qurey5-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenHybTrace_6("hybrid-qurey13-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenHybTrace_7("hybrid-qurey8-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u, 0x8000000u);
    GenHybTrace_8("hybrid-qurey9-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u, 0x8000000u);
    GenHybTrace_9("hybrid-qurey10-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenHybTrace_10("hybrid-qurey11-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
//    GenHybTrace_11("hybrid-qurey11-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u,6);
//    GenHybTrace_12("hybrid-qurey12-layout2.trace", num_tuples, 25, 25, 25, "random_file.trace", "R25_W25_r25_w25");
   GenHybTrace_13("hybrid-qurey14-1-layout2.trace", num_tuples,sizeof(IMDBOverCLRow), 0x0u,1);
   GenHybTrace_13("hybrid-qurey14-32-layout2.trace", num_tuples,sizeof(IMDBOverCLRow), 0x0u,32);
   GenHybTrace_13("hybrid-qurey14-64-layout2.trace", num_tuples,sizeof(IMDBOverCLRow), 0x0u,64);
   GenHybTrace_13("hybrid-qurey14-96-layout2.trace", num_tuples,sizeof(IMDBOverCLRow), 0x0u,96);
   GenHybTrace_13("hybrid-qurey14-128-layout2.trace", num_tuples,sizeof(IMDBOverCLRow), 0x0u,128);
    GenHybTrace_14("hybrid-qurey15-1-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u,1);
    GenHybTrace_14("hybrid-qurey15-32-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u,32);
    GenHybTrace_14("hybrid-qurey15-64-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u,64);
    GenHybTrace_14("hybrid-qurey15-96-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u,96);
    GenHybTrace_14("hybrid-qurey15-128-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u,128);
//    GenHybTrace_15("hybrid-qurey15a-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u,1);
//    GenHybTrace_15("hybrid-qurey15b-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u,128);
//    GenHybTrace_16("hybrid-qurey16-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u);
//    GenHybTrace_17("hybrid-qurey17-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u);
//    GenHybTrace_18("hybrid-qurey18a-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,1);
//    GenHybTrace_18("hybrid-qurey18b-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,128);
//    GenHybTrace_19("hybrid-qurey19a-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,1);
//    GenHybTrace_19("hybrid-qurey19b-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,128);
//    GenHybTrace_20("hybrid-qurey20a-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,1,0);
//    GenHybTrace_20("hybrid-qurey20b-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,3,1);
//    GenHybTrace_20("hybrid-qurey20c-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,1,1);
//    GenHybTrace_20("hybrid-qurey20d-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,1,3);
//    GenHybTrace_20("hybrid-qurey20e-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,0,1);
//    GenHybTrace_21("hybrid-qurey21a-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,1,0);
//    GenHybTrace_21("hybrid-qurey21b-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,3,1);
//    GenHybTrace_21("hybrid-qurey21c-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,1,1);
//    GenHybTrace_21("hybrid-qurey21d-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,1,3);
//    GenHybTrace_21("hybrid-qurey21e-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,0,1);
    GenHybTrace_22("hybrid-qurey4-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenHybTrace_23("hybrid-qurey6-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenHybTrace_24("hybrid-qurey7-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
  } else if (argv[1][0] == 'd') { // generate hybrid trace
    printf("Generate DRAM query trace\n");
    GenDRAMTrace_1("DRAM-qurey1-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenDRAMTrace_2("DRAM-qurey2-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenDRAMTrace_3("DRAM-qurey12-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenDRAMTrace_4("DRAM-qurey3-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenDRAMTrace_5("DRAM-qurey5-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenDRAMTrace_6("DRAM-qurey13-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenDRAMTrace_7("DRAM-qurey8-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u, 0x8000000u);
    GenDRAMTrace_8("DRAM-qurey9-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u, 0x8000000u);
    GenDRAMTrace_9("DRAM-qurey10-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenDRAMTrace_10("DRAM-qurey11-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
//    GenDRAMTrace_11("DRAM-qurey11-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u, 6);
//    GenDRAMTrace_12("DRAM-qurey12-layout2.trace", num_tuples, 25, 25, 25, "random_file.trace", "R25_W25_r25_w25");
    GenDRAMTrace_13("DRAM-qurey14-layout2.trace", num_tuples,sizeof(IMDBOverCLRow), 0x0u, 128);
    GenDRAMTrace_14("DRAM-qurey15-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u, 128);
//    GenDRAMTrace_15("DRAM-qurey15-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u, 128);
//    GenDRAMTrace_16("DRAM-qurey16-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u, 128);
//    GenDRAMTrace_17("DRAM-qurey17-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u, 128);
//    GenDRAMTrace_18("DRAM-qurey18-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u, 128);
//    GenDRAMTrace_19("DRAM-qurey19-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u, 128);
//    GenDRAMTrace_20("DRAM-qurey20a-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,1,0);
//    GenDRAMTrace_20("DRAM-qurey20b-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,3,1);
//    GenDRAMTrace_20("DRAM-qurey20c-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,1,1);
//    GenDRAMTrace_20("DRAM-qurey20d-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,1,3);
//    GenDRAMTrace_21("DRAM-qurey21a-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,1,0);
//    GenDRAMTrace_21("DRAM-qurey21b-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,3,1);
//    GenDRAMTrace_21("DRAM-qurey21c-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,1,1);
//    GenDRAMTrace_21("DRAM-qurey21d-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,1,3);
//    GenDRAMTrace_21("DRAM-qurey21e-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,0,1);
//    GenDRAMTrace_20("DRAM-qurey20e-layout2.trace", num_tuples,sizeof(IMDB_Large), 0x0u,0,1);
    GenDRAMTrace_22("DRAM-qurey4-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenDRAMTrace_23("DRAM-qurey6-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
    GenDRAMTrace_24("DRAM-qurey7-layout2.trace", num_tuples,sizeof(IMDBRow), 0x0u);
  } else if (argv[1][0] == 'm'){ // random pattern
//    // R15_W15_r35_w35
//    GenHybTrace_12("hybrid-qurey12_R15_W15_r35_w35-layout2.trace", num_tuples, 15, 15, 35, "random_R15_W15_r35_w35.trace", "R15_W15_r35_w35");
//    GenDRAMTrace_12("DRAM-qurey12_R15_W15_r35_w35-layout2.trace", num_tuples, 15, 15, 35, "random_R15_W15_r35_w35.trace", "R15_W15_r35_w35");
//    // R35_W35_r15_w15
//    GenHybTrace_12("hybrid-qurey12_R35_W35_r15_w15-layout2.trace", num_tuples, 35, 35, 15, "random_R35_W35_r15_w15.trace", "R35_W35_r15_w15");
//    GenDRAMTrace_12("DRAM-qurey12_R35_W35_r15_w15-layout2.trace", num_tuples, 35, 35, 15, "random_R35_W35_r15_w15.trace", "R35_W35_r15_w15");
//    // R35_W15_r35_w15
//    GenHybTrace_12("hybrid-qurey12_R35_W15_r35_w15-layout2.trace", num_tuples, 35, 15, 35, "random_R35_W15_r35_w15.trace", "R35_W15_r35_w15");
//    GenDRAMTrace_12("DRAM-qurey12_R35_W15_r35_w15-layout2.trace", num_tuples, 35, 15, 35, "random_R35_W15_r35_w15.trace", "R35_W15_r35_w15");
//    // R15_W35_r15_w35
//    GenHybTrace_12("hybrid-qurey12_R15_W35_r15_w35-layout2.trace", num_tuples, 15, 35, 15, "random_R15_W35_r15_w35.trace", "R15_W35_r15_w35");
//    GenDRAMTrace_12("DRAM-qurey12_R15_W35_r15_w35-layout2.trace", num_tuples, 15, 35, 15, "random_R15_W35_r15_w35.trace", "R15_W35_r15_w35");

//    GenHybTrace_12("hybrid-qurey12_R100_W0_r0_w0-layout2.trace", num_tuples, 100, 0, 0, "random_R100_W0_r0_w0.trace", "R100_W0_r0_w0");
//    GenDRAMTrace_12("DRAM-qurey12_R100_W0_r0_w0-layout2.trace", num_tuples, 100, 0, 0, "random_R100_W0_r0_w0.trace", "R100_W0_r0_w0");
//    
//    GenHybTrace_12("hybrid-qurey12_R75_W0_r25_w0-layout2.trace", num_tuples, 75, 0, 25, "random_R75_W0_r25_w0.trace", "R75_W0_r25_w0");
//    GenDRAMTrace_12("DRAM-qurey12_R75_W0_r25_w0-layout2.trace", num_tuples, 75, 0, 25, "random_R75_W0_r25_w0.trace", "R75_W0_r25_w0");
//    
//    GenHybTrace_12("hybrid-qurey12_R50_W0_r50_w0-layout2.trace", num_tuples, 50, 0, 50, "random_R50_W0_r50_w0.trace", "R50_W0_r50_w0");
//    GenDRAMTrace_12("DRAM-qurey12_R50_W0_r50_w0-layout2.trace", num_tuples, 50, 0, 50, "random_R50_W0_r50_w0.trace", "R50_W0_r50_w0");
//    
//    GenHybTrace_12("hybrid-qurey12_R25_W0_r75_w0-layout2.trace", num_tuples, 25, 0, 75, "random_R25_W0_r75_w0.trace", "R25_W0_r75_w0");
//    GenDRAMTrace_12("DRAM-qurey12_R25_W0_r75_w0-layout2.trace", num_tuples, 25, 0, 75, "random_R25_W0_r75_w0.trace", "R25_W0_r75_w0");
//    
//    GenHybTrace_12("hybrid-qurey12_R0_W0_r100_w0-layout2.trace", num_tuples, 0, 0, 100, "random_R0_W0_r100_w0.trace", "R0_W0_r100_w0");
//    GenDRAMTrace_12("DRAM-qurey12_R0_W0_r100_w0-layout2.trace", num_tuples, 0, 0, 100, "random_R0_W0_r100_w0.trace", "R0_W0_r100_w0");
//    
//    GenHybTrace_12("hybrid-qurey12_R0_W100_r0_w0-layout2.trace", num_tuples, 0, 100, 0, "random_R0_W100_r0_w0.trace", "R0_W100_r0_w0");
//    GenDRAMTrace_12("DRAM-qurey12_R0_W100_r0_w0-layout2.trace", num_tuples, 0, 100, 0, "random_R0_W100_r0_w0.trace", "R0_W100_r0_w0");
//    
//    GenHybTrace_12("hybrid-qurey12_R0_W75_r0_w25-layout2.trace", num_tuples, 0, 75, 0, "random_R0_W75_r0_w25.trace", "R0_W75_r0_w25");
//    GenDRAMTrace_12("DRAM-qurey12_R0_W75_r0_w25-layout2.trace", num_tuples, 0, 75, 0, "random_R0_W75_r0_w25.trace", "R0_W75_r0_w25");
//    
//    GenHybTrace_12("hybrid-qurey12_R0_W50_r0_w50-layout2.trace", num_tuples, 0, 50, 0, "random_R0_W50_r0_w50.trace", "R0_W50_r0_w50");
//    GenDRAMTrace_12("DRAM-qurey12_R0_W50_r0_w50-layout2.trace", num_tuples, 0, 50, 0, "random_R0_W50_r0_w50.trace", "R0_W50_r0_w50");
//    
//    GenHybTrace_12("hybrid-qurey12_R0_W25_r0_w75-layout2.trace", num_tuples, 0, 25, 0, "random_R0_W25_r0_w75.trace", "R0_W25_r0_w75");
//    GenDRAMTrace_12("DRAM-qurey12_R0_W25_r0_w75-layout2.trace", num_tuples, 0, 25, 0, "random_R0_W25_r0_w75.trace", "R0_W25_r0_w75");
//    
//    GenHybTrace_12("hybrid-qurey12_R0_W0_r0_w100-layout2.trace", num_tuples, 0, 0, 0, "random_R0_W0_r0_w100.trace", "R0_W0_r0_w100");
//    GenDRAMTrace_12("DRAM-qurey12_R0_W0_r0_w100-layout2.trace", num_tuples, 0, 0, 0, "random_R0_W0_r0_w100.trace", "R0_W0_r0_w100");
//    
  } else if (argv[1][0] == 't') { // test functions
    ParsePrintColAddr(0xDEADBEEF);
    uint32_t temp_col_addr = ColAddr2RowAddr(0xDEADBEEF);
    printf("Covert column address 0xDEADBEEF to row address: 0x%08x\n",
           temp_col_addr);
    ParsePrintRowAddr(temp_col_addr);
    uint32_t temp_row_addr = RowAddr2ColAddr(temp_col_addr);
    printf("Covert row address back to column address: 0x%08x\n",
           temp_row_addr);
    assert(0xDEADBEEF == temp_row_addr);
  }
  return 0;
}


