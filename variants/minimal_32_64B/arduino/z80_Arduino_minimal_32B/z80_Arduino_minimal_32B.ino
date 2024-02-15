// Z80+Arduino 
// Minimal 32B
//

#include "CPUMemoryLoader.h"
#include "ROM_Data.h"
#include "CPU_Board.h"

uint8_t DEBUG = 1;

CPUBoard cpu;
MemoryLoader memory(16);

uint8_t read_status = 0;
uint8_t write_status = 0;
uint8_t out_status = 0;
uint8_t in_status = 0;

void debug_print(String, String, uint16_t, uint8_t, uint8_t);

void setup() {
  cpu.init();
  cpu.setClock(100);
  memory.init(&cpu);
  Serial.begin(9600);
}

void loop()
{
  unsigned long currentMillis = millis();
  cpu.tick(currentMillis);
  
  // Read control lines
  uint8_t rd, wr, mreq, iorq, m1;
  mreq = digitalRead(IN_MREQ);
  rd = digitalRead(IN_RD);
  wr = digitalRead(IN_WR);
  iorq = digitalRead(IN_IORQ);
  m1 = digitalRead(IN_M1);

  // Read address lines
  uint8_t addr = cpu.getAddr();
  
  // CPU Fetch Data. Active signas are LOW
  if(read_status == 0 && mreq == LOW && rd == LOW) {
	uint8_t data = memory.getByte(addr);
  	cpu.setData(data);
    read_status = 1;
    debug_print("MREQ", "RD", addr, data, m1);
  } else if (read_status == 1 && (mreq == HIGH || rd == HIGH)) {
  	read_status = 0;
    cpu.setAsDataInput();
  }

  if(write_status == 0 && mreq == LOW && wr == LOW) {
	uint8_t data = cpu.getData();
	memory.setByte(addr, data);
    write_status = 1;
    debug_print("MREQ", "WR", addr, data, m1);
  } else if (write_status == 1 && (mreq == HIGH || wr == HIGH)) {
  	write_status = 0;
    cpu.setAsDataInput();
  }
  
  // CPU send data to Port. Active signas are LOW
    if(out_status==0 && iorq == LOW && rd == LOW) {
    uint8_t data = 0xAA;
  	cpu.setData(data);
  	out_status = 1;
  	debug_print("IORQ", "RD", addr, data, m1);
  } else if (out_status==1 && (iorq == HIGH || rd == HIGH)) {
  	out_status = 0;
  }
  
  if(in_status==0 && iorq == LOW && wr == LOW) {
    in_status = 1;
    uint8_t data = cpu.getData();
    debug_print("IORQ", "WR", addr, data, m1);
  } else if (in_status==1 && (iorq == HIGH || wr == HIGH)) {
  	in_status = 0;
  }
}

/**
 * Print debug information
 * 
 * String area - Show Is it Memory (MREQ) or IO port (IORQ)
 * String operation - RD - read, WR - write
 * uint16_t addr - Physical 16bit address
 * uint8_t data - 8bit data
 * uint_8t m1 - State of M1
 */  
void debug_print(String area, String operation, uint16_t addr, uint8_t data, uint8_t m1) {
    if(DEBUG) {
	  Serial.flush();
	  Serial.print(area);
      Serial.print('\t');
      Serial.print(operation);
      Serial.print('\t');
      Serial.print(addr,  HEX);
      Serial.print('\t');
      Serial.print(data);
      Serial.print('\t');
      Serial.print("M1:");
      Serial.println(m1);
	}
}
