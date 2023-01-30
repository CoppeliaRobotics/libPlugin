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
    namespace util
    {
        static std::string sprintf(boost::format &fmt)
        {
            return fmt.str();
        }

        template<typename Arg, typename... Args>
        std::string sprintf(boost::format &fmt, Arg &&arg, Args &&...args)
        {
            return sprintf(fmt % std::forward<Arg>(arg), std::forward<Args>(args)...);
        }

        template<typename... Args>
        std::string sprintf(const std::string &msg, Args &&...args)
        {
            return msg;
        }

        template<typename Arg, typename... Args>
        std::string sprintf(const std::string &fmt, Arg &&arg, Args &&...args)
        {
            boost::format f(fmt);
            return sprintf(f, std::forward<Arg>(arg), std::forward<Args>(args)...);
        }
    } // namespace util

	/*! \brief A basic exception class
	 */
    struct exception : public ::std::exception
    {
        std::string s;

        template<typename... Arguments>
        exception(const std::string &fmt, Arguments&&... args)
            : s(util::sprintf(fmt, std::forward<Arguments>(args)...))
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

    struct api_error : public exception
    {
        std::string func;
        std::string error;

        api_error(const std::string &func_)
            : api_error(func_, "error")
        {
        }

        api_error(const std::string &func_, const std::string &error_)
            : func(func_),
              error(error_),
              exception("%s: %s", func_, error_)
        {
        }

        ~api_error() throw()
        {
        }
    };

    void enableStackDebug();
    void disableStackDebug();
    bool isStackDebugEnabled();

    int registerScriptCallbackFunction(const std::string &funcNameAtPluginName, const std::string &callTips, void (*callBack)(struct SScriptCallBack *cb));
    int registerScriptVariable(const std::string &varName, const char *varValue, int stackID);
    int registerScriptVariable(const std::string &varName, const std::string &varValue, int stackID);
    template<typename T>
    int registerScriptVariable(const std::string &varName, const T &varValue, int stackID)
    {
        return registerScriptVariable(varName, std::to_string(varValue), stackID);
    }

    void callScriptFunctionEx(int scriptHandleOrType, const std::string &functionNameAtScriptName, int stackID);
    void executeScriptString(int scriptHandleOrType, const std::string &stringAtScriptName, int stackID);

    int createStack();
    void releaseStack(int stackHandle);
    int copyStack(int stackHandle);
    void pushNullOntoStack(int stackHandle);
    void pushBoolOntoStack(int stackHandle, bool value);
    void pushInt32OntoStack(int stackHandle, int value);
    void pushFloatOntoStack(int stackHandle, float value);
    void pushDoubleOntoStack(int stackHandle, double value);
    void pushStringOntoStack(int stackHandle, const char *value, int stringSize);
    void pushUInt8TableOntoStack(int stackHandle, const unsigned char *values, int valueCnt);
    void pushInt32TableOntoStack(int stackHandle, const int *values, int valueCnt);
    void pushFloatTableOntoStack(int stackHandle, const float *values, int valueCnt);
    void pushDoubleTableOntoStack(int stackHandle, const double *values, int valueCnt);
    void pushTableOntoStack(int stackHandle);
    void insertDataIntoStackTable(int stackHandle);
    int getStackSize(int stackHandle);
    int popStackItem(int stackHandle, int count);
    void moveStackItemToTop(int stackHandle, int cIndex);
    int isStackValueNull(int stackHandle);
    int getStackBoolValue(int stackHandle, bool *boolValue);
    int getStackInt32Value(int stackHandle, int *numberValue);
    int getStackFloatValue(int stackHandle, float *numberValue);
    int getStackDoubleValue(int stackHandle, double *numberValue);
    char* getStackStringValue(int stackHandle, int *stringSize);
    int getStackTableInfo(int stackHandle, int infoType);
    int getStackUInt8Table(int stackHandle, unsigned char *array, int count);
    int getStackInt32Table(int stackHandle, int *array, int count);
    int getStackFloatTable(int stackHandle, float *array, int count);
    int getStackDoubleTable(int stackHandle, double *array, int count);
    void unfoldStackTable(int stackHandle);
    void unfoldStackTable(int stackHandle);
    void debugStack(int stackHandle, int index = -1);

    void pushStringOntoStack(int stackHandle, const std::string &value);
    void pushUInt8TableOntoStack(int stackHandle, const std::vector<unsigned char> &values);
    void pushInt32TableOntoStack(int stackHandle, const std::vector<int> &values);
    void pushFloatTableOntoStack(int stackHandle, const std::vector<float> &values);
    void pushDoubleTableOntoStack(int stackHandle, const std::vector<double> &values);
    int getStackStringValue(int stackHandle, std::string *stringValue);
    int getStackUInt8Table(int stackHandle, std::vector<unsigned char> *v);
    int getStackInt32Table(int stackHandle, std::vector<int> *v);
    int getStackFloatTable(int stackHandle, std::vector<float> *v);
    int getStackDoubleTable(int stackHandle, std::vector<double> *v);

    int getBoolParam(int parameter);
    int getInt32Param(int parameter);
    unsigned long long int getUInt64Param(int parameter);
    double getFloatParam(int parameter);
    std::array<double, 3> getArrayParam(int parameter);
    std::string getStringParam(int parameter);
    void setBoolParam(int parameter, bool value);
    void setInt32Param(int parameter, int value);
    void setFloatParam(int parameter, double value);
    void setArrayParam(int parameter, std::array<double, 3> value);
    void setStringParam(int parameter, char *value);
    void setStringParam(int parameter, const std::string &value);

    std::string getObjectAlias(int objectHandle, int options);
    double getObjectFloatParam(int objectHandle, int parameterID);
    int getObjectInt32Param(int objectHandle, int parameterID);
    std::string getObjectStringParam(int objectHandle, int parameterID);
    void setObjectAlias(int objectHandle, const std::string &alias, int options = 0);
    void setObjectFloatParam(int objectHandle, int parameterID, double parameter);
    void setObjectInt32Param(int objectHandle, int parameterID, int parameter);
    void setObjectStringParam(int objectHandle, int parameterID, const std::string &parameter);
    int getScriptInt32Param(int scriptHandle, int parameterID);
//    void getScriptProperty(int scriptHandle, int *scriptProperty, int *associatedObjectHandle);

    boost::optional<std::string> getNamedStringParam(const std::string &parameter);
    boost::optional<bool> getNamedBoolParam(const std::string &parameter);
    boost::optional<double> getNamedFloatParam(const std::string &parameter);
    boost::optional<int> getNamedInt32Param(const std::string &parameter);
    void setNamedStringParam(const std::string &parameter, const std::string &value);
    void setNamedBoolParam(const std::string &parameter, bool value);
    void setNamedFloatParam(const std::string &parameter, double value);
    void setNamedInt32Param(const std::string &parameter, int value);

    void* createBuffer(int size);
    void releaseBuffer(const void *buffer);

    std::string getLastError();
    void setLastError(const std::string &func, const std::string &msg);

    int getObjectChild(int objectHandle, int index);
//    int getObjectHandle(const std::string &objectName);
    std::array<double, 12> getObjectMatrix(int objectHandle, int relativeToObjectHandle);
    void setObjectMatrix(int objectHandle, int relativeToObjectHandle, const std::array<double, 12> &matrix);
//    std::string getObjectName(int objectHandle);
//    void setObjectName(int objectHandle, const std::string &objectName);
    std::array<double, 3> getObjectOrientation(int objectHandle, int relativeToObjectHandle);
    void setObjectOrientation(int objectHandle, int relativeToObjectHandle, const std::array<double, 3> &eulerAngles);
    int getObjectParent(int objectHandle);
    void setObjectParent(int objectHandle, int parentObjectHandle, bool keepInPlace);
    std::array<double, 3> getObjectPosition(int objectHandle, int relativeToObjectHandle);
    void setObjectPosition(int objectHandle, int relativeToObjectHandle, const std::array<double, 3> &position);
    std::array<double, 4> getObjectQuaternion(int objectHandle, int relativeToObjectHandle);
    void setObjectQuaternion(int objectHandle, int relativeToObjectHandle, const std::array<double, 4> &quaternion);
    int getObjectType(int objectHandle);
    int getObjectUniqueIdentifier(int objectHandle);
    std::pair<std::array<double, 3>, std::array<double, 3>> getObjectVelocity(int objectHandle);
    int getObjects(int index, int objectType);
    std::vector<int> getObjectsInTree(int treeBaseHandle, int objectType, int options = 0);
    std::vector<int> getObjectSel();
    long long int getObjectUid(int objectHandle);
//    int getObjectSelectionSize();
    std::vector<int> ungroupShape(int shapeHandle);
    int groupShapes(const std::vector<int> &shapeHandles, bool merge = false);
    void copyPasteObjects(std::vector<int> &shapeHandles, int options);
    std::vector<int> copyPasteObjects(const std::vector<int> &shapeHandles, int options);
    boost::optional<std::array<float, 3>> getShapeColor(int shapeHandle, boost::optional<std::string> colorName, int colorComponent);
    boost::optional<std::array<float, 3>> getShapeColor(int shapeHandle, int colorComponent);
    int getShapeViz(int shapeHandle, int index, struct SShapeVizInfo* info);
    void removeObjects(const std::vector<int> &objectHandles);
    int getLightParameters(int objectHandle);
    int getLightParameters(int objectHandle, std::array<double, 3> &diffuse);
    int getLightParameters(int objectHandle, std::array<double, 3> &diffuse, std::array<double, 3> &specular);

    void setModuleInfo(const std::string &moduleName, int infoType, const std::string &stringInfo);
    void setModuleInfo(const std::string &moduleName, int infoType, int intInfo);
    void getModuleInfo(const std::string &moduleName, int infoType, std::string &stringInfo);
    void getModuleInfo(const std::string &moduleName, int infoType, int &intInfo);
    std::string getModuleInfoStr(const std::string &moduleName, int infoType);
    int getModuleInfoInt(const std::string &moduleName, int infoType);

    void setModuleInfo(int infoType, const std::string &stringInfo);
    void setModuleInfo(int infoType, int intInfo);
    void getModuleInfo(int infoType, std::string &stringInfo);
    void getModuleInfo(int infoType, int &intInfo);
    std::string getModuleInfoStr(int infoType);
    int getModuleInfoInt(int infoType);

    void * getMainWindow(int type);

    int eventNotification(const std::string &event);

    int getSimulationState();
    double getSimulationTime();
    double getSimulationTimeStep();

    int programVersion();
    std::string versionString(int v);

    extern std::string pluginNameAndVersion;

    template<typename... Arguments>
    void addLog(int verbosity, const std::string &fmt, Arguments&&... args)
    {
        ::simAddLog(pluginNameAndVersion.c_str(), verbosity, util::sprintf(fmt, std::forward<Arguments>(args)...).c_str());
    }
} // namespace sim

#endif // SIMPLUSPLUS_LIB_H_INCLUDED
