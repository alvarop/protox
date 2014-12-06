# Misc project notes

## A7105 Strobe Commands

 CMD | Description
---- | -----------
0x80 | Sleep Mode
0x90 | Idle Mode
0xA0 | Standby Mode
0xB0 | PLL Mode
0xC0 | RX Mode
0xD0 | TX Mode
0xE0 | FIFO Write pointer reset
0xF0 | FIFO Read pointer reset

## A7105 Registers

 Wr  |  Rd  | Description
---- | ---- | ------------ 
0x00 | 0x40 | Mode Register
0x01 | 0x41 | Mode Control Register
0x02 | 0x42 | Calibration Control Register
0x03 | 0x43 | FIFO Register I
0x04 | 0x44 | FIFO Register II
0x05 | 0x45 | FIFO DATA Register
0x06 | 0x46 | ID DATA Register
0x07 | 0x47 | RC OSC Register I
0x08 | 0x48 | RC OSC Register II
0x09 | 0x49 | RC OSC Register III
0x0A | 0x4A | CKO Pin Control Register
0x0B | 0x4B | GIO1 Pin Control Register I
0x0C | 0x4C | GIO2 Pin Control Register II
0x0D | 0x4D | Clock Register
0x0E | 0x4E | Data Rate Register
0x0F | 0x4F | PLL Register I
0x10 | 0x50 | PLL Register II
0x11 | 0x51 | PLL Register III
0x12 | 0x52 | PLL Register IV
0x13 | 0x53 | PLL Register V
0x14 | 0x54 | TX Register I
0x15 | 0x55 | TX Register II
0x16 | 0x56 | Delay Register I
0x17 | 0x57 | Delay Register II
0x18 | 0x58 | RX Register
0x19 | 0x59 | RX Gain Register I
0x1A | 0x5A | RX Gain Register II
0x1B | 0x5B | RX Gain Register III
0x1C | 0x5C | RX Gain Register IV
0x1D | 0x5D | RSSI Threshold Register
0x1E | 0x5E | ADC Control Register
0x1F | 0x5F | Code Register I
0x20 | 0x60 | Code Register II
0x21 | 0x61 | Code Register III
0x22 | 0x62 | IF Calibration Register I
0x23 | 0x63 | IF Calibration Register II
0x24 | 0x64 | VCO current Calibration Register
0x25 | 0x65 | VCO Single band Calibration Register I
0x26 | 0x66 | VCO Single band Calibration Register II
0x27 | 0x67 | Battery detect Register
0x28 | 0x68 | TX test Register
0x29 | 0x69 | Rx DEM test Register I
0x2A | 0x6A | Rx DEM test Register II
0x2B | 0x6B | Charge Pump Current Register
0x2C | 0x6C | Crystal test Register
0x2D | 0x6D | PLL test Register
0x2E | 0x6E | VCO test Register I
0x2F | 0x6F | VCO test Register II
0x30 | 0x70 | IFAT Register
0x31 | 0x71 | RScale Register
0x32 | 0x72 | Filter test Register

## Packet Format(s)

### Sync

Packet used during sync

Byte | Description
---- | -----------
0x00 | Packet Type - (0x1, 0x2, 0x3, 0x5, 0x9, 0xA)
0x01 | sync 0 ?
0x02 | sync 1 ?
0x03 | sync 2 ?
0x04 | sync 3 ?
0x05 | sync 4 ?
0x06 | sync 5 ?
0x07 | sync 6 ?
0x08 | sync 7 ?
0x09 | sync 8 ?
0x0A | 
0x0B | 
0x0C | 
0x0D | 
0x0E | 
0x0F | Checksum

## Documentation links
* A7105 Datasheet - http://www.avantcom.com.tw/AVANTCOM/TC/DATA/PRODUCT/SOLVE/18_3.pdf
* STM8S003F3P6 Datasheet - http://www.st.com/web/en/resource/technical/document/datasheet/DM00024550.pdf

## Similar Projects:
* https://bitbucket.org/PhracturedBlue/deviation/src/92e1705cf895b415ab16f6e1d7df93ee11d55afe/doc/Flysky.txt?at#default
* http://www.cloud-rocket.com/2014/07/reverse-engineering-wltoys-v929v911protocol/
* https://github.com/goebish/bradwii-X4
* https://github.com/hackocopter/JD385_Documentation
* http://www.deviationtx.com/forum/protocol-development/1848-new-hubsan-upgraded-version-on-the-way - These guys already got the new controller and figured out a whole lot
