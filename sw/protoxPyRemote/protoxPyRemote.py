#!/usr/bin/env python
#
# Very quick program that gets DS3 control input (via usb) or DS4 input
# via bluetooth and sends it to my protoX controller
#
import pygame, sys, time
from pygame.locals import *
import serial
import threading
import sys

pygame.init()
pygame.joystick.init()
controller = pygame.joystick.Joystick(0)
controller.init()
screen = pygame.display.set_mode((400,300))
pygame.display.set_caption('protoxController')

interval = 0.05

throttleAxis = 1
rollAxis = 2
pitchAxis = 3
yawAxis = 0

startButton = 3

class serialReadThread(threading.Thread):
	def __init__(self, inStream):
		super(serialReadThread, self).__init__()
		self.stream = inStream
		self.running = 1

	def run(self):
		while self.running:
			try:
				line = self.stream.readline(50)
				if line:
					print line
			except serial.SerialException:
				print "serial error"

# 
# Convert -1.0 to 1.0 controller input to a 8 bit integer value 
# 
def getAxes():
	throttle = int(controller.get_axis(throttleAxis) * -128)
	if(throttle < 0):
		throttle = 0
	roll = int(controller.get_axis(rollAxis) * -64 + 128)
	pitch = int(controller.get_axis(pitchAxis) * 64 + 128)
	yaw = int(controller.get_axis(yawAxis) * 64 + 128)

	return [throttle, pitch, yaw, roll]

if len(sys.argv) < 2:
	print 'Usage: ', sys.argv[0], '/path/to/serial/device'
	sys.exit()

streamFileName = sys.argv[1]

stream = serial.Serial(streamFileName)

# Start readThread as daemon so it will automatically close on program exit
readThread = serialReadThread(stream)
readThread.daemon = True
readThread.start()

connecting = True

stream.write('help\n')
stream.write('init\n')
stream.write('remote start\n')

# 
# TODO - change firmware to print something when paired
# 		so we don't have to manually press start to continue
# 
#  Wait until the protoX pairs (lights stop blinking), then press start to continue
# 
while connecting:
	if controller.get_button(startButton) == 1:
		connecting = False

	for event in pygame.event.get():
		if event.type == QUIT:
			loopQuit = True
		elif event.type == pygame.KEYDOWN:
			if event.key == pygame.K_ESCAPE:
				loopQuit = True
	time.sleep(0.01)

# 
# Main control loop
# 
loopQuit = False
while loopQuit == False:

	[throttle, pitch, yaw, roll] = getAxes()

	print('remote ' + str(throttle) + ' ' + str(pitch) + ' ' + str(yaw) + ' ' + str(roll) + ' ' + '\n')
	stream.write('remote ' + str(throttle) + ' ' + str(pitch) + ' ' + str(yaw) + ' ' + str(roll) + ' ' + '\n')
	
	for event in pygame.event.get():
		if event.type == QUIT:
			loopQuit = True
		elif event.type == pygame.KEYDOWN:
			if event.key == pygame.K_ESCAPE:
				loopQuit = True

	# pygame.display.update()
	time.sleep(interval)

pygame.quit()
sys.exit()
