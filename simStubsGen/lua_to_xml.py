import argparse
import re
from parse import parse

parser = argparse.ArgumentParser(description='Generate xml from lua functions annotated with doc-strings.')
parser.add_argument('xml_file', type=str, default=None, help='the callbacks.xml file')
parser.add_argument('lua_file', type=str, default=None, help='the input lua file')
parser.add_argument('out_xml', type=str, default=None, help='the output lua.xml file')
args = parser.parse_args()

if args is False:
    SystemExit

plugin = parse(args.xml_file)

fun = None
ins, outs = [], []
cats = []

with open(args.out_xml, 'w') as fout:
    fout.write('<?xml version="1.0" encoding="UTF-8" standalone="no" ?>\n')
    fout.write(f'<plugin name="{plugin.name}"')
    if plugin.short_name:
        fout.write(f' short-name="{plugin.short_name}"')
    if plugin.version:
        fout.write(f' version="{plugin.version}"')
    fout.write('>\n')

    def processTableType(t):
        if '.' in t:
            table, itemtype = t.split('.')
            if table == 'table':
                return 'table" item-type="%s' % itemtype
        return t

    def output():
        if fun:
            f, fdesc = fun
            fout.write('    <command name="{}">\n'.format(f))
            fout.write('        <description>{}</description>\n'.format(fdesc))
            if cats:
                fout.write('        <categories>\n')
                for cat in cats:
                    fout.write('            <category name="{}" />\n'.format(cat))
                fout.write('        </categories>\n')
            fout.write('        <params>\n')
            for (t, n, d) in ins:
                t = processTableType(t)
                fout.write('            <param name="{}" type="{}">\n'.format(n, t))
                fout.write('                <description>{}</description>\n'.format(d))
                fout.write('            </param>\n')
            fout.write('        </params>\n')
            fout.write('        <return>\n')
            for (t, n, d) in outs:
                t = processTableType(t)
                fout.write('            <param name="{}" type="{}">'.format(n, t))
                fout.write('                <description>{}</description>\n'.format(d))
                fout.write('            </param>\n')
            fout.write('        </return>\n')
            fout.write('    </command>\n')

    with open(args.lua_file, 'r') as f:
        for line in f:
            m = re.match(r'\s*--\s*@([^\s]+)\s+(.*)$', line)
            if m:
                tag, line = map(lambda s: s.strip(), m.groups())
                if tag == 'fun':
                    m = re.match(r'([^\s]+)\s*(.*)$', line)
                    if m:
                        name, description = map(lambda s: s.strip(), m.groups())
                        fun = (name, description)
                elif tag in ('arg', 'ret'):
                    m = re.match(r'([^\s]+)\s+([^\s]+)\s*(.*)$', line)
                    if m:
                        dtype, name, description = map(lambda s: s.strip(), m.groups())
                        if tag == 'arg':
                            ins.append((dtype, name, description))
                        elif tag == 'ret':
                            outs.append((dtype, name, description))
                elif tag == 'cats':
                    cats = [x.strip() for x in line.split(',')]
            else:
                output()
                fun = None
                ins, outs = [], []
                cats = []
        output()

    fout.write('</plugin>\n')

