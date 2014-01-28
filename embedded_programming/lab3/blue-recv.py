#!/usr/bin/env python

# Simple example to connect a datagram bluetooth socket.
import bluetooth

# Required to be odd number in range 0x1001-0x8FFF
PORT = 0x1001
BD_ADDR = "01:23:45:67:89:AB" # Change this addr to device. 

sock = bluetooth.BluetoothSocket(bluetooth.L2CAP)
sock.connect((BD_ADDR, PORT))

sock.send("Hello")

sock.close()
