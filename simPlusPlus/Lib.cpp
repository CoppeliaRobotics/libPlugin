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

#ifndef NDEBUG

template<typename... Arguments>
void addStackDebugLog(const std::string &fmt, Arguments&&... args)
{
    if(debugStackEnabled)
    {
        auto msg = util::sprintf(fmt, std::forward<Arguments>(args)...);
        addLog(sim_verbosity_debug, "DEBUG_STUBS: %s", msg);
    }
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
    addStackDebugLog("simPushBoolOntoStack %d", value);

    if(simPushBoolOntoStack(stackHandle, value) == -1)
        throw api_error("simPushBoolOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushInt32OntoStack(simInt stackHandle, simInt value)
{
    addStackDebugLog("simPushInt32OntoStack %d", value);

    if(simPushInt32OntoStack(stackHandle, value) == -1)
        throw api_error("simPushInt32OntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushFloatOntoStack(simInt stackHandle, simFloat value)
{
    addStackDebugLog("simPushFloatOntoStack %f", value);

    if(simPushFloatOntoStack(stackHandle, value) == -1)
        throw api_error("simPushFloatOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushDoubleOntoStack(simInt stackHandle, simDouble value)
{
    addStackDebugLog("simPushDoubleOntoStack %g", value);

    if(simPushDoubleOntoStack(stackHandle, value) == -1)
        throw api_error("simPushDoubleOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushStringOntoStack(simInt stackHandle, const simChar *value, simInt stringSize)
{
    addStackDebugLog("simPushStringOntoStack \"%s\" [%d]", value, stringSize);

    if(simPushStringOntoStack(stackHandle, value, stringSize) == -1)
        throw api_error("simPushStringOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushUInt8TableOntoStack(simInt stackHandle, const simUChar *values, simInt valueCnt)
{
    addStackDebugLog("simPushUInt8TableOntoStack <%d values>", valueCnt);

    if(simPushUInt8TableOntoStack(stackHandle, values, valueCnt) == -1)
        throw api_error("simPushUInt8TableOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushInt32TableOntoStack(simInt stackHandle, const simInt *values, simInt valueCnt)
{
    addStackDebugLog("simPushInt32TableOntoStack <%d values>", valueCnt);

    if(simPushInt32TableOntoStack(stackHandle, values, valueCnt) == -1)
        throw api_error("simPushInt32TableOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushFloatTableOntoStack(simInt stackHandle, const simFloat *values, simInt valueCnt)
{
    addStackDebugLog("simPushFloatTableOntoStack <%d values>", valueCnt);

    if(simPushFloatTableOntoStack(stackHandle, values, valueCnt) == -1)
        throw api_error("simPushFloatTableOntoStack");

    addStackDebugDump(stackHandle);
}

simVoid pushDoubleTableOntoStack(simInt stackHandle, const simDouble *values, simInt valueCnt)
{
    addStackDebugLog("simPushDoubleTableOntoStack <%d values>", valueCnt);

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

    addStackDebugLog("simGetStackSize -> %d", ret);

    if(ret == -1)
        throw api_error("simGetStackSize");
    return ret;
}

simInt popStackItem(simInt stackHandle, simInt count)
{
    simInt ret = simPopStackItem(stackHandle, count);

    addStackDebugLog("simPopStackItem %d -> %d", count, ret);

    if(ret == -1)
        throw api_error("simPopStackItem");

    addStackDebugDump(stackHandle);

    return ret;
}

simVoid moveStackItemToTop(simInt stackHandle, simInt cIndex)
{
    addStackDebugLog("simMoveStackItemToTop %d", cIndex);

    if(simMoveStackItemToTop(stackHandle, cIndex) == -1)
        throw api_error("simMoveStackItemToTop");

    addStackDebugDump(stackHandle);
}

simInt isStackValueNull(simInt stackHandle)
{
    simInt ret = simIsStackValueNull(stackHandle);
    if(ret == -1)
        throw api_error("simIsStackValueNull");

    addStackDebugLog("simIsStackValueNull -> %d", ret);

    return ret;
}

simInt getStackBoolValue(simInt stackHandle, simBool *boolValue)
{
    simInt ret = simGetStackBoolValue(stackHandle, boolValue);
    if(ret == -1)
        throw api_error("simGetStackBoolValue");

    addStackDebugLog("simGetStackBoolValue -> %d, value = %d", ret, boolValue);

    return ret;
}

simInt getStackInt32Value(simInt stackHandle, simInt *numberValue)
{
    simInt ret = simGetStackInt32Value(stackHandle, numberValue);
    if(ret == -1)
        throw api_error("simGetStackInt32Value");

    addStackDebugLog("simGetStackInt32Value -> %d, value = %d", ret, numberValue);

    return ret;
}

simInt getStackFloatValue(simInt stackHandle, simFloat *numberValue)
{
    simInt ret = simGetStackFloatValue(stackHandle, numberValue);
    if(ret == -1)
        throw api_error("simGetStackFloatValue");

    addStackDebugLog("simGetStackFloatValue -> %d, value = %f", ret, numberValue);

    return ret;
}

simInt getStackDoubleValue(simInt stackHandle, simDouble *numberValue)
{
    simInt ret = simGetStackDoubleValue(stackHandle, numberValue);
    if(ret == -1)
        throw api_error("simGetStackDoubleValue");

    addStackDebugLog("simGetStackDoubleValue -> %d, value = %g", ret, numberValue);

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
            addStackDebugLog("simGetStackStringValue -> value = %s", s);
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

    addStackDebugLog("simGetStackTableInfo %d -> %d", infoType, ret);

    if(ret == -1)
        throw api_error("simGetStackTableInfo");
    return ret;
}

simInt getStackUInt8Table(simInt stackHandle, simUChar *array, simInt count)
{
    simInt ret = simGetStackUInt8Table(stackHandle, array, count);

    addStackDebugLog("simGetStackUInt8Table count = %d -> %d", count, ret);

    if(ret == -1)
        throw api_error("simGetStackUInt8Table");
    return ret;
}

simInt getStackInt32Table(simInt stackHandle, simInt *array, simInt count)
{
    simInt ret = simGetStackInt32Table(stackHandle, array, count);

    addStackDebugLog("simGetStackInt32Table count = %d -> %d", count, ret);

    if(ret == -1)
        throw api_error("simGetStackInt32Table");
    return ret;
}

simInt getStackFloatTable(simInt stackHandle, simFloat *array, simInt count)
{
    simInt ret = simGetStackFloatTable(stackHandle, array, count);

    addStackDebugLog("simGetStackFloatTable count = %d -> %d", count, ret);

    if(ret == -1)
        throw api_error("simGetStackFloatTable");
    return ret;
}

simInt getStackDoubleTable(simInt stackHandle, simDouble *array, simInt count)
{
    simInt ret = simGetStackDoubleTable(stackHandle, array, count);

    addStackDebugLog("simGetStackDoubleTable, count = %d -> %d", count, ret);

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
    addStackDebugLog("simPushStringOntoStack \"%s\" [%d]", value.c_str(), value.size());

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

simFloat getObjectFloatParameter(simInt objectHandle, simInt parameterID, simFloat *parameter)
{
    simFloat ret;
    if(simGetObjectFloatParameter(objectHandle, parameterID, &ret) == -1)
        throw api_error("simGetObjectFloatParameter");
    return ret;
}

simInt getObjectInt32Parameter(simInt objectHandle, simInt parameterID, simInt *parameter)
{
    simInt ret;
    if(simGetObjectInt32Parameter(objectHandle, parameterID, &ret) == -1)
        throw api_error("simGetObjectInt32Parameter");
    return ret;
}

std::string getObjectStringParameter(simInt objectHandle, simInt parameterID)
{
    simChar *ret;
    simInt len;
    if((ret = simGetObjectStringParameter(objectHandle, parameterID, &len)) == NULL)
        throw api_error("simGetObjectStringParameter");
    std::string s(ret, len);
    releaseBuffer(ret);
    return s;
}

simVoid setObjectFloatParameter(simInt objectHandle, simInt parameterID, simFloat parameter)
{
    if(simSetObjectFloatParameter(objectHandle, parameterID, parameter) == -1)
        throw api_error("simSetObjectFloatParameter");
}

simVoid setObjectInt32Parameter(simInt objectHandle, simInt parameterID, simInt parameter)
{
    if(simSetObjectInt32Parameter(objectHandle, parameterID, parameter) == -1)
        throw api_error("simSetObjectInt32Parameter");
}

simVoid setObjectStringParameter(simInt objectHandle, simInt parameterID, const std::string &parameter)
{
    // XXX: fix const ptrs in the regular API
    if(simSetObjectStringParameter(objectHandle, parameterID, const_cast<simChar*>(parameter.data()), parameter.size()) == -1)
        throw api_error("simSetObjectStringParameter");
}

boost::optional<std::string> getStringNamedParam(const std::string &parameter)
{
    simChar *ret;
    simInt len;
    if((ret = simGetStringNamedParam(parameter.c_str(), &len)) == NULL)
        return {};
    std::string s(ret, len);
    releaseBuffer(ret);
    return s;
}

simVoid setStringNamedParam(const std::string &parameter, const std::string &value)
{
    if(simSetStringNamedParam(parameter.c_str(), value.c_str(), value.size()) == -1)
        throw api_error("simSetStringNamedParam");
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

std::string getLastError()
{
    simChar *ret = simGetLastError();
    if(ret == NULL)
        throw api_error("simGetLastError");
    std::string s(ret);
    releaseBuffer(ret);
    return s;
}

simVoid setLastError(const std::string &func, const std::string &msg)
{
    if(simSetLastError(func.c_str(), msg.c_str()) == -1)
        throw api_error("simSetLastError");
}

simInt getObjectChild(simInt objectHandle, simInt index)
{
    simInt ret;
    if((ret = simGetObjectChild(objectHandle, index)) == -1)
        throw api_error("simGetObjectChild");
    return ret;
}

simInt getObjectHandle(const std::string &objectName)
{
    simInt ret;
    if((ret = simGetObjectHandle(objectName.c_str())) == -1)
        throw api_error("simGetObjectHandle");
    return ret;
}

std::array<simFloat, 12> getObjectMatrix(simInt objectHandle, simInt relativeToObjectHandle)
{
    std::array<simFloat, 12> ret;
    if(simGetObjectMatrix(objectHandle, relativeToObjectHandle, ret.data()) == -1)
        throw api_error("simGetObjectMatrix");
    return ret;
}

simVoid setObjectMatrix(simInt objectHandle, simInt relativeToObjectHandle, const std::array<simFloat, 12> &matrix)
{
    if(simSetObjectMatrix(objectHandle, relativeToObjectHandle, matrix.data()) == -1)
        throw api_error("simSetObjectMatrix");
}

std::string getObjectName(simInt objectHandle)
{
    simChar *ret = simGetObjectName(objectHandle);
    if(ret == NULL)
        throw api_error("simGetObjectName");
    std::string s(ret);
    releaseBuffer(ret);
    return s;
}

simVoid setObjectName(simInt objectHandle, const std::string &objectName)
{
    if(simSetObjectName(objectHandle, objectName.c_str()) == -1)
        throw api_error("simSetObjectName");
}

std::array<simFloat, 3> getObjectOrientation(simInt objectHandle, simInt relativeToObjectHandle)
{
    std::array<simFloat, 3> ret;
    if(simGetObjectOrientation(objectHandle, relativeToObjectHandle, ret.data()) == -1)
        throw api_error("simGetObjectOrientation");
    return ret;
}

simVoid setObjectOrientation(simInt objectHandle, simInt relativeToObjectHandle, const std::array<simFloat, 3> &eulerAngles)
{
    if(simSetObjectOrientation(objectHandle, relativeToObjectHandle, eulerAngles.data()) == -1)
        throw api_error("simSetObjectOrientation");
}

simInt getObjectParent(simInt objectHandle)
{
    simInt ret;
    if((ret = simGetObjectParent(objectHandle)) == -1)
        throw api_error("simGetObjectParent");
    return ret;
}

simVoid setObjectParent(simInt objectHandle, simInt parentObjectHandle, simBool keepInPlace)
{
    if(simSetObjectParent(objectHandle, parentObjectHandle, keepInPlace) == -1)
        throw api_error("simSetObjectParent");
}

std::array<simFloat, 3> getObjectPosition(simInt objectHandle, simInt relativeToObjectHandle)
{
    std::array<simFloat, 3> ret;
    if(simGetObjectPosition(objectHandle, relativeToObjectHandle, ret.data()) == -1)
        throw api_error("simGetObjectPosition");
    return ret;
}

simVoid setObjectPosition(simInt objectHandle, simInt relativeToObjectHandle, const std::array<simFloat, 3> &position)
{
    if(simSetObjectPosition(objectHandle, relativeToObjectHandle, position.data()) == -1)
        throw api_error("simSetObjectPosition");
}

std::array<simFloat, 4> getObjectQuaternion(simInt objectHandle, simInt relativeToObjectHandle)
{
    std::array<simFloat, 4> ret;
    if(simGetObjectQuaternion(objectHandle, relativeToObjectHandle, ret.data()) == -1)
        throw api_error("simGetObjectQuaternion");
    return ret;
}

simVoid setObjectQuaternion(simInt objectHandle, simInt relativeToObjectHandle, const std::array<simFloat, 4> &quaternion)
{
    if(simSetObjectQuaternion(objectHandle, relativeToObjectHandle, quaternion.data()) == -1)
        throw api_error("simSetObjectQuaternion");
}

simInt getObjectType(simInt objectHandle)
{
    simInt ret;
    if((ret = simGetObjectType(objectHandle)) == -1)
        throw api_error("simGetObjectType");
    return ret;
}

simInt getObjectUniqueIdentifier(simInt objectHandle)
{
    simInt ret;
    if(simGetObjectUniqueIdentifier(objectHandle, &ret) == -1)
        throw api_error("simGetObjectUniqueIdentifier");
    return ret;
}

std::pair<std::array<simFloat, 3>, std::array<simFloat, 3>> getObjectVelocity(simInt objectHandle)
{
    std::array<simFloat, 3> lin, ang;
    if(simGetObjectVelocity(objectHandle, lin.data(), ang.data()) == -1)
        throw api_error("simGetObjectVelocity");
    return std::make_pair(lin, ang);
}

simInt getObjects(simInt index, simInt objectType)
{
    simInt ret;
    if((ret = simGetObjects(index, objectType)) == -1)
        throw api_error("simGetObjects");
    return ret;
}

std::vector<simInt> getObjectsInTree(simInt treeBaseHandle, simInt objectType, simInt options)
{
    simInt *ret;
    simInt count;
    if((ret = simGetObjectsInTree(treeBaseHandle, objectType, options, &count)) == NULL)
        throw api_error("simGetObjectsInTree");
    std::vector<simInt> v;
    for(size_t i = 0; i < count; i++) v.push_back(ret[i]);
    return v;
}

std::vector<simInt> getObjectSelection()
{
    std::vector<simInt> handles;
    handles.resize(getObjectSelectionSize());
    simInt returnedSize;
    if((returnedSize = simGetObjectSelection(handles.data())) == -1)
        throw api_error("simGetObjectSelection");
    if(returnedSize != handles.size())
        throw api_error("simGetObjectSelection", "returned size does not match getObjectSelectionSize()");
    return handles;
}

simInt getObjectSelectionSize()
{
    simInt ret;
    if((ret = simGetObjectSelectionSize()) == -1)
        throw api_error("simGetObjectSelectionSize");
    return ret;
}

} // namespace sim