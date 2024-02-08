# Z80-Arduino test

Connect Arduino Nano to Z80 CPU and use it as 16B memory.

## Pin connections

|  Z80 |  Arduino pins  |  pin mode |
|------|----------------|-----------|
|  M1  |      D2        |    IN     |
|  MREQ|      A0        |    IN     |
|  IORQ|      D12       |    IN     |
|  RD  |      A1        |    IN     |
|  WR  |      A2        |    IN     |
|  A0  |      D3        |    IN     |
|  A1  |      A3        |    IN     |
|  A2  |      A4        |    IN     |
|  A3  |      A5        |    IN     |
|  D0  |      D8        |   OUT     |
|  D1  |      D9        |   OUT     |
|  D2  |      D10       |   OUT     |
|  D3  |      D11       |   OUT     |
|  D4  |      D4        |   OUT     |
|  D5  |      D5        |   OUT     |
|  D6  |      D6        |   OUT     |
|  D7  |      D7        |   OUT     |
| CLK  |      D13       |   OUT     |

![Z80 Arduino Schema](./z80-arduino-test.png)

## Start

- Power ON

- Press reset a few seconds

- See Outputs on Serial monitor



