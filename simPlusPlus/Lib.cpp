#include <simPlusPlus/Lib.h>
#include <simPlusPlus/Plugin.h>

namespace sim
{

static bool debugStackEnabled = false;

void enableStackDebug()
{
    debugStackEnabled = true;
}

void disableStackDebug()
{
    debugStackEnabled = false;
}

bool isStackDebugEnabled()
{
    return debugStackEnabled;
}

void addLog(int verbosity, const std::string &msg)
{
    ::simAddLog(pluginName.c_str(), verbosity, msg.c_str());
}

void addLog(int verbosity, boost::format &fmt)
{
    addLog(verbosity, fmt.str());
}

#ifndef NDEBUG

void addStackDebugLog(const std::string &msg)
{
    if(debugStackEnabled)
        addLog(sim_verbosity_debug, "DEBUG_STUBS: %s", msg);
}

void addStackDebugLog(boost::format fmt)
{
    addStackDebugLog(fmt.str());
}

void addStackDebugDump(int stackHandle)
{
    if(debugStackEnabled)
        debugStack(stackHandle);
}

#else // RELEASE
#define addStackDebugLog(x)
#define addStackDebugDump(x)
#endif

simInt registerScriptCallbackFunction(const std::string &funcNameAtPluginName, const std::string &callTips, simVoid (*callBack)(struct SScriptCallBack *cb))
{
    simInt ret = simRegisterScriptCallbackFunction(funcNameAtPluginName.c_str(), callTips.c_str(), callBack);
    if(ret == 0)
    {
        addLog(sim_verbosity_warnings, "replaced function '%s'", funcNameAtPluginName);
    }
    if(ret == -1)
        throw api_error("simRegisterScriptCallbackFunction");
    return ret;
}

simInt registerScriptVariable(const std::string &varName, const std::string &varValue, simInt stackID)
{
    simInt ret = simRegisterScriptVariable(varName.c_str(), varValue.c_str(), stackID);
    if(ret == 0)
    {
        addLog(sim_verbosity_warnings, "replaced variable '%s'", varName);
    }
    if(ret == -1)
        throw api_error("simRegisterScriptVariable");
    return ret;
}

simVoid callScriptFunctionEx(simInt scriptHandleOrType, const std::string &functionNameAtScriptName, simInt stackId)
{
    if(simCallScriptFunctionEx(scriptHandleOrType, functionNameAtScriptName.c_str(), stackId) == -1)
        throw api_error("simCallScriptFunctionEx");
}

simInt createStack()
{
    addStackDebugLog("simCreateStack");

    simInt ret = simCreateStack();
    if(ret == -1)
        throw api_error("simCreateStack");

    addStackDebugDump(ret);

    return ret;
}

simVoid releaseStack(simInt stackHandle)
{
    addStackDebugLog("simReleaseStack");

    if(simReleaseStack(stackHandle) != 1)
        throw api_error("simReleaseStack");
}

simInt copyStack(simInt stackHandle)
{
    addStackDebugLog("simCopyStack");

    simInt ret = simCopyStack(stackHandle);
    if(ret == -1)
        throw api_error("simCopyStack");
    return ret;
}

simVoid pushNullOntoStack(simInt stackHandle)
{
    addStackDebugLog("simPushNullOntoStack");

    if(simPushNullOntoStack(stackHandle) == -1)
        throw api_error("simPushNullOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushBoolOntoStack(simInt stackHandle, simBool value)
{
    addStackDebugLog(boost::format("simPushBoolOntoStack %d") % value);

    if(simPushBoolOntoStack(stackHandle, value) == -1)
        throw api_error("simPushBoolOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushInt32OntoStack(simInt stackHandle, simInt value)
{
    addStackDebugLog(boost::format("simPushInt32OntoStack %d") % value);

    if(simPushInt32OntoStack(stackHandle, value) == -1)
        throw api_error("simPushInt32OntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushFloatOntoStack(simInt stackHandle, simFloat value)
{
    addStackDebugLog(boost::format("simPushFloatOntoStack %f") % value);

    if(simPushFloatOntoStack(stackHandle, value) == -1)
        throw api_error("simPushFloatOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushDoubleOntoStack(simInt stackHandle, simDouble value)
{
    addStackDebugLog(boost::format("simPushDoubleOntoStack %g") % value);

    if(simPushDoubleOntoStack(stackHandle, value) == -1)
        throw api_error("simPushDoubleOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushStringOntoStack(simInt stackHandle, const simChar *value, simInt stringSize)
{
    addStackDebugLog(boost::format("simPushStringOntoStack \"%s\" [%d]") % value % stringSize);

    if(simPushStringOntoStack(stackHandle, value, stringSize) == -1)
        throw api_error("simPushStringOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushUInt8TableOntoStack(simInt stackHandle, const simUChar *values, simInt valueCnt)
{
    addStackDebugLog(boost::format("simPushUInt8TableOntoStack <%d values>") % valueCnt);

    if(simPushUInt8TableOntoStack(stackHandle, values, valueCnt) == -1)
        throw api_error("simPushUInt8TableOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushInt32TableOntoStack(simInt stackHandle, const simInt *values, simInt valueCnt)
{
    addStackDebugLog(boost::format("simPushInt32TableOntoStack <%d values>") % valueCnt);

    if(simPushInt32TableOntoStack(stackHandle, values, valueCnt) == -1)
        throw api_error("simPushInt32TableOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushFloatTableOntoStack(simInt stackHandle, const simFloat *values, simInt valueCnt)
{
    addStackDebugLog(boost::format("simPushFloatTableOntoStack <%d values>") % valueCnt);

    if(simPushFloatTableOntoStack(stackHandle, values, valueCnt) == -1)
        throw api_error("simPushFloatTableOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushDoubleTableOntoStack(simInt stackHandle, const simDouble *values, simInt valueCnt)
{
    addStackDebugLog(boost::format("simPushDoubleTableOntoStack <%d values>") % valueCnt);

    if(simPushDoubleTableOntoStack(stackHandle, values, valueCnt) == -1)
        throw api_error("simPushDoubleTableOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushTableOntoStack(simInt stackHandle)
{
    addStackDebugLog("simPushTableOntoStack");

    if(simPushTableOntoStack(stackHandle) == -1)
        throw api_error("simPushTableOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid insertDataIntoStackTable(simInt stackHandle)
{
    addStackDebugLog("simInsertDataIntoStackTable");

    if(simInsertDataIntoStackTable(stackHandle) == -1)
        throw api_error("simInsertDataIntoStackTable");

    addStackDebugDump(stackHandle);
}

simInt getStackSize(simInt stackHandle)
{
    simInt ret = simGetStackSize(stackHandle);

    addStackDebugLog(boost::format("simGetStackSize -> %d") % ret);

    if(ret == -1)
        throw api_error("simGetStackSize");
    return ret;
}

simInt popStackItem(simInt stackHandle, simInt count)
{
    simInt ret = simPopStackItem(stackHandle, count);

    addStackDebugLog(boost::format("simPopStackItem %d -> %d") % count % ret);

    if(ret == -1)
        throw api_error("simPopStackItem");

    addStackDebugDump(stackHandle);

    return ret;
}

simVoid moveStackItemToTop(simInt stackHandle, simInt cIndex)
{
    addStackDebugLog(boost::format("simMoveStackItemToTop %d") % cIndex);

    if(simMoveStackItemToTop(stackHandle, cIndex) == -1)
        throw api_error("simMoveStackItemToTop");

    addStackDebugDump(stackHandle);
}

simInt isStackValueNull(simInt stackHandle)
{
    simInt ret = simIsStackValueNull(stackHandle);
    if(ret == -1)
        throw api_error("simIsStackValueNull");

    addStackDebugLog(boost::format("simIsStackValueNull -> %d") % ret);

    return ret;
}

simInt getStackBoolValue(simInt stackHandle, simBool *boolValue)
{
    simInt ret = simGetStackBoolValue(stackHandle, boolValue);
    if(ret == -1)
        throw api_error("simGetStackBoolValue");

    addStackDebugLog(boost::format("simGetStackBoolValue -> %d, value = %d") % ret % boolValue);

    return ret;
}

simInt getStackInt32Value(simInt stackHandle, simInt *numberValue)
{
    simInt ret = simGetStackInt32Value(stackHandle, numberValue);
    if(ret == -1)
        throw api_error("simGetStackInt32Value");

    addStackDebugLog(boost::format("simGetStackInt32Value -> %d, value = %d") % ret % numberValue);

    return ret;
}

simInt getStackFloatValue(simInt stackHandle, simFloat *numberValue)
{
    simInt ret = simGetStackFloatValue(stackHandle, numberValue);
    if(ret == -1)
        throw api_error("simGetStackFloatValue");

    addStackDebugLog(boost::format("simGetStackFloatValue -> %d, value = %f") % ret % numberValue);

    return ret;
}

simInt getStackDoubleValue(simInt stackHandle, simDouble *numberValue)
{
    simInt ret = simGetStackDoubleValue(stackHandle, numberValue);
    if(ret == -1)
        throw api_error("simGetStackDoubleValue");

    addStackDebugLog(boost::format("simGetStackDoubleValue -> %d, value = %g") % ret % numberValue);

    return ret;
}

simChar* getStackStringValue(simInt stackHandle, simInt *stringSize)
{
    simChar *ret = simGetStackStringValue(stackHandle, stringSize);

#ifndef NDEBUG
    if(debugStackEnabled)
    {
        if(ret)
        {
            std::string s(ret, *stringSize);
            addStackDebugLog(boost::format("simGetStackStringValue -> value = %s") % s);
        }
        else
        {
            addStackDebugLog("simGetStackStringValue -> null");
        }
    }
#endif // NDEBUG

    // if stringSize is NULL, we cannot distinguish error (-1) from type error (0)
    if(ret == NULL && stringSize && *stringSize == -1)
        throw api_error("simGetStackStringValue");

    return ret;
}

simInt getStackTableInfo(simInt stackHandle, simInt infoType)
{
    simInt ret = simGetStackTableInfo(stackHandle, infoType);

    addStackDebugLog(boost::format("simGetStackTableInfo %d -> %d") % infoType % ret);

    if(ret == -1)
        throw api_error("simGetStackTableInfo");
    return ret;
}

simInt getStackUInt8Table(simInt stackHandle, simUChar *array, simInt count)
{
    simInt ret = simGetStackUInt8Table(stackHandle, array, count);

    addStackDebugLog(boost::format("simGetStackUInt8Table count = %d -> %d") % count % ret);

    if(ret == -1)
        throw api_error("simGetStackUInt8Table");
    return ret;
}

simInt getStackInt32Table(simInt stackHandle, simInt *array, simInt count)
{
    simInt ret = simGetStackInt32Table(stackHandle, array, count);

    addStackDebugLog(boost::format("simGetStackInt32Table count = %d -> %d") % count % ret);

    if(ret == -1)
        throw api_error("simGetStackInt32Table");
    return ret;
}

simInt getStackFloatTable(simInt stackHandle, simFloat *array, simInt count)
{
    simInt ret = simGetStackFloatTable(stackHandle, array, count);

    addStackDebugLog(boost::format("simGetStackFloatTable count = %d -> %d") % count % ret);

    if(ret == -1)
        throw api_error("simGetStackFloatTable");
    return ret;
}

simInt getStackDoubleTable(simInt stackHandle, simDouble *array, simInt count)
{
    simInt ret = simGetStackDoubleTable(stackHandle, array, count);

    addStackDebugLog(boost::format("simGetStackDoubleTable, count = %d -> %d") % count % ret);

    if(ret == -1)
        throw api_error("simGetStackDoubleTable");
    return ret;
}

simVoid unfoldStackTable(simInt stackHandle)
{
    addStackDebugLog("simUnfoldStackTable");

    if(simUnfoldStackTable(stackHandle) == -1)
        throw api_error("simUnfoldStackTable");

#ifndef NDEBUG
    if(debugStackEnabled)
    {
        simDebugStack(stackHandle, -1);
    }
#endif // NDEBUG
}

simVoid debugStack(simInt stackHandle, simInt index)
{
    if(simDebugStack(stackHandle, index) == -1)
        throw api_error("simDebugStack");
}

simVoid pushStringOntoStack(simInt stackHandle, const std::string &value)
{
    addStackDebugLog(boost::format("simPushStringOntoStack \"%s\" [%d]") % value.c_str() % value.size());

    if(simPushStringOntoStack(stackHandle, value.c_str(), value.size()) == -1)
        throw api_error("simPushStringOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushUInt8TableOntoStack(simInt stackHandle, const std::vector<simUChar> &values)
{
    pushUInt8TableOntoStack(stackHandle, values.data(), values.size());
}

simVoid pushInt32TableOntoStack(simInt stackHandle, const std::vector<simInt> &values)
{
    pushInt32TableOntoStack(stackHandle, values.data(), values.size());
}

simVoid pushFloatTableOntoStack(simInt stackHandle, const std::vector<simFloat> &values)
{
    pushFloatTableOntoStack(stackHandle, values.data(), values.size());
}

simVoid pushDoubleTableOntoStack(simInt stackHandle, const std::vector<simDouble> &values)
{
    pushDoubleTableOntoStack(stackHandle, values.data(), values.size());
}

simVoid getStackStringValue(simInt stackHandle, std::string *stringValue)
{
    simInt stringSize = -1;
    simChar *ret = getStackStringValue(stackHandle, &stringSize);
    *stringValue = std::string(ret, stringSize);
}

simInt getStackUInt8Table(simInt stackHandle, std::vector<simUChar> *v)
{
    simInt count = getStackTableInfo(stackHandle, 0);
    v->resize(count);
    return getStackUInt8Table(stackHandle, v->data(), count);
}

simInt getStackInt32Table(simInt stackHandle, std::vector<simInt> *v)
{
    simInt count = getStackTableInfo(stackHandle, 0);
    v->resize(count);
    return getStackInt32Table(stackHandle, v->data(), count);
}

simInt getStackFloatTable(simInt stackHandle, std::vector<simFloat> *v)
{
    simInt count = getStackTableInfo(stackHandle, 0);
    v->resize(count);
    return getStackFloatTable(stackHandle, v->data(), count);
}

simInt getStackDoubleTable(simInt stackHandle, std::vector<simDouble> *v)
{
    simInt count = getStackTableInfo(stackHandle, 0);
    v->resize(count);
    return getStackDoubleTable(stackHandle, v->data(), count);
}

simInt getBoolParameter(simInt parameter)
{
    simInt ret;
    if((ret = simGetBoolParameter(parameter)) == -1)
        throw api_error("simGetBoolParameter");
    return ret;
}

simInt getInt32Parameter(simInt parameter)
{
    simInt ret;
    if(simGetInt32Parameter(parameter, &ret) == -1)
        throw api_error("simGetInt32Parameter");
    return ret;
}

simUInt64 getUInt64Parameter(simInt parameter)
{
    simUInt64 ret;
    if(simGetUInt64Parameter(parameter, &ret) == -1)
        throw api_error("simGetUInt64Parameter");
    return ret;
}

simFloat getFloatParameter(simInt parameter)
{
    simFloat ret;
    if(simGetFloatParameter(parameter, &ret) == -1)
        throw api_error("simGetFloatParameter");
    return ret;
}

std::array<simFloat, 3> getArrayParameter(simInt parameter)
{
    std::array<simFloat, 3> ret;
    if(simGetArrayParameter(parameter, ret.data()) == -1)
        throw api_error("simGetArrayParameter");
    return ret;
}

std::string getStringParameter(simInt parameter)
{
    simChar *ret;
    if((ret = simGetStringParameter(parameter)) == nullptr)
        throw api_error("simGetStringParameter");
    std::string s(ret);
    releaseBuffer(ret);
    return s;
}

simVoid setBoolParameter(simInt parameter, simBool value)
{
    if(simSetBoolParameter(parameter, value) == -1)
        throw api_error("simSetBoolParameter");
}

simVoid setInt32Parameter(simInt parameter, simInt value)
{
    if(simSetInt32Parameter(parameter, value) == -1)
        throw api_error("simSetInt32Parameter");
}

simVoid setFloatParameter(simInt parameter, simFloat value)
{
    if(simSetFloatParameter(parameter, value) == -1)
        throw api_error("simSetFloatParameter");
}

simVoid setArrayParameter(simInt parameter, std::array<simFloat, 3> value)
{
    if(simSetArrayParameter(parameter, value.data()) == -1)
        throw api_error("simSetArrayParameter");
}

simVoid setStringParameter(simInt parameter, simChar *value)
{
    if(simSetStringParameter(parameter, value) == -1)
        throw api_error("simSetStringParameter");
}

simVoid setStringParameter(simInt parameter, const std::string &value)
{
    setStringParameter(parameter, value.c_str());
}

simChar* createBuffer(simInt size)
{
    simChar *ret = simCreateBuffer(size);
    if(ret == NULL)
        throw api_error("simCreateBuffer");
    return ret;
}

simVoid releaseBuffer(simChar *buffer)
{
    if(simReleaseBuffer(buffer) == -1)
        throw api_error("simReleaseBuffer");
}

simVoid setLastError(const std::string &func, const std::string &msg)
{
    if(simSetLastError(func.c_str(), msg.c_str()) == -1)
        throw api_error("simSetLastError");
}

} // namespace sim
