#!/usr/bin/python2

#   _^_   Smart_Home project
#  /|o|\      (c) 2015
#  =====
 
import cgi
import socket
import sys

# Output must start with this string for correct cgi server running
result = "Content-type: text/html\n\n"

# cgi parse
form = cgi.FieldStorage()
if not form.has_key("dev"):
    result += "ERROR: Field 'dev' is not filled"
    sys.stdout.write(result); sys.exit(1)

dev = form.getvalue('dev')

opts = {}
for key in form:
    if key != 'dev':
        opts[key] = form.getvalue(key)

# send command to the coordinator
cmd = dev
for key in opts:
   cmd += " %s %s" % (key, opts[key])

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_address = ('localhost', 4000)

try:
    # Send data
    sent = sock.sendto(cmd, server_address)
    # Receive response
    data, server = sock.recvfrom(4096)
finally:
    sock.close()

result += dev + ' ' + data

sys.stdout.write(result)
