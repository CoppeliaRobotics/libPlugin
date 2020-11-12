import argparse
import sys
from parse import parse

parser = argparse.ArgumentParser(description='Generate CMake metadata.')
parser.add_argument('xml_file', type=str, default=None, help='the (merged) XML file')
parser.add_argument('out_file', type=str, default=None, help='the output CMake file')
args = parser.parse_args()

if args is False:
    SystemExit

plugin = parse(args.xml_file)

def output_cmake_var(f, cmake_name, value):
    f.write(f'set({cmake_name} "{value}")\n')

with open(args.out_file, 'wt') as f:
    output_cmake_var(f, 'PLUGIN_NAME', plugin.name)
    output_cmake_var(f, 'PLUGIN_VERSION', plugin.version)
    if plugin.short_name:
        output_cmake_var(f, 'PLUGIN_SHORT_NAME', plugin.short_name)
