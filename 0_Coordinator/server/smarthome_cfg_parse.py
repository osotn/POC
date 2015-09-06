#!/usr/bin/python2

#   _^_   Smart_Home project
#  /|o|\      (c) 2015
#  =====

import argparse
import xml.etree.ElementTree as etree

parser = argparse.ArgumentParser()
parser.add_argument('-d', '--device')
parser.add_argument('-f', '--file', default='../etc/cfg.xml')
args = parser.parse_args()

tree = etree.parse(args.file)
root = tree.getroot()

devices = root.findall('device')

# TODO output format description
result = ""

for device in devices:
  if device.attrib['name'] == args.device:
    options = device.findall('opt')
    # format:  <dev_name> <ip> <option number>
    result += "%s %s %d" % (args.device, device.attrib['ip'], len(options))
    for option in options:
      # option format: <name> <id> <default> <type> [<name> <value>]
      result +="\n%s %s %s %s" % (option.attrib['name'], option.attrib['id'], option.attrib['default'], option.attrib['type'])
      values = option.find('values')
      for key in values.attrib:
        result += " %s %s" % (key, values.attrib[key])

print result
