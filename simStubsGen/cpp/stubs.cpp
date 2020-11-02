#py from parse import parse, escape
#py import model
#py plugin = parse(pycpp.params['xml_file'])
#include "stubs.h"
#include <simPlusPlus/Lib.h>

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <boost/lexical_cast.hpp>

FuncTracer::FuncTracer(const std::string &f, int l)
    : f_(f),
      l_(l)
{
    sim::addLog(l_, f_ + " [enter]");
}

FuncTracer::~FuncTracer()
{
    sim::addLog(l_, f_ + " [leave]");
}

#ifndef NDEBUG

template<typename... Arguments>
void addStubsDebugLog(const std::string &fmt, Arguments&&... args)
{
    if(sim::isStackDebugEnabled())
    {
        auto msg = sim::util::sprintf(fmt, std::forward<Arguments>(args)...);
        sim::addLog(sim_verbosity_debug, "STUBS DEBUG: %s", msg);
    }
}

static void addStubsDebugStackDump(int stackHandle)
{
    if(sim::isStackDebugEnabled())
        sim::debugStack(stackHandle);
}

#else // RELEASE
#define addStubsDebugLog(...)
#define addStubsDebugStackDump(x)
#endif

#ifdef QT_COMPIL

Qt::HANDLE UI_THREAD = NULL;
Qt::HANDLE SIM_THREAD = NULL;

std::string threadNickname()
{
    Qt::HANDLE h = QThread::currentThreadId();
    if(h == UI_THREAD) return "UI";
    if(h == SIM_THREAD) return "SIM";
    std::stringstream ss;
    ss << h;
    return ss.str();
}

void uiThread()
{
    Qt::HANDLE h = QThread::currentThreadId();
    if(UI_THREAD != NULL && UI_THREAD != h)
        sim::addLog(sim_verbosity_warnings, "UI thread has already been set");
    UI_THREAD = h;
}

void simThread()
{
    Qt::HANDLE h = QThread::currentThreadId();
    if(SIM_THREAD != NULL && SIM_THREAD != h)
        sim::addLog(sim_verbosity_warnings, "SIM thread has already been set");
    SIM_THREAD = h;
}

#endif // QT_COMPIL

void read__bool(int stack, bool *value)
{
    simBool v;
    if(sim::getStackBoolValue(stack, &v) == 1)
    {
        *value = v;
        sim::popStackItem(stack, 1);
    }
    else
    {
        throw sim::exception("expected bool");
    }
}

void read__int(int stack, int *value)
{
    int v;
    if(sim::getStackInt32Value(stack, &v) == 1)
    {
        *value = v;
        sim::popStackItem(stack, 1);
    }
    else
    {
        throw sim::exception("expected int");
    }
}

void read__float(int stack, float *value)
{
    simFloat v;
    if(sim::getStackFloatValue(stack, &v) == 1)
    {
        *value = v;
        sim::popStackItem(stack, 1);
    }
    else
    {
        throw sim::exception("expected float");
    }
}

void read__double(int stack, double *value)
{
    simDouble v;
    if(sim::getStackDoubleValue(stack, &v) == 1)
    {
        *value = v;
        sim::popStackItem(stack, 1);
    }
    else
    {
        throw sim::exception("expected double");
    }
}

void read__std__string(int stack, std::string *value)
{
    std::string v;
    if(sim::getStackStringValue(stack, &v) == 1)
    {
        *value = v;
        sim::popStackItem(stack, 1);
    }
    else
    {
        throw sim::exception("expected string");
    }
}

void read__boost__optional__bool__(int stack, boost::optional<bool> *value)
{
    simBool v;
    if(sim::getStackBoolValue(stack, &v) == 1)
    {
        *value = v;
        sim::popStackItem(stack, 1);
    }
    else if(sim::isStackValueNull(stack) == 1)
    {
        *value = boost::none;
    }
    else
    {
        throw sim::exception("expected bool or nil");
    }
}

void read__boost__optional__int__(int stack, boost::optional<int> *value)
{
    int v;
    if(sim::getStackInt32Value(stack, &v) == 1)
    {
        *value = v;
        sim::popStackItem(stack, 1);
    }
    else if(sim::isStackValueNull(stack) == 1)
    {
        *value = boost::none;
    }
    else
    {
        throw sim::exception("expected int or nil");
    }
}

