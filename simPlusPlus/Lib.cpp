#include <simPlusPlus/Lib.h>
#include <simPlusPlus/Plugin.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

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

int programVersion()
{
    int simVer = sim::getInt32Param(sim_intparam_program_version);
    int simRev = sim::getInt32Param(sim_intparam_program_revision);
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

// int simRunSimulator(const char *applicationName, int options, void(*initCallBack)(), void(*loopCallBack)(), void(*deinitCallBack)());

// int simRunSimulatorEx(const char *applicationName, int options, void(*initCallBack)(), void(*loopCallBack)(), void(*deinitCallBack)(), int stopDelay, const char *sceneOrModelToLoad);

// char *simGetSimulatorMessage(int *messageID, int *auxiliaryData, int *returnedDataSize);

void * getMainWindow(int type)
{
    return simGetMainWindow(type);
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

// int simLoadModule(const char *filenameAndPath, const char *pluginName);

// int simUnloadModule(int pluginhandle);

void setBoolParam(int parameter, bool value)
{
    if(simSetBoolParam(parameter, value) == -1)
        throw api_error("simSetBoolParameter");
}

bool getBoolParam(int parameter)
{
    int ret;
    if((ret = simGetBoolParam(parameter)) == -1)
        throw api_error("simGetBoolParam");
    return ret > 0;
}

void setInt32Param(int parameter, int value)
{
    if(simSetInt32Param(parameter, value) == -1)
        throw api_error("simSetInt32Param");
}

int getInt32Param(int parameter)
{
    int ret;
    if(simGetInt32Param(parameter, &ret) == -1)
        throw api_error("simGetInt32Param");
    return ret;
}

unsigned long long int getUInt64Param(int parameter)
{
    unsigned long long int ret;
    if(simGetUInt64Param(parameter, &ret) == -1)
        throw api_error("simGetUInt64Param");
    return ret;
}

void setStringParam(int parameter, char *value)
{
    if(simSetStringParam(parameter, value) == -1)
        throw api_error("simSetStringParam");
}

void setStringParam(int parameter, const std::string &value)
{
    if(simSetStringParam(parameter, value.c_str()) == -1)
        throw api_error("simSetStringParam");
}

std::string getStringParam(int parameter)
{
    char *ret;
    if((ret = simGetStringParam(parameter)) == nullptr)
        throw api_error("simGetStringParam");
    std::string s(ret);
    releaseBuffer(ret);
    return s;
}

void setNamedStringParam(const std::string &parameter, const std::string &value)
{
    if(simSetNamedStringParam(parameter.c_str(), value.c_str(), value.size()) == -1)
        throw api_error("simSetNamedStringParam");
}

void setNamedBoolParam(const std::string &parameter, bool value)
{
    setNamedStringParam(parameter, value ? "true" : "false");
}

void setNamedFloatParam(const std::string &parameter, double value)
{
    setNamedStringParam(parameter, std::to_string(value));
}

void setNamedInt32Param(const std::string &parameter, int value)
{
    setNamedStringParam(parameter, std::to_string(value));
}

boost::optional<std::string> getNamedStringParam(const std::string &parameter)
{
    char *ret;
    int len;
    if((ret = simGetNamedStringParam(parameter.c_str(), &len)) == NULL)
        return {};
    std::string s(ret, len);
    releaseBuffer(ret);
    return s;
}

boost::optional<bool> getNamedBoolParam(const std::string &parameter)
{
    auto v = getNamedStringParam(parameter);
    if(!v) return {};
    auto s = *v;
    if(s == "true") return true;
    if(s == "false") return false;
    if(s == "on") return true;
    if(s == "off") return false;
    if(s == "1") return true;
    if(s == "0") return false;
    throw api_error("simGetNamedBoolParam");
}

boost::optional<double> getNamedFloatParam(const std::string &parameter)
{
    auto v = getNamedStringParam(parameter);
    if(!v) return {};
    return std::stod(*v);
}

boost::optional<int> getNamedInt32Param(const std::string &parameter)
{
    auto v = getNamedStringParam(parameter);
    if(!v) return {};
    return std::stoi(*v);
}

// int simGetObject(const char *objectPath, int index, int proxy, int options);

long long int getObjectUid(int objectHandle)
{
    long long int ret = simGetObjectUid(objectHandle);
    if(ret == -1)
        throw api_error("simGetObjectUid");
    return ret;
}

// int simGetObjectFromUid(long long int uid, int options);

int getScriptHandleEx(int scriptType, int objHandle, boost::optional<std::string> scriptName)
{
    return simGetScriptHandleEx(scriptType, objHandle, scriptName ? scriptName->c_str() : nullptr);
}

void removeObjects(const std::vector<int> &objectHandles)
{
    if(simRemoveObjects(objectHandles.data(), objectHandles.size()) == -1)
        throw api_error("simRemoveObjects");
}

int removeModel(int objectHandle)
{
    int n = simRemoveModel(objectHandle);
    if(n == -1)
        throw api_error("simRemoveModel");
    return n;
}

std::string getObjectAlias(int objectHandle, int options)
{
    char *ret;
    if((ret = simGetObjectAlias(objectHandle, options)) == NULL)
        throw api_error("simGetObjectAlias");
    std::string s(ret);
    releaseBuffer(ret);
    return s;
}

void setObjectAlias(int objectHandle, const std::string &alias, int options)
{
    if(simSetObjectAlias(objectHandle, alias.c_str(), options) == -1)
        throw api_error("simSetObjectAlias");
}

int getObjectParent(int objectHandle)
{
    int ret;
    if((ret = simGetObjectParent(objectHandle)) == -1)
        throw api_error("simGetObjectParent");
    return ret;
}

int getObjectChild(int objectHandle, int index)
{
    int ret;
    if((ret = simGetObjectChild(objectHandle, index)) == -1)
        throw api_error("simGetObjectChild");
    return ret;
}

std::vector<int> getObjectChildren(int objectHandle)
{
    std::vector<int> ret;
    for(int index = 0, childHandle; ; index++)
    {
        if((childHandle = simGetObjectChild(objectHandle, index)) == -1) break;
        ret.push_back(childHandle);
    }
    return ret;
}

void setObjectParent(int objectHandle, int parentObjectHandle, bool keepInPlace)
{
    if(simSetObjectParent(objectHandle, parentObjectHandle, keepInPlace) == -1)
        throw api_error("simSetObjectParent");
}

int getObjectType(int objectHandle)
{
    int ret;
    if((ret = simGetObjectType(objectHandle)) == -1)
        throw api_error("simGetObjectType");
    return ret;
}

int getJointType(int objectHandle)
{
    int type = simGetJointType(objectHandle);
    if(type == -1)
        throw api_error("simGetJointType");
    return type;
}

// int simReservedCommand(int v, int w);

int getSimulationState()
{
    int ret = simGetSimulationState();
    if(ret == -1)
        throw api_error("simGetSimulationState");
    return ret;
}

void loadScene(const char *filename)
{
    if(simLoadScene(filename) == -1)
        throw api_error("simLoadScene");
}

void loadScene(const std::string &filename)
{
    return loadScene(filename.c_str());
}

void closeScene()
{
    if(simCloseScene() == -1)
        throw api_error("simCloseScene");
}

void saveScene(const char *filename)
{
    if(simSaveScene(filename) == -1)
        throw api_error("simSaveScene");
}

void saveScene(const std::string &filename)
{
    return saveScene(filename.c_str());
}

void loadModel(const char *filename)
{
    if(simLoadModel(filename) == -1)
        throw api_error("simLoadModel");
}

void loadModel(const std::string &filename)
{
    return loadModel(filename.c_str());
}

void saveModel(int baseOfModelHandle, const char *filename)
{
    if(simSaveModel(baseOfModelHandle, filename) == -1)
        throw api_error("simSaveModel");
}

void saveModel(int baseOfModelHandle, const std::string &filename)
{
    return saveModel(baseOfModelHandle, filename.c_str());
}

bool doesFileExist(const char *filename)
{
    int ret = simDoesFileExist(filename);
    if(ret == -1)
        throw api_error("simDoesFileExist");
    return ret > 0;
}

bool doesFileExist(const std::string &filename)
{
    return doesFileExist(filename.c_str());
}

std::vector<int> getObjectSel()
{
    int *buf;
    int count;
    if((buf = simGetObjectSel(&count)) == NULL)
        throw api_error("simGetObjectSel");
    std::vector<int> handles;
    handles.resize(count);
    for(size_t i = 0; i < count; i++)
        handles[i] = buf[i];
    releaseBuffer(buf);
    return handles;
}

// int simSetObjectSel(const int *handles, int cnt);

// int simAssociateScriptWithObject(int scriptHandle, int associatedObjectHandle);

// int simHandleMainScript();

// int simResetScript(int scriptHandle);

// int simAddScript(int scriptProperty);

// int simRemoveScript(int scriptHandle);

// int simRefreshDialogs(int refreshDegree);

// int simResetProximitySensor(int sensorHandle);

void * createBuffer(int size)
{
    void *ret = simCreateBuffer(size);
    if(ret == NULL)
        throw api_error("simCreateBuffer");
    return ret;
}

void releaseBuffer(const void *buffer)
{
    if(simReleaseBuffer(buffer) == -1)
        throw api_error("simReleaseBuffer");
}

bool checkCollision(int entity1Handle, int entity2Handle)
{
    int ret = simCheckCollision(entity1Handle, entity2Handle);
    if(ret == -1)
        throw api_error("simCheckCollision");
    return ret > 0;
}

// int simGetRealTimeSimulation();

// int simIsRealTimeSimulationStepNeeded();

// int simGetSimulationPassesPerRenderingPass();

// int simAdvanceSimulationByOneStep();

// int simStartSimulation();

// int simStopSimulation();

// int simPauseSimulation();

// char *simGetModuleName(int index, unsigned char *moduleVersion);

// int simAdjustView(int viewHandleOrIndex, int associatedViewableObjectHandle, int options, const char *viewLabel);

void setLastError(const std::string &func, const std::string &msg)
{
    if(simSetLastError(func.c_str(), msg.c_str()) == -1)
        throw api_error("simSetLastError");
}

// int simResetGraph(int graphHandle);

// int simDestroyGraphCurve(int graphHandle, int curveId);

// int simDuplicateGraphCurveToStatic(int graphHandle, int curveId, const char *curveName);

// int simSetNavigationMode(int navigationMode);

// int simGetNavigationMode();

// int simSetPage(int index);

// int simGetPage();

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

// int simRegisterScriptFuncHook(int scriptHandle, const char *funcToHook, const char *userFunction, bool executeBefore, int options);

void copyPasteObjects(std::vector<int> &shapeHandles, int options)
{
    if(simCopyPasteObjects(shapeHandles.data(), shapeHandles.size(), options) == -1)
        throw api_error("simCopyPasteObjects");
}

std::vector<int> copyPasteObjects(const std::vector<int> &shapeHandles, int options)
{
    std::vector<int> ret(shapeHandles);
    copyPasteObjects(ret, options);
    return ret;
}

// int simRemoveDrawingObject(int objectHandle);

int announceSceneContentChange()
{
    int ret = simAnnounceSceneContentChange();
    if(ret == -1)
        throw api_error("simAnnounceSceneContentChange");
    return ret;
}

// int simSetInt32Signal(const char *signalName, int signalValue);

// int simGetInt32Signal(const char *signalName, int *signalValue);

// int simClearInt32Signal(const char *signalName);

// int simClearFloatSignal(const char *signalName);

// int simSetStringSignal(const char *signalName, const char *signalValue, int stringLength);

// char *simGetStringSignal(const char *signalName, int *stringLength);

// int simClearStringSignal(const char *signalName);

// char *simGetSignalName(int signalIndex, int signalType);

void setObjectProperty(int objectHandle, int prop)
{
    if(simSetObjectProperty(objectHandle, prop) == -1)
        throw api_error("simSetObjectProperty");
}

int getObjectProperty(int objectHandle)
{
    int ret = simGetObjectProperty(objectHandle);
    if(ret == -1)
        throw api_error("simGetObjectProperty");
    return ret;
}

// int simSetObjectSpecialProperty(int objectHandle, int prop);

// int simGetObjectSpecialProperty(int objectHandle);

// int simSetExplicitHandling(int objectHandle, int explicitFlags);

// int simGetExplicitHandling(int objectHandle);

int getLinkDummy(int dummyHandle)
{
    return simGetLinkDummy(dummyHandle);
}

void setLinkDummy(int dummyHandle, int linkedDummyHandle)
{
    if(simSetLinkDummy(dummyHandle, linkedDummyHandle) == -1)
        throw api_error("simSetLinkDummy");
}

void setModelProperty(int objectHandle, int modelProperty)
{
    if(simSetModelProperty(objectHandle, modelProperty) == -1)
        throw api_error("simSetModelProperty");
}

int getModelProperty(int objectHandle)
{
    int ret = simGetModelProperty(objectHandle);
    if(ret == -1)
        throw api_error("simGetModelProperty");
    return ret;
}

void resetDynamicObject(int objectHandle)
{
    if(simResetDynamicObject(objectHandle) == -1)
        throw api_error("simResetDynamicObject");
}

void setJointMode(int jointHandle, int jointMode)
{
    if(simSetJointMode(jointHandle, jointMode, 0) == -1)
        throw api_error("simSetJointMode");
}

int getJointMode(int jointHandle)
{
    int ret = simGetJointMode(jointHandle, nullptr);
    if(ret == -1)
        throw api_error("simGetJointMode");
    return ret;
}

// int simSerialOpen(const char *portString, int baudRate, void *reserved1, void *reserved2);

// int simSerialClose(int portHandle);

// int simSerialSend(int portHandle, const char *data, int dataLength);

// int simSerialRead(int portHandle, char *buffer, int dataLengthToRead);

// int simSerialCheck(int portHandle);

// int simAuxiliaryConsoleClose(int consoleHandle);

// int simAuxiliaryConsoleShow(int consoleHandle, bool showState);

// int simAuxiliaryConsolePrint(int consoleHandle, const char *text);

int getObjectInt32Param(int objectHandle, int parameterID)
{
    int ret;
    if(simGetObjectInt32Param(objectHandle, parameterID, &ret) == -1)
        throw api_error("simGetObjectInt32Param");
    return ret;
}

void setObjectInt32Param(int objectHandle, int parameterID, int parameter)
{
    if(simSetObjectInt32Param(objectHandle, parameterID, parameter) == -1)
        throw api_error("simSetObjectInt32Param");
}

std::string getObjectStringParam(int objectHandle, int parameterID)
{
    char *ret;
    int len;
    if((ret = simGetObjectStringParam(objectHandle, parameterID, &len)) == NULL)
        throw api_error("simGetObjectStringParam");
    std::string s(ret, len);
    releaseBuffer(ret);
    return s;
}

void setObjectStringParam(int objectHandle, int parameterID, const std::string &parameter)
{
    // XXX: fix const ptrs in the regular API
    if(simSetObjectStringParam(objectHandle, parameterID, const_cast<char*>(parameter.data()), parameter.size()) == -1)
        throw api_error("simSetObjectStringParam");
}

// int simSetSimulationPassesPerRenderingPass(int p);

void persistentDataWrite(const std::string &dataName, const std::string &dataValue, int options)
{
    if(simPersistentDataWrite(dataName.c_str(), dataValue.c_str(), dataValue.length(), options) == -1)
        throw api_error("simPersistentDataWrite");
}

std::string persistentDataRead(const std::string &dataName)
{
    int length = 0;
    char *buf = simPersistentDataRead(dataName.c_str(), &length);
    if(!buf)
        throw api_error("simPersistentDataRead");
    std::string ret(buf, length);
    releaseBuffer(buf);
    return ret;
}

bool isHandle(int generalObjectHandle, int generalObjectType)
{
    int ret = simIsHandle(generalObjectHandle, generalObjectType);
    if(ret == -1)
        throw api_error("simIsHandle");
    return ret > 0;
}

// int simResetVisionSensor(int visionSensorHandle);

void setVisionSensorImg(int sensorHandle, const unsigned char *img, int options, std::array<int, 2> pos, std::array<int, 2> size)
{
    if(simSetVisionSensorImg(sensorHandle, img, options, pos.data(), size.data()) == -1)
        throw api_error("simSetVisionSensorImg");
}

// int simRuckigRemove(int objHandle);

int groupShapes(const std::vector<int> &shapeHandles, bool merge)
{
    int groupedHandle = simGroupShapes(shapeHandles.data(), shapeHandles.size() * (merge ? -1 : 1));
    if(groupedHandle == -1)
        throw api_error("simGroupShapes");
    return groupedHandle;
}

std::vector<int> ungroupShape(int shapeHandle)
{
    int shapeCount = 0;
    int *shapeHandles = simUngroupShape(shapeHandle, &shapeCount);
    if(shapeHandles == nullptr)
        throw api_error("simUngroupShape");
    std::vector<int> ret(shapeCount, -1);
    for(size_t i = 0; i < shapeCount; i++)
        ret[-1] = shapeHandles[i];
    releaseBuffer(shapeHandles);
    return ret;
}

void quitSimulator()
{
    simQuitSimulator(true);
}

void setShapeMaterial(int shapeHandle, int materialIdOrShapeHandle)
{
    if(simSetShapeMaterial(shapeHandle, materialIdOrShapeHandle) == -1)
        throw api_error("simSetShapeMaterial");
}

// int simGetTextureId(const char *textureName, int *resolution);

// unsigned char *simReadTexture(int textureId, int options, int posX, int posY, int sizeX, int sizeY);

void writeCustomDataBlock(int objectHandle, const char *tagName, const char *data, int dataSize)
{
    if(simWriteCustomDataBlock(objectHandle, tagName, data, dataSize) == -1)
        throw api_error("simWriteCustomDataBlock");
}

void writeCustomDataBlock(int objectHandle, const std::string &tagName, const std::string &data)
{
    return writeCustomDataBlock(objectHandle, tagName.c_str(), data.c_str(), data.length());
}

char * readCustomDataBlock(int objectHandle, const char *tagName, int *dataSize)
{
    return simReadCustomDataBlock(objectHandle, tagName, dataSize);
}

boost::optional<std::string> readCustomDataBlock(int objectHandle, const std::string &tagName)
{
    int size = 0;
    char *buf = readCustomDataBlock(objectHandle, tagName.c_str(), &size);
    if(!buf) return {};
    std::string s(buf, size);
    releaseBuffer(buf);
    return s;
}

char * readCustomDataBlockTags(int objectHandle, int *tagCount)
{
    return simReadCustomDataBlockTags(objectHandle, tagCount);
}

std::vector<std::string> readCustomDataBlockTags(int objectHandle)
{
    std::vector<std::string> ret;
    int count = 0;
    char *buf = readCustomDataBlockTags(objectHandle, &count);
    if(buf)
    {
        int len = 0;
        char *tmp = buf;
        for(int i = 0; i < count; i++)
        {
            while(*tmp) {tmp++; len++;}
        }
        std::string s(buf, len);
        boost::split(ret, s, boost::is_any_of("\0"));
        releaseBuffer(buf);
    }
    return ret;
}

int getObjects(int index, int objectType)
{
    int ret;
    if((ret = simGetObjects(index, objectType)) == -1)
        throw api_error("simGetObjects");
    return ret;
}

std::vector<int> getObjects(int objectType)
{
    std::vector<int> ret;
    int i = 0, handle = -1;
    while(1)
    {
        handle = simGetObjects(i++, sim_handle_all);
        if(handle == -1) break;
        ret.push_back(handle);
    }
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

// int simGetShapeTextureId(int shapeHandle);

// int simCreateCollectionEx(int options);

// int simAddItemToCollection(int collectionHandle, int what, int objectHandle, int options);

// int simDestroyCollection(int collectionHandle);

// int *simGetCollectionObjects(int collectionHandle, int *objectCount);

int getScriptInt32Param(int scriptHandle, int parameterID)
{
    int param;
    if(simGetScriptInt32Param(scriptHandle, parameterID, &param) == -1)
        throw api_error("simGetScriptInt32Param");
    return param;
}

// int simSetScriptInt32Param(int scriptHandle, int parameterID, int parameter);

// char *simGetScriptStringParam(int scriptHandle, int parameterID, int *parameterLength);

// int simSetScriptStringParam(int scriptHandle, int parameterID, const char *parameter, int parameterLength);

// int simReorientShapeBoundingBox(int shapeHandle, int relativeToHandle, int reservedSetToZero);

// int simSaveImage(const unsigned char *image, const int *resolution, int options, const char *filename, int quality, void *reserved);

// unsigned char *simLoadImage(int *resolution, int options, const char *filename, void *reserved);

// unsigned char *simGetScaledImage(const unsigned char *imageIn, const int *resolutionIn, int *resolutionOut, int options, void *reserved);

void callScriptFunctionEx(int scriptHandleOrType, const std::string &functionNameAtScriptName, int stackId)
{
    if(simCallScriptFunctionEx(scriptHandleOrType, functionNameAtScriptName.c_str(), stackId) == -1)
        throw api_error("simCallScriptFunctionEx");
}

// char *simGetExtensionString(int objectHandle, int index, const char *key);

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

// int simPushInt64OntoStack(int stackHandle, long long int value);

void pushStringOntoStack(int stackHandle, const char *value, int stringSize)
{
    addStackDebugLog("simPushStringOntoStack \"%s\" [%d]", value, stringSize);

    if(simPushStringOntoStack(stackHandle, value, stringSize) == -1)
        throw api_error("simPushStringOntoStack");

    addStackDebugDump(stackHandle);
}

void pushStringOntoStack(int stackHandle, const std::string &value)
{
    addStackDebugLog("simPushStringOntoStack \"%s\" [%d]", value.c_str(), value.size());

    if(simPushStringOntoStack(stackHandle, value.c_str(), value.size()) == -1)
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

void pushUInt8TableOntoStack(int stackHandle, const std::vector<unsigned char> &values)
{
    pushUInt8TableOntoStack(stackHandle, values.data(), values.size());
}

void pushInt32TableOntoStack(int stackHandle, const int *values, int valueCnt)
{
    addStackDebugLog("simPushInt32TableOntoStack <%d values>", valueCnt);

    if(simPushInt32TableOntoStack(stackHandle, values, valueCnt) == -1)
        throw api_error("simPushInt32TableOntoStack");

    addStackDebugDump(stackHandle);
}

void pushInt32TableOntoStack(int stackHandle, const std::vector<int> &values)
{
    pushInt32TableOntoStack(stackHandle, values.data(), values.size());
}

// int simPushInt64TableOntoStack(int stackHandle, const long long int *values, int valueCnt);

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

// int simGetStackInt64Value(int stackHandle, long long int *numberValue);

char * getStackStringValue(int stackHandle, int *stringSize)
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

int getStackUInt8Table(int stackHandle, std::vector<unsigned char> *v)
{
    int count = getStackTableInfo(stackHandle, 0);
    v->resize(count);
    return getStackUInt8Table(stackHandle, v->data(), count);
}


int getStackInt32Table(int stackHandle, int *array, int count)
{
    int ret = simGetStackInt32Table(stackHandle, array, count);

    addStackDebugLog("simGetStackInt32Table count = %d -> %d", count, ret);

    if(ret == -1)
        throw api_error("simGetStackInt32Table");
    return ret;
}

int getStackInt32Table(int stackHandle, std::vector<int> *v)
{
    int count = getStackTableInfo(stackHandle, 0);
    v->resize(count);
    return getStackInt32Table(stackHandle, v->data(), count);
}

// int simGetStackInt64Table(int stackHandle, long long int *array, int count);

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

int getEngineInt32Param(int paramId, int objectHandle, const void *object)
{
    bool ok = false;
    int ret = simGetEngineInt32Param(paramId, objectHandle, object, &ok);
    if(!ok)
        throw api_error("simGetEngineInt32Param");
    return ret;
}

bool getEngineBoolParam(int paramId, int objectHandle, const void *object)
{
    bool ok = false;
    bool ret = simGetEngineBoolParam(paramId, objectHandle, object, &ok);
    if(!ok)
        throw api_error("simGetEngineBoolParam");
    return ret;
}

void setEngineInt32Param(int paramId, int objectHandle, const void *object, int val)
{
    if(simSetEngineInt32Param(paramId, objectHandle, object, val) != 1)
        throw api_error("simSetEngineInt32Param");
}

void setEngineBoolParam(int paramId, int objectHandle, const void *object, bool val)
{
    if(simSetEngineBoolParam(paramId, objectHandle, object, val) != 1)
        throw api_error("simSetEngineBoolParam");
}

// int simInsertObjectIntoOctree(int octreeHandle, int objectHandle, int options, const unsigned char *color, unsigned int tag, void *reserved);

// int simSubtractObjectFromOctree(int octreeHandle, int objectHandle, int options, void *reserved);

// char *simOpenTextEditor(const char *initText, const char *xml, int *various);

// char *simPackTable(int stackHandle, int *bufferSize);

// int simUnpackTable(int stackHandle, const char *buffer, int bufferSize);

// int simSetReferencedHandles(int objectHandle, int count, const int *referencedHandles, const int *reserved1, const int *reserved2);

// int simGetReferencedHandles(int objectHandle, int **referencedHandles, int **reserved1, int **reserved2);

void executeScriptString(int scriptHandleOrType, const std::string &stringAtScriptName, int stackID)
{
    if(simExecuteScriptString(scriptHandleOrType, stringAtScriptName.c_str(), stackID) == -1)
        throw api_error("simExecuteScriptString");
}

std::vector<std::string> getApiFunc(int scriptHandleOrType, const std::string &apiWord)
{
    std::vector<std::string> ret;
    char *buf = simGetApiFunc(scriptHandleOrType, apiWord.c_str());
    if(buf)
    {
        boost::split(ret, buf, boost::is_any_of(" "));
        releaseBuffer(buf);
    }
    return ret;
}

std::string getApiInfo(int scriptHandleOrType, const std::string &apiWord)
{
    char *buf = simGetApiInfo(scriptHandleOrType, apiWord.c_str());
    if(buf)
    {
        std::string s(buf);
        releaseBuffer(buf);
        return s;
    }
    return "";
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

void setModuleInfo(int infoType, const std::string &stringInfo)
{
    setModuleInfo(pluginName, infoType, stringInfo);
}

void setModuleInfo(int infoType, int intInfo)
{
    setModuleInfo(pluginName, infoType, intInfo);
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

bool isDeprecated(const std::string &funcOrConst)
{
    int ret = simIsDeprecated(funcOrConst.c_str());
    return ret > 0;
}

std::vector<std::string> getPersistentDataTags()
{
    std::vector<std::string> ret;
    int count = 0;
    char *buf = simGetPersistentDataTags(&count);
    if(buf)
    {
        boost::split(ret, buf, boost::is_any_of("\0"));
        releaseBuffer(buf);
    }
    return ret;
}

int eventNotification(const std::string &event)
{
    return simEventNotification(event.c_str());
}

void addLog(boost::optional<std::string> pluginName, int verbosityLevel, boost::optional<std::string> logMsg)
{
    if(simAddLog(pluginName ? pluginName->c_str() : nullptr, verbosityLevel, logMsg ? logMsg->c_str() : nullptr) == -1)
        throw api_error("simAddLog");
}

// int simIsDynamicallyEnabled(int objectHandle);

// int simInitScript(int scriptHandle);

// int simModuleEntry(int handle, const char *label, int state);

// int simCheckExecAuthorization(const char *what, const char *args);

void pushFloatOntoStack(int stackHandle, float value)
{
    addStackDebugLog("simPushFloatOntoStack %f", value);

    if(simPushFloatOntoStack(stackHandle, value) == -1)
        throw api_error("simPushFloatOntoStack");

    addStackDebugDump(stackHandle);
}

void pushFloatTableOntoStack(int stackHandle, const float *values, int valueCnt)
{
    addStackDebugLog("simPushFloatTableOntoStack <%d values>", valueCnt);

    if(simPushFloatTableOntoStack(stackHandle, values, valueCnt) == -1)
        throw api_error("simPushFloatTableOntoStack");

    addStackDebugDump(stackHandle);
}

void pushFloatTableOntoStack(int stackHandle, const std::vector<float> &values)
{
    pushFloatTableOntoStack(stackHandle, values.data(), values.size());
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

int getStackFloatTable(int stackHandle, float *array, int count)
{
    int ret = simGetStackFloatTable(stackHandle, array, count);

    addStackDebugLog("simGetStackFloatTable count = %d -> %d", count, ret);

    if(ret == -1)
        throw api_error("simGetStackFloatTable");
    return ret;
}

int getStackFloatTable(int stackHandle, std::vector<float> *v)
{
    int count = getStackTableInfo(stackHandle, 0);
    v->resize(count);
    return getStackFloatTable(stackHandle, v->data(), count);
}

// float *simGetVisionSensorDepth(int sensorHandle, int options, const int *pos, const int *size, int *resolution);

// int simSetVisionSensorDepth(int sensorHandle, int options, const float *depth);

// float *simCheckVisionSensorEx(int visionSensorHandle, int entityHandle, bool returnImage);

// int simRuckigPos(int dofs, double baseCycleTime, int flags, const double *currentPos, const double *currentVel, const double *currentAccel, const double *maxVel, const double *maxAccel, const double *maxJerk, const bool *selection, const double *targetPos, const double *targetVel, double *reserved1, int *reserved2);

// int simRuckigVel(int dofs, double baseCycleTime, int flags, const double *currentPos, const double *currentVel, const double *currentAccel, const double *maxAccel, const double *maxJerk, const bool *selection, const double *targetVel, double *reserved1, int *reserved2);

// int simRuckigStep(int objHandle, double cycleTime, double *newPos, double *newVel, double *newAccel, double *syncTime, double *reserved1, int *reserved2);

void pushDoubleOntoStack(int stackHandle, double value)
{
    addStackDebugLog("simPushDoubleOntoStack %g", value);

    if(simPushDoubleOntoStack(stackHandle, value) == -1)
        throw api_error("simPushDoubleOntoStack");

    addStackDebugDump(stackHandle);
}

void pushDoubleTableOntoStack(int stackHandle, const double *values, int valueCnt)
{
    addStackDebugLog("simPushDoubleTableOntoStack <%d values>", valueCnt);

    if(simPushDoubleTableOntoStack(stackHandle, values, valueCnt) == -1)
        throw api_error("simPushDoubleTableOntoStack");

    addStackDebugDump(stackHandle);
}

void pushDoubleTableOntoStack(int stackHandle, const std::vector<double> &values)
{
    pushDoubleTableOntoStack(stackHandle, values.data(), values.size());
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

int getStackDoubleTable(int stackHandle, double *array, int count)
{
    int ret = simGetStackDoubleTable(stackHandle, array, count);

    addStackDebugLog("simGetStackDoubleTable, count = %d -> %d", count, ret);

    if(ret == -1)
        throw api_error("simGetStackDoubleTable");
    return ret;
}

int getStackDoubleTable(int stackHandle, std::vector<double> *v)
{
    int count = getStackTableInfo(stackHandle, 0);
    v->resize(count);
    return getStackDoubleTable(stackHandle, v->data(), count);
}

// int simExtLaunchUIThread(const char *applicationName, int options, const char *sceneOrModelOrUiToLoad, const char *applicationDir_);

// int simExtCanInitSimThread();

// int simExtSimThreadInit();

// int simExtSimThreadDestroy();

// int simExtPostExitRequest();

// int simExtGetExitRequest();

// int simExtStep(bool stepIfRunning);

// void _simSetDynamicSimulationIconCode(void *object, int code);

// void _simSetDynamicObjectFlagForVisualization(void *object, int flag);

// int _simGetObjectListSize(int objType);

// const void * _simGetObjectFromIndex(int objType, int index);

// int _simGetObjectID(const void *object);

// int _simGetObjectType(const void *object);

// const void ** _simGetObjectChildren(const void *object, int *count);

// const void * _simGetGeomProxyFromShape(const void *shape);

// const void * _simGetParentObject(const void *object);

// const void * _simGetObject(int objID);

// const void * _simGetIkGroupObject(int ikGroupID);

// int _simMpHandleIkGroupObject(const void *ikGroup);

// bool _simIsShapeDynamicallyStatic(const void *shape);

// int _simGetTreeDynamicProperty(const void *object);

// int _simGetDummyLinkType(const void *dummy, int *linkedDummyID);

// int _simGetJointMode(const void *joint);

// bool _simIsJointInHybridOperation(const void *joint);

// void _simDisableDynamicTreeForManipulation(const void *object, bool disableFlag);

// bool _simIsShapeDynamicallyRespondable(const void *shape);

// int _simGetDynamicCollisionMask(const void *shape);

// const void * _simGetLastParentForLocalGlobalCollidable(const void *shape);

// bool _simGetStartSleeping(const void *shape);

// bool _simGetWasPutToSleepOnce(const void *shape);

// bool _simGetDynamicsFullRefreshFlag(const void *object);

// void _simSetDynamicsFullRefreshFlag(const void *object, bool flag);

// void _simClearAdditionalForceAndTorque(const void *shape);

// int _simGetJointType(const void *joint);

// const void * _simGetGeomWrapFromGeomProxy(const void *geomData);

// int _simGetPurePrimitiveType(const void *geomInfo);

// bool _simIsGeomWrapGeometric(const void *geomInfo);

// bool _simIsGeomWrapConvex(const void *geomInfo);

// int _simGetGeometricCount(const void *geomInfo);

// void _simGetAllGeometrics(const void *geomInfo, void **allGeometrics);

// void _simMakeDynamicAnnouncement(int announceType);

// int _simGetTimeDiffInMs(int previousTime);

// bool _simDoEntitiesCollide(int entity1ID, int entity2ID, int *cacheBuffer, bool overrideCollidableFlagIfShape1, bool overrideCollidableFlagIfShape2, bool pathPlanningRoutineCalling);

// int _simGetJointDynCtrlMode(const void *joint);

// int simFloatingViewRemove(int floatingViewHandle);

int getShapeViz(int shapeHandle, int index, struct SShapeVizInfo *info)
{
    int ret = simGetShapeViz(shapeHandle, index, info);
    if(ret == -1)
        throw api_error("simGetShapeViz");
    return ret;
}

void setArrayParam(int parameter, std::array<double, 3> value)
{
    if(simSetArrayParam(parameter, value.data()) == -1)
        throw api_error("simSetArrayParam");
}

std::array<double, 3> getArrayParam(int parameter)
{
    std::array<double, 3> ret;
    if(simGetArrayParam(parameter, ret.data()) == -1)
        throw api_error("simGetArrayParam");
    return ret;
}

// int simSetFloatSignal(const char *signalName, double signalValue);

// int simGetFloatSignal(const char *signalName, double *signalValue);

void setFloatParam(int parameter, double value)
{
    if(simSetFloatParam(parameter, value) == -1)
        throw api_error("simSetFloatParam");
}

double getFloatParam(int parameter)
{
    double ret;
    if(simGetFloatParam(parameter, &ret) == -1)
        throw api_error("simGetFloatParam");
    return ret;
}

double getObjectFloatParam(int objectHandle, int parameterID)
{
    double ret;
    if(simGetObjectFloatParam(objectHandle, parameterID, &ret) == -1)
        throw api_error("simGetObjectFloatParam");
    return ret;
}

void setObjectFloatParam(int objectHandle, int parameterID, double parameter)
{
    if(simSetObjectFloatParam(objectHandle, parameterID, parameter) == -1)
        throw api_error("simSetObjectFloatParam");
}

// double *simGetObjectFloatArrayParam(int objectHandle, int parameterID, int *size);

// int simSetObjectFloatArrayParam(int objectHandle, int parameterID, const double *params, int size);

double getEngineFloatParam(int paramId, int objectHandle, const void *object)
{
    bool ok = false;
    double ret = simGetEngineFloatParam(paramId, objectHandle, object, &ok);
    if(!ok)
        throw api_error("simGetEngineFloatParam");
    return ret;
}

void setEngineFloatParam(int paramId, int objectHandle, const void *object, double val)
{
    if(simSetEngineFloatParam(paramId, objectHandle, object, val) != 1)
        throw api_error("simSetEngineFloatParam");
}

// int simTransformImage(unsigned char *image, const int *resolution, int options, const double *floatParams, const int *intParams, void *reserved);

// const double *simGetOctreeVoxels(int octreeHandle, int *ptCnt, void *reserved);

std::vector<double> getPointCloudPoints(int pointCloudHandle)
{
    int count = 0;
    const double *buf = simGetPointCloudPoints(pointCloudHandle, &count, 0);
    std::vector<double> ret;
    if(buf)
    {
        ret.resize(count * 3);
        for(size_t i = 0; i < count * 3; i++)
            ret[i] = buf[i];
    }
    return ret;
}

void getObjectMatrix(int objectHandle, int relativeToObjectHandle, double *matrix)
{
    if(simGetObjectMatrix(objectHandle, relativeToObjectHandle, matrix) == -1)
        throw api_error("simGetObjectMatrix");
}

std::array<double, 12> getObjectMatrix(int objectHandle, int relativeToObjectHandle)
{
    std::array<double, 12> ret;
    getObjectMatrix(objectHandle, relativeToObjectHandle, ret.data());
    return ret;
}

void setObjectMatrix(int objectHandle, int relativeToObjectHandle, const double *matrix)
{
    if(simSetObjectMatrix(objectHandle, relativeToObjectHandle, matrix) == -1)
        throw api_error("simSetObjectMatrix");
}

void setObjectMatrix(int objectHandle, int relativeToObjectHandle, const std::array<double, 12> &matrix)
{
    setObjectMatrix(objectHandle, relativeToObjectHandle, matrix.data());
}

void getObjectPose(int objectHandle, int relativeToObjectHandle, double *pose)
{
    if(simGetObjectPose(objectHandle, relativeToObjectHandle, pose) == -1)
        throw api_error("simGetObjectPose");
}

std::array<double, 7> getObjectPose(int objectHandle, int relativeToObjectHandle)
{
    std::array<double, 7> ret;
    getObjectPose(objectHandle, relativeToObjectHandle, ret.data());
    return ret;
}

void setObjectPose(int objectHandle, int relativeToObjectHandle, const double *pose)
{
    if(simSetObjectPose(objectHandle, relativeToObjectHandle, pose) == -1)
        throw api_error("simSetObjectPose");
}

void setObjectPose(int objectHandle, int relativeToObjectHandle, std::array<double, 7> pose)
{
    setObjectPose(objectHandle, relativeToObjectHandle, pose.data());
}

void getObjectPosition(int objectHandle, int relativeToObjectHandle, double *position)
{
    if(simGetObjectPosition(objectHandle, relativeToObjectHandle, position) == -1)
        throw api_error("simGetObjectPosition");
}

std::array<double, 3> getObjectPosition(int objectHandle, int relativeToObjectHandle)
{
    std::array<double, 3> ret;
    getObjectPosition(objectHandle, relativeToObjectHandle, ret.data());
    return ret;
}

void setObjectPosition(int objectHandle, int relativeToObjectHandle, const double *position)
{
    if(simSetObjectPosition(objectHandle, relativeToObjectHandle, position) == -1)
        throw api_error("simSetObjectPosition");
}

void setObjectPosition(int objectHandle, int relativeToObjectHandle, const std::array<double, 3> &position)
{
    setObjectPosition(objectHandle, relativeToObjectHandle, position.data());
}

void getObjectOrientation(int objectHandle, int relativeToObjectHandle, double *eulerAngles)
{
    if(simGetObjectOrientation(objectHandle, relativeToObjectHandle, eulerAngles) == -1)
        throw api_error("simGetObjectOrientation");
}

std::array<double, 3> getObjectOrientation(int objectHandle, int relativeToObjectHandle)
{
    std::array<double, 3> ret;
    getObjectOrientation(objectHandle, relativeToObjectHandle, ret.data());
    return ret;
}

void setObjectOrientation(int objectHandle, int relativeToObjectHandle, const double *eulerAngles)
{
    if(simSetObjectOrientation(objectHandle, relativeToObjectHandle, eulerAngles) == -1)
        throw api_error("simSetObjectOrientation");
}

void setObjectOrientation(int objectHandle, int relativeToObjectHandle, const std::array<double, 3> &eulerAngles)
{
    setObjectOrientation(objectHandle, relativeToObjectHandle, eulerAngles.data());
}

double getJointPosition(int objectHandle)
{
    double position;
    if(simGetJointPosition(objectHandle, &position) == -1)
        throw api_error("simGetJointPosition");
    return position;
}

void setJointPosition(int objectHandle, double position)
{
    if(simSetJointPosition(objectHandle, position) == -1)
        throw api_error("simGetJointPosition");
}

void setJointTargetPosition(int objectHandle, double targetPosition)
{
    if(simSetJointTargetPosition(objectHandle, targetPosition) == -1)
        throw api_error("simSetJointTargetPosition");
}

double getJointTargetPosition(int objectHandle)
{
    double targetPosition;
    if(simGetJointTargetPosition(objectHandle, &targetPosition) == -1)
        throw api_error("simGetJointTargetPosition");
    return targetPosition;
}

double getJointTargetForce(int jointHandle)
{
    double forceOrTorque;
    if(simGetJointTargetForce(jointHandle, &forceOrTorque) == -1)
        throw api_error("simGetJointTargetForce");
    return forceOrTorque;
}

void setJointTargetForce(int objectHandle, double forceOrTorque, bool signedValue)
{
    if(simSetJointTargetForce(objectHandle, forceOrTorque, signedValue) == -1)
        throw api_error("simSetJointTargetForce");
}

void getObjectChildPose(int objectHandle, double *pose)
{
    if(simGetObjectChildPose(objectHandle, pose) == -1)
        throw api_error("simGetObjectChildPose");
}

std::array<double, 7> getObjectChildPose(int objectHandle)
{
    std::array<double, 7> pose;
    getObjectChildPose(objectHandle, pose.data());
    return pose;
}

void setObjectChildPose(int objectHandle, const double *pose)
{
    if(simSetObjectChildPose(objectHandle, pose) == -1)
        throw api_error("simSetObjectChildPose");
}

void setObjectChildPose(int objectHandle, std::array<double, 7> pose)
{
    setObjectChildPose(objectHandle, pose.data());
}

void getJointInterval(int objectHandle, bool *cyclic, double *interval)
{
    if(simGetJointInterval(objectHandle, cyclic, interval) == -1)
        throw api_error("simGetJointInterval");
}

std::array<double, 2> getJointInterval(int objectHandle, bool *cyclic)
{
    std::array<double, 2> interval;
    getJointInterval(objectHandle, cyclic, interval.data());
    return interval;
}

void setJointInterval(int objectHandle, bool cyclic, const double *interval)
{
    if(simSetJointInterval(objectHandle, cyclic, interval) == -1)
        throw api_error("simSetJointInterval");
}

void setJointInterval(int objectHandle, bool cyclic, std::array<double, 2> interval)
{
    setJointInterval(objectHandle, cyclic, interval.data());
}

// int simBuildIdentityMatrix(double *matrix);

// int simBuildMatrix(const double *position, const double *eulerAngles, double *matrix);

// int simBuildPose(const double *position, const double *eulerAngles, double *pose);

// int simGetEulerAnglesFromMatrix(const double *matrix, double *eulerAngles);

// int simInvertMatrix(double *matrix);

std::array<double, 12> multiplyMatrices(std::array<double, 12> matrixIn1, std::array<double, 12> matrixIn2)
{
    std::array<double, 12> ret;
    if(simMultiplyMatrices(matrixIn1.data(), matrixIn2.data(), ret.data()) == -1)
        throw api_error("simMultiplyMatrices");
    return ret;
}

std::array<double, 7> multiplyPoses(std::array<double, 7> poseIn1, std::array<double, 7> poseIn2)
{
    std::array<double, 7> ret;
    if(simMultiplyPoses(poseIn1.data(), poseIn2.data(), ret.data()) == -1)
        throw api_error("simMultiplyPoses");
    return ret;
}

std::array<double, 7> invertPose(std::array<double, 7> pose)
{
    std::array<double, 7> ret(pose);
    if(simInvertPose(ret.data()) == -1)
        throw api_error("simInvertPose");
    return ret;
}

std::array<double, 7> interpolatePoses(std::array<double, 7> poseIn1, std::array<double, 7> poseIn2, double interpolFactor)
{
    std::array<double, 7> ret;
    if(simInterpolatePoses(poseIn1.data(), poseIn2.data(), interpolFactor, ret.data()) == -1)
        throw api_error("simInterpolatePoses");
    return ret;
}

std::array<double, 12> poseToMatrix(std::array<double, 7> poseIn)
{
    std::array<double, 12> ret;
    if(simPoseToMatrix(poseIn.data(), ret.data()) == -1)
        throw api_error("simPoseToMatrix");
    return ret;
}

std::array<double, 7> matrixToPose(std::array<double, 12> matrixIn)
{
    std::array<double, 7> ret;
    if(simMatrixToPose(matrixIn.data(), ret.data()) == -1)
        throw api_error("simMatrixToPose");
    return ret;
}

std::array<double, 12> interpolateMatrices(std::array<double, 12> matrixIn1, std::array<double, 12> matrixIn2, double interpolFactor)
{
    std::array<double, 12> ret;
    if(simInterpolateMatrices(matrixIn1.data(), matrixIn2.data(), interpolFactor, ret.data()) == -1)
        throw api_error("simInterpolateMatrices");
    return ret;
}

std::array<double, 3> transformVector(std::array<double, 12> matrix, std::array<double, 3> vect)
{
    std::array<double, 3> ret(vect);
    if(simTransformVector(matrix.data(), ret.data()) == -1)
        throw api_error("simTransformVector");
    return ret;
}

double getSimulationTime()
{
    double ret = simGetSimulationTime();
    if(ret < 0)
        throw api_error("simGetSimulationTime");
    return ret;
}

double getSystemTime()
{
    return simGetSystemTime();
}

// int simHandleProximitySensor(int sensorHandle, double *detectedPoint, int *detectedObjectHandle, double *normalVector);

// int simReadProximitySensor(int sensorHandle, double *detectedPoint, int *detectedObjectHandle, double *normalVector);

// int simHandleDynamics(double deltaTime);

// int simCheckProximitySensor(int sensorHandle, int entityHandle, double *detectedPoint);

// int simCheckProximitySensorEx(int sensorHandle, int entityHandle, int detectionMode, double detectionThreshold, double maxAngle, double *detectedPoint, int *detectedObjectHandle, double *normalVector);

// int simCheckProximitySensorEx2(int sensorHandle, double *vertexPointer, int itemType, int itemCount, int detectionMode, double detectionThreshold, double maxAngle, double *detectedPoint, double *normalVector);

// int simCheckCollisionEx(int entity1Handle, int entity2Handle, double **intersectionSegments);

// int simCheckDistance(int entity1Handle, int entity2Handle, double threshold, double *distanceData);

// int simSetSimulationTimeStep(double timeStep);

double getSimulationTimeStep()
{
    double ret = simGetSimulationTimeStep();
    if(ret < 0)
        throw api_error("simGetSimulationTimeStep");
    return ret;
}

// int simAdjustRealTimeTimer(int instanceIndex, double deltaTime);

// int simFloatingViewAdd(double posX, double posY, double sizeX, double sizeY, int options);

// int simHandleGraph(int graphHandle, double simulationTime);

// int simSetGraphStreamTransformation(int graphHandle, int streamId, int trType, double mult, double off, int movingAvgPeriod);

// int simAddGraphCurve(int graphHandle, const char *curveName, int dim, const int *streamIds, const double *defaultValues, const char *unitStr, int options, const float *color, int curveWidth);

// int simSetGraphStreamValue(int graphHandle, int streamId, double value);

// int simSetJointTargetVelocity(int objectHandle, double targetVelocity);

// int simGetJointTargetVelocity(int objectHandle, double *targetVelocity);

// int simScaleObjects(const int *objectHandles, int objectCount, double scalingFactor, bool scalePositionsToo);

// int simAddDrawingObject(int objectType, double size, double duplicateTolerance, int parentObjectHandle, int maxItemCount, const float *color, const float *setToNULL, const float *setToNULL2, const float *setToNULL3);

// int simAddGraphStream(int graphHandle, const char *streamName, const char *unitStr, int options, const float *color, double cyclicRange);

// int simAddDrawingObjectItem(int objectHandle, const double *itemData);

// double simGetObjectSizeFactor(int objectHandle);

// int simReadForceSensor(int objectHandle, double *forceVector, double *torqueVector);

// int simSetLightParameters(int objectHandle, int state, const float *setToNULL, const float *diffusePart, const float *specularPart);

int getLightParameters(int objectHandle)
{
    int ret = simGetLightParameters(objectHandle, nullptr, nullptr, nullptr);
    if(ret == -1)
        throw api_error("simGetLightParameters");
    return ret;
}

int getLightParameters(int objectHandle, std::array<double, 3> &diffuse)
{
    int ret = simGetLightParameters(objectHandle, nullptr, diffuse.data(), nullptr);
    if(ret == -1)
        throw api_error("simGetLightParameters");
    return ret;
}

int getLightParameters(int objectHandle, std::array<double, 3> &diffuse, std::array<double, 3> &specular)
{
    int ret = simGetLightParameters(objectHandle, nullptr, diffuse.data(), specular.data());
    if(ret == -1)
        throw api_error("simGetLightParameters");
    return ret;
}

// int simGetVelocity(int shapeHandle, double *linearVelocity, double *angularVelocity);

std::pair<std::array<double, 3>, std::array<double, 3>> getObjectVelocity(int objectHandle)
{
    std::array<double, 3> lin, ang;
    if(simGetObjectVelocity(objectHandle, lin.data(), ang.data()) == -1)
        throw api_error("simGetObjectVelocity");
    return std::make_pair(lin, ang);
}

// int simGetJointVelocity(int jointHandle, double *velocity);

// int simAddForceAndTorque(int shapeHandle, const double *force, const double *torque);

// int simAddForce(int shapeHandle, const double *position, const double *force);

// int simSetObjectColor(int objectHandle, int index, int colorComponent, const float *rgbData);

// int simGetObjectColor(int objectHandle, int index, int colorComponent, float *rgbData);

// int simSetShapeColor(int shapeHandle, const char *colorName, int colorComponent, const float *rgbData);

boost::optional<std::array<float, 3>> getShapeColor(int shapeHandle, boost::optional<std::string> colorName, int colorComponent)
{
    std::array<float, 3> rgbData;
    int ret = simGetShapeColor(shapeHandle, colorName ? colorName->c_str() : 0, colorComponent, rgbData.data());
    if(ret == -1)
        throw api_error("simGetShapeColor");
    if(ret == 0)
        return {};
    return rgbData;
}

boost::optional<std::array<float, 3>> getShapeColor(int shapeHandle, int colorComponent)
{
    return getShapeColor(shapeHandle, {}, colorComponent);
}

// int simGetContactInfo(int dynamicPass, int objectHandle, int index, int *objectHandles, double *contactInfo);

// int simAuxiliaryConsoleOpen(const char *title, int maxLines, int mode, const int *position, const int *size, const float *textColor, const float *backgroundColor);

int importShape(const char *pathAndFilename, int options, double scalingFactor)
{
    int handle = simImportShape(0, pathAndFilename, options, 0, scalingFactor);
    if(handle == -1)
        throw api_error("simImportShape");
    return handle;
}

int importShape(const std::string &pathAndFilename, int options, double scalingFactor)
{
    return importShape(pathAndFilename.c_str(), options, scalingFactor);
}

// int simImportMesh(int fileformat, const char *pathAndFilename, int options, double identicalVerticeTolerance, double scalingFactor, double ***vertices, int **verticesSizes, int ***indices, int **indicesSizes, double ***reserved, char ***names);

// int simExportMesh(int fileformat, const char *pathAndFilename, int options, double scalingFactor, int elementCount, const double **vertices, const int *verticesSizes, const int **indices, const int *indicesSizes, double **reserved, const char **names);

int createMeshShape(int options, double shadingAngle, const double *vertices, int verticesSize, const int *indices, int indicesSize)
{
    int handle = simCreateMeshShape(options, shadingAngle, vertices, verticesSize, indices, indicesSize, nullptr);
    if(handle == -1)
        throw api_error("simCreateMeshShape");
    return handle;
}

int createMeshShape(int options, double shadingAngle, const std::vector<double> &vertices, const std::vector<int> &indices)
{
    return createMeshShape(options, shadingAngle, vertices.data(), vertices.size(), indices.data(), indices.size());
}

int createPrimitiveShape(int primitiveType, const double *sizes, int options)
{
    int handle = simCreatePrimitiveShape(primitiveType, sizes, options);
    if(handle == -1)
        throw api_error("simCreatePrimitiveShape");
    return handle;
}

int createPrimitiveShape(int primitiveType, std::array<double, 3> sizes, int options)
{
    return createPrimitiveShape(primitiveType, sizes.data(), options);
}

int createHeightfieldShape(int options, double shadingAngle, int xPointCount, int yPointCount, double xSize, const double *heights)
{
    int handle = simCreateHeightfieldShape(options, shadingAngle, xPointCount, yPointCount, xSize, heights);
    if(handle == -1)
        throw api_error("simCreateHeightfieldShape");
    return handle;
}

int createHeightfieldShape(int options, double shadingAngle, int xPointCount, int yPointCount, double xSize, const std::vector<double> &heights)
{
    return createHeightfieldShape(options, shadingAngle, xPointCount, yPointCount, xSize, heights.data());
}

void getShapeMesh(int shapeHandle, double **vertices, int *verticesSize, int **indices, int *indicesSize, double **normals)
{
    if(simGetShapeMesh(shapeHandle, vertices, verticesSize, indices, indicesSize, normals) == -1)
        throw api_error("simGetShapeMesh");
}

void getShapeMesh(int shapeHandle, std::vector<double> vertices, std::vector<int> indices, boost::optional<std::vector<double>> normals)
{
    double *verticesBuf;
    int verticesSize = 0;
    int *indicesBuf;
    int indicesSize = 0;
    double *normalsBuf;
    getShapeMesh(shapeHandle, &verticesBuf, &verticesSize, &indicesBuf, &indicesSize, normals ? &normalsBuf : nullptr);
    vertices.assign(verticesBuf, verticesBuf + verticesSize);
    indices.assign(indicesBuf, indicesBuf + indicesSize);
    if(normals)
        normals->assign(normalsBuf, normalsBuf + indicesSize * 3);
    releaseBuffer(verticesBuf);
    releaseBuffer(indicesBuf);
    releaseBuffer(normalsBuf);
}

int createJoint(int jointType, int jointMode, int options, const double *sizes)
{
    int handle = simCreateJoint(jointType, jointMode, options, sizes, nullptr, nullptr);
    if(handle == -1)
        throw api_error("simCreateJoint");
    return handle;
}

int createJoint(int jointType, int jointMode, int options, boost::optional<std::array<double, 2>> sizes)
{
    return createJoint(jointType, jointMode, options, sizes ? sizes->data() : nullptr);
}

int createDummy(double size)
{
    int handle = simCreateDummy(size, nullptr);
    if(handle == -1)
        throw api_error("simCreateDummy");
    return handle;
}

int createForceSensor(int options, const int *intParams, const double *floatParams)
{
    int handle = simCreateForceSensor(options, intParams, floatParams, nullptr);
    if(handle == -1)
        throw api_error("simCreateForceSensor");
    return handle;
}

int createForceSensor(int options, int type, int valueCount, int thresholdCount, double size, double forceThreshold, double torqueThreshold)
{
    int intParams[] = {
        type,
        valueCount,
        thresholdCount,
        0,
        0
    };
    double floatParams[] = {
        size,
        forceThreshold,
        torqueThreshold,
        0.0,
        0.0
    };
    return createForceSensor(options, &intParams[0], &floatParams[0]);
}

int createVisionSensor(int options, const int *intParams, const double *floatParams)
{
    int handle = simCreateVisionSensor(options, intParams, floatParams, nullptr);
    if(handle == -1)
        throw api_error("simCreateVisionSensor");
    return handle;
}

int createVisionSensor(int options, std::array<int, 2> resolution, double clipNear, double clipFar, double viewAngleOrOrthoSize, double xSize, std::array<float, 3> nullPixelColor)
{
    int intParams[] = {
        resolution[0],
        resolution[1],
        0,
        0
    };
    double floatParams[] = {
        clipNear,
        clipFar,
        viewAngleOrOrthoSize,
        xSize,
        0.0,
        0.0,
        nullPixelColor[0],
        nullPixelColor[1],
        nullPixelColor[2],
        0.0,
        0.0
    };
    return createVisionSensor(options, &intParams[0], &floatParams[0]);
}

int createProximitySensor(int sensorType, int options, const int *intParams, const double *floatParams)
{
    int handle = simCreateProximitySensor(sensorType, 16, options, intParams, floatParams, nullptr);
    if(handle == -1)
        throw api_error("simCreateProximitySensor");
    return handle;
}

int createProximitySensor(int sensorType, int options, int faceCount, int faceCountFar, int subdivisions, int subdivisionsFar, int randDetSampleCountPerReading, int randDetIndividualRayDetCntForTrig, double offset, double range, std::array<double, 2> size, std::array<double, 2> sizeFar, double insideGap, double radius, double radiusFar, double angle, double thresholdAngle, double smallestDetDist, double sensPointSize)
{
    int intParams[] = {
        faceCount,
        faceCountFar,
        subdivisions,
        subdivisionsFar,
        randDetSampleCountPerReading,
        randDetIndividualRayDetCntForTrig,
        0,
        0
    };
    double floatParams[] = {
        offset,
        range,
        size[0],
        size[1],
        sizeFar[0],
        sizeFar[1],
        insideGap,
        radius,
        radiusFar,
        angle,
        thresholdAngle,
        smallestDetDist,
        sensPointSize,
        0.0,
        0.0
    };
    return createProximitySensor(sensorType, options, &intParams[0], &floatParams[0]);
}

std::pair<std::array<double, 3>, double> getRotationAxis(std::array<double, 12> matrixStart, std::array<double, 12> matrixGoal)
{
    std::array<double, 3> axis;
    double angle;
    if(simGetRotationAxis(matrixStart.data(), matrixGoal.data(), axis.data(), &angle) == -1)
        throw api_error("simGetRotationAxis");
    return std::make_pair(axis, angle);
}

std::pair<std::array<double, 3>, double> getRotationAxis(std::array<double, 7> matrixStart, std::array<double, 7> matrixGoal)
{
    std::array<double, 3> axis;
    double angle;
    if(simGetRotationAxis(matrixStart.data(), matrixGoal.data(), axis.data(), &angle) == -1)
        throw api_error("simGetRotationAxis");
    return std::make_pair(axis, angle);
}

// int simRotateAroundAxis(const double *matrixIn, const double *axis, const double *axisPos, double angle, double *matrixOut);

// int simGetJointForce(int jointHandle, double *forceOrTorque);

// int simCameraFitToView(int viewHandleOrIndex, int objectCount, const int *objectHandles, int options, double scaling);

// int simHandleVisionSensor(int visionSensorHandle, double **auxValues, int **auxValuesCount);

// int simReadVisionSensor(int visionSensorHandle, double **auxValues, int **auxValuesCount);

// int simCheckVisionSensor(int visionSensorHandle, int entityHandle, double **auxValues, int **auxValuesCount);

unsigned char * getVisionSensorImg(int sensorHandle, int options, double rgbaCutOff, std::array<int, 2> pos, std::array<int, 2> size, std::array<int, 2> *resolution)
{
    unsigned char *buf = simGetVisionSensorImg(sensorHandle, options, rgbaCutOff, pos.data(), size.data(), resolution ? resolution->data() : nullptr);
    if(!buf)
        throw api_error("simGetVisionSensorImg");
    return buf;
}

std::array<int, 2> getVisionSensorRes(int visionSensorHandle)
{
    std::array<int, 2> ret;
    if(simGetVisionSensorRes(visionSensorHandle, ret.data()) == -1)
        throw api_error("simGetVisionSensorRes");
    return ret;
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

// int simConvexDecompose(int shapeHandle, int options, const int *intParams, const double *floatParams);

void writeTexture(int textureId, int options, const char *data, int posX, int posY, int sizeX, int sizeY, double interpol)
{
    if(simWriteTexture(textureId, options, data, posX, posY, sizeX, sizeY, interpol) == -1)
        throw api_error("simWriteTexture");
}

// int simCreateTexture(const char *fileName, int options, const double *planeSizes, const double *scalingUV, const double *xy_g, int fixedResolution, int *textureId, int *resolution, const void *reserved);

// int simGetShapeGeomInfo(int shapeHandle, int *intData, double *floatData, void *reserved);

// int simScaleObject(int objectHandle, double xScale, double yScale, double zScale, int options);

// int simSetShapeTexture(int shapeHandle, int textureId, int mappingMode, int options, const double *uvScaling, const double *position, const double *orientation);

// int simGetQHull(const double *inVertices, int inVerticesL, double **verticesOut, int *verticesOutL, int **indicesOut, int *indicesOutL, int reserved1, const double *reserved2);

// int simGetDecimatedMesh(const double *inVertices, int inVerticesL, const int *inIndices, int inIndicesL, double **verticesOut, int *verticesOutL, int **indicesOut, int *indicesOutL, double decimationPercent, int reserved1, const double *reserved2);

// int simComputeMassAndInertia(int shapeHandle, double density);

// int simCreateOctree(double voxelSize, int options, double pointSize, void *reserved);

int createPointCloud(double maxVoxelSize, int maxPtCntPerVoxel, int options, double pointSize)
{
    int ret = simCreatePointCloud(maxVoxelSize, maxPtCntPerVoxel, options, pointSize, nullptr);
    if(ret == -1)
        throw api_error("simCreatePointCloud");
    return ret;
}

// int simSetPointCloudOptions(int pointCloudHandle, double maxVoxelSize, int maxPtCntPerVoxel, int options, double pointSize, void *reserved);

// int simGetPointCloudOptions(int pointCloudHandle, double *maxVoxelSize, int *maxPtCntPerVoxel, int *options, double *pointSize, void *reserved);

// int simInsertVoxelsIntoOctree(int octreeHandle, int options, const double *pts, int ptCnt, const unsigned char *color, const unsigned int *tag, void *reserved);

// int simRemoveVoxelsFromOctree(int octreeHandle, int options, const double *pts, int ptCnt, void *reserved);

// int simInsertPointsIntoPointCloud(int pointCloudHandle, int options, const double *pts, int ptCnt, const unsigned char *color, void *optionalValues);

// int simRemovePointsFromPointCloud(int pointCloudHandle, int options, const double *pts, int ptCnt, double tolerance, void *reserved);

// int simIntersectPointsWithPointCloud(int pointCloudHandle, int options, const double *pts, int ptCnt, double tolerance, void *reserved);

int insertObjectIntoPointCloud(int pointCloudHandle, int objectHandle, int options, double gridSize, boost::optional<std::array<unsigned char, 3>> color, boost::optional<float> duplicateTolerance)
{
    std::array<unsigned char, 3> color_;
    unsigned char *colorPtr = nullptr;
    if(color)
    {
        color_ = *color;
        colorPtr = color_.data();
    }

    int optionalValues[2];
    void *optionalValuesPtr = nullptr;
    if(duplicateTolerance)
    {
        optionalValuesPtr = &optionalValues[0];
        ((int *)optionalValuesPtr)[0] = 1;
        ((float *)optionalValuesPtr)[1] = *duplicateTolerance;
    }

    int ret = simInsertObjectIntoPointCloud(pointCloudHandle, objectHandle, options, gridSize, colorPtr, optionalValuesPtr);
    if(ret == -1)
        throw api_error("simInsertObjectIntoPointCloud");
    return ret;
}

// int simSubtractObjectFromPointCloud(int pointCloudHandle, int objectHandle, int options, double tolerance, void *reserved);

// int simCheckOctreePointOccupancy(int octreeHandle, int options, const double *points, int ptCnt, unsigned int *tag, unsigned long long int *location, void *reserved);

// int simApplyTexture(int shapeHandle, const double *textureCoordinates, int textCoordSize, const unsigned char *texture, const int *textureResolution, int options);

// int simSetJointDependency(int jointHandle, int masterJointHandle, double offset, double multCoeff);

// int simGetJointDependency(int jointHandle, int *masterJointHandle, double *offset, double *multCoeff);

double getShapeMass(int shapeHandle)
{
    double mass = 0.0;
    if(simGetShapeMass(shapeHandle, &mass) == -1)
        throw api_error("simGetShapeMass");
    return mass;
}

void setShapeMass(int shapeHandle, double mass)
{
    if(simSetShapeMass(shapeHandle, mass) == -1)
        throw api_error("simSetShapeMass");
}

void getShapeInertia(int shapeHandle, double *inertiaMatrix, double *transformationMatrix)
{
    if(simGetShapeInertia(shapeHandle, inertiaMatrix, transformationMatrix) == -1)
        throw api_error("simGetShapeInertia");
}

std::pair<std::array<double, 9>, std::array<double, 12>> getShapeInertia(int shapeHandle)
{
    std::array<double, 9> inertiaMatrix;
    std::array<double, 12> transformationMatrix;
    getShapeInertia(shapeHandle, inertiaMatrix.data(), transformationMatrix.data());
    return std::make_pair(inertiaMatrix, transformationMatrix);
}

void setShapeInertia(int shapeHandle, const double *inertiaMatrix, const double *transformationMatrix)
{
    if(simSetShapeInertia(shapeHandle, inertiaMatrix, transformationMatrix) == -1)
        throw api_error("simSetShapeInertia");
}

void setShapeInertia(int shapeHandle, std::array<double, 9> inertiaMatrix, std::array<double, 12> transformationMatrix)
{
    return setShapeInertia(shapeHandle, inertiaMatrix.data(), transformationMatrix.data());
}

// int simGenerateShapeFromPath(const double *path, int pathSize, const double *section, int sectionSize, int options, const double *upVector, double reserved);

// double simGetClosestPosOnPath(const double *path, int pathSize, const double *pathLengths, const double *absPt);

// int simExtCallScriptFunction(int scriptHandleOrType, const char *functionNameAtScriptName, const int *inIntData, int inIntCnt, const double *inFloatData, int inFloatCnt, const char **inStringData, int inStringCnt, const char *inBufferData, int inBufferCnt, int **outIntData, int *outIntCnt, double **outFloatData, int *outFloatCnt, char ***outStringData, int *outStringCnt, char **outBufferData, int *outBufferSize);

// void simGetObjectLocalTransformation(const void *object, double *pos, double *quat, bool excludeFirstJointTransformation);

// void simSetObjectLocalTransformation(void *object, const double *pos, const double *quat, double simTime);

// void simDynReportObjectCumulativeTransformation(void *object, const double *pos, const double *quat, double simTime);

// void simSetObjectCumulativeTransformation(void *object, const double *pos, const double *quat, bool keepChildrenInPlace);

// void simGetObjectCumulativeTransformation(const void *object, double *pos, double *quat, bool excludeFirstJointTransformation);

// void simSetJointVelocity(const void *joint, double vel);

// void simSetJointPosition(const void *joint, double pos);

// double simGetJointPosition(const void *joint);

// void simSetDynamicMotorPositionControlTargetPosition(const void *joint, double pos);

// void simGetInitialDynamicVelocity(const void *shape, double *vel);

// void simSetInitialDynamicVelocity(void *shape, const double *vel);

// void simGetInitialDynamicAngVelocity(const void *shape, double *angularVel);

// void simSetInitialDynamicAngVelocity(void *shape, const double *angularVel);

// void simSetShapeDynamicVelocity(void *shape, const double *linear, const double *angular, double simTime);

// void simGetAdditionalForceAndTorque(const void *shape, double *force, double *torque);

// bool simGetJointPositionInterval(const void *joint, double *minValue, double *rangeValue);

// double simGetDynamicMotorTargetPosition(const void *joint);

// double simGetDynamicMotorTargetVelocity(const void *joint);

// double simGetDynamicMotorMaxForce(const void *joint);

// double simGetDynamicMotorUpperLimitVelocity(const void *joint);

// void simSetDynamicMotorReflectedPositionFromDynamicEngine(void *joint, double pos, double simTime);

// void simSetJointSphericalTransformation(void *joint, const double *quat, double simTime);

// void simAddForceSensorCumulativeForcesAndTorques(void *forceSensor, const double *force, const double *torque, int totalPassesCount, double simTime);

// void simAddJointCumulativeForcesOrTorques(void *joint, double forceOrTorque, int totalPassesCount, double simTime);

// double simGetMass(const void *geomInfo);

// double simGetLocalInertiaInfo(const void *object, double *pos, double *quat, double *diagI);

// void simGetPurePrimitiveSizes(const void *geometric, double *sizes);

// void simGetVerticesLocalFrame(const void *geometric, double *pos, double *quat);

// const double *simGetHeightfieldData(const void *geometric, int *xCount, int *yCount, double *minHeight, double *maxHeight);

// void simGetCumulativeMeshes(const void *geomInfo, double **vertices, int *verticesSize, int **indices, int *indicesSize);

// void simGetGravity(double *gravity);

// bool simGetDistanceBetweenEntitiesIfSmaller(int entity1ID, int entity2ID, double *distance, double *ray, int *cacheBuffer, bool overrideMeasurableFlagIfNonCollection1, bool overrideMeasurableFlagIfNonCollection2, bool pathPlanningRoutineCalling);

// int simHandleJointControl(const void *joint, int auxV, const int *inputValuesInt, const double *inputValuesFloat, double *outputValues);

// int simHandleCustomContact(int objHandle1, int objHandle2, int engine, int *dataInt, double *dataFloat);

// double simGetPureHollowScaling(const void *geometric);

// void simDynCallback(const int *intData, const double *floatData);

} // namespace sim
