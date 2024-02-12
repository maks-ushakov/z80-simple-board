// Z80+Arduino 
// Arduino as ROM and Output Port
//
// Data lines are half of ATmega Port B and Port D.
// It is an Arduino digital pins from D4 to D11

#define CLK_PIN 13
#define IN_M1 2
#define IN_MREQ A0
#define IN_IORQ 12
#define IN_RD A1
#define IN_WR A2
#define IN_A0 3
#define IN_A1 A3
#define IN_A2 A4
#define IN_A3 A5

uint8_t DEBUG = 1;

uint8_t control_pins[] = {
  IN_M1,
  IN_MREQ,
  IN_RD,
  IN_WR,
  IN_IORQ,
};
uint8_t addr_pins[] = {
  IN_A0,
  IN_A1,
  IN_A2,
  IN_A3
};

// Variables will change:
int clk_CPU_State = LOW;             // used to set the CLK
long previousMillis = 0;        // will store last time CLK was updated
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 100;           // interval at which to change CLK state (milliseconds)

uint8_t read_status = 0;
uint8_t write_status = 0;
uint8_t out_status = 0;
uint8_t in_status = 0;
/*
// NOP
uint8_t code[] = {
  	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
*/
// MEM WR TEST
uint8_t code[] = {
  	0x3e, 0x64, 0x06, 0x17, 0x90, 0x0e, 0x00, 0xed, 0x79, 0x76,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*
// MEM and IO TEST
uint8_t code[] = {
  	0x3e, 0x64, 0x06, 0x17, 0x90, 0x0e, 0x00, 0xed, 0x79, 0x76,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
*/
/*
// Add
uint8_t code[] = {
  	0x3e, 0x64, 0x06, 0x17, 0x90, 0x0e, 0x00, 0xed, 0x79, 0x76,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
*/
/*
// Sub
uint8_t code[] = {
  	0x3e, 0x64, 0x06, 0x17, 0x90, 0x0e, 0x00, 0xed, 0x79, 0x76,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
*/
uint8_t getAddr();
void setData(uint8_t);
void debug_print(String, String, uint16_t, uint8_t, uint8_t);

void setup() {
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

  Serial.begin(115200);
}

void loop()
{
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you toggle the CLK_PIN 
    previousMillis = currentMillis;   
    // if the CLK_PIN is off turn it on and vice-versa:
    if (clk_CPU_State == LOW)
      clk_CPU_State = HIGH;
    else
      clk_CPU_State = LOW;
    // set the CLK_PIN with the clk_CPU_State of the variable:
    digitalWrite(CLK_PIN, clk_CPU_State);
  }
  
  // Read control lines
  uint8_t rd, wr, mreq, iorq, m1;
  mreq = digitalRead(IN_MREQ);
  rd = digitalRead(IN_RD);
  wr = digitalRead(IN_WR);
  iorq = digitalRead(IN_IORQ);
  m1 = digitalRead(IN_M1);

  // Read address lines
  uint8_t addr = getAddr();
  
  // CPU Fetch Data. Active signas are LOW
  if(read_status == 0 && mreq == LOW && rd == LOW) {
	uint8_t data = code[addr];
  	setData(data);
    read_status = 1;
    debug_print("MREQ", "RD", addr, data, m1);
  } else if (read_status == 1 && (mreq == HIGH || rd == HIGH)) {
  	read_status = 0;
    setAsDataInput();
  }

  if(write_status == 0 && mreq == LOW && wr == LOW) {
	uint8_t data = getData();
	code[addr] = data;
    write_status = 1;
    debug_print("MREQ", "WR", addr, data, m1);
  } else if (write_status == 1 && (mreq == HIGH || wr == HIGH)) {
  	write_status = 0;
    setAsDataInput();
  }
  
  // CPU send data to Port. Active signas are LOW
    if(out_status==0 && iorq == LOW && rd == LOW) {
    uint8_t data = 0xAA;
  	setData(data);
  	out_status = 1;
  	debug_print("IORQ", "RD", addr, data, m1);
  } else if (out_status==1 && (iorq == HIGH || rd == HIGH)) {
  	out_status = 0;
  }
  
  if(in_status==0 && iorq == LOW && wr == LOW) {
    in_status = 1;
    uint8_t data = getData();
    debug_print("IORQ", "WR", addr, data, m1);
  } else if (in_status==1 && (iorq == HIGH || wr == HIGH)) {
  	in_status = 0;
  }
}

/**
 * Take address from addr pins
 */ 
uint8_t getAddr() {
	uint8_t addr = 0;
  for(uint8_t i = 0; i < 4; i++) {
    addr += digitalRead(addr_pins[i])<<i;
  }
  return addr;
}

/**
 * Give data to port
 */ 
void setData(uint8_t data) {
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
uint8_t getData() {
  setAsDataInput();
  // get value
  uint8_t data = 0;
  data += PINB & 0xf;
  data += PIND & 0xf0;
  
  return data;
}

/**
 * Set port for receive data
 */
void setAsDataInput() {
  // set  as input
  DDRD = DDRD & 0x0f;
  DDRB = DDRB & 0xf0;
  // no pull_up
  PORTD = PORTD & 0x0f;
  PORTB = PORTB & 0xf0;
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