void read__boost__optional__float__(int stack, boost::optional<float> *value)
{
    simFloat v;
    if(sim::getStackFloatValue(stack, &v) == 1)
    {
        *value = v;
        sim::popStackItem(stack, 1);
    }
    else if(sim::isStackValueNull(stack) == 1)
    {
        *value = boost::none;
    }
    else
    {
        throw sim::exception("expected float or nil");
    }
}

void read__boost__optional__double__(int stack, boost::optional<double> *value)
{
    simDouble v;
    if(sim::getStackDoubleValue(stack, &v) == 1)
    {
        *value = v;
        sim::popStackItem(stack, 1);
    }
    else if(sim::isStackValueNull(stack) == 1)
    {
        *value = boost::none;
    }
    else
    {
        throw sim::exception("expected double or nil");
    }
}

void read__boost__optional__std__string__(int stack, boost::optional<std::string> *value)
{
    std::string v;
    if(sim::getStackStringValue(stack, &v) == 1)
    {
        *value = v;
        sim::popStackItem(stack, 1);
    }
    else if(sim::isStackValueNull(stack) == 1)
    {
        *value = boost::none;
    }
    else
    {
        throw sim::exception("expected string or nil");
    }
}

void check_table_size(size_t sz, size_t minSize, size_t maxSize)
{
    if(minSize == maxSize)
    {
        if(sz != minSize)
            throw sim::exception("must have exactly %d elements", minSize);
    }
    else
    {
        if(sz < minSize)
            throw sim::exception("must have at least %d elements", minSize);
        if(sz > maxSize)
            throw sim::exception("must have at most %d elements", maxSize);
    }
}

template<typename T>
void read__table__unfold(int stack, std::vector<T> *vec, void (*readItemFunc)(int, T*), size_t minSize = 0, size_t maxSize = -1)
{
    int sz = sim::getStackTableInfo(stack, 0);
    if(sz < 0)
        throw sim::exception("expected array (simGetStackTableInfo(stack, 0) returned %d)", sz);

    check_table_size(sz, minSize, maxSize);

    int oldsz = sim::getStackSize(stack);
    sim::unfoldStackTable(stack);
    int sz1 = (sim::getStackSize(stack) - oldsz + 1) / 2;
    if(sz != sz1)
        throw std::runtime_error("simUnfoldStackTable unpacked more elements than simGetStackTableInfo reported");

    vec->resize(sz);

    for(int i = 0; i < sz; i++)
    {
        sim::moveStackItemToTop(stack, oldsz - 1);
        int j;
        read__int(stack, &j);
        sim::moveStackItemToTop(stack, oldsz - 1);
        if constexpr(std::is_same<T, bool>::value)
        {
            T v;
            readItemFunc(stack, &v);
            (*vec)[i] = v;
        }
        else
        {
            readItemFunc(stack, &vec->at(i));
        }
    }
}

template<typename T>
void read__table__oneshot(int stack, std::vector<T> *vec, simInt (*readFunc)(simInt, std::vector<T>*), size_t minSize = 0, size_t maxSize = -1)
{
    int sz = sim::getStackTableInfo(stack, 0);
    if(sz < 0)
        throw sim::exception("expected array (simGetStackTableInfo(stack, 0) returned %d)", sz);

    check_table_size(sz, minSize, maxSize);

    int chk = sim::getStackTableInfo(stack, 2);
    if(chk != 1)
        throw sim::exception("table contains non-numbers (simGetStackTableInfo(stack, 2) returned %d)", chk);

    vec->resize(sz);

    int ret = readFunc(stack, vec);
    if(ret != 1)
        throw sim::exception("readFunc error %d", ret);

    sim::popStackItem(stack, 1);
}

