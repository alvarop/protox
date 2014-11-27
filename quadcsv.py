#!/usr/bin/python

import csv
import sys
import array

# Time it takes for 8 bits to be sent
WORD_TIME = 0.00003128

# If inter-byte delay is longer than this, consider it a different packet
CONSECUTIVE_BYTE_DELAY = 6.0

largePacketsOnly = False

if len (sys.argv) == 1:
	print("usage: quadcsv.py filename.csv")
	sys.exit(0)

# Option to only print larger packets (just for testing)
if len(sys.argv) > 2:
	if sys.argv[2] == 'l':
		largePacketsOnly = True

with open(sys.argv[1], 'rb') as csvfile:
	reader = csv.reader(csvfile)

	# Ignore first line
	reader.next() 
	
	prevtime = 0.0

	# print starttime
	current_packet = array.array('B')
	old_packet = []
	dupe_count = 0

	for row in reader:
		# Timestamp is for start of byte, so we need to take packet time into account (although it varies sometimes...)
		byte_delay = (float(row[0]) - prevtime - WORD_TIME) * 1e6
		
		if byte_delay > CONSECUTIVE_BYTE_DELAY:
			if old_packet == current_packet:
				dupe_count += 1
			else:
				if (largePacketsOnly == True and len(old_packet) > 10) or (largePacketsOnly == False):	
					for byte in old_packet:
							sys.stdout.write(format(byte, '02x') + ' ')

					if dupe_count > 0:
						sys.stdout.write(' (Repeated ' + str(dupe_count) + ' times)')
					
					sys.stdout.write('\n')


				dupe_count = 0

			old_packet = current_packet
			current_packet = array.array('B')

		current_packet.append(int(row[2], 16))			

		prevtime = float(row[0])
