#!/usr/bin/python

import sys
import array

# 
# The protox module allows for the decoding of A7105 SPI packets
# 
# Strobe packets are marked with an S. Register read/writes are marked R/W
# 
# Sample output:
# R RSSI = 0.886V
# R RSSI = 0.872V
# W PLL Register I [82]
# S PLL Mode
# S Standby Mode
# S FIFO Write pointer reset
# W FIFO DATA Register [01 82 C0 AC D8 41 00 00 00 00 00 00 00 00 00 F8]
# S TX Mode
# 

STROBE_BIT = 0x80
READ_BIT = 0x40

#
# Strobe string definitions
#
strobes = {}
strobes[0x80] = 'Sleep Mode'
strobes[0x90] = 'Idle Mode'
strobes[0xA0] = 'Standby Mode'
strobes[0xB0] = 'PLL Mode'
strobes[0xC0] = 'RX Mode'
strobes[0xD0] = 'TX Mode'
strobes[0xE0] = 'FIFO Write pointer reset'
strobes[0xF0] = 'FIFO Read pointer reset'

#
# Register string definitons
# Can be overwritten by function to do further processing
#
regs = {}
regs[0x00] = 'Mode Register'
regs[0x01] = 'Mode Control Register'
regs[0x02] = 'Calibration Control Register'
regs[0x03] = 'FIFO Register I'
regs[0x04] = 'FIFO Register II'
regs[0x05] = 'FIFO DATA Register'
regs[0x06] = 'ID DATA Register'
regs[0x07] = 'RC OSC Register I'
regs[0x08] = 'RC OSC Register II'
regs[0x09] = 'RC OSC Register III'
regs[0x0A] = 'CKO Pin Control Register'
regs[0x0B] = 'GIO1 Pin Control Register I'
regs[0x0C] = 'GIO2 Pin Control Register II'
regs[0x0D] = 'Clock Register'
regs[0x0E] = 'Data Rate Register'
regs[0x0F] = 'PLL Register I'
regs[0x10] = 'PLL Register II'
regs[0x11] = 'PLL Register III'
regs[0x12] = 'PLL Register IV'
regs[0x13] = 'PLL Register V'
regs[0x14] = 'TX Register I'
regs[0x15] = 'TX Register II'
regs[0x16] = 'Delay Register I'
regs[0x17] = 'Delay Register II'
regs[0x18] = 'RX Register'
regs[0x19] = 'RX Gain Register I'
regs[0x1A] = 'RX Gain Register II'
regs[0x1B] = 'RX Gain Register III'
regs[0x1C] = 'RX Gain Register IV'
regs[0x1D] = 'RSSI Threshold Register'
regs[0x1E] = 'ADC Control Register'
regs[0x1F] = 'Code Register I'
regs[0x20] = 'Code Register II'
regs[0x21] = 'Code Register III'
regs[0x22] = 'IF Calibration Register I'
regs[0x23] = 'IF Calibration Register II'
regs[0x24] = 'VCO current Calibration Register'
regs[0x25] = 'VCO Single band Calibration Register I'
regs[0x26] = 'VCO Single band Calibration Register II'
regs[0x27] = 'Battery detect Register'
regs[0x28] = 'TX test Register'
regs[0x29] = 'Rx DEM test Register I'
regs[0x2A] = 'Rx DEM test Register II'
regs[0x2B] = 'Charge Pump Current Register'
regs[0x2C] = 'Crystal test Register'
regs[0x2D] = 'PLL test Register'
regs[0x2E] = 'VCO test Register I'
regs[0x2F] = 'VCO test Register II'
regs[0x30] = 'IFAT Register'
regs[0x31] = 'RScale Register'
regs[0x32] = 'Filter test Register'

#
# Custom packet processing functions
#
def processRSSI(packet):
	rString = ''

	if (packet[0] & READ_BIT) != 0:
		rString += 'RSSI = ' + format((1.2 * packet[1]) / 256.0, "0.3f") + 'V'
	else:
		rString += 'RSSI Thershld Register [' + format(packet[1], '02X') + ']'

	return rString

# Overwrite RSSI register function
regs[0x1D] = processRSSI

#
# Generic packet processing functions
#
def processPacket(packetString):
	packet = array.array('B')

	rString = ''

	# Make byte array instead of strings for actual processing
	for byte in packetString.split():
		packet.append(int(byte,16))

	# Strip read bit so we can use the regs[] array
	reg = packet[0] & ~READ_BIT

	if (packet[0] & READ_BIT) != 0:
		rString += 'R '
	else:
		rString += 'W '

	if reg in regs:
		# 
		# Check if we have a special packet processing function for this register
		# otherwise just print the name/description and values
		# 
		if hasattr(regs[reg], '__call__'):
			rString += regs[reg](packet)
		else:
			rString += regs[reg] + ' ['
			
			for byteIndex in range(1, len(packet)):
				rString += format(packet[byteIndex], '02X') + ' '
			
			rString = rString.strip()
			rString += ']'
	else:
		rString += '- unknown register! ' + format(reg, '02X')
	
	return rString

def processStrobe(strobe):
	return ('S ' + strobes[strobe])

#
# 'public' functions
#

# 
# decodeSPIPacket
# 
# packetString should be a string of space separated bytes
# Examples:
# '26 3b'
# '05 01 82 96 d5 18 f6 00 00 00 00 00 00 00 00 00 04 '
# 
# The return value is a string with the decoded packet
# 
def decodeSPIPacket(packetString):

	if len(packetString) == 0:
		return ''
	
	firstByte = int(packetString.split()[0], 16)
	
	if (firstByte & STROBE_BIT) != 0:
		return processStrobe(firstByte)
	else:
		return processPacket(packetString)

if __name__ == '__main__':
	# TODO - process lines from a previously processed file?
	print('Standalone operation not yet implemented...')