#py for struct in plugin.structs:
void read__`struct.name`(int stack, `struct.name` *value)
{
    addStubsDebugLog("read__`struct.name`: begin reading...");
    addStubsDebugStackDump(stack);

    try
    {
        simInt info = sim::getStackTableInfo(stack, 0);
        if(info == sim_stack_table_empty)
            return;
        if(info != sim_stack_table_map)
        {
            throw sim::exception("expected a map");
        }

        int oldsz = sim::getStackSize(stack);
        sim::unfoldStackTable(stack);
        int numItems = (sim::getStackSize(stack) - oldsz + 1) / 2;

        while(numItems >= 1)
        {
            sim::moveStackItemToTop(stack, oldsz - 1); // move key to top
            std::string key;
            read__std__string(stack, &key);

            sim::moveStackItemToTop(stack, oldsz - 1); // move value to top
            if(0) {}
#py for field in struct.fields:
            else if(key == "`field.name`")
            {
                addStubsDebugLog("read__`struct.name`: reading field \"`field.name`\" (`field.ctype()`)...");
                try
                {
#py if isinstance(field, model.ParamTable):
#py if field.itype == 'float':
                    read__table__oneshot(stack, &(value->`field.name`), sim::getStackFloatTable, `field.minsize`, `field.maxsize`);
#py elif field.itype == 'double':
                    read__table__oneshot(stack, &(value->`field.name`), sim::getStackDoubleTable, `field.minsize`, `field.maxsize`);
#py elif field.itype == 'int':
                    read__table__oneshot(stack, &(value->`field.name`), sim::getStackInt32Table, `field.minsize`, `field.maxsize`);
#py else:
                    read__table__unfold(stack, &(value->`field.name`), read__`field.ctype_normalized()`, `field.minsize`, `field.maxsize`);
#py endif
#py else:
                    read__`field.ctype_normalized()`(stack, &(value->`field.name`));
#py endif
                }
                catch(std::exception &ex)
                {
                    throw sim::exception("field '`field.name`': %s", ex.what());
                }
            }
#py endfor
            else
            {
                throw sim::exception("unexpected key: %s", key);
            }

            numItems = (sim::getStackSize(stack) - oldsz + 1) / 2;
        }
    }
    catch(std::exception &ex)
    {
        throw sim::exception("read__`struct.name`: %s", ex.what());
    }

    addStubsDebugLog("read__`struct.name`: finished reading");
}

#py endfor
void write__bool(const bool &value, int stack)
{
    simBool v = value;
    sim::pushBoolOntoStack(stack, v);
}

void write__int(const int &value, int stack)
{
    simInt v = value;
    sim::pushInt32OntoStack(stack, v);
}

void write__float(const float &value, int stack)
{
    simFloat v = value;
    sim::pushFloatOntoStack(stack, v);
}

void write__double(const double &value, int stack)
{
    simDouble v = value;
    sim::pushDoubleOntoStack(stack, v);
}

void write__std__string(const std::string &value, int stack)
{
    const simChar *v = value.c_str();
    sim::pushStringOntoStack(stack, v, value.length());
}

void write__boost__optional__bool__(const boost::optional<bool> &value, int stack)
{
    if(value)
    {
        simBool v = value.get();
        sim::pushBoolOntoStack(stack, v);
    }
    else sim::pushNullOntoStack(stack);
}

void write__boost__optional__int__(const boost::optional<int> &value, int stack)
{
    if(value)
    {
        simInt v = value.get();
        sim::pushInt32OntoStack(stack, v);
    }
    else sim::pushNullOntoStack(stack);
}

void write__boost__optional__float__(const boost::optional<float> &value, int stack)
{
    if(value)
    {
        simFloat v = value.get();
        sim::pushFloatOntoStack(stack, v);
    }
    else sim::pushNullOntoStack(stack);
}

void write__boost__optional__double__(const boost::optional<double> &value, int stack)
{
    if(value)
    {
        simDouble v = value.get();
        sim::pushDoubleOntoStack(stack, v);
    }
    else sim::pushNullOntoStack(stack);
}

void write__boost__optional__std__string__(const boost::optional<std::string> &value, int stack)
{
    if(value)
    {
        std::string s = value.get();
        const simChar *v = s.c_str();
        sim::pushStringOntoStack(stack, v, s.length());
    }
    else sim::pushNullOntoStack(stack);
}

template<typename T>
void write__table__insert(const std::vector<T> &vec, void (*writeItemFunc)(const T&, int), int stack)
{
    sim::pushTableOntoStack(stack);
    for(size_t i = 0; i < vec.size(); i++)
    {
        write__int(i + 1, stack);
        writeItemFunc(vec.at(i), stack);
        sim::insertDataIntoStackTable(stack);
    }
}

template<typename T>
void write__table__oneshot(const std::vector<T> &vec, void (*writeFunc)(simInt, const std::vector<T>&), int stack)
{
    writeFunc(stack, vec);
}

