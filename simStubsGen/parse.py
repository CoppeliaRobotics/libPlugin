import os
import sys
import xml.etree.ElementTree as ET

try:
    import xmlschema
except ModuleNotFoundError:
    print('\n\n{}\n\n'.format('\n'.join('   ' + x for x in [
        'error: Python package "xmlschema" not found.',
        '       Please install it, e.g.: python3 -m pip install xmlschema'
    ])))
    sys.exit(1)

import model

def parse(xml_file):
    schema = xmlschema.XMLSchema11(os.path.dirname(os.path.realpath(__file__)) + '/xsd/callbacks.xsd')
    schema.validate(xml_file)
    tree = ET.parse(xml_file)
    root = tree.getroot()
    return model.Plugin(root)

def escape(s, method='C'):
    if isinstance(s, str) and method == 'C':
        return s.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n').replace('\r', '')
    else:
        return s
