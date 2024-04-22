/**
 * @file
 * Minimal OS Monitor
 */
 
#define NOT_PRESSED 0xFF

__sfr __at(0x00) D_ADDR_H ; 
__sfr __at(0x01) D_ADDR_L  ;
__sfr __at(0x02) D_DATA   ;
__sfr __at(0x03) KBD_PORT  ;

#define LAST_RAM  0x8000
#define FIRST_RAM 0x0C00

#define C_SETHB   0x10
#define C_SETLB   0x11
#define C_SETDAT  0x12
#define C_RUN     0x13

char getMemoryValue(unsigned int addr) {
		return 0;
}
void setMemoryValue(unsigned int addr, char data) {

}
void execute(unsigned int addr) {
	
}
 
void main() {
	unsigned int address = FIRST_RAM;
	while(1) {
		char currentData = getMemoryValue(address);
		char outData;
		outData = address >> 8;
		D_ADDR_H = outData;
		outData = address;
		D_ADDR_L = outData;
		outData = currentData;
		D_DATA = outData;
		while(1) {
			char key = NOT_PRESSED;
			while (key == NOT_PRESSED) {
			  key = KBD_PORT;
			}
			if (key < 0x10) {
				currentData = currentData * 0x10 + key;
				D_DATA = currentData;
			}
			
			if (key == C_SETHB) {
			  address = address & 0x00ff + currentData * 0x100;
			  break;
			}

			if (key == C_SETLB) {
			  address = address & 0xff00 + currentData;
			  break;
			}

			if (key == C_SETDAT) {
			  setMemoryValue(address, currentData);
			  address++;
			  break;
			}

			if (key == C_RUN) {
			  execute(address);
			}
		}
	}
}
