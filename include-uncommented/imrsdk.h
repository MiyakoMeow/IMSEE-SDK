#ifndef _IMRSDK_H_
#define _IMRSDK_H_
#include "imrdata.h"
#include "types.h"

#ifdef WIN32
#ifdef indem_EXPORTS
#define INDEM_DLL_EXPORT __declspec(dllexport)
#else
#define INDEM_DLL_EXPORT __declspec(dllimport)
#endif
#define CALL_METHOD __cdecl
#else
#define CALL_METHOD
#define INDEM_DLL_EXPORT
#endif
#define MRSDK_VERSION 6
#define PLG_NOT_EXIST 1
struct MrDetectionData; using DataCallback = std::function<void(int, void *pData, void *pParam)>; using ModuleIMUCallback = std::function<void(indem::ImuData imu)>; using ModuleImageCallback = std::function<void(double time, unsigned char *pLeft, unsigned char *pRight, int width, using ImgCallback = std::function<void(double time, cv::Mat left, cv::Mat right)>; using RectifiedImgCallback = std::function<void(double time, cv::Mat left, cv::Mat right)>; using DisparityCallback = std::function<void(double time, cv::Mat disparity)>; using DepthCallback = std::function<void(double time, cv::Mat depth)>; using PointCloudCallback = std::function<void(double time, cv::Mat points)>; using DetectorImgCallback = std::function<void(indem::DetectorInfo info)>; using HotplugCallback = std::function<void(int iType, void *pParam)>; typedef void(CALL_METHOD *PluginCallback)(int ret, void *pResult, void *pParam); namespace indem { enum IMG_RESOLUTION { IMG_640, IMG_1280, IMG_DEFAULT, }; struct MRCONFIG { char capturePath[128] = {'\0'}; char loadPath[128] = {'\0'}; bool bSlam = false; char slamPath[128] = {'\0'}; int imuFrequency = 1000; int imgFrequency = 50; int poseFrequency = 0; IMG_RESOLUTION imgResolution = IMG_DEFAULT; MRCONFIG(int){}; }; enum MRCapbility { GPU_NVidia, }; INDEM_DLL_EXPORT void RegistDisconnectCallback(HotplugCallback cb, void *param); class CIMRSDKImpl; class INDEM_DLL_EXPORT CIMRSDK { public: CIMRSDK(); ~CIMRSDK(); bool Init(MRCONFIG config = {0}); void Release(); int GetCapbility(MRCapbility cap); ModuleInfo GetModuleInfo(); MoudleAllParam GetModuleParams(); void RegistModulePoseCallback(DataCallback cb, void *param); void RegistModuleCameraCallback(ModuleImageCallback cb, void *param); void RegistImgCallback(ImgCallback cb); void RegistRectifiedImgCallback(RectifiedImgCallback cb); void RegistDisparityCallback(DisparityCallback cb); void RegistDepthCallback(DepthCallback cb); void RegistPointCloudCallback(PointCloudCallback cb); void RegistDetectorCallback(DetectorImgCallback cb); void RegistModuleIMUCallback(ModuleIMUCallback cb); void BeforeSlamInit(void *inParam); bool ReInitSlam(double t, float *pos, float *quart, float *speed, float *bias); bool ImportMap(const char *fullpath); bool ExportMap(const char *fullpath); int InvokeSlamMethod(const char *methodName, void *inParam, void *outParam); void BeforePluginInit(const char *pluginName, void *param); int LoadPlugin(const char *pluginName); int AddPluginCallback(const char *pluginName, const char *callbackName, PluginCallback cb, void *param); int InvokePluginMethod(const char *pluginName, const char *methodName, void *inParam, void *outParam); void ReleasePlugin(const char *pluginName); bool EnableRectifyProcessor(); bool EnableDisparityProcessor(); bool EnableDepthProcessor(); bool EnablePointProcessor(); bool EnableDetectorProcessor(); bool DisableAllProcessors(); bool EnableLRConsistencyCheck(); bool DisableLRConsistencyCheck(); bool SetDepthCalMode(indem::DepthCalMode mode); static void ListPluginsInfo(int *pluginNum, char **pluginsName); static void ListPluginInfo(const char *pluginsName, int *major, int *minor, char *developer); private: CIMRSDKImpl *m_pImpl; }; };
#endif