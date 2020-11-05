from sys import argv, exit
from parse import parse

if len(argv) != 2:
    print(f'usage: {argv[0]} <xml-file>')
    exit(1)

xmlFile = argv[1]
plugin = parse(xmlFile)

def output_cmake_var(cmake_name, value):
    print(f'set({cmake_name} "{value}")')

output_cmake_var('PLUGIN_NAME', plugin.name)
if plugin.short_name:
    output_cmake_var('PLUGIN_SHORT_NAME', plugin.short_name)
