#ifndef Z80A_COMMANDS_H_
#define Z80A_COMMANDS_H_ 100

#include <WString.h>
#include "CPUMemoryLoader.h"
#include "Config.h"

#if defined(CPU_IO_MOCKED)
  #include "MockIO.h"
#endif

enum {
  CMD_ST_READY,
  CMD_ST_WAIT,
  CMD_ST_EXEC
};

enum {
    // CLI Commands
    CMD_NONE,
    CMD_INVALID,
    CMD_HELP, // Print help
    CMD_CHECKSUM, // Calculate checksum
    CMD_COMPARE, // Calculate checksum
    CMD_DUMP, // Dump memory, show block
    CMD_ERASE, // Erase memory, set value 0
    CMD_FILL,  // Fill block with value
    CMD_POKE, // Set memory cells up to 32 values
    CMD_SEARCH, // Search bit in memory
    CMD_TRANSFER, // Move block of code in memory
    CMD_READ, // Read from board memory and save to XMODEM CRC file
    CMD_WRITE, // Write to board memory from XMODEM CRC file
    CMD_WRITE_HEX, // Write data from Intel HEX file

    CMD_FREQ, // Set frequency

    CMD_IO_SHOW_OUT, // O Show output cells
    CMD_IO_SET,  // Set input buffer

    CMD_INFO, // Show config info
    CMD_MODE, // Set command mode (_H_EX | _O_CTAL). default HEX

    CMD_ZAP,
    CMD_LAST_STATUS
};

class Commands {
  public:
  Commands();
  void init(MemoryLoader *);
  void setCommandLine(String *);
  int8_t parseCommand();
  void execute(int8_t);
  void printByte(uint16_t addr);
  void printBlock(uint16_t, uint16_t);
  void pokeBytes();
  void dumpMemory();
  void printHelp();
  void printInfo();
  void printInvalid();
  void fill();
  void erase();
  #if defined(CPU_IO_MOCKED)
    void setIO(MockIO *);
    void setInputs();
    void showOutPorts();
  #endif
  private:
  MemoryLoader * memory;
  String * cmdLine;
  int8_t currentIndex = 0;
  uint8_t currentCMD = CMD_NONE;
  uint8_t getByte(uint8_t);
  uint16_t getWord(uint8_t);
  uint32_t getDWord(uint8_t);
  int8_t nextNonSpace(int8_t);
  void fillMemory(uint16_t, uint16_t, uint8_t);
  uint8_t parseHex(char);
  #if defined(CPU_IO_MOCKED)
    MockIO * io;
    void fillInput(uint8_t);
    void fillOutput(uint8_t);
  #endif
};

// Read a line of data from the serial connection.
char * readLine(char * buffer, int len);

uint8_t readCommandLine(String * commandBuffer, int len);

#endif