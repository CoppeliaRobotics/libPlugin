import argparse
import json
import sys
from parse import parse

parser = argparse.ArgumentParser(description='Generate API index.')
parser.add_argument('xml_file', type=str, default=None, help='the (merged) XML file')
parser.add_argument('out_file', type=str, default=None, help='the output JSON file')
args = parser.parse_args()

if args is False:
    SystemExit

plugin = parse(args.xml_file)

if not plugin.short_name:
    raise AttributeError('plugin short-name not defined')

mapping = {}

htmFile = f'sim{plugin.short_name}.htm'

for cmd in plugin.commands:
    mapping[f'{cmd.name}'] = f'{htmFile}#{cmd.name}'
for enum in plugin.enums:
    mapping[f'{enum.name}'] = f'{htmFile}#enum:{enum.name}'
    for item in enum.items:
        mapping[f'{enum.name}.{item.name}'] = f'{htmFile}#enum:{enum.name}'
for scrfun in plugin.script_functions:
    mapping[f'{scrfun.name}'] = f'{htmFile}#scriptfun:{scrfun.name}'
for struct in plugin.structs:
    mapping[f'{struct.name}'] = f'{htmFile}#struct:{struct.name}'

with open(args.out_file, 'w') as f:
    json.dump({f'sim{plugin.short_name}': mapping}, f, indent=4)
