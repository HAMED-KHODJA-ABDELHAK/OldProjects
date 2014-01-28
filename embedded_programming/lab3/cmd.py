#!/usr/bin/env python
# Command format: t-240,r-220...

ARG_SEP = ","
VAL_SEP = "-"

def cast_string(s):
    if not isinstance(s, str):
        return str(s)
    if s.isdigit():
        return int(s)
    try:
        return float(s)
    except:
        return s

# Substitute below, pwmWrite for the applicable commands.
def changePWM(pin, val):
	val = cast_string(val)
	
	print (val + 1)
	if pin == "r":
		print "ROLL"
	elif pin == "t":
		print "THROTTLE"
	elif pin == "y":
		print "YAW"
	elif pin == "p":
		print "PITCH"
	else:
		print "BADCMD"

# Lowercase the line, split on the arg separator then call changePWM with each pair.
def processCmd(line):
	line = line.lower()
	vals = line.split(ARG_SEP)
	
	for v in vals:
		s1 = v.split(VAL_SEP)
		pin = s1[0]
		val = s1[1]
		changePWM(pin, val)

# Continuously read all characters, when hit an END_MSG char, send to parse.
line = ""
while True:
	line = raw_input()
   	if line.strip() == "": # empty line signals stop
		break 
	else:
		processCmd(line)


