#ifndef SIMPLUSPLUS_LIB_H_INCLUDED
#define SIMPLUSPLUS_LIB_H_INCLUDED

#if __cplusplus <= 199711L
    #error simPlusPlus needs at least a C++11 compliant compiler
#endif

#include <string>
#include <vector>
#include <array>
#include <stdexcept>
#include <boost/optional.hpp>
#include <boost/format.hpp>

#include <simLib.h>

namespace sim
{
	/*! \brief A basic exception class
	 */
    struct exception : public ::std::exception
    {
        std::string s;

        exception(std::string s_)
            : s(s_)
        {
        }

        exception(boost::format f)
            : s(f.str())
        {
        }

        ~exception() throw()
        {
        }

        const char* what() const throw()
        {
            return s.c_str();
        }
    };

    struct api_error : public ::std::exception
    {
        std::string func;
        std::string error;
        std::string msg;

        api_error(const std::string &func_)
            : api_error(func_, "error")
        {
        }

        api_error(const std::string &func_, const std::string &error_)
            : func(func_),
              error(error_),
              msg((boost::format("%s: %s") % func % error).str())
        {
        }

        ~api_error() throw()
        {
        }

        const char* what() const throw()
        {
            return msg.c_str();
        }
    };

    void enableStackDebug();
    void disableStackDebug();
    bool isStackDebugEnabled();

    void addLog(int verbosity, const std::string &msg);
    void addLog(int verbosity, boost::format &fmt);

    simInt registerScriptCallbackFunction(const std::string &funcNameAtPluginName, const std::string &callTips, simVoid (*callBack)(struct SScriptCallBack *cb));
    simInt registerScriptVariable(const std::string &varName, const std::string &varValue, simInt stackID);

    simVoid callScriptFunctionEx(simInt scriptHandleOrType, const std::string &functionNameAtScriptName, simInt stackId);

    simInt createStack();
    simVoid releaseStack(simInt stackHandle);
    simInt copyStack(simInt stackHandle);
    simVoid pushNullOntoStack(simInt stackHandle);
    simVoid pushBoolOntoStack(simInt stackHandle, simBool value);
    simVoid pushInt32OntoStack(simInt stackHandle, simInt value);
    simVoid pushFloatOntoStack(simInt stackHandle, simFloat value);
    simVoid pushDoubleOntoStack(simInt stackHandle, simDouble value);
    simVoid pushStringOntoStack(simInt stackHandle, const simChar *value, simInt stringSize);
    simVoid pushUInt8TableOntoStack(simInt stackHandle, const simUChar *values, simInt valueCnt);
    simVoid pushInt32TableOntoStack(simInt stackHandle, const simInt *values, simInt valueCnt);
    simVoid pushFloatTableOntoStack(simInt stackHandle, const simFloat *values, simInt valueCnt);
    simVoid pushDoubleTableOntoStack(simInt stackHandle, const simDouble *values, simInt valueCnt);
    simVoid pushTableOntoStack(simInt stackHandle);
    simVoid insertDataIntoStackTable(simInt stackHandle);
    simInt getStackSize(simInt stackHandle);
    simInt popStackItem(simInt stackHandle, simInt count);
    simVoid moveStackItemToTop(simInt stackHandle, simInt cIndex);
    simInt isStackValueNull(simInt stackHandle);
    simInt getStackBoolValue(simInt stackHandle, simBool *boolValue);
    simInt getStackInt32Value(simInt stackHandle, simInt *numberValue);
    simInt getStackFloatValue(simInt stackHandle, simFloat *numberValue);
    simInt getStackDoubleValue(simInt stackHandle, simDouble *numberValue);
    simChar* getStackStringValue(simInt stackHandle, simInt *stringSize);
    simInt getStackTableInfo(simInt stackHandle, simInt infoType);
    simInt getStackUInt8Table(simInt stackHandle, simUChar *array, simInt count);
    simInt getStackInt32Table(simInt stackHandle, simInt *array, simInt count);
    simInt getStackFloatTable(simInt stackHandle, simFloat *array, simInt count);
    simInt getStackDoubleTable(simInt stackHandle, simDouble *array, simInt count);
    simVoid unfoldStackTable(simInt stackHandle);
    simVoid unfoldStackTable(simInt stackHandle);
    simVoid debugStack(simInt stackHandle, simInt index = -1);

    simVoid pushStringOntoStack(simInt stackHandle, const std::string &value);
    simVoid pushUInt8TableOntoStack(simInt stackHandle, const std::vector<simUChar> &values);
    simVoid pushInt32TableOntoStack(simInt stackHandle, const std::vector<simInt> &values);
    simVoid pushFloatTableOntoStack(simInt stackHandle, const std::vector<simFloat> &values);
    simVoid pushDoubleTableOntoStack(simInt stackHandle, const std::vector<simDouble> &values);
    simVoid getStackStringValue(simInt stackHandle, std::string *stringValue);
    simInt getStackUInt8Table(simInt stackHandle, std::vector<simUChar> *v);
    simInt getStackInt32Table(simInt stackHandle, std::vector<simInt> *v);
    simInt getStackFloatTable(simInt stackHandle, std::vector<simFloat> *v);
    simInt getStackDoubleTable(simInt stackHandle, std::vector<simDouble> *v);

    simInt getBoolParameter(simInt parameter);
    simInt getInt32Parameter(simInt parameter);
    simUInt64 getUInt64Parameter(simInt parameter);
    simFloat getFloatParameter(simInt parameter);
    std::array<simFloat, 3> getArrayParameter(simInt parameter);
    std::string getStringParameter(simInt parameter);
    simVoid setBoolParameter(simInt parameter, simBool value);
    simVoid setInt32Parameter(simInt parameter, simInt value);
    simVoid setFloatParameter(simInt parameter, simFloat value);
    simVoid setArrayParameter(simInt parameter, std::array<simFloat, 3> value);
    simVoid setStringParameter(simInt parameter, simChar *value);
    simVoid setStringParameter(simInt parameter, const std::string &value);

    simChar* createBuffer(simInt size);
    simVoid releaseBuffer(simChar *buffer);

    simVoid setLastError(const std::string &func, const std::string &msg);

    template<class T, class... Args>
    void addLog(int verbosity, boost::format& f, T&& t, Args&&... args)
    {
        addLog(verbosity, f % std::forward<T>(t), std::forward<Args>(args)...);
    }

    template<typename... Arguments>
    void addLog(int verbosity, const std::string &fmt, Arguments&&... args)
    {
        boost::format f(fmt);

        addLog(verbosity, f, std::forward<Arguments>(args)...);
    }
} // namespace sim

#endif // SIMPLUSPLUS_LIB_H_INCLUDED
