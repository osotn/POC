#!/usr/bin/python2

#   _^_   Smart_Home project
#  /|o|\      (c) 2015
#  =====

import sys
import argparse
import xml.etree.ElementTree as etree

# Input argument parser
parser = argparse.ArgumentParser()
parser.add_argument('-d', '--device', nargs='+')
parser.add_argument('-a', '--addr', nargs='+')
parser.add_argument('-f', '--file', default='../etc/cfg.xml')
args = parser.parse_args()

# XML paser
tree = etree.parse(args.file)
root = tree.getroot()
devices = root.findall('device')

# There are TWO input format:
#  1. -d <device_name> [<option_name> <option_value_name>]
#  2. -a <device_ip> [<option_id> <option_value>]
#
# Output:
#  1. OK <device_ip> [<option_id>=<option value>]
#  2. OK [<option_name> <option_value_name>]

if (args.device is None and args.addr is None) or (args.device is not None and args.addr is not None):
  result = "FAILED Define neither 'device' or 'addr' option"
  sys.exit(1)
   
name2value = True
if args.device is None:
  name2value = False

result = "OK"

# Device's name or ip
if name2value:
  device_id = args.device[0]
  args_opts = args.device[1:]
  dev_key = 'name'
  opt_key = 'name'
  opt_out = 'id'
  token = '='
else:
  device_id = args.addr[0]
  args_opts = args.addr[1:]
  dev_key = 'ip'
  opt_key = 'id'
  opt_out = 'name'
  token = ' '

# by devices
the_device = None
for device in devices:
  if device.attrib[dev_key] == device_id:
    the_device = device

if the_device is None:
  result = "FAILED Device %s is absent in %s" % (device_id, args.file)
  print result; sys.exit(1)

if name2value:
  ip = the_device.attrib['ip']
  if ip is None:
    result = "FAILED IP is absent for device %s in %s" % (device_id, args.file)
    print result; sys.exit(1)
  
  result += " %s" % (ip) 

options = {}
opts = the_device.findall('opt')
for opt in opts:
  options[opt.attrib[opt_key]] = opt

for i in range(0, len(args_opts), 2):
  # option name/id
  option_in = args_opts[i]
  if not option_in in options:
    # ignore unknown option
    break
  opt = options[option_in]
  option_out = opt.attrib[opt_out]
  if option_out is None:
    result = "FAILED Tag '%s is absent for option %s of device %s in %s" % (opt_out, option_in, device_id, args.file)
    print result; sys.exit(1)
  # value
  opt_values = {}
  values = opt.find("values")
  for k in values.attrib:
    if name2value:
      opt_values[k] = values.attrib[k]
    else:
      opt_values[values.attrib[k]] = k

  if (i+1) >= len(args_opts):
    result = "FAILED Option without value"
    print result; sys.exit(1)

  value_in = args_opts[i+1]
  if not value_in in opt_values:
    result = "FAILED Value %s is absent in option %s for device %s in %s" % (value_in, option_out, device_id, args.file)
    print result; sys.exit(1)

  value = opt_values[value_in]
  result += " %s%s%s" % (option_out, token, value)
  
print result; sys.exit(0)