#py for struct in plugin.structs:
void write__`struct.name`(const `struct.name` &value, int stack)
{
    addStubsDebugLog("write__`struct.name`: begin writing...");

    try
    {
        sim::pushTableOntoStack(stack);

#py for field in struct.fields:
        addStubsDebugLog("write__`struct.name`: writing field \"`field.name`\" (`field.ctype()`)...");
        try
        {
            sim::pushStringOntoStack(stack, "`field.name`", 0);
#py if isinstance(field, model.ParamTable):
#py if field.itype == 'float':
            write__table__oneshot(value.`field.name`, sim::pushFloatTableOntoStack, stack);
#py elif field.itype == 'double':
            write__table__oneshot(value.`field.name`, sim::pushDoubleTableOntoStack, stack);
#py elif field.itype == 'int':
            write__table__oneshot(value.`field.name`, sim::pushInt32TableOntoStack, stack);
#py else:
            write__table__insert(value.`field.name`, write__`field.ctype_normalized()`, stack);
#py endif
#py else:
            write__`field.ctype_normalized()`(value.`field.name`, stack);
#py endif
            sim::insertDataIntoStackTable(stack);
        }
        catch(std::exception &ex)
        {
            throw sim::exception("field '`field.name`': %s", ex.what());
        }
#py endfor
    }
    catch(std::exception &ex)
    {
        throw sim::exception("write__`struct.name`: %s", ex.what());
    }

    addStubsDebugLog("write__`struct.name`: finished writing");
}

#py endfor
#py for struct in plugin.structs:
`struct.name`::`struct.name`()
{
#py for field in struct.fields:
#py if field.default:
    `field.name` = `field.cdefault()`;
#py endif
#py endfor
}

#py endfor
std::string versionString(int v)
{
    std::stringstream ss;
    int revision = v % 100;
    v /= 100;
    int patch = v % 100;
    v /= 100;
    int minor = v % 100;
    v /= 100;
    int major = v % 100;
    ss << major << "." << minor << "." << patch << "rev" << revision;
    return ss.str();
}

void checkRuntimeVersion()
{
    simInt simVer = sim::getInt32Parameter(sim_intparam_program_version);
    simInt simRev = sim::getInt32Parameter(sim_intparam_program_revision);
    simVer = simVer * 100 + simRev;

    // version required by simStubsGen:
    int minVer = 4010000; // 4.1.0rev0
    if(simVer < minVer)
        throw sim::exception("requires at least %s (libPlugin)", versionString(minVer));

    // version required by plugin:
    if(simVer < SIM_REQUIRED_PROGRAM_VERSION_NB)
        throw sim::exception("requires at least %s", versionString(SIM_REQUIRED_PROGRAM_VERSION_NB));

    // warn if the app older than the headers used to compile:
    if(simVer < SIM_PROGRAM_FULL_VERSION_NB)
        sim::addLog(sim_verbosity_warnings, "has been built for %s", versionString(SIM_PROGRAM_FULL_VERSION_NB));
}

