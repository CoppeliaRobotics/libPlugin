# CoppeliaSim Stubs generator

This utility is used to generate boilerplate code for CoppeliaSim Lua callbacks.
It reads an XML file containing a description of the callbacks, script
functions, and enums, and it produces documentation, C++ source and header files,
and other stuff:

![overview diagram](doc/overview.png)

Additionally, it can parse special comments in the lua files to generate
similar documentation for pure Lua functions. See [below](#lua-comments).


## Dependencies

- Python interpreter (3.7 or greater)
- xsltproc program in your PATH (an XSLT processor) if you want to generate documentation (windows binary available [here](https://github.com/CoppeliaRobotics/xsltproc-win/raw/master/xsltproc-win.zip))

## Usage

This tool is normally used within the [libPlugin](https://github.com/CoppeliaRobotics/libPlugin) framework.

For CMake, use the `coppeliasim_generate_stubs` CMake macro, e.g.:

```cmake
coppeliasim_generate_stubs(
    ${CMAKE_CURRENT_BINARY_DIR}/generated
    XML_FILE ${CMAKE_CURRENT_SOURCE_DIR}/callbacks.xml
    LUA_FILE ${CMAKE_CURRENT_SOURCE_DIR}/simExtOMPL.lua)
```

The `XML_FILE` parameter is mandatory.

The `LUA_FILE` is optional; passing a `LUA_FILE` parameter will also parse the [lua comments](#lua-comments) for generating documentation and calltips.

The tool can also be invoked standalone:

```text
$ python3 generate.py --help
usage: generate.py [-h] [--xml-file XML_FILE] [--lua-file LUA_FILE]
                   [--gen-stubs] [--gen-lua-xml] [--gen-reference-xml]
                   [--gen-reference-html] [--gen-lua-calltips]
                   [--gen-deprecated-txt] [--gen-all] [--verbose]
                   output_dir

Generate various things for CoppeliaSim plugin.

positional arguments:
  output_dir            the output directory

optional arguments:
  -h, --help            show this help message and exit
  --xml-file XML_FILE   the XML file with the callback definitions
  --lua-file LUA_FILE   an optional LUA file containing docstrings
  --gen-stubs           generate C++ stubs
  --gen-lua-xml         generate XML translation of Lua docstrings
  --gen-reference-xml   generate merged XML (from callbacks.xml and lua.xml)
  --gen-reference-html  generate HTML documentation (from reference.xml or callbacks.xml)
  --gen-lua-calltips    generate C++ code for Lua calltips
  --gen-deprecated-txt  generate deprecated functions mapping for CoppeliaSim
  --gen-all             generate everything
  --verbose             print commands being executed
```

## Example

The `callbacks.xml` files has this structure:

```xml
<plugin name="PluginName" short-name="PLG" author="you@example.com">
    <description>An example plugin</description>

    <!--
        put <command>, <script-function>, <enum> or <struct> elements here
    -->
</plugin>
```

### Commands and Script Functions

Each `<command>` or `<script-function>` element serves both as a declaration and documentation for a Lua command/callback of the plugin. It can contain a description of the command itself, and several input and output parameters, with annotated type and documentation.

See [below](#parameter-specification) for how to specify parameters.

Example:

```xml
<command name="test">
    <description>Perform some computation with the given parameters.</description>
    <params>
        <param name="a" type="int">
            <description>An integer argument</description>
        </param>
        <param name="b" type="string">
            <description>An string argument</description>
        </param>
        <param name="c" type="table" item-type="float">
            <description>A vector of floats</description>
        </param>
    </params>
    <return>
        <param name="x" type="float">
            <description>A float return value</description>
        </param>
        <param name="y" type="string">
            <description>A string return value</description>
        </param>
    </return>
</command>
```

The tool will generate C++ code:
 - a `test_in` data structure for the input arguments
 - a `test_out` data structure for the output arguments

In C++ code, the corresponding callback must be implemented:

```C++
void test(SScriptCallBack *p, const char *cmd, test_in *in, test_out *out)
{
    ...
}
```

### Data Structures

The `<struct>` element will generate a C++ struct, and documentation for the data structure. It can contain a description of the data structure itself, and several parameters, with annotated type and documentation.

See [below](#parameter-specification) for how to specify parameters.

Example:

```xml
<struct name="mystruct">
    <description>Extra options for the curve creation function <command-ref name="addCurve"/>.</description>
    <param name="a" type="int" default="1">
        <description>an integer field</description>
    </param>
    <param name="b" type="bool" default="true">
        <description>a boolean field</description>
    </param>
</struct>
```

See [below](#parameter-specification) for how to specify parameters.

Then in C++ it can be used like this:

```C++
mystruct x;
x.a = 3;
x.b = false;
```

or passed as parameter to commands.

### Enumerations

The `<enum>` element will generate a C++ enum, and documentation for that. It can contain several `<item>` elements.

The element `<item>` must have a `name` attribute, and an optional `value` attribute to explicitly specify a value. If such value is not given, it will be determined by adding the `<item>` position (0-based) to `base`.

Example:

```xml
<enum name="myenum" item-prefix="foo_" base="32000">
    <item name="alpha" />
    <item name="beta" />
    <item name="gamma" />
</enum>
```

It will generate an enum named `myenum` with items `sim_plugname_foo_alpha`, `sim_plugname_foo_beta`, `sim_plugname_foo_gamma`.

The enum items will be available also in Lua as `simPLG.myenum.alpha`, `simPLG.myenum.beta`, `simPLG.myenum.gamma`.

### Parameter specification

The `<param>` element can have the following attributes:

- `name`: (mandatory) the name of the field. Cannot start with an underscore, and must be a valid C and a valid Lua identifier.
- `type`: (mandatory) the type of the field (`int`, `float`, `double`, `bool`, `string`, `table`, or the name of a *struct*).
- `nullable`: if `true`, the fields accepts also a nil value; the C++ type will be wrapped into a `boost::optional` (only valid for `int`, `float`, `double`, `bool`, `string`).
- `default`: the default value of the field (no more fields without default value should follow); in case of tables specify a list of values as `{1, 2, 3}`; in case of strings, the string value must be enclosed by `"`s, but the `"` must be escaped as `&quot;`, i.e.: `default="&quot;some value&quot;"`.
- `skip`: if `true`, the value will be left on the stack; no other fields not marked as `skip` should follow.

If `type` is `table` the following attributes are valid:

- `item-type`: indicates the tape of an item in the table (`int`, `float`, `bool`, `string`).
- `min-size`: minimum size of the table.
- `max-size`: minimum size of the table.
- `size`: shorthand for setting `min-size` and `max-size` to the same value.

### Allowed markup

The `<description>` elements can contain basic HTML markup elements, such as `<br>`, `<sub>`, `<sup>`, `<em>`, `<strong>`, `<ul>`, `<ol>`, `<li>`, `<code>`, and any special cross-referencing elements described below.

### Cross-references

The `<description>` element of the various items can reference other items in the document, via the tags:

- `<command-ref>`: reference to a command
- `<enum-ref>`: reference to an enum
- `<struct-ref>`: reference to a struct

The `name` attribute is mandatory.

Example:

```xml
<description>This command computes a value based on the result of <command-ref name="foo" /></description>
```

The above elements can be contained also in a `<see-also>` node child of the `<command>` or `<script-function>` elements. Those will render in the apposite "see also" section of the documentation.

Example:

```xml
<command name="addCurve">
    <description>Adds a curve to a plot widget.</description>
    <see-also>
        <enum-ref name="curve_type" />
        <enum-ref name="curve_style" />
        <struct-ref name="curve_options" />
    </see-also>
    <params>
        <param name="handle" type="int">
            <description>ui handle</description>
        </param>
        <param name="id" type="int">
            <description>widget id</description>
        </param>
        <param name="type" type="int">
            <description>type of the curve. see <enum-ref name="curve_type"/>.</description>
        </param>
    </params>
    <return>
    </return>
</command>
```

### Automatic cross-references

Elements `<command>`, `<script-function>` can also have categories.

Commands in the same category will automatically have the "see also" section populated with cross-reference to other items in the same category.

A `<category>` element with the `indirect` attribute equal to `true` will cause that command to reference other commands in that category, but not vice-versa.

Example:

```xml
<command name="addCurveTimePoints">
    <description>Adds time points to the specified curve of the plot widget.</description>
    <categories>
        <category name="plot" />
        <category name="widgets" indirect="true" />
    </categories>
    ...
</command>
```

## Lua comments

As plugins can provide additional functions via lua files, special comments can be added to lua files to generate a similar documentation (html reference, calltips, etc...).

Comments must start at the very beginning of a line, and are in the form:

```lua
--@key value
```

Where `key` is an identifier. Valid values for `key` are:

- `fun`: must be the first one of a block. `value` must contain the name of the function and a description;
- `arg`: describe an input argument. `value` must contain the type (`int`, `float`, `string`, `table`, whereas the latter can be specialized with the item type, e.g.: `table.int`, `table.string`, etc...);
- `ret`: similar to `arg` but describe a return value;
- `cats`: a list of [categories](#automatic-cross-references), separated by comma (spaces are ignored).

Example:

```lua
--@fun getPathStateCount get the number of states in the given path
--@arg int taskHandle the handle of the task
--@arg table.float path the path, as returned by simOMPL.getPath
--@ret int count the number of states in the path
--@cats path, state
function simOMPL.getPathStateCount(taskHandle,path)
    local n=simOMPL.getStateSpaceDimension(taskHandle)
    return #path/n
end
```

## Complete example

See [simExtPluginSkeletonNG](https://github.com/CoppeliaRobotics/simExtPluginSkeletonNG) for an example of a CoppeliaSim plugin using this framework.

