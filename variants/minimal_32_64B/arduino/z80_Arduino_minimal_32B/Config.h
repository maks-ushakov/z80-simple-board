#ifndef Z80A_CONFIG_H_
#define Z80A_CONFIG_H_ 100

#define CPU_BOARD_VERSION "0.1.10"
#define CPU_NAME "z80"


// memory size in bytes
#define MEMORY_SIZE 32

// use Main controller as memory device
// Comment this out for using extermal memory
#define CPU_MEMORY_MOCKED 1
#define MEMORY_LINES 5

// Uncommet next lines if you use multiplexer for adress lines
// #define MEMORY_MOCK_MUX

// Uncoment  one of followed
// #define MEMORY_MOCK_EXTRA_256
// #define MEMORY_MOCK_EXTRA_512
// #define MEMORY_MOCK_EXTRA_1024

// Use Main Controller as IO device
// Comment this out for using extermal IO
#define CPU_IO_MOCKED
#define CPU_IO_NUM_PORTS 5

// Uncomment this out if you not need reaction to CPU MREQ+WR signals
// #define CPU_ROM_ONLY

// Uncomment this out if you want to load code on startup
// #define MEM_AUTOLOAD_ON // Autoload memory

#if defined(MEM_AUTOLOAD_ON)
#include "ROM_Data.h"
#endif


#endif