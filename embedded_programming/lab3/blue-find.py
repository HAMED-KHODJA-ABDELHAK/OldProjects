#!/usr/bin/env python

# Uses the default bluetooth to scan and print all address/names.
import bluetooth

print "Searching for bluetooth devices."

nearby_devices = bluetooth.discover_devices(lookup_names = True)

print "Found %d devices" % len(nearby_devices)

for addr, name in nearby_devices:
	print "%s - %s" % (addr, name)

