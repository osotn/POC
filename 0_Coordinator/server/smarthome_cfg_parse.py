#!/usr/bin/python2

#   _^_   Smart_Home project
#  /|o|\      (c) 2015
#  =====

import sys
import argparse
import xml.etree.ElementTree as etree

# Input argument parser
parser = argparse.ArgumentParser()
parser.add_argument('-d', '--device', nargs='+', required=True)
parser.add_argument('-f', '--file', default='../etc/cfg.xml')
args = parser.parse_args()

# XML paser
tree = etree.parse(args.file)
root = tree.getroot()
devices = root.findall('device')

# Input format: -d <device_name> [<option_name> <option_value_name>]
# Output:       OK <device_ip> [<option_id> <option value>]
result = "OK "


device_name = args.device[0]

for device in devices:
  if device.attrib['name'] == device_name:
    the_device = device

if the_device is None:
  result = "FAILED Device %s is absent in %s" % (device_name, args.file)
  sys.exit(1)

ip = device.attrib['ip']
if ip is None:
  result = "FAILED IP is absent for device %s in %s" (device_name, args.file)
  print result; sys.exit(1)
    
result += "%s" % (ip) 

options = {}
opts = the_device.findall('opt')
for opt in opts:
  options[opt.attrib["name"]] = opt

for i in range(1, len(args.device), 2):
  # option name
  option_name = args.device[i]
  opt = options[option_name]  
  if opt is None:
    result = "FAILED Option %s is absent for device %s in %s" (option_name, device_name, args.file)
    print result; sys.exit(1)
  option_id = opt.attrib['id']
  if option_id is None:
    result = "FAILED Tag 'id' is absent for option %s of device %s in %s" (option_name, device_name, args.file)
    print result; sys.exit(1)
  # option value
  opt_values = {}
  values = opt.find("values")
  for k in values.attrib:
    opt_values[k] = values.attrib[k]
  if (i+1) >= len(args.device):
    result = "FAILED Option without value"
    print result; sys.exit(1)
  value_name = args.device[i+1]
  value = opt_values[value_name]
  if value is None:
    result = "FAILED Value %s is absent in option %s for device &s in %s" (value_name, option_name, device_name, args.file)
    print result; sys.exit(1)
  
  result += " %s=%s" % (option_id, value)
  
print result; sys.exit(0)
