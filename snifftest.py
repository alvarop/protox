#!/usr/bin/python

import sys
import io
import os
import threading
import Queue
import signal
import serial
import array
from time import gmtime, strftime, sleep

ch = '0'
quadCount = 0
remoteCount = 0

def signal_handler(signal, frame):
	print('exiting')
	sys.exit(0)

def packetValid(packet):
	checksum = 0
	for byte in packet:
		checksum = (checksum - int(byte,16)) & 0xFF

	return (checksum == 0)

# 
# Decode protoX packets
# Only decoding 0x20 (control) packets right now
# 
def decodePacket(packet):
	rString = ''

	if(packet[0] == 0x20):
		# Throttle
		rString += 'T: ' + format(packet[2]/255.0 * 100, '2.0f') + ' '

		# Rudder (Yaw left/right)
		rString += 'R: ' + format((packet[4] - 128)/127.0 * 100, '2.0f') + ' '

		# Elevator (Pitch forward/back)
		rString += 'E: ' + format(-(packet[6] - 128)/127.0 * 100, '2.0f') + ' '

		# Aileron (Roll left/right)
		rString += 'A: ' + format(-(packet[8] - 128)/127.0 * 100, '2.0f') + ' '
		
		if((packet[9] & 0x08)):
			rString += 'Flips Enabled'
		else :
			rString += 'Flips Disabled'
	# else:
		# Ignore reply packets for now

	return rString

# 
# First pairing stage processing
# 
def pairingS1(line):
	global stateFn
	packetBytes = line.split()

	if len(packetBytes) == 16 and packetValid(packetBytes) and int(packetBytes[0], 16) == 4:
		sys.stdout.write('pairingS1 - New ID: ')
		sys.stdout.write(packetBytes[2] + ' ' + packetBytes[3] + ' ' + packetBytes[4] + ' ' + packetBytes[5] + '\n')
		writeThread.write('remote id ' + packetBytes[2] + ' ' + packetBytes[3] + ' ' + packetBytes[4] + ' ' + packetBytes[5] + '\n')
		stateFn = pairingS2
	elif len(packetBytes) == 16 and packetValid(packetBytes) and int(packetBytes[0], 16) == 0x20:
		print('must have missaed a packet, go to decoding')
		stateFn = decoding
	elif len(packetBytes) == 16 and packetValid(packetBytes) and int(packetBytes[0], 16) == 0x01:
		print('must have missaed a packet, go back to sniffing')
		stateFn = sniffing
	else:
		sys.stdout.write('pairingS1 - ')
		sys.stdout.write(line)

# 
# Second pairing stage processing
# 
def pairingS2(line):
	global stateFn
	global quadCount
	global remoteCount

	packetBytes = line.split()
	
	sys.stdout.write('pairingS2 - ')
	sys.stdout.write(line)

	if len(packetBytes) == 16 and packetValid(packetBytes) and int(packetBytes[0], 16) == 0xA:
		sys.stdout.write('pairingS2 - ' + packetBytes[1] + '\n')
		quadCount = int(packetBytes[1], 16)
	elif len(packetBytes) == 16 and packetValid(packetBytes) and int(packetBytes[0], 16) == 0x9:
		sys.stdout.write('pairingS2 - ' + packetBytes[2] + '\n')
		remoteCount = int(packetBytes[2], 16)
	elif len(packetBytes) == 16 and packetValid(packetBytes) and int(packetBytes[0], 16) == 0x20:
		print('must have missaed a packet, go to decoding')
		stateFn = decoding
	elif len(packetBytes) == 16 and packetValid(packetBytes) and int(packetBytes[0], 16) == 0x01:
		print('must have missaed a packet, go back to sniffing')
		stateFn = sniffing
	# else:
		# sys.stdout.write('pairingS2 - ')
		# sys.stdout.write(line)

	sys.stdout.write('pairingS2 - R: ' + str(remoteCount) + ' Q: ' + str(quadCount) + '\n')

	if remoteCount == 9 or quadCount == 9:
		writeThread.write('remote crcon\n')
		stateFn = decoding

