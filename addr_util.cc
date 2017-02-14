/*
 * addr_util.cc
 */

#include "addr_util.h"
#include "gen_trace.h"
#include <stdio.h>
#include <assert.h>

uint32_t ColAddr2RowAddr(uint32_t col_addr) {
  Address addr = {};
  ParseColAddr(col_addr, addr);
  uint32_t temp_addr = GenRowAddr(addr);
  return temp_addr;
}

uint32_t RowAddr2ColAddr(uint32_t row_addr) {
  Address addr = {};
  ParseRowAddr(row_addr, addr);
  uint32_t temp_addr = GenColAddr(addr);
  return temp_addr;
}

int ParseRowAddr(uint32_t row_addr, Address& result) {
  uint32_t high_row_shift = kBankBit + kChannelBit + kLowRowBit + kColumnBit
      + kIntraBusBit;
  uint32_t bank_shift = high_row_shift - kBankBit;
  uint32_t channel_shift = bank_shift - kChannelBit;
  uint32_t low_row_shift = channel_shift - kLowRowBit;
  uint32_t column_shift = low_row_shift - kColumnBit;

  result.highrow = row_addr >> high_row_shift;
  result.bank = (row_addr >> bank_shift) & ((1 << kBankBit) - 1);
  result.channel = (row_addr >> channel_shift) & ((1 << kChannelBit) - 1);
  result.lowrow = (row_addr >> low_row_shift) & ((1 << kLowRowBit) - 1);
  result.column = (row_addr >> column_shift) & ((1 << kColumnBit) - 1);
  result.intrabus = row_addr & ((1 << kIntraBusBit) - 1);

  return kSuccess;
}

int ParseColAddr(uint32_t col_addr, Address& result) {
  uint32_t high_row_shift = kBankBit + kChannelBit + kColumnBit + kLowRowBit
      + kIntraBusBit;
  uint32_t bank_shift = high_row_shift - kBankBit;
  uint32_t channel_shift = bank_shift - kChannelBit;
  uint32_t column_shift = channel_shift - kColumnBit;
  uint32_t low_row_shift = column_shift - kLowRowBit;

  result.highrow = col_addr >> high_row_shift;
  result.bank = (col_addr >> bank_shift) & ((1 << kBankBit) - 1);
  result.channel = (col_addr >> channel_shift) & ((1 << kChannelBit) - 1);
  result.column = (col_addr >> column_shift) & ((1 << kColumnBit) - 1);
  result.lowrow = (col_addr >> low_row_shift) & ((1 << kLowRowBit) - 1);
  result.intrabus = col_addr & ((1 << kIntraBusBit) - 1);

  return kSuccess;
}


int ParsePrintRowAddr(uint32_t row_addr) {
  uint32_t high_row_shift = kBankBit + kChannelBit + kLowRowBit + kColumnBit
      + kIntraBusBit;
  uint32_t bank_shift = high_row_shift - kBankBit;
  uint32_t channel_shift = bank_shift - kChannelBit;
  uint32_t low_row_shift = channel_shift - kLowRowBit;
  uint32_t column_shift = low_row_shift - kColumnBit;
  assert(column_shift == (unsigned)kIntraBusBit);

  printf("Row address to be parsed: 0x%08x\n", row_addr);

  uint32_t high_row_bit = row_addr >> high_row_shift;
  printf("HighRow bits: 0x%x (%u)\n", high_row_bit, high_row_bit);
  uint32_t bank_bit = (row_addr >> bank_shift) & ((1 << kBankBit) - 1);
  printf("Bank bits: 0x%x (%u)\n", bank_bit, bank_bit);
  uint32_t channel_bit = (row_addr >> channel_shift) & ((1 << kChannelBit) - 1);
  printf("Channel bits: 0x%x (%u)\n", channel_bit, channel_bit);
  uint32_t low_row_bit = (row_addr >> low_row_shift) & ((1 << kLowRowBit) - 1);
  printf("LowRow bits: 0x%x (%u)\n", low_row_bit, low_row_bit);
  uint32_t column_bit = (row_addr >> column_shift) & ((1 << kColumnBit) - 1);
  printf("Column bits: 0x%x (%u)\n", column_bit, column_bit);
  uint32_t intra_bus_bit = row_addr & ((1 << kIntraBusBit) - 1);
  printf("IntraBus bits: 0x%x (%u)\n", intra_bus_bit, intra_bus_bit);

  return kSuccess;
}

int ParsePrintColAddr(uint32_t col_addr) {
  uint32_t high_row_shift = kBankBit + kChannelBit + kColumnBit + kLowRowBit
      + kIntraBusBit;
  uint32_t bank_shift = high_row_shift - kBankBit;
  uint32_t channel_shift = bank_shift - kChannelBit;
  uint32_t column_shift = channel_shift - kColumnBit;
  uint32_t low_row_shift = column_shift - kLowRowBit;
  assert(low_row_shift == (unsigned )kIntraBusBit);

  printf("Column address to be parsed: 0x%08x\n", col_addr);

  uint32_t high_row_bit = col_addr >> high_row_shift;
  printf("HighRow bits: 0x%x (%u)\n", high_row_bit, high_row_bit);
  uint32_t bank_bit = (col_addr >> bank_shift) & ((1 << kBankBit) - 1);
  printf("Bank bits: 0x%x (%u)\n", bank_bit, bank_bit);
  uint32_t channel_bit = (col_addr >> channel_shift) & ((1 << kChannelBit) - 1);
  printf("Channel bits: 0x%x (%u)\n", channel_bit, channel_bit);
  uint32_t column_bit = (col_addr >> column_shift) & ((1 << kColumnBit) - 1);
  printf("Column bits: 0x%x (%u)\n", column_bit, column_bit);
  uint32_t low_row_bit = (col_addr >> low_row_shift) & ((1 << kLowRowBit) - 1);
  printf("LowRow bits: 0x%x (%u)\n", low_row_bit, low_row_bit);
  uint32_t intra_bus_bit = col_addr & ((1 << kIntraBusBit) - 1);
  printf("IntraBus bits: 0x%x (%u)\n", intra_bus_bit, intra_bus_bit);

  return kSuccess;
}

uint32_t CalTupleRowAddrLayout1(int n, int tuple_size, uint32_t row_base_addr) {
  return (row_base_addr + (uint32_t)tuple_size * (uint32_t)n);
}
uint32_t CalTupleColAddrLayout2(int n, int tuple_size, uint32_t col_base_addr) {
  int column_span = n / kColumnBufferSize;
  int remain_tuple = n % kColumnBufferSize;
  int column_shift = kLowRowBit + kIntraBusBit;
  // Overflow, alignment?
  uint32_t column_offset = (uint32_t)((((tuple_size - 1) / kCellSize + 1)
      << column_shift) * column_span);
  return col_base_addr + column_offset + (remain_tuple << kIntraBusBit);
}
