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
        addLog(sim_verbosity_debug, "STACK DEBUG: %s", msg);
    }
}

static void addStackDebugDump(int stackHandle)
{
    if(debugStackEnabled)
        debugStack(stackHandle);
}

#else // RELEASE
#define addStackDebugLog(...)
#define addStackDebugDump(x)
#endif

int registerScriptCallbackFunction(const std::string &funcNameAtPluginName, const std::string &callTips, void (*callBack)(struct SScriptCallBack *cb))
{
    int ret = simRegisterScriptCallbackFunction(funcNameAtPluginName.c_str(), callTips.c_str(), callBack);
    if(ret == 0)
    {
        addLog(sim_verbosity_warnings, "replaced function '%s'", funcNameAtPluginName);
    }
    if(ret == -1)
        throw api_error("simRegisterScriptCallbackFunction");
    return ret;
}

int registerScriptVariable(const std::string &varName, const char *varValue, int stackID)
{
    int ret = simRegisterScriptVariable(varName.c_str(), varValue, stackID);
    if(ret == 0)
    {
        addLog(sim_verbosity_warnings, "replaced variable '%s'", varName);
    }
    if(ret == -1)
        throw api_error("simRegisterScriptVariable");
    return ret;
}

int registerScriptVariable(const std::string &varName, const std::string &varValue, int stackID)
{
    return registerScriptVariable(varName, varValue.c_str(), stackID);
}

void callScriptFunctionEx(int scriptHandleOrType, const std::string &functionNameAtScriptName, int stackId)
{
    if(simCallScriptFunctionEx(scriptHandleOrType, functionNameAtScriptName.c_str(), stackId) == -1)
        throw api_error("simCallScriptFunctionEx");
}

int createStack()
{
    addStackDebugLog("simCreateStack");

    int ret = simCreateStack();
    if(ret == -1)
        throw api_error("simCreateStack");

    addStackDebugDump(ret);

    return ret;
}

void releaseStack(int stackHandle)
{
    addStackDebugLog("simReleaseStack");

    if(simReleaseStack(stackHandle) != 1)
        throw api_error("simReleaseStack");
}

int copyStack(int stackHandle)
{
    addStackDebugLog("simCopyStack");

    int ret = simCopyStack(stackHandle);
    if(ret == -1)
        throw api_error("simCopyStack");
    return ret;
}

void pushNullOntoStack(int stackHandle)
{
    addStackDebugLog("simPushNullOntoStack");

    if(simPushNullOntoStack(stackHandle) == -1)
        throw api_error("simPushNullOntoStack");

    addStackDebugDump(stackHandle);
}

void pushBoolOntoStack(int stackHandle, bool value)
{
    addStackDebugLog("simPushBoolOntoStack %d", value);

    if(simPushBoolOntoStack(stackHandle, value) == -1)
        throw api_error("simPushBoolOntoStack");

    addStackDebugDump(stackHandle);
}

void pushInt32OntoStack(int stackHandle, int value)
{
    addStackDebugLog("simPushInt32OntoStack %d", value);

    if(simPushInt32OntoStack(stackHandle, value) == -1)
        throw api_error("simPushInt32OntoStack");

    addStackDebugDump(stackHandle);
}

void pushFloatOntoStack(int stackHandle, float value)
{
    addStackDebugLog("simPushFloatOntoStack %f", value);

    if(simPushFloatOntoStack(stackHandle, value) == -1)
        throw api_error("simPushFloatOntoStack");

    addStackDebugDump(stackHandle);
}

void pushDoubleOntoStack(int stackHandle, double value)
{
    addStackDebugLog("simPushDoubleOntoStack %g", value);

    if(simPushDoubleOntoStack(stackHandle, value) == -1)
        throw api_error("simPushDoubleOntoStack");

    addStackDebugDump(stackHandle);
}

void pushStringOntoStack(int stackHandle, const char *value, int stringSize)
{
    addStackDebugLog("simPushStringOntoStack \"%s\" [%d]", value, stringSize);

    if(simPushStringOntoStack(stackHandle, value, stringSize) == -1)
        throw api_error("simPushStringOntoStack");

    addStackDebugDump(stackHandle);
}

