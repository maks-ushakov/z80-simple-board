/*
Simplest OS
*/
#include "ByteHandler.h"
#include "CPU.h"

#define NOT_PRESSED 0xFF

#define D_ADDR_H  0x00
#define D_ADDR_L  0x01
#define D_DATA    0x02
#define KBD_PORT  0x03

#define LAST_RAM  0x8000
#define FIRST_RAM 0x0C00

#define C_SETHB   0x10
#define C_SETLB   0x11
#define C_SETDAT  0x12
#define C_RUN     0x13

CPU cpu;
byte current_number;

class Addr {
  Addr();
  byte low;
  byte high;
  word get() {
    return wordFromBytes(low, high);
  };
  void set(word a) {
    low = lowByte(a);
    high = hightByte(a);
  };
  void inc() {increaseWord(low, high)};
};

Addr addr;

void setup() {
  cpu.init(LAST_RAM);
  addr.set(FIRST_RAM);
}

void loop() {
  // load data from address
  current_number = cpu.mem(addr.get());
  // Show data and address values
  cpu.print(D_ADDR_H, addr.high);
  cpu.print(D_ADDR_L, addr.low);
  cpu.print(D_DATA, current_number);

  while(true) {
    // get pressed key
    byte key = NOT_PRESSED;
    while (key == NOT_PRESSED) {
      key = skan_kbd();
    }
    // Process key code
    if (isDigitKey(key)) {
      byte temp = key;
      // Move current value to high nibble
      //current_number = current_number << 4;
      //current_number = current_number & 0xf0;
      current_number *= 0x0f;
      // add new value from key code;
      current_number = current_number | temp;
      cpu.print(D_DATA, current_number);
    }

    if (key == C_SETHB) {
      addr.high = current_number;
      break;
    }

    if (key == C_SETLB) {
      addr.low = current_number;
      break;
    }

    if (key == C_SETDAT) {
      cpu.mem[addr.get()] = current_number;
      addr.inc();
      break;
    }

    if (key == C_RUN) {
      cpu.exec(addr.get());
    }
  }
}
