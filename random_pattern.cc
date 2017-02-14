/*
 * random_pattern.cc
 */

#include <stdio.h>
#include <stdlib.h>
#include "gen_trace.h"
#include "addr_util.h"


#define POS_RANGE_POW 30

int random_pattern(int num_query, int r_ratio, int w_ratio, int cr_ratio, const char* random_file){
  int cw_ratio = 100 - r_ratio - w_ratio - cr_ratio;
  int r_total = r_ratio;
  int w_total = r_total + w_ratio;
  int cr_total = w_total + cr_ratio;
  int cw_total = cr_total + cw_ratio;
  int i;
  printf("Generating random access pattern\n");
  FILE *fp = fopen(random_file, "w");
  if (fp == NULL) return kFailure;

  for (i = 0; i < num_query; i ++){
    // generate operation
    int op = rand() % 100;
    if (op < r_total){
      op = 0;
    }
    else if (op < w_total){
      op = 1;
    }
    else if (op < cr_total){
      op = 2;
    }
    else{
      op = 3;
    }

    // generate position
    // temporary range is 0 ~ 0xfff
    unsigned long pos = 0;
    if (POS_RANGE_POW <= 12){
      pos = rand() & ((1 << POS_RANGE_POW) - 1);
    } else if (POS_RANGE_POW <= 24){
      pos = rand() & ((1 << (POS_RANGE_POW - 12)) - 1);
      pos = pos << 12;
      pos = pos | rand() & 0xfff;
    }
    else if (POS_RANGE_POW <= 32){
      pos = rand() & ((1 << (POS_RANGE_POW - 24)) - 1);
      pos = pos << 12;
      pos = pos | rand() & 0xfff;
      pos = pos << 12;
      pos = pos | rand() & 0xfff;
    }
    fprintf(fp, "%d %d\n", op, GetCacheLineAddr(pos));
  }
  fclose(fp);
  return kSuccess;
}