void pushUInt8TableOntoStack(int stackHandle, const unsigned char *values, int valueCnt)
{
    addStackDebugLog("simPushUInt8TableOntoStack <%d values>", valueCnt);

    if(simPushUInt8TableOntoStack(stackHandle, values, valueCnt) == -1)
        throw api_error("simPushUInt8TableOntoStack");

    addStackDebugDump(stackHandle);
}

void pushInt32TableOntoStack(int stackHandle, const int *values, int valueCnt)
{
    addStackDebugLog("simPushInt32TableOntoStack <%d values>", valueCnt);

    if(simPushInt32TableOntoStack(stackHandle, values, valueCnt) == -1)
        throw api_error("simPushInt32TableOntoStack");

    addStackDebugDump(stackHandle);
}

void pushFloatTableOntoStack(int stackHandle, const float *values, int valueCnt)
{
    addStackDebugLog("simPushFloatTableOntoStack <%d values>", valueCnt);

    if(simPushFloatTableOntoStack(stackHandle, values, valueCnt) == -1)
        throw api_error("simPushFloatTableOntoStack");

    addStackDebugDump(stackHandle);
}

void pushDoubleTableOntoStack(int stackHandle, const double *values, int valueCnt)
{
    addStackDebugLog("simPushDoubleTableOntoStack <%d values>", valueCnt);

    if(simPushDoubleTableOntoStack(stackHandle, values, valueCnt) == -1)
        throw api_error("simPushDoubleTableOntoStack");

    addStackDebugDump(stackHandle);
}

void pushTableOntoStack(int stackHandle)
{
    addStackDebugLog("simPushTableOntoStack");

    if(simPushTableOntoStack(stackHandle) == -1)
        throw api_error("simPushTableOntoStack");

    addStackDebugDump(stackHandle);
}

void insertDataIntoStackTable(int stackHandle)
{
    addStackDebugLog("simInsertDataIntoStackTable");

    if(simInsertDataIntoStackTable(stackHandle) == -1)
        throw api_error("simInsertDataIntoStackTable");

    addStackDebugDump(stackHandle);
}

int getStackSize(int stackHandle)
{
    int ret = simGetStackSize(stackHandle);

    addStackDebugLog("simGetStackSize -> %d", ret);

    if(ret == -1)
        throw api_error("simGetStackSize");
    return ret;
}

int popStackItem(int stackHandle, int count)
{
    int ret = simPopStackItem(stackHandle, count);

    addStackDebugLog("simPopStackItem %d -> %d", count, ret);

    if(ret == -1)
        throw api_error("simPopStackItem");

    addStackDebugDump(stackHandle);

    return ret;
}

void moveStackItemToTop(int stackHandle, int cIndex)
{
    addStackDebugLog("simMoveStackItemToTop %d", cIndex);

    if(simMoveStackItemToTop(stackHandle, cIndex) == -1)
        throw api_error("simMoveStackItemToTop");

    addStackDebugDump(stackHandle);
}

int isStackValueNull(int stackHandle)
{
    int ret = simIsStackValueNull(stackHandle);
    if(ret == -1)
        throw api_error("simIsStackValueNull");

    addStackDebugLog("simIsStackValueNull -> %d", ret);

    return ret;
}

int getStackBoolValue(int stackHandle, bool *boolValue)
{
    int ret = simGetStackBoolValue(stackHandle, boolValue);
    if(ret == -1)
        throw api_error("simGetStackBoolValue");

#ifndef NDEBUG
    if(debugStackEnabled)
    {
        if(ret)
            addStackDebugLog("simGetStackBoolValue -> %s", *boolValue ? "true" : "false");
        else
            addStackDebugLog("simGetStackBoolValue -> not a bool");
    }
#endif

    return ret;
}

int getStackInt32Value(int stackHandle, int *numberValue)
{
    int ret = simGetStackInt32Value(stackHandle, numberValue);
    if(ret == -1)
        throw api_error("simGetStackInt32Value");

#ifndef NDEBUG
    if(debugStackEnabled)
    {
        if(ret)
            addStackDebugLog("simGetStackInt32Value -> %d", *numberValue);
        else
            addStackDebugLog("simGetStackInt32Value -> not an int");
    }
#endif

    return ret;
}

