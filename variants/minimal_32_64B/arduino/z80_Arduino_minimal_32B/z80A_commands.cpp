#include "Arduino.h"
#include "z80A_commands.h"

uint8_t readCommandLine(String * buffer, int len) {
  if (Serial.available() == 0) {
    return CMD_ST_WAIT;
  }
  uint8_t index = buffer->length();
  char incoming = char(Serial.read());
  if ((incoming != '\b') && (index > 0)) {
    buffer->concat(incoming);
  } else {
    buffer->remove(index-1);
  }
  if (isPrintable(incoming) || incoming == '\b') {
    Serial.write(incoming);
  }

  if (incoming == '\n' || incoming == '\r' || buffer->length() > len) {
    buffer->trim();
    return CMD_ST_READY;
  }

  return CMD_ST_WAIT;
}

Commands::Commands() {}

void Commands::init(MemoryLoader * mem) {
  memory = mem;
}

void Commands::setCommandLine(String * cmd) {
  cmdLine = cmd;
  currentIndex = 0;
};


int8_t Commands::parseCommand()
{
  char c = cmdLine->charAt(0);
  int8_t cmd = CMD_INVALID;
  if(!isPrintable(c) || isControl(c)) {
    return CMD_NONE;
  }

    // Convert the command to lowercase.
    if ((c >= 'A') && (c <= 'Z')) {
        c |= 0x20;
    }

    switch (c)
    {
        case '?':  cmd = CMD_HELP;      break;

        case 'k':  cmd = CMD_CHECKSUM;  break;
        case 'c':  cmd = CMD_COMPARE;  break;
        case 'd':  cmd = CMD_DUMP;      break;
        case 'e':  cmd = CMD_ERASE;     break;
        case 'f':  cmd = CMD_FILL;      break;
        case 'm':
        case 'p':  cmd = CMD_POKE;      break;
        case 's':  cmd = CMD_SEARCH;    break;
        case 't':  cmd = CMD_TRANSFER;  break;
        case 'r':  cmd = CMD_READ;      break;
        case 'w':  cmd = CMD_WRITE;     break;

        // Debug
        case '%':  cmd = CMD_INFO;      break;
        case 'z':  cmd = CMD_ZAP;       break;
        case '/':  cmd = CMD_LAST_STATUS;break;


        case '#': cmd = CMD_FREQ;       break;
        // IO Mocked
        case 'o': cmd = CMD_IO_SHOW_OUT;    break;
        case 'i': cmd = CMD_IO_SET;     break;

        default:   cmd = CMD_INVALID;   break;
    }

    return cmd;
}

void Commands::execute(int8_t command) {
  switch(command) {
    case CMD_HELP: printHelp(); break;
    case CMD_INFO: printInfo(); break;
    case CMD_INVALID: printInvalid(); break;
    case CMD_POKE: pokeBytes(); break;
    case CMD_DUMP: dumpMemory(); break;
    case CMD_FILL: fill(); break;
    case CMD_ERASE: erase(); break;
  #if defined(CPU_IO_MOCKED)
    case CMD_IO_SHOW_OUT: showOutPorts(); break;
    case CMD_IO_SET: setInputs(); break;
  #endif
    default: break;
  };
}

// Read a line of data from the serial connection.
char * readLine(char * buffer, int len)
{
    for (int i = 0; (i < len); i++)
    {
         buffer[i] = 0;
    }

    // read serial data until linebreak or buffer is full
    char c = ' ';
    int indx = 0;
    do {
        if (Serial.available())
        {
            c = Serial.read();
            if ((c == '\b') && (indx > 0))
            {
                // Backspace, forget last character
                --indx;
            }
            buffer[indx++] = c;
            Serial.write(c);
        }
    } while ((c != '\n') && (c != '\r') && (indx < len));

    buffer[indx - 1] = '\0';
    return buffer;
}


void Commands::printByte(uint16_t addr) {
  Serial.print(addr, HEX);
  Serial.print(" : ");
  Serial.println(memory->getByte(addr));
};

void Commands::printBlock(uint16_t startAddr, uint16_t endAddr) {
  
};

