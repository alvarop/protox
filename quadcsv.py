#!/usr/bin/python

import csv
import sys
import array

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
					for byte in old_packet:
							sys.stdout.write(format(byte, '02x') + ' ')

					if dupe_count > 0:
						sys.stdout.write(' (Repeated ' + str(dupe_count) + ' times)')
					
					sys.stdout.write('\n')

				dupe_count = 0

			packet_index = current_index
			old_packet = current_packet
			current_packet = array.array('B')

		current_packet.append(int(row[2], 16))			

		prevtime = float(row[0])
