import argparse
import re
from parse import parse

parser = argparse.ArgumentParser(description='Generate the deprecated.txt for CoppeliaSim.')
parser.add_argument('xml_file', type=str, default=None, help='the callbacks.xml file')
parser.add_argument('out_file', type=str, default=None, help='the output deprecated.txt file')
args = parser.parse_args()

if args is False:
    SystemExit

plugin = parse(args.xml_file)

functions = []
variables = []

with open(args.out_file, 'w') as ftxt:
    for cmd in plugin.commands:
        if plugin.short_name:
            fold = '{}{}'.format(plugin.command_prefix, cmd.name)
            fnew = 'sim{}.{}'.format(plugin.short_name, cmd.name)
            ftxt.write('{},{},\n'.format(fold, fnew))

    for enum in plugin.enums:
        for item in enum.items:
            if plugin.short_name:
                vold = 'sim_{}_{}{}'.format(plugin.name.lower(), enum.item_prefix, item)
                vnew = 'sim{}.{}.{}'.format(plugin.short_name, enum.name, item)
                ftxt.write('{},{},\n'.format(vold,vnew))

