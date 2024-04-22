#ifndef MOCK_IO_H_
#define MOCK_IO_H_ 100

#include <WString.h>
#include "Config.h"

#define MOCK_IO_BUFFER_SIZE 16

class MockIO {
public:
  MockIO();
  void init();
  void setOutPort(uint8_t port, int8_t data);
  void setOutBuffer(int8_t data);
  void pushOutBuffer(int8_t data);
  void eraseOutBuffer();
  void eraseInBuffer();
  void setInPort(uint8_t port, int8_t data);
  int8_t getInputData();
  int8_t showInput(uint8_t);
  uint8_t getOutputData(uint8_t);
  uint8_t getOutputBufferData(uint8_t);
  void fill_io(uint8_t);
private:
  uint8_t io_indx = 0;
  uint8_t out_buff_indx = 0;
  uint8_t set_indx = 0;
  uint8_t input_data[MOCK_IO_BUFFER_SIZE];
  uint8_t out_data[CPU_IO_NUM_PORTS];
  uint8_t out_buffer[MOCK_IO_BUFFER_SIZE];
  bool checkPort(uint8_t);
  bool checkIndex(uint8_t);
  void next_ioindx();
};

#endif