void Commands::printHelp() {
  Serial.print(F("z80+Arduino"));
  Serial.println();
  Serial.println(F("Valid commands are:"));
  Serial.println(F("  ?                - Show this command list"));
  Serial.println(F("  %                - Show config info"));
  Serial.println(F("  D sssss eeeee    - Dump bytes from memory to terminal"));
  Serial.println(F("  P sssss dd dd... - Poke (write) values to memory (up to 32 values)"));
  Serial.println(F("  M sssss dd dd... - Set memory cells (up to 32 values)"));
  Serial.println(F("  F m ssss eeeee dd- Fill memory cells with value"));
  Serial.println(F("  E [m|o|i]        - Erase whole memory, output or input"));
  #if defined(CPU_IO_MOCKED)
    Serial.println(F("IO Port commands"));
    Serial.println(F("  F [o|i] dd       - Fill  output or input with value"));
    Serial.println(F("  I sss dd         - Load data to Input buffer"));
    Serial.println(F("  O sssss eeeee    - Show values by ports"));
  #endif
}

void Commands::printInfo() {
  Serial.print(CPU_NAME);
  Serial.println(F("+Arduino"));
  String version = "Version: ";
  version.concat(CPU_BOARD_VERSION);
  Serial.println(version);
  String cpu = "CPU:\t";
  cpu.concat(CPU_NAME);
  Serial.println(cpu);
  String memory = "\n\rMemory:\t";
  String mem_type = "REAL";
  #if defined(CPU_MEMORY_MOCKED)
    mem_type = "MOCKED";
  #endif
  memory.concat(mem_type);
  String mem_size = "\n\rMemory size:\t";
  mem_size.concat(MEMORY_SIZE);
  mem_size.concat("B");
  memory.concat(mem_size);
  String autoload = "\n\rAutoload:\t";
  String aload_type = "OFF";
  #if defined(MEM_AUTOLOAD_ON)
    aload_type = "ON";
  #endif
  autoload.concat(aload_type);
  memory.concat(autoload);
  Serial.println(memory);

  String io_ports = "\n\rIO PORTS: ";
  String io_type = "REAL";
  String io_details = "";
  #if defined(CPU_IO_MOCKED)
    io_type = "MOCKED";
    io_details = "\n\rOUT PORTS: ";
    io_details.concat(CPU_IO_NUM_PORTS);
  #endif;
  io_ports.concat(io_type);
  io_ports.concat(io_details);
  Serial.println(io_ports);
}

void Commands::printInvalid() {
  Serial.println("Invalid command");
}

void Commands::pokeBytes() {
  int cmdLength = cmdLine->length();
  uint32_t addr = getDWord(1);
  uint8_t data[32];
  for(int i = 0; i < 32; i++) {
    data[i] = 0;
  }
  currentIndex = nextNonSpace(currentIndex);
  uint8_t dataIndex = 0;
  while(currentIndex > 0 && currentIndex < cmdLength) {
    data[dataIndex] = getByte(currentIndex);
    dataIndex++;
  }

  uint8_t dataLength = dataIndex;
  for(int i = 0; i <= dataLength; i++) {
    memory->setByte(addr+i, data[i]);
  }
}

void Commands::dumpMemory() {
  int cmdLength = cmdLine->length();
  uint32_t startAddr = getDWord(1);
  uint32_t endAddr = getDWord(currentIndex);
  if (endAddr == 0) {
    Serial.print(startAddr, HEX);
    Serial.print(":");
    Serial.print("\t");
    Serial.println(memory->getByte(startAddr), HEX);
  } else {
    for(uint32_t i = startAddr; i <= endAddr; i+= 8) {
      Serial.print(i, HEX);
      Serial.print(":");
      for(uint32_t j = 0; j < 8; j++) {
        Serial.print("\t");
        Serial.print(memory->getByte(i + j), HEX);
      }
      Serial.println();
    }
  }
}

void Commands::fill() {
  int cmdLength = cmdLine->length();
  currentIndex = nextNonSpace(1);
  char area = cmdLine->charAt(currentIndex);
  uint32_t startAddr = 0;
  uint32_t endAddr = 0;
  area = tolower(area);
  if (area == 'm') {
    startAddr = getDWord(currentIndex + 1);
    endAddr = getDWord(currentIndex + 1);

    if (endAddr == 0) {
      endAddr = startAddr;
    }

    if (endAddr > MEMORY_SIZE) {
      endAddr = MEMORY_SIZE;
    }
  }
  int8_t value = 0;
  currentIndex = nextNonSpace(currentIndex+1);
  if(currentIndex < cmdLength) {
    value = getByte(currentIndex);
  }

  if (area == 'm') {
    fillMemory(startAddr, endAddr, value);
  }
#if defined(CPU_IO_MOCKED)
  if (area == 'i') {
    fillInput(value);
  }

  if (area == 'o') {
    fillOutput(value);
  }
#endif
};

