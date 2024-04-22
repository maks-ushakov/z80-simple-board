#include "Arduino.h"
#include "CPU_Board.h"

CPUBoard::CPUBoard() {
  control_pins[0] = IN_M1;
  control_pins[0] = IN_MREQ;
  control_pins[0] = IN_RD;
  control_pins[0] = IN_WR;
  control_pins[0] = IN_IORQ;

  addr_pins[0] = IN_A0;
  addr_pins[1] = IN_A1;
  addr_pins[2] = IN_A2;
  addr_pins[3] = IN_A3;
};

void CPUBoard::init() {
  // put your setup code here, to run once:
  // Set Clock output
  pinMode(CLK_PIN, OUTPUT);

    // Set Address lines as input
  for (uint8_t i; i < 4; i++) {
  	pinMode(addr_pins[i], INPUT);
  }
  // set Control lines as input
  for (uint8_t i; i < 5; i++) {
  	pinMode(control_pins[i], INPUT);
  }
  
  setAsDataInput();
}

void CPUBoard::tick(unsigned long currentMillis) {
    if(currentMillis - previousMillis > _interval) {
    // if the CLK_PIN is off turn it on and vice-versa:
    if (_clk_State == LOW)
      _clk_State = HIGH;
    else
      _clk_State = LOW;
    // set the CLK_PIN with the clk_CPU_State of the variable:
    digitalWrite(CLK_PIN, _clk_State);
    // save the last time you toggle the CLK_PIN 
    previousMillis = currentMillis; 
  }
};

void CPUBoard::setClock(long period) {
  _interval = period;
}

void CPUBoard::setAsDataInput() {
  // set  as input
  DDRD = DDRD & 0x0f;
  DDRB = DDRB & 0xf0;
  // no pull_up
  PORTD = PORTD & 0x0f;
  PORTB = PORTB & 0xf0;
}

/**
 * Take address from addr pins
 */ 
uint8_t CPUBoard::getAddr() {
	uint8_t addr = 0;
  for(uint8_t i = 0; i < 4; i++) {
    addr += digitalRead(addr_pins[i])<<i;
  }
  return addr;
}

/**
 * Give data to port
 */ 
void CPUBoard::setData(uint8_t data) {
  DDRD = DDRD | 0xF0;
  DDRB = DDRB | 0x0F;
  uint8_t portdClone, portbClone;
  portdClone = PORTD & 0x0F | (data & 0xF0);
  portbClone = PORTB & 0xF0 | (data & 0xF);
  PORTD = portdClone;
  PORTB = portbClone;
}

/**
 * Take data from port
 */ 
uint8_t CPUBoard::getData() {
  setAsDataInput();
  // get value
  uint8_t data = 0;
  data += PINB & 0xf;
  data += PIND & 0xf0;
  
  return data;
}
