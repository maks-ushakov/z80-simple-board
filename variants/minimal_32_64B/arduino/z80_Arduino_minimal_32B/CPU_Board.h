#ifndef CPU_BOARD_H_
#define CPU_BOARD_H_ 100

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

class CPUBoard 
{
  public:
    CPUBoard();
    void init();
    void tick(unsigned long currentMillis);
    uint8_t getAddr();
    void setData(uint8_t data);
    uint8_t getData();
    void setAsDataInput();
    void setClock(long);
  private:
    uint8_t control_pins[8], addr_pins[16];
    long _interval = 100;           // interval at which to change CLK state (milliseconds)
    // Variables will change:
    uint8_t _clk_State = LOW;
    long previousMillis = 0;        // will store last time CLK was updated
    // the follow variables is a long because the time, measured in miliseconds,
    // will quickly become a bigger number than can be stored in an int.
};

#endif