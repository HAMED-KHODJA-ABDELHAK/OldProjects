#!/usr/bin/env python

# Example server side of socket connection.
import bluetooth

# Required to be odd number in range 0x1001-0x8FFF
PORT = 0x1001

s_sock = bluetooth.BluetoothSocket(bluetooth.L2CAP)
s_sock.bind(("", PORT))
s_sock.listen(1)

c_sock, address = s_sock.accept()
print "Accepted connection from ", address

data = c_sock.recv(64)
print "Received [%s]" % data

c_sock.close()
s_sock.close()