int getStackFloatValue(int stackHandle, float *numberValue)
{
    int ret = simGetStackFloatValue(stackHandle, numberValue);
    if(ret == -1)
        throw api_error("simGetStackFloatValue");

#ifndef NDEBUG
    if(debugStackEnabled)
    {
        if(ret)
            addStackDebugLog("simGetStackFloatValue -> %f", *numberValue);
        else
            addStackDebugLog("simGetStackFloatValue -> not a float");
    }
#endif

    return ret;
}

int getStackDoubleValue(int stackHandle, double *numberValue)
{
    int ret = simGetStackDoubleValue(stackHandle, numberValue);
    if(ret == -1)
        throw api_error("simGetStackDoubleValue");

#ifndef NDEBUG
    if(debugStackEnabled)
    {
        if(ret)
            addStackDebugLog("simGetStackDoubleValue -> %g", *numberValue);
        else
            addStackDebugLog("simGetStackDoubleValue -> not a double");
    }
#endif

    return ret;
}

char* getStackStringValue(int stackHandle, int *stringSize)
{
    char *ret = simGetStackStringValue(stackHandle, stringSize);

#ifndef NDEBUG
    if(debugStackEnabled)
    {
        if(ret)
            addStackDebugLog("simGetStackStringValue -> \"%s\"", ret);
        else
            addStackDebugLog("simGetStackStringValue -> not a string");
    }
#endif

    // if stringSize is NULL, we cannot distinguish error (-1) from type error (0)
    if(ret == NULL && stringSize && *stringSize == -1)
        throw api_error("simGetStackStringValue");

    // ret might be NULL, indicating type error:
    return ret;
}

int getStackTableInfo(int stackHandle, int infoType)
{
    int ret = simGetStackTableInfo(stackHandle, infoType);

#ifndef NDEBUG
    if(debugStackEnabled)
    {
        std::string infoTypeStr = "???", retStr = "???";
        switch(infoType)
        {
        case 0: infoTypeStr = "check table type"; break;
        case 1: infoTypeStr = "check array of nil"; break;
        case 2: infoTypeStr = "check array of number"; break;
        case 3: infoTypeStr = "check array of boolean"; break;
        case 4: infoTypeStr = "check array of string"; break;
        case 5: infoTypeStr = "check array of table"; break;
        }
        if(infoType == 0)
        {
            switch(ret)
            {
            case -4: retStr = "sim_stack_table_circular_ref"; break;
            case -3: retStr = "sim_stack_table_not_table"; break;
            case -2: retStr = "sim_stack_table_map"; break;
            case  0: retStr = "sim_stack_table_empty"; break;
            default:
                if(ret > 0)
                    retStr = util::sprintf("array of %d elements", ret);
                break;
            }
        }
        else retStr = ret ? "yes" : "no";
        addStackDebugLog("simGetStackTableInfo %d (%s) -> %d (%s)", infoType, infoTypeStr, ret, retStr);
    }
#endif // NDEBUG

    if(ret == -1)
        throw api_error("simGetStackTableInfo");
    return ret;
}

int getStackUInt8Table(int stackHandle, unsigned char *array, int count)
{
    int ret = simGetStackUInt8Table(stackHandle, array, count);

    addStackDebugLog("simGetStackUInt8Table count = %d -> %d", count, ret);

    if(ret == -1)
        throw api_error("simGetStackUInt8Table");
    return ret;
}

int getStackInt32Table(int stackHandle, int *array, int count)
{
    int ret = simGetStackInt32Table(stackHandle, array, count);

    addStackDebugLog("simGetStackInt32Table count = %d -> %d", count, ret);

    if(ret == -1)
        throw api_error("simGetStackInt32Table");
    return ret;
}

int getStackFloatTable(int stackHandle, float *array, int count)
{
    int ret = simGetStackFloatTable(stackHandle, array, count);

    addStackDebugLog("simGetStackFloatTable count = %d -> %d", count, ret);

    if(ret == -1)
        throw api_error("simGetStackFloatTable");
    return ret;
}

