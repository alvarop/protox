#!/usr/bin/python

import csv
import sys
import array
import a7105

# 
# Use this script to process Saleae Logic captured SPI data (from CSV) to more manageable chunks
# 
# It works *most* of the time, but we do get some split packets from time to time. (due to a bad CS signal)
# I also added duplicate packet detection so we don't print the same thing over and over
# 
# Input file looks like this:
# 4.406020080000000,41055,0x40,
# 4.406054800000000,41055,0x19,
# 4.406096400000000,41056,0x40,
# 4.406135600000000,41056,0x19,
# 4.406178720000000,41057,0xA0,
# 4.406216720000000,41058,0xE0,
# 4.406253680000000,41059,0x05,
# 4.406290560000000,41059,0x01,
# 4.406327600000000,41059,0x46,
# 4.406364720000000,41059,0xE4,
# 4.406401760000000,41059,0x59,
# 4.406438880000000,41059,0x10,
# 4.406476000000000,41059,0x3F,
# 4.406513040000000,41059,0x00,
# 4.406550160000000,41059,0x00,
# 4.406587280000000,41059,0x00,
# 4.406624320000000,41059,0x00,
# 4.406661440000000,41059,0x00,
# 4.406698560000000,41059,0x00,
# 4.406735680000000,41059,0x00,
# 4.406772720000000,41059,0x00,
# 4.406809840000000,41059,0x00,
# 4.406847040000000,41059,0x2D,
# 4.406884320000000,41060,0xD0,
# 4.406921760000000,41061,0x40,
# 4.406956480000000,41061,0x1B,
# 4.406995680000000,41062,0x40,
# 4.407030400000000,41062,0x1B,
# 
# Output looks like:
# 40 19  (Repeated 148 times)
# a0 
# e0 
# 05 01 46 e4 59 10 3f 00 00 00 00 00 00 00 00 00 2d 
# d0 
# 40 1b  (Repeated 26 times)
# 40 1a 
# c0 
# 40 19  (Repeated 148 times)
#

largePacketsOnly = False
decodePackets = False

if len (sys.argv) == 1:
	print("usage: quadcsv.py filename.csv")
	sys.exit(0)

# Option to only print larger packets (just for testing)
if len(sys.argv) > 2:
	if sys.argv[2] == 'l':
		largePacketsOnly = True

	if sys.argv[2] == 'd':
		decodePackets = True

with open(sys.argv[1], 'rb') as csvfile:
	reader = csv.reader(csvfile)

	# Ignore first line
	reader.next() 
	
	packet_index = 0

	# print starttime
	current_packet = array.array('B')
	old_packet = []
	dupe_count = 0

	for row in reader:
		
		# Sometimes row[1] is just ''
		try:
			current_index = int(row[1])
		except:
			current_index = -1
		
		if current_index != packet_index:
			if old_packet == current_packet:
				dupe_count += 1
			else:
				if (largePacketsOnly == True and len(old_packet) > 10) or (largePacketsOnly == False):	

					packetString = ""
					for byte in old_packet:
						packetString += format(byte, '02x') + ' '

					packetString = packetString.strip()

					if decodePackets == False:
						sys.stdout.write(packetString);
					else:
						sys.stdout.write(a7105.decodeSPIPacket(packetString))

					if dupe_count > 0:
						sys.stdout.write(' (Repeated ' + str(dupe_count) + ' times)')
					
					sys.stdout.write('\n')

				dupe_count = 0

			packet_index = current_index
			old_packet = current_packet
			current_packet = array.array('B')

		current_packet.append(int(row[2], 16))			

		prevtime = float(row[0])
