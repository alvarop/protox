# Radio Configuration and Startup 

Trying to figure out what the initial configuration for the radio is and startup procedure.

## Remote

### Step 1 - Initial Register Configuration

NOTE: The registers are written in order except for register 0x00 which is written first, then register 0x06. The rest follow in order. Values marked with N/A are not set initially

 Reg | Value | Description
---- | ---- | ------------ 
0x00 | 0x00 | Mode Register
0x01 | 0x62 | Mode Control Register
0x02 | 0x00 | Calibration Control Register
0x03 | 0x0f | FIFO Register I
0x04 | 0xc0 | FIFO Register II
0x05 | N/A | FIFO DATA Register
0x06 | ID(4B) | ID DATA Register
0x07 | 0x00 | RC OSC Register I
0x08 | 0x00 | RC OSC Register II
0x09 | 0x04 | RC OSC Register III
0x0A | 0x00 | CKO Pin Control Register
0x0B | 0x01 | GIO1 Pin Control Register I
0x0C | 0x01 | GIO2 Pin Control Register II
0x0D | 0x05 | Clock Register
0x0E | 0x04 | Data Rate Register
0x0F | 0x50 | PLL Register I
0x10 | 0x9e | PLL Register II
0x11 | 0x4b | PLL Register III
0x12 | 0x00 | PLL Register IV
0x13 | 0x00 | PLL Register V
0x14 | 0x16 | TX Register I
0x15 | 0x2b | TX Register II
0x16 | 0x12 | Delay Register I
0x17 | 0x00 | Delay Register II
0x18 | 0x62 | RX Register
0x19 | 0x80 | RX Gain Register I
0x1A | 0x80 | RX Gain Register II
0x1B | 0x00 | RX Gain Register III
0x1C | 0x0a | RX Gain Register IV
0x1D | 0x32 | RSSI Threshold Register
0x1E | 0xc3 | ADC Control Register
0x1F | 0x07 | Code Register I
0x20 | 0x17 | Code Register II
0x21 | 0x00 | Code Register III
0x22 | 0x00 | IF Calibration Register I
0x23 | N/A | IF Calibration Register II
0x24 | 0x00 | VCO current Calibration Register
0x25 | 0x00 | VCO Single band Calibration Register I
0x26 | 0x3b | VCO Single band Calibration Register II
0x27 | 0x00 | Battery detect Register
0x28 | 0x17 | TX test Register
0x29 | 0x47 | Rx DEM test Register I
0x2A | 0x80 | Rx DEM test Register II
0x2B | 0x03 | Charge Pump Current Register
0x2C | 0x01 | Crystal test Register
0x2D | 0x45 | PLL test Register
0x2E | 0x18 | VCO test Register I
0x2F | 0x00 | VCO test Register II
0x30 | 0x01 | IFAT Register
0x31 | 0x0f | RScale Register
0x32 | N/A | Filter test Register


### Step 2 - Internal Calibration

The device is set to standby mode by using the 0xA0 strobe command.

Register 0x02 is set to 1, which enables the IF Filter Bank calibration (I have no idea what that means.) The register is then read until the bit is cleared, which indicates calibration complete.

The IF calibration register I (0x22) is then read back. I'm not sure why, since it is overwritten next, along with another value for register 0x23. For reference, the read back value I saw was 0x06 and the values written were 0x13 and 0x3b.

The PLL Register I (0x0F) is set to 0x00. (This sets the transmit/receive frequency or channel)

The device is then set to PLL mode by using the 0xB0 strobe command.

Register 0x02 is set to 2, which enables the VCO Bank calibration. The register is then read until the bit is cleared, which indicates calibration complete.

The IF calibration register I (0x22) is read back again. I'm not sure why, since we just calibrated the VCO... (In my case, the value was 0x03)

The PLL Register I (0x0F) is now set to 0x78. 

The device is again set to PLL mode by using the 0xB0 strobe command.

Register 0x02 is set to 2, which enables the VCO Bank calibratio (Yes, again.) The register is then read until the bit is cleared, which indicates calibration complete.

The IF calibration register I (0x22) is read back. (In my case, the value was 0x03)

The VCO Single band Calibration Register I (0x25) is then set (In my case, it was 0x0B).

The device is set back to standby mode by using the 0xA0 strobe command.


### Step 3 - Channel Select

#### Step 3.1 - Scan Setup 

The ID register (0x6) is read back (Maybe to confirm it's still what we set it to in the beginning?)

The PLL Register I (0x0F) is now set to 0xA0.

The device is set to PLL mode by using the 0xB0 strobe command.

The device is then set to RX mode by using the 0xC0 strobe command.

The ADC Control Register (0x1E) is set to 0xC3, which means the RSSI margin is 20, RSSI measurements are in continuous mode, and the measurement is set to RSSI or carrier-detect measurement.

The channel scan is then done multiple times, with PLL values of: 0x14, 0x1e, 0x28, 0x32, 0x3c, 0x46, 0x50, 0x5a, 0x64, 0x6e, 0x78, 0x82

#### Step 3.2 Channel Scan

The PLL Register I (0x0F) is set to 0xXX (see above values).

The device is set to PLL mode by using the 0xB0 strobe command.

The device is then set to RX mode by using the 0xC0 strobe command.

The RSSI Threshold register is then read 15 times

### Step 3.3 Channel Selection

The best channel is selected by [Not sure yet]

The PLL Register I (0x0F) is set to the best channel

The device is set to PLL mode by using the 0xB0 strobe command.

## Quad

### Step 1 - Initial Register Configuration

The initial register configuration seems to be the same on the quad and the remote

### Step 2 - Internal Calibration
The internal calibration seems to be the same on the quad and the remote

### Step 3 - Channel Scan
The quad also has a channel scan after the calibration, but it is very different from the remote's. It constantly cycles through twelve channels until it receives a packet from the remote.

The quad first starts on channel 20 (2410 MHz) and increases in 5MHz increments (10 channels) every ~14ms or so until it reaches channel 130 (2465 MHz.) The base is transmitting on a single channel at approximatley the same rate, so it should take less than ~170ms to find the correct frequency.