bool registerScriptStuff()
{
    try
    {
        checkRuntimeVersion();

        auto dbg = sim::getStringNamedParam("simStubsGen.debug");
        if(dbg && *dbg != "0")
            sim::enableStackDebug();

        try
        {
#py if plugin.short_name:
            sim::registerScriptVariable("sim`plugin.short_name`", "require('simExt`plugin.name`')", 0);
#py endif

#py if plugin.short_name:
            // register new-style short-version varables
#py for enum in plugin.enums:
            sim::registerScriptVariable("sim`plugin.short_name`.`enum.name`", "{}", 0);
#py for item in enum.items:
            sim::registerScriptVariable("sim`plugin.short_name`.`enum.name`.`item.name`", boost::lexical_cast<std::string>(sim_`plugin.short_name.lower()`_`enum.item_prefix``item.name`), 0);
#py endfor
#py endfor
            // register new-style short-version commands
#py for cmd in plugin.commands:
            sim::registerScriptCallbackFunction("sim`plugin.short_name`.`cmd.name`@`plugin.name`", "`escape(cmd.help_out_args_text)`sim`plugin.short_name`.`cmd.name`(`escape(cmd.help_in_args_text)`)`escape(cmd.documentation)`", `cmd.c_name`_callback);
#py endfor
#py endif

#py if plugin.short_name:
            // commands simExt<PLUGIN_NAME>_<COMMAND_NAME> (deprecated)
#py for cmd in plugin.commands:
            sim::registerScriptCallbackFunction("`plugin.command_prefix``cmd.name`@`plugin.name`", "`escape(cmd.help_text)`\n\n(DEPRECATED, please use sim`plugin.short_name`.`cmd.name`)", `cmd.c_name`_callback);
#py endfor
            // register variables (deprecated)
#py for enum in plugin.enums:
#py for item in enum.items:
            sim::registerScriptVariable("sim_`plugin.name.lower()`_`enum.item_prefix``item.name`", boost::lexical_cast<std::string>(sim_`plugin.name.lower()`_`enum.item_prefix``item.name`), -1);
#py endfor
#py endfor
#py else:
            // commands simExt<PLUGIN_NAME>_<COMMAND_NAME>
#py for cmd in plugin.commands:
            sim::registerScriptCallbackFunction("`plugin.command_prefix``cmd.name`@`plugin.name`", "`escape(cmd.help_text)``escape(cmd.documentation)`", `cmd.c_name`_callback);
#py endfor
            // register variables
#py for enum in plugin.enums:
#py for item in enum.items:
            sim::registerScriptVariable("sim_`plugin.name.lower()`_`enum.item_prefix``item.name`", boost::lexical_cast<std::string>(sim_`plugin.name.lower()`_`enum.item_prefix``item.name`), 0);
#py endfor
#py endfor
#py endif

#py if pycpp.params['have_lua_calltips'] == 'True':
#include "lua_calltips.cpp"
#py endif
        }
        catch(std::exception &ex)
        {
            throw sim::exception("Initialization failed (registerScriptStuff): %s", ex.what());
        }
    }
    catch(sim::exception& ex)
    {
        sim::addLog(sim_verbosity_errors, ex.what());
        return false;
    }
    return true;
}

#py for enum in plugin.enums:
const char* `enum.name.lower()`_string(`enum.name` x)
{
    switch(x)
    {
#py for item in enum.items:
        case sim_`plugin.name.lower()`_`enum.item_prefix``item.name`: return "sim_`plugin.name.lower()`_`enum.item_prefix``item.name`";
#py endfor
        default: return "???";
    }
}

#py endfor
#py for cmd in plugin.commands:
`cmd.c_in_name`::`cmd.c_in_name`()
{
    _scriptID = -1;
    _stackID = -1;
#py for p in cmd.params:
#py if p.cdefault() is not None:
    `p.name` = `p.cdefault()`;
#py endif
#py endfor
}

`cmd.c_out_name`::`cmd.c_out_name`()
{
#py for p in cmd.returns:
#py if p.cdefault() is not None:
    `p.name` = `p.cdefault()`;
#py endif
#py endfor
}

void `cmd.c_name`(SScriptCallBack *p, `cmd.c_in_name` *in_args, `cmd.c_out_name` *out_args)
{
    `cmd.c_name`(p, "`plugin.command_prefix``cmd.c_name`", in_args, out_args);
}

#py if len(cmd.returns) == 1:
`cmd.returns[0].ctype()` `cmd.c_name`(`cmd.c_arg_list(pre_args=['SScriptCallBack *p'])`)
{
    `cmd.c_in_name` in_args;
    if(p)
    {
        in_args._scriptID = p->scriptID;
        in_args._stackID = p->stackID;
    }
#py for p in cmd.params:
    in_args.`p.name` = `p.name`;
#py endfor
    `cmd.c_out_name` out_args;
    `cmd.c_name`(p, &in_args, &out_args);
    return out_args.`cmd.returns[0].name`;
}

#py endif
#py if len(cmd.returns) == 0:
void `cmd.c_name`(`cmd.c_arg_list(pre_args=['SScriptCallBack *p'])`)
{
    `cmd.c_in_name` in_args;
    if(p)
    {
        in_args._scriptID = p->scriptID;
        in_args._stackID = p->stackID;
    }
#py for p in cmd.params:
    in_args.`p.name` = `p.name`;
#py endfor
    `cmd.c_out_name` out_args;
    `cmd.c_name`(p, &in_args, &out_args);
}

#py endif
void `cmd.c_name`(`cmd.c_arg_list(pre_args=['SScriptCallBack *p', '%s *out_args' % cmd.c_out_name])`)
{
    `cmd.c_in_name` in_args;
    if(p)
    {
        in_args._scriptID = p->scriptID;
        in_args._stackID = p->stackID;
    }
#py for p in cmd.params:
    in_args.`p.name` = `p.name`;
#py endfor
    `cmd.c_name`(p, &in_args, out_args);
}

