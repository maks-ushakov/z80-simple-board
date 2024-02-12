// C++ code
//
#define LED_M1 4
#define LED_MREQ 5
#define LED_RD 6
#define LED_WR 7
#define LED_A0 8 
#define LED_A1 9
#define LED_A2 10 
#define LED_A3 11

#define IN_M1 2
#define IN_MREQ A0
#define IN_RD A1
#define IN_WR A2
#define IN_A0 3
#define IN_A1 A3
#define IN_A2 A4
#define IN_A3 A5

uint8_t inputs[] = {
  IN_M1,
  IN_MREQ,
  IN_RD,
  IN_WR,
  IN_A0,
  IN_A1,
  IN_A2,
  IN_A3
};

uint8_t outputs[] = {
  LED_M1, 
  LED_MREQ,
  LED_RD,
  LED_WR,
  LED_A0,
  LED_A1,
  LED_A2,
  LED_A3
};
// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 50;           // interval at which to blink (milliseconds)

void setup() {
  // put your setup code here, to run once:
pinMode(LED_BUILTIN, OUTPUT);
  for (uint8_t i = 0; i < 8; i++) {
  	pinMode(inputs[i], INPUT);
    pinMode(outputs[i], OUTPUT);
    //digitalWrite(outputs[i], LOW);
  }
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(outputs[i], HIGH);
    delay(500);
    digitalWrite(outputs[i], LOW);
    delay(500);
  }

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
  
  int data = LOW;
    for (uint8_t i = 0; i < 8; i++) {
  	data = digitalRead(inputs[i]);
    digitalWrite(outputs[i], data);
  }
}