#ifndef CPU_MEMORY_LOADER_H_
#define CPU_MEMORY_LOADER_H_ 1

#include "CPU_Board.h"

struct MemoryChunk {
  uint16_t startAddress;
  uint16_t size;
  const uint8_t *memChunk; 
};

class MemoryLoader {
  public:
    MemoryLoader(uint16_t maxAddr);
    void init(CPUBoard * board);
    uint8_t getByte(uint16_t addr);
    uint8_t setByte(uint16_t addr, uint8_t data);
    void printByte(uint16_t addr);
    void printBlock(uint16_t addr, uint16_t numByte, uint8_t base);
    uint8_t setBlock(uint16_t addr, uint8_t* buffer, uint16_t size, bool pgmem = 0);
    void loadChunk(MemoryChunk);
  private:
    uint16_t _maxAddr;
    CPUBoard * _board;
    uint8_t checkAddr(uint16_t addr);
};

#endif