#include "Arduino.h"
#include "MockIO.h"

MockIO::MockIO() {}

void MockIO::init() {
  fill_io(0);
  eraseOutBuffer();
}

void MockIO::setOutPort(uint8_t port, int8_t data) {
  if(checkPort(port)) {
    out_data[port] = data;
  }
  pushOutBuffer(data);
}

void MockIO::setInPort(uint8_t index, int8_t data) {
  if(checkIndex(index)) {
    input_data[index] = data;
  }
}

int8_t MockIO::showInput(uint8_t index) {
  if(checkIndex(index)) {
    return input_data[index];
  }
  return 0;
}

int8_t MockIO::getInputData() {
  int8_t data = input_data[io_indx];
  next_ioindx();
  return data;
}

uint8_t MockIO::getOutputData(uint8_t port) {
  if(checkPort(port)) {
    uint8_t data = out_data[port];
    return data;
  }
  return 0x00;
}

uint8_t MockIO::getOutputBufferData(uint8_t index) {
  return out_buffer[index];
};

void MockIO::next_ioindx() {
  io_indx++;
  if (io_indx < MOCK_IO_BUFFER_SIZE) {
    io_indx = 0;
  }
};

void MockIO::fill_io(uint8_t value) {
  for(uint8_t i = 0; i < CPU_IO_NUM_PORTS; i++) {
    out_data[i] = value;
  }
  for(uint8_t i = 0; i < MOCK_IO_BUFFER_SIZE; i++) {
    input_data[i] = value;
  }
};

bool MockIO::checkPort(uint8_t port) {
  return port < CPU_IO_NUM_PORTS;
}

bool MockIO::checkIndex(uint8_t indx) {
  return indx < MOCK_IO_BUFFER_SIZE;
}

void MockIO::setOutBuffer(int8_t data) {

};

void MockIO::pushOutBuffer(int8_t data) {
  out_buffer[out_buff_indx] = data;
  out_buff_indx++;
  if (out_buff_indx >= MOCK_IO_BUFFER_SIZE) {
    out_buff_indx = 0;
  }
};

void MockIO::eraseOutBuffer() {
  for(uint8_t i = 0; i < MOCK_IO_BUFFER_SIZE; i++) {
    out_buffer[i] = 0;
  }
  out_buff_indx = 0;
};

void MockIO::eraseInBuffer() {
  for(uint8_t i = 0; i < MOCK_IO_BUFFER_SIZE; i++) {
    input_data[i] = 0;
  }
  io_indx = 0;
};
