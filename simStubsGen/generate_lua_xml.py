import argparse
import re
import sys
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

    def outputParam(t, n, d):
        fout.write(f'            <param name="{n}" type="{t["type"]}"')
        if 'item_type' in t:
            fout.write(f' item-type="{t["item_type"]}"')
        if 'min_size' in t:
            fout.write(f' min-size="{t["min_size"]}"')
        if 'max_size' in t:
            fout.write(f' max-size="{t["max_size"]}"')
        if 'size' in t:
            fout.write(f' size="{t["size"]}"')
        if 'nullable' in t:
            fout.write(f' nullable="{t["item_type"]}"')
        if 'default' in t:
            fout.write(f' default="{t["default"]}"')
        fout.write(f'>\n')
        fout.write(f'                <description>{d}</description>\n')
        fout.write(f'            </param>\n')

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
            for (t, n, d) in ins: outputParam(t, n, d)
            fout.write('        </params>\n')
            fout.write('        <return>\n')
            for (t, n, d) in outs: outputParam(t, n, d)
            fout.write('        </return>\n')
            fout.write('    </command>\n')

    def error(msg):
        global args, lineno
        print(f'{args.lua_file}:{lineno}: {msg}')
        sys.exit(2)

    with open(args.lua_file, 'r') as f:
        for lineno, line in enumerate(f):
            lineno += 1
            if m := re.match(r'\s*--\s*@(\w+)\b\s*(.*?)\s*$', line):
                tag, line = m.groups()
                if tag in ('func', 'fun'):
                    if m := re.match(r'(\w+)\s*(.*?)\s*$', line):
                        name, description = m.groups()
                        fun = (name, description)
                    else:
                        error(f'bad arguments: must be: @func <funcName> [description]')
                elif tag in ('arg', 'ret'):
                    if m := re.match(r'(\w+)\s+(\w+)\s*(.*?)$', line):
                        dtype, name, description = m.groups()
                        typeSpec = {'type': dtype}
                    elif m := re.match(r'table\.(\w+)\s+(\w+)\s*(.*?)$', line):
                        itype, name, description = m.groups()
                        typeSpec = {'type': 'table', 'item_type': itype}
                    elif m := re.match(r'\{([^\s]*)\}\s+(\w+)\s*(.*?)$', line):
                        spec, name, description = m.groups()
                        typeSpec = {}
                        for s in spec.spit(','):
                            s = s.strip()
                            k, v = s.split('=')
                            if k in ('type', 'item_type', 'default'):
                                typeSpec[k] = v
                            elif k in ('min_size', 'max_size', 'size'):
                                try:
                                    typeSpec[k] = int(v)
                                except ValueError as e:
                                    error(f'bad value for {k}: {v} ({e})')
                            elif k in ('nullable'):
                                try:
                                    typeSpec[k] = {'true': True, 'false': False}[v]
                                except KeyError as e:
                                    error(f'bad value for {k}: must be true or false')
                            else:
                                error(f'bad key in typeSpec: {k}')
                    else:
                        error(f'bad arguments: must be: @{tag} <typeSpec> <name> [description]')
                    if tag == 'arg':
                        ins.append((typeSpec, name, description))
                    elif tag == 'ret':
                        outs.append((typeSpec, name, description))
                elif tag == 'cats':
                    cats = [x.strip() for x in line.split(',')]
                else:
                    error(f'unknown tag: @{tag}')
            else:
                output()
                fun = None
                ins, outs = [], []
                cats = []
        output()

    fout.write('</plugin>\n')

