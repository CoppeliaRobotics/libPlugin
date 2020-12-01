import argparse
import re
import sys

parser = argparse.ArgumentParser(description='Generate calltips for Lua functions.')
parser.add_argument('long_plugin_name', type=str, default=None, help='the long pugin name')
parser.add_argument('short_plugin_name', type=str, default=None, help='the short pugin name')
parser.add_argument('lua_file', type=str, default=None, help='the input lua file')
parser.add_argument('out_file', type=str, default=None, help='the output .cpp file')
args = parser.parse_args()

if args is False:
    SystemExit

fun = None
ins, outs = [], []

with open(args.out_file, 'w') as fout:
    def output():
        if fun:
            f, fdesc = fun
            calltip = (','.join(x[0]+' '+x[1] for x in outs)+('=' if outs else '')+'sim'+args.short_plugin_name+'.'+f+'('+','.join(x[0]+' '+x[1] for x in ins)+')')
            if fdesc:
                calltip += '\\n\\n' + fdesc
            while calltip[-2:] == '\\n': calltip = calltip[:-2]
            fout.write('sim::registerScriptCallbackFunction("sim{}.{}@{}", "{}", NULL);\n'.format(args.short_plugin_name, f, args.long_plugin_name, calltip))

    def strip_table_type(s):
        return 'table' if s.startswith('table.') else s

    with open(args.lua_file, 'r') as f:
        for lineno, line in enumerate(f):
            lineno += 1
            m1 = re.match(r'\s*--\s*@(\w+)\b(.*)$', line)
            if m1:
                key = m1.group(1)
                rest = m1.group(2).strip()
                if key == 'fun':
                    m2 = re.match(r'(\w+)(|\s+.*)$', rest)
                    if m2:
                        fun = (m2.group(1), m2.group(2))
                    else:
                        print('%s:%d: bad arguments. must be: @fun <funcName> [description]' % (args.lua_file, lineno))
                        sys.exit(2)
                elif key in ('arg', 'ret'):
                    m2 = re.match(r'(\w+|table\.\w+)\s+(\w+)\s+(.*)$', rest)
                    if m2:
                        item = (strip_table_type(m2.group(1)), m2.group(2), m2.group(3))
                        (outs if key == 'ret' else ins).append(item)
                    else:
                        print('%s:%d: bad arguments. must be: @%s <type> <name> <description>' % (args.lua_file, lineno, key))
                        sys.exit(2)
            else:
                output()
                fun = None
                ins, outs = [], []
        output()

