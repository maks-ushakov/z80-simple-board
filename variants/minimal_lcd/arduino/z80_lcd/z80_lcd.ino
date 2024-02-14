// C++ code
//

#define IN_M1 2
#define IN_MREQ A0
#define IN_RD A1
#define IN_WR A2
#define IN_A0 3
#define IN_A1 A3
#define IN_A2 A4
#define IN_A3 A5
#define IN_A4 12

uint8_t control_pins[] = {
  IN_M1,
  IN_MREQ,
  IN_RD,
  IN_WR,
};
uint8_t addr_pins[] = {
  IN_A0,
  IN_A1,
  IN_A2,
  IN_A3,
  IN_A4
};

// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 5;           // interval at which to blink (milliseconds)

uint8_t read_status = 0;
uint8_t out_status = 0;
uint8_t code[] = {
	// 0000			org 0 
	//0x0e, 0x00, // 0000 0e 00		ld c, 0 
	//0x06, 0xf0, // 0002 06 0f		ld b, 15 
	// 0004			loop: 
	//0xed, 0x41, // 0004 ed 41		out (c), b 
	//0x10, 0xfc, // 0006 10 fc		djnz loop 
	//0x3e, 0x55, // 0008 3e 37		ld a, 55 
	//0x0c,// 000a 0c			inc c 
	//0xed, 0x79, // 000b ed 79		out (c), a 
	//0x76, // 000d 76			halt 
  //	0x00, 0x00
//  0000			org 0 
// 0000 0e 00		ld c, 0; set port 0 / LCD Inst 
0x00, 0x00,
0x0e, 0x00,
//0002 3e 04		ld a, 00000100b ; Entry mode 
0x3e, 0x04,
//0004 ed 79		out (c), a 
0xed, 0x79,
//0006 3e 0f		ld a, 00001111b; Display on 
0x3e, 0x0f,
//0008 ed 79		out (c), a 
0xed, 0x79,
//000a 3e 01		ld a, 00000001b; clear 
0x3e, 0x01,
//000c ed 79		out (c), a 
0xed, 0x79,
//000e 0c			inc c; set port 1/ LCD Data 
0x0c,
//000f 3e 48		ld a, 'H' 
0x3e, 0x48,
//0011 ed 79		out (c), a 
0xed, 0x79,
//0013 3e 69		ld a, 'i' 
0x3e, 0x69,
//0015 ed 79		out (c), a 
0xed, 0x79,
//0017 3e 21		ld a, '!' 
0x3e, 0x21,
//0019 ed 79		out (c), a 
0xed, 0x79,
//001b 76			halt 
0x76,
0x00, 0x00,
0x00
};

uint8_t getAddr();
void setData(uint8_t);

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  for (uint8_t i; i < 5; i++) {
  	pinMode(addr_pins[i], INPUT);
  }
  
  for (uint8_t i; i < 4; i++) {
  	pinMode(control_pins[i], INPUT);
  }
  
  setAsDataInput();
Serial.begin(115200);
}

void loop()
{
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(LED_BUILTIN, ledState);
  }
  uint8_t rd, wr, mreq, iorq;
  mreq = digitalRead(IN_MREQ);
  rd = digitalRead(IN_RD);
  wr = digitalRead(IN_WR);
  iorq = HIGH;
  uint8_t addr = getAddr();
  if(read_status == 0 && mreq == LOW && rd == LOW) {
  	setData(code[addr]);
    read_status = 1;
  } else if (read_status == 1 && (mreq == HIGH || rd == HIGH)) {
  	read_status = 0;
    setAsDataInput();
  }
  
  if(out_status==0 && iorq == LOW && wr == LOW) {
    out_status = 1;
    uint8_t data = getData();
    Serial.flush();
  	Serial.print(addr,  HEX);
    Serial.print("-> ");
    Serial.println(data,  HEX);
  } else if (out_status==1 && (iorq == HIGH || wr == HIGH)) {
  	out_status = 0;
  }
}
uint8_t getAddr() {
	uint8_t addr = 0;
  for(uint8_t i = 0; i < 5; i++) {
  addr += digitalRead(addr_pins[i])<<i;
  }
  return addr;
}

void setData(uint8_t data) {
  DDRD = DDRD | 0xF0;
  DDRB = DDRB | 0x0F;
  uint8_t portdClone, portbClone;
  portdClone = PORTD & 0x0f | (data & 0xf0);
  portbClone = PORTB & 0xf0 | (data & 0xF);
  PORTD = portdClone;
  PORTB = portbClone;
}

uint8_t getData() {
  setAsDataInput();
  // get value
  uint8_t data = 0;
  data += PINB & 0xF;
  data += PIND & 0xf0;
  
  return data;
}

void setAsDataInput() {
  // set  as input
  DDRD = DDRD & 0x0F;
  DDRB = DDRB & 0xF0;
  // no pull_up
  PORTD = PORTD & 0x0f;
  PORTB = PORTB & 0xf0;
}