void `cmd.c_name`_callback(SScriptCallBack *p)
{
    addStubsDebugLog("`cmd.c_name`_callback: reading input arguments...");
    addStubsDebugStackDump(p->stackID);

    const char *cmd = "`plugin.command_prefix``cmd.c_name`";

    `cmd.c_in_name` in_args;
    if(p)
    {
        in_args._scriptID = p->scriptID;
        in_args._stackID = p->stackID;
    }
    `cmd.c_out_name` out_args;

    try
    {
        // check argument count

        int numArgs = sim::getStackSize(p->stackID);
        if(numArgs < `cmd.params_min`)
            throw sim::exception("not enough arguments");
        if(numArgs > `cmd.params_max`)
            throw sim::exception("too many arguments");

        // read input arguments from stack

#py for i, p in enumerate(cmd.params):
        if(numArgs >= `i+1`)
        {
            addStubsDebugLog("`cmd.c_name`_callback: reading input argument `i+1` \"`p.name`\" (`p.ctype()`)...");
            try
            {
                // moving the first arg (stack pos 0) to top (the other end of the stack)
                // which then it will be popped, thus consuming items from the beginning (stack bottom)
                sim::moveStackItemToTop(p->stackID, 0);
#py if isinstance(p, model.ParamTable):
#py if p.itype == 'float':
                read__table__oneshot(p->stackID, &(in_args.`p.name`), sim::getStackFloatTable, `p.minsize`, `p.maxsize`);
#py elif p.itype == 'double':
                read__table__oneshot(p->stackID, &(in_args.`p.name`), sim::getStackDoubleTable, `p.minsize`, `p.maxsize`);
#py elif p.itype == 'int':
                read__table__oneshot(p->stackID, &(in_args.`p.name`), sim::getStackInt32Table, `p.minsize`, `p.maxsize`);
#py else:
                read__table__unfold(p->stackID, &(in_args.`p.name`), read__`p.ctype_normalized()`, `p.minsize`, `p.maxsize`);
#py endif
#py else:
                read__`p.ctype_normalized()`(p->stackID, &(in_args.`p.name`));
#py endif
            }
            catch(std::exception &ex)
            {
                throw sim::exception("read in arg `i+1` (`p.name`): %s", ex.what());
            }
        }

#py endfor

        addStubsDebugLog("`cmd.c_name`_callback: stack content after reading input arguments:");
        addStubsDebugStackDump(p->stackID);

#py if cmd.clear_stack_after_reading_input:
        addStubsDebugLog("`cmd.c_name`_callback: clearing stack content after reading input arguments");
        // clear stack
        sim::popStackItem(p->stackID, 0);

#py endif

        addStubsDebugLog("`cmd.c_name`_callback: calling callback (`cmd.c_name`)");
        `cmd.c_name`(p, cmd, &in_args, &out_args);
    }
    catch(std::exception &ex)
    {
        sim::setLastError(cmd, ex.what());
    }

    try
    {
        addStubsDebugLog("`cmd.c_name`_callback: writing output arguments...");
        addStubsDebugStackDump(p->stackID);

#py if cmd.clear_stack_before_writing_output:
        addStubsDebugLog("`cmd.c_name`_callback: clearing stack content before writing output arguments");
        // clear stack
        sim::popStackItem(p->stackID, 0);

#py endif

        // write output arguments to stack

#py for i, p in enumerate(cmd.returns):
        addStubsDebugLog("`cmd.c_name`_callback: writing output argument `i+1` \"`p.name`\" (`p.ctype()`)...");
        try
        {
#py if isinstance(p, model.ParamTable):
#py if p.itype == 'float':
            write__table__oneshot(out_args.`p.name`, sim::pushFloatTableOntoStack, p->stackID);
#py elif p.itype == 'double':
            write__table__oneshot(out_args.`p.name`, sim::pushDoubleTableOntoStack, p->stackID);
#py elif p.itype == 'int':
            write__table__oneshot(out_args.`p.name`, sim::pushInt32TableOntoStack, p->stackID);
#py else:
            write__table__insert(out_args.`p.name`, write__`p.ctype_normalized()`, p->stackID);
#py endif
#py else:
            write__`p.ctype_normalized()`(out_args.`p.name`, p->stackID);
#py endif
        }
        catch(std::exception &ex)
        {
            throw sim::exception("write out arg `i+1` (`p.name`): %s", ex.what());
        }
#py endfor

        addStubsDebugLog("`cmd.c_name`_callback: stack content after writing output arguments:");
        addStubsDebugStackDump(p->stackID);
    }
    catch(std::exception &ex)
    {
        sim::setLastError(cmd, ex.what());
        // clear stack
        try { sim::popStackItem(p->stackID, 0); } catch(...) {}
    }

    addStubsDebugLog("`cmd.c_name`_callback: finished");
}

