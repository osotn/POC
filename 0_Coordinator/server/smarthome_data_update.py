#!/usr/bin/python2

#   _^_   Smart_Home project
#  /|o|\      (c) 2015
#  =====

import sys
import argparse
import xml.etree.ElementTree as etree

parser = argparse.ArgumentParser()
parser.add_argument('-a', '--addr')
parser.add_argument('-w', '--write', nargs='*')
parser.add_argument('-f', '--file', default='../etc/data.xml')
args = parser.parse_args()

tree = etree.parse(args.file)
root = tree.getroot()

slaves = root.findall('slave')

# TODO output format description
result = "OK"

the_slave = None
for slave in slaves:
  if ('addr' in slave.attrib) and (slave.attrib['addr'] == args.addr):
    the_slave = slave

if args.write is None:
  # READ
  if the_slave is None:
    result = "FAILED IP address %s is absent in %s" % (args.addr, args.file)
    sys.stdout.write(result); sys.exit(1)

  options = the_slave.findall('opt')
  # format: [<opt_id>=<opt_value>]
  for option in options:
    result +=" %s=%s" % (option.attrib['id'], option.attrib['value'])
else:
  # WRITE
  if the_slave is None:
    the_slave = etree.SubElement(root, "slave")
    the_slave.attrib['add'] = args.addr

  the_option = None
  for i in range(0, len(args.write), 2):
    the_option = None
    options = the_slave.findall('opt')
    for option in options:
      if option.attrib['id'] == args.write[i]:
        the_option = option

    if the_option is None:
      the_option = etree.SubElement(the_slave, "opt")
      the_option.attrib['id'] = args.write[i]

    the_option.attrib['value'] = args.write[i+1]  
  
  tree.write(args.file)
    
sys.stdout.write(result)
