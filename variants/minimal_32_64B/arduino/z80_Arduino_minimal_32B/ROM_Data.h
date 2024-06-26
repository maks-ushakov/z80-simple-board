#ifndef CPU_ROM_DATA_H_
#define CPU_ROM_DATA_H_ 1

#include "CPUMemoryLoader.h"

const uint8_t _wr_test_code[] PROGMEM = {
  	0x3e, 0x64, 0x06, 0x17, 0x90, 0x0e, 0x00, 0xed, 0x79, 0x76
};

const uint8_t _code_io[] PROGMEM = {
  	0x3e, 0x64, 0x06, 0x17, 0x90, 0x0e, 0x00, 0xed, 0x79, 0x76,
};

const MemoryChunk memoryChunks[] = {
  {0x0000, 10, _wr_test_code },
  {0x0010, 10, _code_io }
};
 const uint8_t NUM_CHUNKS = 2;


#endif

/*
// MEM WR TEST
uint8_t memory[] = {
  	0x3e, 0x64, 0x06, 0x17, 0x90, 0x0e, 0x00, 0xed, 0x79, 0x76,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
*/
/*
// MEM and IO TEST
uint8_t code[] = {
  	0x3e, 0x64, 0x06, 0x17, 0x90, 0x0e, 0x00, 0xed, 0x79, 0x76,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
*/
/*
// Add
uint8_t code[] = {
  	0x3e, 0x64, 0x06, 0x17, 0x90, 0x0e, 0x00, 0xed, 0x79, 0x76,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
*/
/*
// Sub
uint8_t code[] = {
  	0x3e, 0x64, 0x06, 0x17, 0x90, 0x0e, 0x00, 0xed, 0x79, 0x76,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
*/
