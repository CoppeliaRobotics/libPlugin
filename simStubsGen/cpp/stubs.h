#py from parse import parse
#py import model
#py plugin = parse(pycpp.params['xml_file'])
#ifndef STUBS_H__INCLUDED
#define STUBS_H__INCLUDED

#include <simPlusPlus/Lib.h>
#include <string>
#include <vector>
#include <boost/assign/list_of.hpp>
#include <boost/format.hpp>
#include <boost/optional.hpp>

class FuncTracer
{
    int l_;
    std::string f_;
public:
    FuncTracer(const std::string &f, int l = sim_verbosity_trace);
    ~FuncTracer();
};

#ifndef __FUNC__
#ifdef __PRETTY_FUNCTION__
#define __FUNC__ __PRETTY_FUNCTION__
#else
#define __FUNC__ __func__
#endif
#endif // __FUNC__

#define TRACE_FUNC FuncTracer __funcTracer__##__LINE__((boost::format("%s:%d:%s:") % __FILE__ % __LINE__ % __FUNC__).str())

#ifdef QT_COMPIL
#include <QThread>

extern Qt::HANDLE UI_THREAD;
extern Qt::HANDLE SIM_THREAD;

std::string threadNickname();
void uiThread();
void simThread();

#define ASSERT_THREAD(ID) \
    if(UI_THREAD == NULL) {\
        sim::addLog(sim_verbosity_debug, "warning: cannot check ASSERT_THREAD(" #ID ") because global variable UI_THREAD is not set yet.");\
    } else if(strcmp(#ID, "UI") == 0) {\
        if(QThread::currentThreadId() != UI_THREAD) {\
            sim::addLog(sim_verbosity_errors, "%s:%d %s should be called from UI thread", __FILE__, __LINE__, __FUNC__);\
            exit(1);\
        }\
    } else if(strcmp(#ID, "!UI") == 0) {\
        if(QThread::currentThreadId() == UI_THREAD) {\
            sim::addLog(sim_verbosity_errors, "%s:%d %s should NOT be called from UI thread", __FILE__, __LINE__, __FUNC__);\
            exit(1);\
        }\
    } else {\
        sim::addLog(sim_verbosity_debug, "warning: cannot check ASSERT_THREAD(" #ID "). Can check only UI and !UI.");\
    }
#endif // QT_COMPIL

template<typename T>
struct Grid
{
    std::vector<int> dims;
    std::vector<T> data;
};

struct ReadOptions
{
    size_t minSize{0};
    size_t maxSize{std::numeric_limits<size_t>::max()};

    ReadOptions& setTableBounds(size_t minSize_, size_t maxSize_)
    {
        minSize = minSize_;
        maxSize = maxSize_;
        return *this;
    }
};

struct WriteOptions
{
    void *dummy{nullptr};
};

#py for struct in plugin.structs:
struct `struct.name`
{
#py for field in struct.fields:
    `field.ctype()` `field.name`;
#py endfor

    `struct.name`();
    `struct.name`(`', '.join(f'const {f.ctype()} &{f.name}_' for f in struct.fields)`) : `', '.join(f'{f.name}({f.name}_)' for f in struct.fields)` {}
};

#py endfor
void readFromStack(int stack, bool *value, const ReadOptions &rdopt = {});
void readFromStack(int stack, int *value, const ReadOptions &rdopt = {});
void readFromStack(int stack, long *value, const ReadOptions &rdopt = {});
void readFromStack(int stack, float *value, const ReadOptions &rdopt = {});
void readFromStack(int stack, double *value, const ReadOptions &rdopt = {});
void readFromStack(int stack, std::string *value, const ReadOptions &rdopt = {});
#py for struct in plugin.structs:
void readFromStack(int stack, `struct.name` *value, const ReadOptions &rdopt = {});
#py endfor
void writeToStack(const bool &value, int stack, const WriteOptions &wropt = {});
void writeToStack(const int &value, int stack, const WriteOptions &wropt = {});
void writeToStack(const long &value, int stack, const WriteOptions &wropt = {});
void writeToStack(const float &value, int stack, const WriteOptions &wropt = {});
void writeToStack(const double &value, int stack, const WriteOptions &wropt = {});
void writeToStack(const std::string &value, int stack, const WriteOptions &wropt = {});
#py for struct in plugin.structs:
void writeToStack(const `struct.name` &value, int stack, const WriteOptions &wropt = {});
#py endfor

bool registerScriptStuff();

#py for enum in plugin.enums:
enum `enum.name`
{
#py for i, item in enumerate(enum.items):
    sim_`plugin.name.lower()`_`enum.item_prefix``item.name` = `item.value`,
#py endfor
};

const char* `enum.name.lower()`_string(`enum.name` x);

#py endfor
#py for cmd in plugin.commands:
struct `cmd.c_in_name`
{
    int _scriptID;
    int _stackID;
#py for p in cmd.params:
    `p.ctype()` `p.name`;
#py endfor

    `cmd.c_in_name`();
};

struct `cmd.c_out_name`
{
#py for p in cmd.returns:
    `p.ctype()` `p.name`;
#py endfor

    `cmd.c_out_name`();
};

void `cmd.c_name`(SScriptCallBack *p, `cmd.c_in_name` *in, `cmd.c_out_name` *out);
#py if len(cmd.returns) == 1:
`cmd.returns[0].ctype()` `cmd.c_name`(`cmd.c_arg_list(pre_args=['SScriptCallBack *p'], defaults=True)`);
#py endif
#py if len(cmd.returns) == 0:
void `cmd.c_name`(`cmd.c_arg_list(pre_args=['SScriptCallBack *p'], defaults=True)`);
#py endif
void `cmd.c_name`(`cmd.c_arg_list(pre_args=['SScriptCallBack *p', '%s *out' % cmd.c_out_name], defaults=True)`);
void `cmd.c_name`_callback(SScriptCallBack *p);

#py endfor
#py for fn in plugin.script_functions:
struct `fn.c_in_name`
{
#py for p in fn.params:
    `p.ctype()` `p.name`;
#py endfor

    `fn.c_in_name`();
};

struct `fn.c_out_name`
{
#py for p in fn.returns:
    `p.ctype()` `p.name`;
#py endfor

    `fn.c_out_name`();
};

#py if len(fn.returns) == 1:
`fn.returns[0].ctype()` `fn.c_name`(`fn.c_arg_list(pre_args=['simInt scriptId', 'const char *func'], defaults=True)`);
#py endif
#py if len(fn.returns) == 0:
void `fn.c_name`(`fn.c_arg_list(pre_args=['simInt scriptId', 'const char *func'], defaults=True)`);
#py endif
bool `fn.c_name`(simInt scriptId, const char *func, `fn.c_in_name` *in_args, `fn.c_out_name` *out_args);

#py endfor
// following functions must be implemented in the plugin

#py for cmd in plugin.commands:
void `cmd.c_name`(SScriptCallBack *p, const char *cmd, `cmd.c_in_name` *in, `cmd.c_out_name` *out);
#py endfor

#endif // STUBS_H__INCLUDED
