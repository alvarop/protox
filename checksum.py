#!/usr/bin/python

import sys
import array

# Testing proto-x checksum
# 
# Sample packets:
#
# 02 46 e4 59 10 3f 00 00 00 00 00 00 00 00 00 2c 
# 03 46 e4 59 10 3f 00 00 00 00 00 00 00 00 00 2b 
# 04 46 e4 59 10 3f 00 00 00 00 00 00 00 00 00 2a 
# 01 46 e4 59 10 3f 00 00 00 00 00 00 00 00 00 2d 
# 02 46 e4 59 10 3f 03 07 00 00 00 00 00 00 00 22 
#
# 20 00 00 00 80 00 6c 00 80 06 19 00 00 00 00 55 
# 20 00 00 00 80 00 6c 00 80 06 19 00 00 00 00 55 
# 20 00 00 00 80 00 6c 00 80 06 19 00 00 00 00 55 
# 20 00 00 00 80 00 6f 00 80 06 19 00 00 00 00 52 
# 20 00 00 00 80 00 74 00 80 06 19 00 00 00 00 4d 
# 20 00 00 00 80 00 76 00 80 06 19 00 00 00 00 4b 
# 20 00 00 00 80 00 78 00 80 06 19 00 00 00 00 49 
# 20 00 00 00 80 00 7a 00 80 06 19 00 00 00 00 47 
#  
# The last byte changes whenever any single byte changes
# If a byte changes by 2, the checksum changes by -2
#
# Looks like the checksum is computed by addingf the 2's complement of each byte
# You can just subtract each byte (starting from 0). The resulting value is the checksum
# If the checksum is included in the calculation, the result is zero
#

def get_checksum(packet):
	checksum = 0
	for byte in packet:
		checksum = (checksum - int(byte,16)) & 0xFF

	return checksum

# Packet without checksum (result should be 0x22)
packet = "02 46 e4 59 10 3f 03 07 00 00 00 00 00 00 00"
print(format(get_checksum(packet.split()), '02x'))

# Packet with checksum (result should be 0x00)
packet = "20 00 00 00 80 00 af 00 80 06 19 00 00 00 00 12"
print(format(get_checksum(packet.split()), '02x'))