int getStackDoubleTable(int stackHandle, double *array, int count)
{
    int ret = simGetStackDoubleTable(stackHandle, array, count);

    addStackDebugLog("simGetStackDoubleTable, count = %d -> %d", count, ret);

    if(ret == -1)
        throw api_error("simGetStackDoubleTable");
    return ret;
}

void unfoldStackTable(int stackHandle)
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

void debugStack(int stackHandle, int index)
{
    if(simDebugStack(stackHandle, index) == -1)
        throw api_error("simDebugStack");
}

void pushStringOntoStack(int stackHandle, const std::string &value)
{
    addStackDebugLog("simPushStringOntoStack \"%s\" [%d]", value.c_str(), value.size());

    if(simPushStringOntoStack(stackHandle, value.c_str(), value.size()) == -1)
        throw api_error("simPushStringOntoStack");

    addStackDebugDump(stackHandle);
}

void pushUInt8TableOntoStack(int stackHandle, const std::vector<unsigned char> &values)
{
    pushUInt8TableOntoStack(stackHandle, values.data(), values.size());
}

void pushInt32TableOntoStack(int stackHandle, const std::vector<int> &values)
{
    pushInt32TableOntoStack(stackHandle, values.data(), values.size());
}

void pushFloatTableOntoStack(int stackHandle, const std::vector<float> &values)
{
    pushFloatTableOntoStack(stackHandle, values.data(), values.size());
}

void pushDoubleTableOntoStack(int stackHandle, const std::vector<double> &values)
{
    pushDoubleTableOntoStack(stackHandle, values.data(), values.size());
}

int getStackStringValue(int stackHandle, std::string *stringValue)
{
    int stringSize = -1;
    char *ret = getStackStringValue(stackHandle, &stringSize);
    if(ret)
    {
        *stringValue = std::string(ret, stringSize);
        releaseBuffer(ret);
        return 1;
    }
    else
    {
        return 0;
    }
}

int getStackUInt8Table(int stackHandle, std::vector<unsigned char> *v)
{
    int count = getStackTableInfo(stackHandle, 0);
    v->resize(count);
    return getStackUInt8Table(stackHandle, v->data(), count);
}

int getStackInt32Table(int stackHandle, std::vector<int> *v)
{
    int count = getStackTableInfo(stackHandle, 0);
    v->resize(count);
    return getStackInt32Table(stackHandle, v->data(), count);
}

int getStackFloatTable(int stackHandle, std::vector<float> *v)
{
    int count = getStackTableInfo(stackHandle, 0);
    v->resize(count);
    return getStackFloatTable(stackHandle, v->data(), count);
}

int getStackDoubleTable(int stackHandle, std::vector<double> *v)
{
    int count = getStackTableInfo(stackHandle, 0);
    v->resize(count);
    return getStackDoubleTable(stackHandle, v->data(), count);
}

int getBoolParameter(int parameter)
{
    int ret;
    if((ret = simGetBoolParameter(parameter)) == -1)
        throw api_error("simGetBoolParameter");
    return ret;
}

int getInt32Parameter(int parameter)
{
    int ret;
    if(simGetInt32Parameter(parameter, &ret) == -1)
        throw api_error("simGetInt32Parameter");
    return ret;
}

unsigned long long int getUInt64Parameter(int parameter)
{
    unsigned long long int ret;
    if(simGetUInt64Parameter(parameter, &ret) == -1)
        throw api_error("simGetUInt64Parameter");
    return ret;
}

double getFloatParameter(int parameter)
{
    double ret;
    if(simGetFloatParameter(parameter, &ret) == -1)
        throw api_error("simGetFloatParameter");
    return ret;
}

std::array<double, 3> getArrayParameter(int parameter)
{
    std::array<double, 3> ret;
    if(simGetArrayParameter(parameter, ret.data()) == -1)
        throw api_error("simGetArrayParameter");
    return ret;
}

std::string getStringParameter(int parameter)
{
    char *ret;
    if((ret = simGetStringParameter(parameter)) == nullptr)
        throw api_error("simGetStringParameter");
    std::string s(ret);
    releaseBuffer(ret);
    return s;
}

void setBoolParameter(int parameter, bool value)
{
    if(simSetBoolParameter(parameter, value) == -1)
        throw api_error("simSetBoolParameter");
}

