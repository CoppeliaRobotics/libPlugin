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

        const char * what() const throw()
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

    extern std::string pluginNameAndVersion;


    void enableStackDebug();
    void disableStackDebug();
    bool isStackDebugEnabled();

    int programVersion();
    std::string versionString(int v);



    // runSimulator

    // runSimulatorEx

    // getSimulatorMessage

    void * getMainWindow(int type);

    std::string getLastError();

    // loadModule

    // unloadModule

    void setBoolParam(int parameter, bool value);

    bool getBoolParam(int parameter);

    void setInt32Param(int parameter, int value);

    int getInt32Param(int parameter);

    unsigned long long int getUInt64Param(int parameter);

    void setStringParam(int parameter, char *value);
    void setStringParam(int parameter, const std::string &value);

    std::string getStringParam(int parameter);

    void setNamedStringParam(const std::string &parameter, const std::string &value);
    void setNamedBoolParam(const std::string &parameter, bool value);
    void setNamedFloatParam(const std::string &parameter, double value);
    void setNamedInt32Param(const std::string &parameter, int value);

    boost::optional<std::string> getNamedStringParam(const std::string &parameter);
    boost::optional<bool> getNamedBoolParam(const std::string &parameter);
    boost::optional<double> getNamedFloatParam(const std::string &parameter);
    boost::optional<int> getNamedInt32Param(const std::string &parameter);

    // getObject

    long long int getObjectUid(int objectHandle);

    // getObjectFromUid

    int getScriptHandleEx(int scriptType, int objHandle, boost::optional<std::string> scriptName = {});

    void removeObjects(const std::vector<int> &objectHandles);

    int removeModel(int objectHandle);

    std::string getObjectAlias(int objectHandle, int options);

    void setObjectAlias(int objectHandle, const std::string &alias, int options = 0);

    int getObjectParent(int objectHandle);

    int getObjectChild(int objectHandle, int index);
    std::vector<int> getObjectChildren(int objectHandle);

    void setObjectParent(int objectHandle, int parentObjectHandle, bool keepInPlace);

    int getObjectType(int objectHandle);

    int getJointType(int objectHandle);

    // reservedCommand

    int getSimulationState();

    void loadScene(const char *filename);
    void loadScene(const std::string &filename);

    void closeScene();

    void saveScene(const char *filename);
    void saveScene(const std::string &filename);

    void loadModel(const char *filename);
    void loadModel(const std::string &filename);

    void saveModel(int baseOfModelHandle, const char *filename);
    void saveModel(int baseOfModelHandle, const std::string &filename);

    bool doesFileExist(const char *filename);
    bool doesFileExist(const std::string &filename);

    std::vector<int> getObjectSel();

    // setObjectSel

    // associateScriptWithObject

    // handleMainScript

    // resetScript

    // addScript

    // removeScript

    // refreshDialogs

    // resetProximitySensor

    void * createBuffer(int size);

    void releaseBuffer(const void *buffer);

    bool checkCollision(int entity1Handle, int entity2Handle);

    // getRealTimeSimulation

    // isRealTimeSimulationStepNeeded

    // getSimulationPassesPerRenderingPass

    // advanceSimulationByOneStep

    // startSimulation

    // stopSimulation

    // pauseSimulation

    // getModuleName

    // adjustView

    void setLastError(const std::string &func, const std::string &msg);

    // resetGraph

    // destroyGraphCurve

    // duplicateGraphCurveToStatic

    // setNavigationMode

    // getNavigationMode

    // setPage

    // getPage

    int registerScriptCallbackFunction(const std::string &funcNameAtPluginName, const std::string &callTips, void (*callBack)(struct SScriptCallBack *cb));

    int registerScriptVariable(const std::string &varName, const char *varValue, int stackID);
    int registerScriptVariable(const std::string &varName, const std::string &varValue, int stackID);
    template<typename T>
    int registerScriptVariable(const std::string &varName, const T &varValue, int stackID)
    {
        return registerScriptVariable(varName, std::to_string(varValue), stackID);
    }

    // registerScriptFuncHook

    void copyPasteObjects(std::vector<int> &shapeHandles, int options);
    std::vector<int> copyPasteObjects(const std::vector<int> &shapeHandles, int options);

    // removeDrawingObject

    int announceSceneContentChange();

    // setInt32Signal

    // getInt32Signal

    // clearInt32Signal

    // clearFloatSignal

    // setStringSignal

    // getStringSignal

    // clearStringSignal

    // getSignalName

    void setObjectProperty(int objectHandle, int prop);

    int getObjectProperty(int objectHandle);

    // setObjectSpecialProperty

    // getObjectSpecialProperty

    // setExplicitHandling

    // getExplicitHandling

    int getLinkDummy(int dummyHandle);

    void setLinkDummy(int dummyHandle, int linkedDummyHandle);

    void setModelProperty(int objectHandle, int modelProperty);

    int getModelProperty(int objectHandle);

    void resetDynamicObject(int objectHandle);

    void setJointMode(int jointHandle, int jointMode);

    int getJointMode(int jointHandle);

    // serialOpen

    // serialClose

    // serialSend

    // serialRead

    // serialCheck

    // auxiliaryConsoleClose

    // auxiliaryConsoleShow

    // auxiliaryConsolePrint

    int getObjectInt32Param(int objectHandle, int parameterID);

    void setObjectInt32Param(int objectHandle, int parameterID, int parameter);

    std::string getObjectStringParam(int objectHandle, int parameterID);

    void setObjectStringParam(int objectHandle, int parameterID, const std::string &parameter);

    // setSimulationPassesPerRenderingPass

    void persistentDataWrite(const std::string &dataName, const std::string &dataValue, int options = 0);

    std::string persistentDataRead(const std::string &dataName);

    bool isHandle(int generalObjectHandle, int generalObjectType);

    // resetVisionSensor

    void setVisionSensorImg(int sensorHandle, const unsigned char *img, int options = 0, std::array<int, 2> pos = {0, 0}, std::array<int, 2> size = {0, 0});

    // ruckigRemove

    int groupShapes(const std::vector<int> &shapeHandles, bool merge = false);

    std::vector<int> ungroupShape(int shapeHandle);

    void quitSimulator();

    void setShapeMaterial(int shapeHandle, int materialIdOrShapeHandle);

    // getTextureId

    // readTexture

    void writeCustomDataBlock(int objectHandle, const char *tagName, const char *data, int dataSize);
    void writeCustomDataBlock(int objectHandle, const std::string &tagName, const std::string &data);

    char * readCustomDataBlock(int objectHandle, const char *tagName, int *dataSize);
    boost::optional<std::string> readCustomDataBlock(int objectHandle, const std::string &tagName);

    char * readCustomDataBlockTags(int objectHandle, int *tagCount);
    std::vector<std::string> readCustomDataBlockTags(int objectHandle);

    int getObjects(int index, int objectType);
    std::vector<int> getObjects(int objectType);

    std::vector<int> getObjectsInTree(int treeBaseHandle, int objectType, int options = 0);

    // getShapeTextureId

    // createCollectionEx

    // addItemToCollection

    // destroyCollection

    // getCollectionObjects

    int getScriptInt32Param(int scriptHandle, int parameterID);

    // setScriptInt32Param

    // getScriptStringParam

    // setScriptStringParam

    // reorientShapeBoundingBox

    // saveImage

    // loadImage

    // getScaledImage

    void callScriptFunctionEx(int scriptHandleOrType, const std::string &functionNameAtScriptName, int stackID);

    // getExtensionString

    int createStack();

    void releaseStack(int stackHandle);

    int copyStack(int stackHandle);

    void pushNullOntoStack(int stackHandle);

    void pushBoolOntoStack(int stackHandle, bool value);

    void pushInt32OntoStack(int stackHandle, int value);

    // pushInt64OntoStack

    void pushStringOntoStack(int stackHandle, const char *value, int stringSize);
    void pushStringOntoStack(int stackHandle, const std::string &value);

    void pushUInt8TableOntoStack(int stackHandle, const unsigned char *values, int valueCnt);
    void pushUInt8TableOntoStack(int stackHandle, const std::vector<unsigned char> &values);

    void pushInt32TableOntoStack(int stackHandle, const int *values, int valueCnt);
    void pushInt32TableOntoStack(int stackHandle, const std::vector<int> &values);

    // pushInt64TableOntoStack

    void pushTableOntoStack(int stackHandle);

    void insertDataIntoStackTable(int stackHandle);

    int getStackSize(int stackHandle);

    int popStackItem(int stackHandle, int count);

    void moveStackItemToTop(int stackHandle, int cIndex);

    int isStackValueNull(int stackHandle);

    int getStackBoolValue(int stackHandle, bool *boolValue);

    int getStackInt32Value(int stackHandle, int *numberValue);

    // getStackInt64Value

    char * getStackStringValue(int stackHandle, int *stringSize);
    int getStackStringValue(int stackHandle, std::string *stringValue);

    int getStackTableInfo(int stackHandle, int infoType);

    int getStackUInt8Table(int stackHandle, unsigned char *array, int count);
    int getStackUInt8Table(int stackHandle, std::vector<unsigned char> *v);

    int getStackInt32Table(int stackHandle, int *array, int count);
    int getStackInt32Table(int stackHandle, std::vector<int> *v);

    // getStackInt64Table

    void unfoldStackTable(int stackHandle);

    void debugStack(int stackHandle, int index = -1);

    int getEngineInt32Param(int paramId, int objectHandle, const void *object);

    bool getEngineBoolParam(int paramId, int objectHandle, const void *object);

    void setEngineInt32Param(int paramId, int objectHandle, const void *object, int val);

    void setEngineBoolParam(int paramId, int objectHandle, const void *object, bool val);

    // insertObjectIntoOctree

    // subtractObjectFromOctree

    // openTextEditor

    // packTable

    // unpackTable

    // setReferencedHandles

    // getReferencedHandles

    void executeScriptString(int scriptHandleOrType, const std::string &stringAtScriptName, int stackID);

    std::vector<std::string> getApiFunc(int scriptHandleOrType, const std::string &apiWord);

    std::string getApiInfo(int scriptHandleOrType, const std::string &apiWord);

    void setModuleInfo(const std::string &moduleName, int infoType, const std::string &stringInfo);
    void setModuleInfo(const std::string &moduleName, int infoType, int intInfo);
    void setModuleInfo(int infoType, const std::string &stringInfo);
    void setModuleInfo(int infoType, int intInfo);

    void getModuleInfo(const std::string &moduleName, int infoType, std::string &stringInfo);
    void getModuleInfo(const std::string &moduleName, int infoType, int &intInfo);
    std::string getModuleInfoStr(const std::string &moduleName, int infoType);
    int getModuleInfoInt(const std::string &moduleName, int infoType);
    void getModuleInfo(int infoType, std::string &stringInfo);
    void getModuleInfo(int infoType, int &intInfo);
    std::string getModuleInfoStr(int infoType);
    int getModuleInfoInt(int infoType);

    bool isDeprecated(const std::string &funcOrConst);

    std::vector<std::string> getPersistentDataTags();

    int eventNotification(const std::string &event);

    void addLog(boost::optional<std::string> pluginName, int verbosityLevel, boost::optional<std::string> logMsg);
    template<typename... Arguments>
    void addLog(int verbosity, const std::string &fmt, Arguments&&... args)
    {
        addLog(pluginNameAndVersion, verbosity, util::sprintf(fmt, std::forward<Arguments>(args)...));
    }

    // isDynamicallyEnabled

    // initScript

    // moduleEntry

    // checkExecAuthorization

    void pushFloatOntoStack(int stackHandle, float value);

    void pushFloatTableOntoStack(int stackHandle, const float *values, int valueCnt);
    void pushFloatTableOntoStack(int stackHandle, const std::vector<float> &values);

    int getStackFloatValue(int stackHandle, float *numberValue);

    int getStackFloatTable(int stackHandle, float *array, int count);
    int getStackFloatTable(int stackHandle, std::vector<float> *v);

    // getVisionSensorDepth

    // setVisionSensorDepth

    // checkVisionSensorEx

    // ruckigPos

    // ruckigVel

    // ruckigStep

    void pushDoubleOntoStack(int stackHandle, double value);

    void pushDoubleTableOntoStack(int stackHandle, const double *values, int valueCnt);
    void pushDoubleTableOntoStack(int stackHandle, const std::vector<double> &values);

    int getStackDoubleValue(int stackHandle, double *numberValue);

    int getStackDoubleTable(int stackHandle, double *array, int count);
    int getStackDoubleTable(int stackHandle, std::vector<double> *v);

    // extLaunchUIThread

    // extCanInitSimThread

    // extSimThreadInit

    // extSimThreadDestroy

    // extPostExitRequest

    // extGetExitRequest

    // extStep

    // _setDynamicSimulationIconCode

    // _setDynamicObjectFlagForVisualization

    // _getObjectListSize

    // _getObjectFromIndex

    // _getObjectID

    // _getObjectType

    // _getObjectChildren

    // _getGeomProxyFromShape

    // _getParentObject

    // _getObject

    // _getIkGroupObject

    // _mpHandleIkGroupObject

    // _isShapeDynamicallyStatic

    // _getTreeDynamicProperty

    // _getDummyLinkType

    // _getJointMode

    // _isJointInHybridOperation

    // _disableDynamicTreeForManipulation

    // _isShapeDynamicallyRespondable

    // _getDynamicCollisionMask

    // _getLastParentForLocalGlobalCollidable

    // _getStartSleeping

    // _getWasPutToSleepOnce

    // _getDynamicsFullRefreshFlag

    // _setDynamicsFullRefreshFlag

    // _clearAdditionalForceAndTorque

    // _getJointType

    // _getGeomWrapFromGeomProxy

    // _getPurePrimitiveType

    // _isGeomWrapGeometric

    // _isGeomWrapConvex

    // _getGeometricCount

    // _getAllGeometrics

    // _makeDynamicAnnouncement

    // _getTimeDiffInMs

    // _doEntitiesCollide

    // _getJointDynCtrlMode

    // floatingViewRemove

    int getShapeViz(int shapeHandle, int index, struct SShapeVizInfo *info);

    void setArrayParam(int parameter, std::array<double, 3> value);

    std::array<double, 3> getArrayParam(int parameter);

    // setFloatSignal

    // getFloatSignal

    void setFloatParam(int parameter, double value);

    double getFloatParam(int parameter);

    double getObjectFloatParam(int objectHandle, int parameterID);

    void setObjectFloatParam(int objectHandle, int parameterID, double parameter);

    // getObjectFloatArrayParam

    // setObjectFloatArrayParam

    double getEngineFloatParam(int paramId, int objectHandle, const void *object);

    void setEngineFloatParam(int paramId, int objectHandle, const void *object, double val);

    // transformImage

    // getOctreeVoxels

    std::vector<double> getPointCloudPoints(int pointCloudHandle);

    void getObjectMatrix(int objectHandle, int relativeToObjectHandle, double *matrix);
    std::array<double, 12> getObjectMatrix(int objectHandle, int relativeToObjectHandle);

    void setObjectMatrix(int objectHandle, int relativeToObjectHandle, const double *matrix);
    void setObjectMatrix(int objectHandle, int relativeToObjectHandle, const std::array<double, 12> &matrix);

    void getObjectPose(int objectHandle, int relativeToObjectHandle, double *pose);
    std::array<double, 7> getObjectPose(int objectHandle, int relativeToObjectHandle);

    void setObjectPose(int objectHandle, int relativeToObjectHandle, const double *pose);
    void setObjectPose(int objectHandle, int relativeToObjectHandle, std::array<double, 7> pose);

    void getObjectPosition(int objectHandle, int relativeToObjectHandle, double *position);
    std::array<double, 3> getObjectPosition(int objectHandle, int relativeToObjectHandle);

    void setObjectPosition(int objectHandle, int relativeToObjectHandle, const double *position);
    void setObjectPosition(int objectHandle, int relativeToObjectHandle, const std::array<double, 3> &position);

    void getObjectOrientation(int objectHandle, int relativeToObjectHandle, double *eulerAngles);
    std::array<double, 3> getObjectOrientation(int objectHandle, int relativeToObjectHandle);

    void setObjectOrientation(int objectHandle, int relativeToObjectHandle, const double *eulerAngles);
    void setObjectOrientation(int objectHandle, int relativeToObjectHandle, const std::array<double, 3> &eulerAngles);

    double getJointPosition(int objectHandle);

    void setJointPosition(int objectHandle, double position);

    void setJointTargetPosition(int objectHandle, double targetPosition);

    double getJointTargetPosition(int objectHandle);

    double getJointTargetForce(int jointHandle);

    void setJointTargetForce(int objectHandle, double forceOrTorque, bool signedValue);

    void getObjectChildPose(int objectHandle, double *pose);
    std::array<double, 7> getObjectChildPose(int objectHandle);

    void setObjectChildPose(int objectHandle, const double *pose);
    void setObjectChildPose(int objectHandle, std::array<double, 7> pose);

    void getJointInterval(int objectHandle, bool *cyclic, double *interval);
    std::array<double, 2> getJointInterval(int objectHandle, bool *cyclic);

    void setJointInterval(int objectHandle, bool cyclic, const double *interval);
    void setJointInterval(int objectHandle, bool cyclic, std::array<double, 2> interval);

    // buildIdentityMatrix

    // buildMatrix

    // buildPose

    // getEulerAnglesFromMatrix

    // invertMatrix

    std::array<double, 12> multiplyMatrices(std::array<double, 12> matrixIn1, std::array<double, 12> matrixIn2);

    std::array<double, 7> multiplyPoses(std::array<double, 7> poseIn1, std::array<double, 7> poseIn2);

    std::array<double, 7> invertPose(std::array<double, 7> pose);

    std::array<double, 7> interpolatePoses(std::array<double, 7> poseIn1, std::array<double, 7> poseIn2, double interpolFactor);

    std::array<double, 12> poseToMatrix(std::array<double, 7> poseIn);

    std::array<double, 7> matrixToPose(std::array<double, 12> matrixIn);

    std::array<double, 12> interpolateMatrices(std::array<double, 12> matrixIn1, std::array<double, 12> matrixIn2, double interpolFactor);

    std::array<double, 3> transformVector(std::array<double, 12> matrix, std::array<double, 3> vect);

    double getSimulationTime();

    double getSystemTime();

    // handleProximitySensor

    // readProximitySensor

    // handleDynamics

    // checkProximitySensor

    // checkProximitySensorEx

    // checkProximitySensorEx2

    // checkCollisionEx

    // checkDistance

    // setSimulationTimeStep

    double getSimulationTimeStep();

    // adjustRealTimeTimer

    // floatingViewAdd

    // handleGraph

    // setGraphStreamTransformation

    // addGraphCurve

    // setGraphStreamValue

    // setJointTargetVelocity

    // getJointTargetVelocity

    // scaleObjects

    // addDrawingObject

    // addGraphStream

    // addDrawingObjectItem

    // getObjectSizeFactor

    // readForceSensor

    // setLightParameters

    int getLightParameters(int objectHandle);
    int getLightParameters(int objectHandle, std::array<double, 3> &diffuse);
    int getLightParameters(int objectHandle, std::array<double, 3> &diffuse, std::array<double, 3> &specular);

    // getVelocity

    std::pair<std::array<double, 3>, std::array<double, 3>> getObjectVelocity(int objectHandle);

    // getJointVelocity

    // addForceAndTorque

    // addForce

    // setObjectColor

    // getObjectColor

    // setShapeColor

    boost::optional<std::array<float, 3>> getShapeColor(int shapeHandle, boost::optional<std::string> colorName, int colorComponent);
    boost::optional<std::array<float, 3>> getShapeColor(int shapeHandle, int colorComponent);

    // getContactInfo

    // auxiliaryConsoleOpen

    int importShape(const char *pathAndFilename, int options, double scalingFactor);
    int importShape(const std::string &pathAndFilename, int options, double scalingFactor);

    // importMesh

    // exportMesh

    int createMeshShape(int options, double shadingAngle, const double *vertices, int verticesSize, const int *indices, int indicesSize);
    int createMeshShape(int options, double shadingAngle, const std::vector<double> &vertices, const std::vector<int> &indices);

    int createPrimitiveShape(int primitiveType, const double *sizes, int options);
    int createPrimitiveShape(int primitiveType, std::array<double, 3> sizes, int options);

    int createHeightfieldShape(int options, double shadingAngle, int xPointCount, int yPointCount, double xSize, const double *heights);
    int createHeightfieldShape(int options, double shadingAngle, int xPointCount, int yPointCount, double xSize, const std::vector<double> &heights);

    void getShapeMesh(int shapeHandle, double **vertices, int *verticesSize, int **indices, int *indicesSize, double **normals = nullptr);
    void getShapeMesh(int shapeHandle, std::vector<double> vertices, std::vector<int> indices, boost::optional<std::vector<double>> normals = {});

    int createJoint(int jointType, int jointMode, int options, const double *sizes);
    int createJoint(int jointType, int jointMode, int options = 0, boost::optional<std::array<double, 2>> sizes = {});

    int createDummy(double size = 0.01);

    int createForceSensor(int options, const int *intParams, const double *floatParams);
    int createForceSensor(int options, int type, int valueCount, int thresholdCount, double size, double forceThreshold, double torqueThreshold);

    int createVisionSensor(int options, const int *intParams, const double *floatParams);
    int createVisionSensor(int options, std::array<int, 2> resolution, double clipNear, double clipFar, double viewAngleOrOrthoSize, double xSize, std::array<float, 3> nullPixelColor);

    int createProximitySensor(int sensorType, int options, const int *intParams, const double *floatParams);
    int createProximitySensor(int sensorType, int options, int faceCount, int faceCountFar, int subdivisions, int subdivisionsFar, int randDetSampleCountPerReading, int randDetIndividualRayDetCntForTrig, double offset, double range, std::array<double, 2> size, std::array<double, 2> sizeFar, double insideGap, double radius, double radiusFar, double angle, double thresholdAngle, double smallestDetDist, double sensPointSize);

    std::pair<std::array<double, 3>, double> getRotationAxis(std::array<double, 12> matrixStart, std::array<double, 12> matrixGoal);
    std::pair<std::array<double, 3>, double> getRotationAxis(std::array<double, 7> matrixStart, std::array<double, 7> matrixGoal);

    // rotateAroundAxis

    // getJointForce

    // cameraFitToView

    // handleVisionSensor

    // readVisionSensor

    // checkVisionSensor

    unsigned char * getVisionSensorImg(int sensorHandle, int options, double rgbaCutOff, std::array<int, 2> pos = {0, 0}, std::array<int, 2> size = {0, 0}, std::array<int, 2> *resolution = nullptr);

    std::array<int, 2> getVisionSensorRes(int visionSensorHandle);

    std::array<double, 4> getObjectQuaternion(int objectHandle, int relativeToObjectHandle);

    void setObjectQuaternion(int objectHandle, int relativeToObjectHandle, const std::array<double, 4> &quaternion);

    // convexDecompose

    void writeTexture(int textureId, int options, const char *data, int posX = 0, int posY = 0, int sizeX = 0, int sizeY = 0, double interpol = 0.0);

    // createTexture

    // getShapeGeomInfo

    // scaleObject

    // setShapeTexture

    // getQHull

    // getDecimatedMesh

    // computeMassAndInertia

    // createOctree

    int createPointCloud(double maxVoxelSize, int maxPtCntPerVoxel, int options, double pointSize);

    // setPointCloudOptions

    // getPointCloudOptions

    // insertVoxelsIntoOctree

    // removeVoxelsFromOctree

    // insertPointsIntoPointCloud

    // removePointsFromPointCloud

    // intersectPointsWithPointCloud

    int insertObjectIntoPointCloud(int pointCloudHandle, int objectHandle, int options, double gridSize, boost::optional<std::array<unsigned char, 3>> color = {}, boost::optional<float> duplicateTolerance = {});

    // subtractObjectFromPointCloud

    // checkOctreePointOccupancy

    // applyTexture

    // setJointDependency

    // getJointDependency

    double getShapeMass(int shapeHandle);

    void setShapeMass(int shapeHandle, double mass);

    void getShapeInertia(int shapeHandle, double *inertiaMatrix, double *transformationMatrix);
    std::pair<std::array<double, 9>, std::array<double, 12>> getShapeInertia(int shapeHandle);

    void setShapeInertia(int shapeHandle, const double *inertiaMatrix, const double *transformationMatrix);
    void setShapeInertia(int shapeHandle, std::array<double, 9> inertiaMatrix, std::array<double, 12> transformationMatrix);

    // generateShapeFromPath

    // getClosestPosOnPath

    // extCallScriptFunction

    // getObjectLocalTransformation

    // setObjectLocalTransformation

    // dynReportObjectCumulativeTransformation

    // setObjectCumulativeTransformation

    // getObjectCumulativeTransformation

    // setJointVelocity

    // setJointPosition

    // getJointPosition

    // setDynamicMotorPositionControlTargetPosition

    // getInitialDynamicVelocity

    // setInitialDynamicVelocity

    // getInitialDynamicAngVelocity

    // setInitialDynamicAngVelocity

    // setShapeDynamicVelocity

    // getAdditionalForceAndTorque

    // getJointPositionInterval

    // getDynamicMotorTargetPosition

    // getDynamicMotorTargetVelocity

    // getDynamicMotorMaxForce

    // getDynamicMotorUpperLimitVelocity

    // setDynamicMotorReflectedPositionFromDynamicEngine

    // setJointSphericalTransformation

    // addForceSensorCumulativeForcesAndTorques

    // addJointCumulativeForcesOrTorques

    // getMass

    // getLocalInertiaInfo

    // getPurePrimitiveSizes

    // getVerticesLocalFrame

    // getHeightfieldData

    // getCumulativeMeshes

    // getGravity

    // getDistanceBetweenEntitiesIfSmaller

    // handleJointControl

    // handleCustomContact

    // getPureHollowScaling

    // dynCallback

} // namespace sim

#endif // SIMPLUSPLUS_LIB_H_INCLUDED
