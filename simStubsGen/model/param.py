import re

class Param(object):
    mapping = {}

    def __init__(self, node):
        if node.tag != 'param':
            raise ValueError('expected <param>, got <%s>' % node.tag)
        self.name = node.attrib['name']
        if self.name == '':
            raise ValueError('attribute "name" of <param> cannot be empty')
        if self.name.startswith('_'):
            raise ValueError('attribute "name" of <param> cannot start with underscore')

        try:
            descnode = node.find('description')
            self.description = '' if descnode is None else descnode.text
        except AttributeError:
            self.description = ''

        if 'type' not in node.attrib:
            raise ValueError('attribute "type" is missing in <param>')
        self.dtype = node.attrib['type']

        self.ctype_base = self.dtype
        self.default = node.attrib.get('default', None)
        self.skip = node.attrib.get('skip', 'false').lower() in ('true', 'yes', '1')
        self.nullable = node.attrib.get('nullable', 'false').lower() in ('true', 'yes', '1')
        self.write_in = True
        self.write_out = True
        if self.dtype == 'table' and self.nullable:
            raise ValueError('cannot have nullable table')

    def mandatory(self):
        return self.default is None

    def optional(self):
        return self.default is not None

    def ctype(self):
        if self.nullable: return 'boost::optional<%s>' % self.ctype_base
        else: return self.ctype_base

    def ctype_normalized(self):
        replacements = {
            '::': '__',
            '<': '__',
            '>': '__',
            ' ': '',
        }
        ret = self.ctype()
        for a, b in replacements.items():
            ret = ret.replace(a, b)
        return ret

    def htype(self):
        return self.dtype

    def cdefault(self):
        return self.default

    def hdefault(self):
        return self.default

    def argmod(self):
        return ''

    @staticmethod
    def register_type(dtype, clazz):
        Param.mapping[dtype] = clazz

    @staticmethod
    def factory(node):
        if 'type' not in node.attrib:
            raise ValueError('attribute "type" is missing in <param>')
        dtype = node.attrib['type']
        if dtype not in Param.mapping:
            print('ERROR: type "{}" not found in mapping; valid types are: {}'.format(dtype, ', '.join('"%s"' % k for k in Param.mapping.keys())))
        return Param.mapping[dtype](node)

    def declaration(self):
        return '{} {}'.format(self.ctype(), self.name)

    def declaration_with_default(self):
        return self.declaration() + (' = {}'.format(self.cdefault()) if self.cdefault() else '')

class ParamInt(Param):
    def __init__(self, node):
        super(ParamInt, self).__init__(node)

    def htype(self):
        return 'number'

class ParamFloat(Param):
    def __init__(self, node):
        super(ParamFloat, self).__init__(node)

    def htype(self):
        return 'number'

class ParamDouble(Param):
    def __init__(self, node):
        super(ParamDouble, self).__init__(node)

    def htype(self):
        return 'number'

class ParamString(Param):
    def __init__(self, node):
        super(ParamString, self).__init__(node)
        self.ctype_base = 'std::string'

    def cdefault(self):
        if self.default is None: return None
        return '"%s"' % self.default.replace('\\','\\\\').replace('"','\\"')

    def hdefault(self):
        if self.default is None: return None
        return "'%s'" % self.default.replace('\\','\\\\').replace('"','\\"')

class ParamBool(Param):
    def __init__(self, node):
        super(ParamBool, self).__init__(node)

class ParamTable(Param):
    def __init__(self, node):
        super(ParamTable, self).__init__(node)
        self.itype = node.attrib.get('item-type', None)
        for (old, new) in (('minsize', 'min-size'), ('maxsize', 'max-size')):
            if old in node.attrib:
                raise AttributeError('Attribute "{}" should be changed to "{}"'.format(old, new))
        self.minsize = int(node.attrib.get('min-size', 0))
        self.maxsize = int(node.attrib['max-size']) if 'max-size' in node.attrib else None
        if 'size' in node.attrib:
            self.minsize = int(node.attrib['size'])
            self.maxsize = int(node.attrib['size'])
        if self.itype is None:
            self.write_in = False
            self.write_out = False

    def item_dummy(self):
        n = type('dummyNode', (object,), dict(tag='param', attrib={'name': 'dummy', 'type': self.itype}))
        return Param.factory(n)

    def ctype(self):
        if self.itype is not None:
            return 'std::vector<%s>' % self.item_dummy().ctype()
        else:
            return 'void *'

    def ctype_normalized(self):
        return self.item_dummy().ctype().replace('::', '__')

    def htype(self):
        if self.minsize and self.maxsize and self.minsize == self.maxsize:
            return 'table_{}'.format(self.minsize)
        return 'table'

    def cdefault(self):
        if self.default is not None:
            m = re.match(r'^\s*{(.*)}\s*$', self.default)
            if not m:
                raise ValueError('invalid default value for array: must be like {value1, value2, ...}')
            raw_vals = m.group(1).strip()
            if not raw_vals:
                return '{}'
            vals = [x.strip() for x in m.group(1).split(',')]
            list_of = ['({})'.format(x) for x in vals]
            return 'boost::assign::list_of{}.convert_to_container<{} >()'.format(''.join(list_of), self.ctype())

class ParamStruct(Param):
    def __init__(self, node, name):
        super(ParamStruct, self).__init__(node)
        self.structname = name
        self.xoptional = False
        if self.default is not None:
            if self.default == '{}':
                self.xoptional = True
            else:
                raise ValueError('default value not supported in <struct>')

    def mandatory(self):
        return not self.xoptional

    def optional(self):
        return self.xoptional

    def cdefault(self):
        if self.xoptional:
            return '{}'

    def argmod(self):
        return '&'

Param.register_type('anything', Param)
Param.register_type('int', ParamInt)
Param.register_type('float', ParamFloat)
Param.register_type('double', ParamDouble)
Param.register_type('string', ParamString)
Param.register_type('bool', ParamBool)
Param.register_type('table', ParamTable)