void setInt32Parameter(int parameter, int value)
{
    if(simSetInt32Parameter(parameter, value) == -1)
        throw api_error("simSetInt32Parameter");
}

void setFloatParameter(int parameter, double value)
{
    if(simSetFloatParameter(parameter, value) == -1)
        throw api_error("simSetFloatParameter");
}

void setArrayParameter(int parameter, std::array<double, 3> value)
{
    if(simSetArrayParameter(parameter, value.data()) == -1)
        throw api_error("simSetArrayParameter");
}

void setStringParameter(int parameter, char *value)
{
    if(simSetStringParameter(parameter, value) == -1)
        throw api_error("simSetStringParameter");
}

void setStringParameter(int parameter, const std::string &value)
{
    if(simSetStringParameter(parameter, value.c_str()) == -1)
        throw api_error("simSetStringParameter");
}

double getObjectFloatParameter(int objectHandle, int parameterID, double *parameter)
{
    double ret;
    if(simGetObjectFloatParameter(objectHandle, parameterID, &ret) == -1)
        throw api_error("simGetObjectFloatParameter");
    return ret;
}

int getObjectInt32Parameter(int objectHandle, int parameterID, int *parameter)
{
    int ret;
    if(simGetObjectInt32Parameter(objectHandle, parameterID, &ret) == -1)
        throw api_error("simGetObjectInt32Parameter");
    return ret;
}

std::string getObjectStringParameter(int objectHandle, int parameterID)
{
    char *ret;
    int len;
    if((ret = simGetObjectStringParameter(objectHandle, parameterID, &len)) == NULL)
        throw api_error("simGetObjectStringParameter");
    std::string s(ret, len);
    releaseBuffer(ret);
    return s;
}

void setObjectFloatParameter(int objectHandle, int parameterID, double parameter)
{
    if(simSetObjectFloatParameter(objectHandle, parameterID, parameter) == -1)
        throw api_error("simSetObjectFloatParameter");
}

void setObjectInt32Parameter(int objectHandle, int parameterID, int parameter)
{
    if(simSetObjectInt32Parameter(objectHandle, parameterID, parameter) == -1)
        throw api_error("simSetObjectInt32Parameter");
}

void setObjectStringParameter(int objectHandle, int parameterID, const std::string &parameter)
{
    // XXX: fix const ptrs in the regular API
    if(simSetObjectStringParameter(objectHandle, parameterID, const_cast<char*>(parameter.data()), parameter.size()) == -1)
        throw api_error("simSetObjectStringParameter");
}

void getScriptProperty(int scriptHandle, int *scriptProperty, int *associatedObjectHandle)
{
    if(simGetScriptProperty(scriptHandle, scriptProperty, associatedObjectHandle) == -1)
        throw api_error("simGetScriptProperty");
}

boost::optional<std::string> getStringNamedParam(const std::string &parameter)
{
    char *ret;
    int len;
    if((ret = simGetStringNamedParam(parameter.c_str(), &len)) == NULL)
        return {};
    std::string s(ret, len);
    releaseBuffer(ret);
    return s;
}

void setStringNamedParam(const std::string &parameter, const std::string &value)
{
    if(simSetStringNamedParam(parameter.c_str(), value.c_str(), value.size()) == -1)
        throw api_error("simSetStringNamedParam");
}

char* createBuffer(int size)
{
    char *ret = simCreateBuffer(size);
    if(ret == NULL)
        throw api_error("simCreateBuffer");
    return ret;
}

void releaseBuffer(char *buffer)
{
    if(simReleaseBuffer(buffer) == -1)
        throw api_error("simReleaseBuffer");
}

std::string getLastError()
{
    char *ret = simGetLastError();
    if(ret == NULL)
        throw api_error("simGetLastError");
    std::string s(ret);
    releaseBuffer(ret);
    return s;
}

void setLastError(const std::string &func, const std::string &msg)
{
    if(simSetLastError(func.c_str(), msg.c_str()) == -1)
        throw api_error("simSetLastError");
}

int getObjectChild(int objectHandle, int index)
{
    int ret;
    if((ret = simGetObjectChild(objectHandle, index)) == -1)
        throw api_error("simGetObjectChild");
    return ret;
}

