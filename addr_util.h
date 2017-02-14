/*
 * addr_util.h
 */

#ifndef ADDR_UTIL_H_
#define ADDR_UTIL_H_

#include "gen_trace.h"

// Aligns the supplied size to the specified PowerOfTwo
#define ALIGN_SIZE( sizeToAlign, PowerOfTwo )       \
        (((sizeToAlign) + (PowerOfTwo) - 1) & ~((PowerOfTwo) - 1))

// Checks whether the supplied size is aligned to the specified PowerOfTwo
#define IS_SIZE_ALIGNED( sizeToTest, PowerOfTwo )  \
        (((sizeToTest) & ((PowerOfTwo) - 1)) == 0)

//comparison success rate, x of 10
#define THRESHOLD 8
#define THRESHOLD_LOW 2

uint32_t GenRowAddr(int high_row, int low_row, int column, int bank,
                    int channel, int intra_bus);
uint32_t GenRowAddr(int high_row, int low_row, int column);
uint32_t GenRowAddr(Address addr);
uint32_t GenColAddr(int high_row, int low_row, int column, int bank,
                       int channel, int intra_bus);
uint32_t GenColAddr(int high_row, int low_row, int column);
uint32_t GenColAddr(Address addr);
uint32_t ColAddr2RowAddr(uint32_t col_addr);
uint32_t RowAddr2ColAddr(uint32_t row_addr);
int ParsePrintRowAddr(uint32_t row_addr);
int ParsePrintColAddr(uint32_t col_addr);
int ParseRowAddr(uint32_t row_addr, Address& result);
int ParseColAddr(uint32_t col_addr, Address& result);

uint32_t CalTupleRowAddrLayout1(int n, int tuple_size, uint32_t base_addr);
uint32_t CalTupleColAddrLayout2(int n, int tuple_size, uint32_t base_addr);

uint32_t GetCacheLineAddr(uint32_t addr);

inline
uint32_t GenRowAddr(int high_row, int low_row, int column) {
  return GenRowAddr(high_row, low_row, column, 0, 0, 0);
}

inline
uint32_t GenColAddr(int high_row, int low_row, int column) {
  return GenColAddr(high_row, low_row, column, 0, 0, 0);
}

inline
uint32_t GenRowAddr(Address addr) {
  return GenRowAddr(addr.highrow, addr.lowrow, addr.column, addr.bank,
                    addr.channel, addr.intrabus);
}

inline
uint32_t GenColAddr(Address addr) {
  return GenColAddr(addr.highrow, addr.lowrow, addr.column, addr.bank,
                    addr.channel, addr.intrabus);
}


inline
uint32_t GenRowAddr(int high_row, int low_row, int column, int bank,
                    int channel, int intra_bus) {
  // TODO: validation of arguments
  uint32_t temp_addr = high_row;
  temp_addr = (temp_addr << kBankBit) | bank;
  temp_addr = (temp_addr << kChannelBit) | channel;
  temp_addr = (temp_addr << kLowRowBit) | low_row;
  temp_addr = (temp_addr << kColumnBit) | column;
  temp_addr = (temp_addr << kIntraBusBit) | intra_bus;
  return temp_addr;
}

inline
uint32_t GenColAddr(int high_row, int low_row, int column, int bank,
                    int channel, int intra_bus) {
  // TODO: validation of arguments
  uint32_t temp_addr = high_row;
  temp_addr = (temp_addr << kBankBit) | bank;
  temp_addr = (temp_addr << kChannelBit) | channel;
  temp_addr = (temp_addr << kColumnBit) | column;
  temp_addr = (temp_addr << kLowRowBit) | low_row;
  temp_addr = (temp_addr << kIntraBusBit) | intra_bus;
  return temp_addr;
}

inline uint32_t GetCacheLineAddr(uint32_t addr) {
  return (addr & (~(kCachelineSize - 1)));
}


#endif /* ADDR_UTIL_H_ */
