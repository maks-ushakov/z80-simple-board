# z80-simple-board
Z80 CPU coupled with Arduino Nano, RAM only

Use Arduino for load code to RAM and check.


## Steps
Test. First run. 
   Arduino Nano as Clock generator and Buffer.

   Resources:|

    - arduino/arduino_z80_test  - arduino sketch
    - schematicks/Z80-ARDUINO-test.sh.pdf

Test. Arduino as Memory and IO port.
   Minimal System. 16B memory. Any IO port is Arduino
   Arduino as universal chipset.

   Resources:

     - arduino/2_arduino_z80_test2_ram_rom_IO - arduino sketch
     - schematicks/z80-test2-ram-rom-port.sch
     - assm/rom16 - Z80 assembler examples

More memory 32B or 64B. Arduino Console (in progress)


LCD as IO port. Bus Buffering (in progress)


32k RAM chip. DMA. CPU give acces to bus (featured)

Block load XMODEM (featured)

Monitor (BIOS) via Arduino Serial (featured)

PPA 8255. Address detector. (featured)

Timer 8253 and Interrupt controller (featured)