int getObjectHandle(const std::string &objectName)
{
    int ret;
    if((ret = simGetObjectHandle(objectName.c_str())) == -1)
        throw api_error("simGetObjectHandle");
    return ret;
}

std::array<double, 12> getObjectMatrix(int objectHandle, int relativeToObjectHandle)
{
    std::array<double, 12> ret;
    if(simGetObjectMatrix(objectHandle, relativeToObjectHandle, ret.data()) == -1)
        throw api_error("simGetObjectMatrix");
    return ret;
}

void setObjectMatrix(int objectHandle, int relativeToObjectHandle, const std::array<double, 12> &matrix)
{
    if(simSetObjectMatrix(objectHandle, relativeToObjectHandle, matrix.data()) == -1)
        throw api_error("simSetObjectMatrix");
}

std::string getObjectName(int objectHandle)
{
    char *ret = simGetObjectName(objectHandle);
    if(ret == NULL)
        throw api_error("simGetObjectName");
    std::string s(ret);
    releaseBuffer(ret);
    return s;
}

void setObjectName(int objectHandle, const std::string &objectName)
{
    if(simSetObjectName(objectHandle, objectName.c_str()) == -1)
        throw api_error("simSetObjectName");
}

std::array<double, 3> getObjectOrientation(int objectHandle, int relativeToObjectHandle)
{
    std::array<double, 3> ret;
    if(simGetObjectOrientation(objectHandle, relativeToObjectHandle, ret.data()) == -1)
        throw api_error("simGetObjectOrientation");
    return ret;
}

void setObjectOrientation(int objectHandle, int relativeToObjectHandle, const std::array<double, 3> &eulerAngles)
{
    if(simSetObjectOrientation(objectHandle, relativeToObjectHandle, eulerAngles.data()) == -1)
        throw api_error("simSetObjectOrientation");
}

int getObjectParent(int objectHandle)
{
    int ret;
    if((ret = simGetObjectParent(objectHandle)) == -1)
        throw api_error("simGetObjectParent");
    return ret;
}

void setObjectParent(int objectHandle, int parentObjectHandle, bool keepInPlace)
{
    if(simSetObjectParent(objectHandle, parentObjectHandle, keepInPlace) == -1)
        throw api_error("simSetObjectParent");
}

std::array<double, 3> getObjectPosition(int objectHandle, int relativeToObjectHandle)
{
    std::array<double, 3> ret;
    if(simGetObjectPosition(objectHandle, relativeToObjectHandle, ret.data()) == -1)
        throw api_error("simGetObjectPosition");
    return ret;
}

void setObjectPosition(int objectHandle, int relativeToObjectHandle, const std::array<double, 3> &position)
{
    if(simSetObjectPosition(objectHandle, relativeToObjectHandle, position.data()) == -1)
        throw api_error("simSetObjectPosition");
}

std::array<double, 4> getObjectQuaternion(int objectHandle, int relativeToObjectHandle)
{
    std::array<double, 4> ret;
    if(simGetObjectQuaternion(objectHandle, relativeToObjectHandle, ret.data()) == -1)
        throw api_error("simGetObjectQuaternion");
    return ret;
}

void setObjectQuaternion(int objectHandle, int relativeToObjectHandle, const std::array<double, 4> &quaternion)
{
    if(simSetObjectQuaternion(objectHandle, relativeToObjectHandle, quaternion.data()) == -1)
        throw api_error("simSetObjectQuaternion");
}

int getObjectType(int objectHandle)
{
    int ret;
    if((ret = simGetObjectType(objectHandle)) == -1)
        throw api_error("simGetObjectType");
    return ret;
}

int getObjectUniqueIdentifier(int objectHandle)
{
    int ret;
    if(simGetObjectUniqueIdentifier(objectHandle, &ret) == -1)
        throw api_error("simGetObjectUniqueIdentifier");
    return ret;
}

std::pair<std::array<double, 3>, std::array<double, 3>> getObjectVelocity(int objectHandle)
{
    std::array<double, 3> lin, ang;
    if(simGetObjectVelocity(objectHandle, lin.data(), ang.data()) == -1)
        throw api_error("simGetObjectVelocity");
    return std::make_pair(lin, ang);
}

