#!/usr/bin/python2

#   _^_   Smart_Home project
#  /|o|\      (c) 2015
#  =====

import socket
import sys
import threading
import time
import Tkinter as tk

HOST = '0.0.0.0'
PORT = 5005

KITCHEN_LED = '1.1.20.1'
KITCHEN_T_SENSOR = '1.1.10.1'

KITCHEN_TEMPERATURE_DEFAULT = 25 # C

# main window
top = tk.Tk()

# UI values
kitchen_led_left  = tk.IntVar()
kitchen_led_main  = tk.IntVar()
kitchen_led_right = tk.IntVar()

kitchen_temperature = tk.StringVar()

# To close communicate thread
run_communicate = True

# Translate string to int 
def bytes2int(str):
    return int(str.encode('hex'), 16)
# Translate hex to bytes array in string
def hex2bytes(h):
    if len(h) > 1 and h[0:2] == '0x':
        h = h[2:]
    if len(h) > 1 and h[-1] == 'L':
        h = h[:-1]
    while len(h) < 8: #4 bytes = 8 hex
        h = "0" + h
    return h.decode('hex')
# Translate int to 
def int2bytes(i):
    h = hex(i)
    print >>sys.stderr, h
    return hex2bytes(h)
# Get answer pkg
def get_answer_pkg(values):
    answer = ""
    for value in values:
        answer += int2bytes(value)
    return answer

# Thread function comunicates with the coordinator.
def communicate():
    # Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    # Bind the socket to the port
    server_address = (HOST, PORT)
    print >> sys.stderr, 'starting up on %s port %s' % server_address
    sock.bind(server_address)

    while True:
        # We need timeout to close thread on application exiting
        sock.settimeout(1) # second
        try:
            data, address = sock.recvfrom(4096)
            print >> sys.stderr, 'received %s bytes from %s' % (len(data), address)
            print >> sys.stderr, data

            answer = None

            if address[0] == KITCHEN_LED:
                # write options
                kitchen_led_left.set(bytes2int(data[0:4]))
                kitchen_led_main.set(bytes2int(data[4:8]))
                kitchen_led_right.set(bytes2int(data[8:12]))
                
                # read options
                values = [kitchen_led_left.get(), kitchen_led_main.get(), kitchen_led_right.get(), 0, 0, 0, 0, 0]
                answer = get_answer_pkg(values)

            if address[0] == KITCHEN_T_SENSOR:
                # read option
                values = [int(kitchen_temperature.get()), 0, 0, 0, 0, 0, 0, 0]
                answer = get_answer_pkg(values)
            
            if answer is not None:
                sent = sock.sendto(answer, address)
                print >> sys.stderr, 'sent %s bytes back to %s' % (sent, address)

        except socket.timeout:
            #print >>sys.stderr, "timeout 1 sec"
            pass

        #print >>sys.stderr, "."
        
        if not run_communicate:
            print >>sys.stderr, "Thread must finish"
            break

# On_closing main windows
def on_closing_windows():
    global run_communicate
    print >>sys.stderr, "on_closing"
    top.destroy()
    run_communicate = False
    time.sleep(2) # wait thread finishing
    sys.exit(0)


top.wm_title("Smart_Home: Slave simulation")
top.wm_protocol("WM_DELETE_WINDOW", on_closing_windows)

###
device = "kitchen_led (%s)" % (KITCHEN_LED) 
label = tk.Label(top, text = device)
label.pack()
#
frame = tk.Frame(top)
frame.pack()
left  = tk.Checkbutton(frame, text = "left", variable = kitchen_led_left)
left.pack(side = tk.LEFT)
main  = tk.Checkbutton(frame, text = "main", variable = kitchen_led_main)
main.pack(side = tk.LEFT)
right = tk.Checkbutton(frame, text = "right", variable = kitchen_led_left)
right.pack(side = tk.LEFT)

###
device = "kitchen_t_sensor (%s)" % (KITCHEN_T_SENSOR) 
label2 = tk.Label(top, text = device)
label2.pack()
#
entry = tk.Entry(top, textvariable = kitchen_temperature, width = 2)
kitchen_temperature.set(KITCHEN_TEMPERATURE_DEFAULT)
entry.pack()

# Run communication thread
comm_thread = threading.Thread(name = 'communication', target = communicate)
comm_thread.start()
# Run window
top.mainloop()
