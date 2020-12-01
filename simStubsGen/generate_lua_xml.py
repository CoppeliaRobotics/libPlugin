import argparse
import re
import sys
import xml.etree.ElementTree as ET
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

def output():
    if fun:
        f, fdesc = fun
        cmd = ET.SubElement(root, 'command')
        cmd.attrib['name'] = f
        d = ET.SubElement(cmd, 'description')
        d.text = fdesc
        if cats:
            cs = ET.SubElement(cmd, 'categories')
            for cat in cats:
                c = ET.SubElement(cs, 'category', name=cat)
        pars = ET.SubElement(cmd, 'params')
        rets = ET.SubElement(cmd, 'return')
        for (src, parent) in ((ins, pars), (outs, rets)):
            for (t, n, d) in src:
                p = ET.SubElement(parent, 'param')
                p.attrib['name'] = n
                p.attrib['type'] = t['type']
                if 'item_type' in t:
                    p.attrib['item-type'] = t['item_type']
                if 'min_size' in t:
                    p.attrib['min-size'] = str(t["min_size"])
                if 'max_size' in t:
                    p.attrib['max-size'] = str(t["max_size"])
                if 'size' in t:
                    p.attrib['size'] = str(t["size"])
                if 'nullable' in t:
                    p.attrib['nullable'] = str(t["item_type"]).lower()
                if 'default' in t:
                    p.attrib['default'] = t["default"]
                d = ET.SubElement(p, 'description')
                d.text = d

def error(msg):
    global args, lineno
    print(f'{args.lua_file}:{lineno}: {msg}')
    sys.exit(2)

root = ET.Element('plugin')
root.attrib['name'] = plugin.name
if plugin.short_name:
    root.attrib['short-name'] = plugin.short_name
if plugin.version:
    root.attrib['version'] = str(plugin.version)

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
                    for s in spec.split(','):
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

tree = ET.ElementTree(root)
tree.write(args.out_xml, encoding='utf-8', xml_declaration=True)