# 
# Control packet decoding
# 
def decoding(line):
	global stateFn
	packetBytes = line.split()

	if len(packetBytes) == 16:
		packet = array.array('B')

		# Make byte array instead of strings for actual processing
		for byte in packetBytes:
			packet.append(int(byte,16))

		packetString = decodePacket(packet)
		if len(packetString) > 1 and packetValid(packetBytes):
			sys.stdout.write('decode - ' +  packetString + '\n')
	else:
		sys.stdout.write('decode - ' + line + '\n')

# 
# Sniffing state, waiting for quad to reply to beacon
# 
def sniffing(line):
	global stateFn
	packetBytes = line.split()
	
	if len(packetBytes) == 16 and packetValid(packetBytes) and int(packetBytes[0], 16) > 1:
		sys.stdout.write('sniffing - reply from quad. Begin pairing.\n')
		stateFn = pairingS1
	elif len(packetBytes) == 16 and not packetValid(packetBytes):
		sys.stdout.write('sniffing - CRC ERR\n')
	else:
		sys.stdout.write('sniffing - ')
		sys.stdout.write(line)

# 
# Searching state. Looking for remote beacon
# 
def searching(line):
	global stateFn
	global ch

	if line.startswith('Found remote in ch'):
		sys.stdout.write('searching - ')
		sys.stdout.write(line)

		ch = line.split()[4]
		print('Starting sniffer')
		stateFn = sniffing
		writeThread.write('remote sniff\n')
	else:
		sys.stdout.write('searching - ')
		sys.stdout.write(line)


def radioInit(line):
	global stateFn

	if line.startswith('Best channel is'):
		stateFn = searching
		writeThread.write('remote find\n')

	sys.stdout.write('radioInit - ' + line)

stateFn = radioInit

def processLine(line):
	stateFn(line)

#
# Read serial stream and add lines to shared queue
#
class serialReadThread(threading.Thread):
	def __init__(self, inStream,):
		super(serialReadThread, self).__init__()
		self.stream = inStream
		self.running = 1

	def run(self):
		while self.running:
			try:
				line = self.stream.readline(50);
				if line:
					processLine(line)
			except serial.SerialException:
				print('serial error')

#
# Write serial stream and add lines to shared queue
#
class serialWriteThread(threading.Thread):
	def __init__(self, outStream):
		super(serialWriteThread, self).__init__()
		self.stream = outStream
		self.running = 1

		self.outQueueLock = threading.Lock()
		self.outQueue = Queue.Queue()
		self.outDataAvailable = threading.Event() # Used to block write thread until data is available

	def run(self):
		while self.running:

			if not self.outQueue.empty():
				self.outQueueLock.acquire()
				line = self.outQueue.get()
				#print line
				self.stream.write(str(line))
				self.outQueueLock.release()
			else:
				self.outDataAvailable.wait()
				self.outDataAvailable.clear()

	def write(self, line):
		self.outQueueLock.acquire()
		self.outQueue.put(line)
		self.outQueueLock.release()
		self.outDataAvailable.set()

def processCommand(command):
	global stateFn
	
	command = command.rstrip()
	if command == 'reset':
		print('Reset!')
		stateFn = radioInit
		quadCount = 0
		remoteCount = 0
		writeThread.write('init\n')
		# writeThread.write('remote crcoff\n')
		
	elif command == 'h':
		writeThread.write('remote t 2\n')
		writeThread.write('remote hijack\n')
	else:
		print('unknown command \'' + command + '\'')
# 
#  Start here :D
# 
if not len(sys.argv) > 1:
	print('Usage: ' + sys.argv[0] + ' </dev/serialdevicename>')
	sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

print('Press Ctrl + C to exit')

stream = serial.Serial(sys.argv[1])

# Start readThread as daemon so it will automatically close on program exit
readThread = serialReadThread(stream)
readThread.daemon = True
readThread.start()

# Start writeThread as daemon so it will automatically close on program exit
writeThread = serialWriteThread(stream)
writeThread.daemon = True
writeThread.start()

writeThread.write('init\n')
# writeThread.write('remote crcoff\n')

while 1:
	sys.stdout.write('> ')
	command = sys.stdin.readline()
	processCommand(command)

sys.exit(0)
