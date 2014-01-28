#!/usr/bin/env python
# See python libs: PyBBIO and XBee-python
# Example command format: 1-H,4-L,2-H.
from xbee import XBee
import serial

CYCLE = 255
HIGH_PWM = 1 * CYCLE 
LOW_PWM = int(.25 * CYCLE)
ARG_SEP = ","
VAL_SEP = "-"
END_MSG = "."

# Continuously read and print packets
def changePWM(pin, duty):
	print "Pin is: " + pin
	if duty  ==  "H":
		print HIGH_PWM
	else:
		print LOW_PWM 

def iterateValues(line):
	vals = line.split(ARG_SEP)
	
	for v in vals:
		s1 = v.split(VAL_SEP)
		changePWM(s1[0], s1[1])

# Connect to the bluetooth serial.			
ser = serial.Serial('/dev/ttyUSB0', 9600)
xbee = XBee(ser)

# Continuously read all characters, when hit an END_MSG char, send to parse.
line = ""
while True:
	try:
		char = xbee.wait_read_frame()
		if char == END_MSG:
			iterateValues(line)
			line = ""
		else:
			line += char
	except KeyboardInterrupt:
		break
        
ser.close()