void Commands::fillMemory(uint16_t from, uint16_t to, uint8_t value) {
  // Serial.print(F("Fill Mem from:"));
  // Serial.print(from, HEX);
  // Serial.print(F(" to:"));
  // Serial.print(to, HEX);
  // Serial.print(F(" with val:"));
  // Serial.println(value, HEX);

  for(int i = from; i <= to; i++) {
    memory->setByte(i, value);
  }
};

void Commands::erase() {
 int cmdLength = cmdLine->length();
  int8_t index = nextNonSpace(1);
  char area = cmdLine->charAt(index);
  uint8_t value = 0;

  if (area == 'm') {
    fillMemory(0, MEMORY_SIZE, value);
  }
#if defined(CPU_IO_MOCKED)
  if (area == 'i') {
    io->eraseInBuffer();
  }

  if (area == 'o') {
    fillOutput(value);
    io->eraseOutBuffer();
  }
#endif
};

uint8_t Commands::getByte(uint8_t from) {
  int8_t index = nextNonSpace(from);
  uint8_t digit = parseHex(cmdLine->charAt(index));
  uint8_t data = digit;
  index++;
  if(isHexadecimalDigit(cmdLine->charAt(index))) {
    digit = parseHex(cmdLine->charAt(index));
    data = data << 4;
    data += digit;
    index++;
  }
  currentIndex = index;
  return data;
}

uint16_t Commands::getWord(uint8_t from) {
  int8_t index = nextNonSpace(from);

  return 0x00;
}

uint32_t Commands::getDWord(uint8_t from) {
  uint32_t value = 0;
  int8_t index = nextNonSpace(from);
  int8_t end = from + 8;
  while(isHexadecimalDigit(cmdLine->charAt(index)) 
    && index > 0 && index < end 
    && index < cmdLine->length()) {
    uint8_t digit = parseHex(cmdLine->charAt(index));
    value = value << 4;
    value += digit;
    index++;
  }
  currentIndex = index;
  return value;
}

int8_t Commands::nextNonSpace(int8_t index) {
  if (index == -1) return -1;
  uint8_t cmdLength = cmdLine->length();
  while(index < cmdLength && isSpace(cmdLine->charAt(index))) {
    index++;
  }

  if (index >= cmdLength) {
    return -1;
  }

  return index;
}

uint8_t Commands::parseHex(char c) {
  if(c >= '0' && c <= '9') {
    return byte(c - '0');
  } 
  
  if (c >= 'A' && c <= 'F') {
    return byte(10 + c - 'A');
  }

  if (c >= 'a' && c <= 'f') {
    return byte(10 + c - 'a');
  }

  return 0x00;
}

#if defined(CPU_IO_MOCKED)
void Commands::setIO(MockIO * ioD) {
  io = ioD;
}

void Commands::setInputs() {
  if (cmdLine->length() > 1) {
    uint32_t port = getDWord(1);
    currentIndex = nextNonSpace(currentIndex);
    io->setInPort(port, getByte(currentIndex));
  }
  Serial.print(F("In buffer: "));
  for(int i = 0; i < MOCK_IO_BUFFER_SIZE; i++) {
    Serial.print(byte(io->showInput(i)), HEX);
    Serial.print(F("  "));
  }
  Serial.println();
}

void Commands::fillInput(uint8_t value) {
  for(int i = 0; i < MOCK_IO_BUFFER_SIZE; i++) {
    io->setInPort(i, value);
  }
};

void Commands::fillOutput(uint8_t value) {
  for(uint8_t p = 0; p < CPU_IO_NUM_PORTS; p++) {
    io->setOutPort(p, value);
  }
};

void Commands::showOutPorts() {
  uint32_t startAddr = getDWord(1);
  uint32_t endAddr = getDWord(currentIndex);
  if(endAddr == 0) {
    endAddr = startAddr;
  }

  if (endAddr >= CPU_IO_NUM_PORTS) {
    endAddr = CPU_IO_NUM_PORTS;
  }
  Serial.print(F("Buffer: "));
  for(uint8_t i = 0; i < MOCK_IO_BUFFER_SIZE; i++) {
    Serial.print(io->getOutputBufferData(i), HEX);
    Serial.print(F("\t"));
  }
  Serial.println();
  for(uint32_t i = startAddr; i <= endAddr; i++) {
    Serial.print(i, HEX);
    Serial.print(F(":"));
    Serial.print(F("\t"));
    Serial.print(io->getOutputData(i), HEX);
    Serial.print(F("\t( "));
    Serial.print(io->getOutputData(i), DEC);
    Serial.print(F(" | "));
    Serial.print(char(io->getOutputData(i)));
    Serial.println(F(" )"));
  }
  // Serial.println();

}
#endif