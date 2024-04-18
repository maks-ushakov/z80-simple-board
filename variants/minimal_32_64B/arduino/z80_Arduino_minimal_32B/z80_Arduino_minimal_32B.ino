/**
 * Z80+Arduino. Main controller
 * Use for handle data on board memory
 * Autoboot (load data) after reset.
 * Read memory cells, read block, write values by hand or from files
 */
#include "Config.h"
#include "CPU_Board.h"
#include "CPUMemoryLoader.h"
#include "z80A_commands.h"

uint8_t DEBUG = 0;

bool showPrompt = true;
String prompt = "> ";

enum {
  CTRL_RUN,
  CTRL_BUSRQ,
  CTRL_BUS_USED,
};

#if defined(CPU_IO_MOCKED)
  #include "MockIO.h"
  uint8_t out_status = 0;
  uint8_t in_status = 0;
  MockIO mockio;
#endif

#if defined(CPU_MEMORY_MOCKED)
uint8_t read_status = 0;
uint8_t write_status = 0;
#endif

CPUBoard cpu;
MemoryLoader memory(MEMORY_SIZE);
Commands mon;
String commandBuffer = " ";

void debug_print(String, String, uint16_t, uint8_t, uint8_t);

void setup() {
  cpu.init();
  cpu.setClock(4);
  memory.init(&cpu);
  mon.init(&memory);
  #if defined(CPU_IO_MOCKED)
    mockio.init();
    mon.setIO(&mockio);
  #endif
  #if defined(MEM_AUTOLOAD_ON)
    for (uint16_t i = 0; i < NUM_CHUNKS; i++) {
      memory.loadChunk(memoryChunks[i]);
    }
  #endif
  Serial.begin(9600);
  Serial.print(F("CPU Board "));
  Serial.print(CPU_NAME);
  Serial.print("+Arduino v");
  Serial.println(CPU_BOARD_VERSION);
  // Serial.flush();
}

void loop()
{
  unsigned long currentMillis = millis();
  cpu.tick(currentMillis);

  if (showPrompt) {
    // Serial.println();
    Serial.print(prompt);
    showPrompt = false;
  }

  uint8_t cmd_status = readCommandLine(&commandBuffer, 80);
  if (cmd_status == CMD_ST_READY) {
    Serial.flush();
    mon.setCommandLine(&commandBuffer);
    int8_t command = mon.parseCommand();
    if (command != CMD_NONE) {
      Serial.println();
      mon.execute(command);
      showPrompt = true;
    }
    commandBuffer = " ";
    cmd_status = CMD_ST_WAIT;
  }
  
  #if defined(CPU_MEMORY_MOCKED) or defined(CPU_IO_MOCKED)
    // Read control lines
    uint8_t rd, wr;
    uint8_t addr = cpu.getAddr();
    rd = digitalRead(IN_RD);
    wr = digitalRead(IN_WR);
    #if defined(CPU_MEMORY_MOCKED)
      uint8_t mreq = digitalRead(IN_MREQ);
      // CPU Fetch Data. Active signas are LOW
      if(read_status == 0 && mreq == LOW && rd == LOW) {
      uint8_t data = memory.getByte(addr);
        cpu.setData(data);
        read_status = 1;
        debug_print("MREQ", "RD", addr, data);
      } else if (read_status == 1 && (mreq == HIGH || rd == HIGH)) {
        read_status = 0;
        cpu.setAsDataInput();
      }

      if(write_status == 0 && mreq == LOW && wr == LOW) {
      uint8_t data = cpu.getData();
      memory.setByte(addr, data);
        write_status = 1;
        debug_print("MREQ", "WR", addr, data);
      } else if (write_status == 1 && (mreq == HIGH || wr == HIGH)) {
        write_status = 0;
        cpu.setAsDataInput();
      }
    #endif
      
    #if defined(CPU_IO_MOCKED)
      uint8_t iorq = digitalRead(IN_IORQ);
      // CPU send data to Port. Active signas are LOW
        if(out_status==0 && iorq == LOW && rd == LOW) {
        uint8_t data = mockio.getInputData();
        cpu.setData(data);
        out_status = 1;
        debug_print("IORQ", "RD", addr, data);
      } else if (out_status==1 && (iorq == HIGH || rd == HIGH)) {
        out_status = 0;
      }
      
      if(in_status==0 && iorq == LOW && wr == LOW) {
        in_status = 1;
        uint8_t data = cpu.getData();
        mockio.setOutPort(addr, data);
        debug_print("IORQ", "WR", addr, data);
      } else if (in_status==1 && (iorq == HIGH || wr == HIGH)) {
        in_status = 0;
      }
    #endif
  #endif
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
void debug_print(String area, String operation, uint16_t addr, uint8_t data) {
    if(DEBUG) {
	    Serial.flush();
	    Serial.print(area);
      Serial.print('\t');
      Serial.print(operation);
      Serial.print('\t');
      Serial.print(addr,  HEX);
      Serial.print('\t');
      Serial.println(data);
	}
}