int getObjects(int index, int objectType)
{
    int ret;
    if((ret = simGetObjects(index, objectType)) == -1)
        throw api_error("simGetObjects");
    return ret;
}

std::vector<int> getObjectsInTree(int treeBaseHandle, int objectType, int options)
{
    int *ret;
    int count;
    if((ret = simGetObjectsInTree(treeBaseHandle, objectType, options, &count)) == NULL)
        throw api_error("simGetObjectsInTree");
    std::vector<int> v;
    for(size_t i = 0; i < count; i++) v.push_back(ret[i]);
    return v;
}

std::vector<int> getObjectSelection()
{
    std::vector<int> handles;
    handles.resize(getObjectSelectionSize());
    int returnedSize;
    if((returnedSize = simGetObjectSelection(handles.data())) == -1)
        throw api_error("simGetObjectSelection");
    if(returnedSize != handles.size())
        throw api_error("simGetObjectSelection", "returned size does not match getObjectSelectionSize()");
    return handles;
}

int getObjectSelectionSize()
{
    int ret;
    if((ret = simGetObjectSelectionSize()) == -1)
        throw api_error("simGetObjectSelectionSize");
    return ret;
}

void setModuleInfo(const std::string &moduleName, int infoType, const std::string &stringInfo)
{
    if(simSetModuleInfo(moduleName.c_str(), infoType, stringInfo.c_str(), 0) == -1)
        throw api_error("simSetModuleInfo");
}

void setModuleInfo(const std::string &moduleName, int infoType, int intInfo)
{
    if(simSetModuleInfo(moduleName.c_str(), infoType, nullptr, intInfo) == -1)
        throw api_error("simSetModuleInfo");
}

void getModuleInfo(const std::string &moduleName, int infoType, std::string &stringInfo)
{
    int intInfo = 0;
    char *s = nullptr;
    if(simGetModuleInfo(moduleName.c_str(), infoType, &s, &intInfo) == -1)
        throw api_error("simGetModuleInfo");
    if(s)
    {
        stringInfo = std::string(s);
        releaseBuffer(s);
    }
}

void getModuleInfo(const std::string &moduleName, int infoType, int &intInfo)
{
    char *s = nullptr;
    if(simGetModuleInfo(moduleName.c_str(), infoType, &s, &intInfo) == -1)
        throw api_error("simGetModuleInfo");
    if(s)
        releaseBuffer(s);
}

std::string getModuleInfoStr(const std::string &moduleName, int infoType)
{
    std::string s;
    getModuleInfo(moduleName, infoType, s);
    return s;
}

int getModuleInfoInt(const std::string &moduleName, int infoType)
{
    int i;
    getModuleInfo(moduleName, infoType, i);
    return i;
}

void setModuleInfo(int infoType, const std::string &stringInfo)
{
    setModuleInfo(pluginName, infoType, stringInfo);
}

void setModuleInfo(int infoType, int intInfo)
{
    setModuleInfo(pluginName, infoType, intInfo);
}

void getModuleInfo(int infoType, std::string &stringInfo)
{
    getModuleInfo(pluginName, infoType, stringInfo);
}

void getModuleInfo(int infoType, int &intInfo)
{
    getModuleInfo(pluginName, infoType, intInfo);
}

std::string getModuleInfoStr(int infoType)
{
    std::string s;
    getModuleInfo(infoType, s);
    return s;
}

int getModuleInfoInt(int infoType)
{
    int i;
    getModuleInfo(infoType, i);
    return i;
}

int programVersion()
{
    int simVer = sim::getInt32Parameter(sim_intparam_program_version);
    int simRev = sim::getInt32Parameter(sim_intparam_program_revision);
    simVer = simVer * 100 + simRev;
    return simVer;
}

std::string versionString(int v)
{
    int revision = v % 100;
    v /= 100;
    int patch = v % 100;
    v /= 100;
    int minor = v % 100;
    v /= 100;
    int major = v % 100;
    return util::sprintf("%d.%d.%drev%d", major, minor, patch, revision);
}

} // namespace sim
