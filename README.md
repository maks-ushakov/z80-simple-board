<p align="center">
    <img src="images/z80_Arduino_Logo_min.png" alt="z80 Arduino logo" />
</p>
<p align="center">
    <p align="center">Simple board based on Z80 CPU and 'modern' Arduino board</p>
    <p align="center"><a href="https://www.youtube.com/playlist?list=PLPrbpMWukRSoZ5geATjLQaH6BlcrjA2ah">Click here to have a look at the videos about variants</a></p>
</p>


# z80-simple-board

## About project

This project has a few purpouses:

- learn old assembly language for Intel 8080
- learn the features for z80
- replace ROM with bootloader based on Arduino
- use Arduino as app loader for RAM area
- use Arduino as simple serial IO for CPU

## Variants

Project is making step by step from simplest minimal to more complicated.

All files for each variant you can find in directory `variants`

|  directory   | Memory| Brief                                       |Ready|
|--------------|-------|---------------------------------------------|-----|
|    test      |   1B  | For testing CPU buses                       |  Y  |
|  minimal_16B |   16B | Minimum to see how CPU work with Memory and IO| Y |
|  minimal_32B |   32B | Minimum for learn assembly language           | N |
|  minimal_lcd |   64B | Minimun with text LCD16x2                     | N |
|  half_32k    |  32KB | Half memory system                            | N |
|  half_monitor|  32KB | Simple Monitor (BIOS)                         | N |
|  half_ppa    |  32KB | Add Peripherial chips                         | N |
|  half_int    |  32KB | For learning interrupts                       | N |


## Structure

Each variant has own directories:

- `arduino` - for arduino scketches (with preloaded Z80 binary)
- `Assm` - assembly files and binares

Also each project has Kicad schematics and Readme with requirements for that variant.


# How to use
## Arduino sketch

Clone this repository
```
git clone https://github.com/maks-ushakov/z80-simple-board.git
```

Choose variant you want to use.

Load Arduino sketch

Change Memory array with z80 C-style-array code you want to be loaded to z80 memory

Load sketch to arduino

## How to make z80 binary from assebler source

You can use any file extensions for sourse and binary files.  Common extensions for sourses are `.s`, '.asm', 'z80', for binary are `.bin` or none. 
I use `.zasm` for assembly source and `z80` for binary.

__If you have z80asm__

```
z80asm --output=<file_name>.z80 <file_name>.zasm
```

__If you use VASM__
```
//TODO
```

## Binary to readable table

```
xxd -g 1 <file_name>.z80
```

## Binary to arduino array

```
xxd -i <file_name>.z80
```

## Load binary with XMODEM protocol

// TODO

## Serial terminals
Minicom
```
minicom -b 115200 -D /dev/ttyUSB0
```

Screen
```
screen /dev/ttyUSB0 115200
```

If you do not have access to ttyUSB0  (Arduino or terminal app still connect to /dev/ttyUSB0)

```
fuser -k /dev/ttyUSB0
```

## Steps
Test. First run. 
   Arduino Nano as Clock generator and Buffer.

   Resources: `variants/test`

Arduino as Memory and IO port.
   Minimal System. 16B memory. Any IO port is Arduino
   Arduino as universal chipset.

   Resources:`variants/minimal_16B`

## TODO

- More memory 32B or 64B. Arduino Console (in progress)

- LCD as IO port. Bus Buffering (in progress)

- 32k RAM chip. DMA. CPU give acces to bus (featured)

- Block load XMODEM (featured)

- Monitor (BIOS) via Arduino Serial (featured)

- PPA 8255. Address detector. (featured)

 -Timer 8253 and Interrupt controller (featured)

## Useful Sources

__Links__:

[z80 User Manual](http://www.zilog.com/docs/z80/um0080.pdf)

[Arduino Language References](https://docs.arduino.cc/programming/)

[Arduino Pinout](https://docs.arduino.cc/resources/pinouts/A000005-full-pinout.pdf)

[z80 info](http://www.z80.info/) - a lot of information about z80, his perioferial chips instruction sets and tutorials

[VASM](http://www.compilers.de/vasm.html) - portable assembler for different CPUs



__Other z80 projects__ when I get some ideas, shematic principles and code examples:

[Grant Searl z80 system](http://searle.x10host.com/) some boards has CP/M and BASIC, they used serial terminal

[Wichit Sirichote z80 kit](https://kswichit.net/Z80/Z80.html) has z80 binary for Monitor(BIOS)

[Wichit Sirichote i8085 kit](https://kswichit.net/mtk-85/index.html) has Assembly source code for Monitor(BIOS)

[CPU ville](http://cpuville.com/Kits/Z80-kits-home.html)


__Books__ (some books you can find on [z80 info Books](http://www.z80.info/z80books.htm)):

Build your own Z80 computer. Design guidelines and application notes. Ciarcia, S. 1981

Programming the Z80. Zaks Rodnay, 1981

From homemade devices on logic elements to microcomputers. USSR. Bartenev V., Alginin B. 1993 (About i8080)

Simplest micro-ECM. USSR. Bureev L.N. 1989 (About i8080 with simplest  BIOS)