#py endfor
#py for fn in plugin.script_functions:
`fn.c_in_name`::`fn.c_in_name`()
{
#py for p in fn.params:
#py if p.default is not None:
    `p.name` = `p.cdefault()`;
#py endif
#py endfor
}

`fn.c_out_name`::`fn.c_out_name`()
{
#py for p in fn.returns:
#py if p.default is not None:
    `p.name` = `p.cdefault()`;
#py endif
#py endfor
}

bool `fn.c_name`(simInt scriptId, const char *func, `fn.c_in_name` *in_args, `fn.c_out_name` *out_args)
{
    addStubsDebugLog("`fn.c_name`: writing input arguments...");

    int stackID = -1;

    try
    {
        stackID = sim::createStack();

        // write input arguments to stack

#py for i, p in enumerate(fn.params):
        addStubsDebugLog("`fn.c_name`: writing input argument `i+1` \"`p.name`\" (`p.ctype()`)...");
        try
        {
#py if isinstance(p, model.ParamTable):
#py if p.itype == 'float':
            write__table__oneshot(in_args.`p.name`, sim::pushFloatTableOntoStack, stackID);
#py elif p.itype == 'double':
            write__table__oneshot(in_args.`p.name`, sim::pushDoubleTableOntoStack, stackID);
#py elif p.itype == 'int':
            write__table__oneshot(in_args.`p.name`, sim::pushInt32TableOntoStack, stackID);
#py else:
            write__table__insert(in_args.`p.name`, write__`p.ctype_normalized()`, stackID);
#py endif
#py else:
            write__`p.ctype_normalized()`(in_args->`p.name`, stackID);
#py endif
        }
        catch(std::exception &ex)
        {
            throw sim::exception("writing input argument `i+1` (`p.name`): %s", ex.what());
        }
#py endfor

        addStubsDebugLog("`fn.c_name`: wrote input arguments:");
        addStubsDebugStackDump(stackID);

        sim::callScriptFunctionEx(scriptId, func, stackID);

        // read output arguments from stack

        addStubsDebugLog("`fn.c_name`: reading output arguments...");

#py for i, p in enumerate(fn.returns):
        addStubsDebugLog("`fn.c_name`: reading output argument `i+1` \"`p.name`\" (`p.ctype()`)...");
        try
        {
            sim::moveStackItemToTop(stackID, 0);
#py if isinstance(p, model.ParamTable):
#py if p.itype == 'float':
            read__table__oneshot(stackID, &(out_args->`p.name`), sim::getStackFloatTable, `p.minsize`, `p.maxsize`);
#py elif p.itype == 'double':
            read__table__oneshot(stackID, &(out_args->`p.name`), sim::getStackDoubleTable, `p.minsize`, `p.maxsize`);
#py elif p.itype == 'int':
            read__table__oneshot(stackID, &(out_args->`p.name`), sim::getStackInt32Table, `p.minsize`, `p.maxsize`);
#py else:
            read__table__unfold(stackID, &(out_args->`p.name`), read__`p.ctype_normalized()`, `p.minsize`, `p.maxsize`);
#py endif
#py else:
            read__`p.ctype_normalized()`(stackID, &(out_args->`p.name`));
#py endif
        }
        catch(std::exception &ex)
        {
            throw sim::exception("read out arg `i+1` (`p.name`): %s", ex.what());
        }
#py endfor

        addStubsDebugLog("`fn.c_name`: stack content after reading output arguments:");
        addStubsDebugStackDump(stackID);

        sim::releaseStack(stackID);
        stackID = -1;
    }
    catch(std::exception &ex)
    {
        if(stackID != -1)
            try { sim::releaseStack(stackID); } catch(...) {}
        sim::setLastError(func, ex.what());
        return false;
    }

    addStubsDebugLog("`fn.c_name`: finished");

    return true;
}

#py endfor
