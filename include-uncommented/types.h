#ifndef _TYPES_H_
#define _TYPES_H_
#include <functional>
#include <iostream>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <stdint.h>
#include <type_traits>
#include <vector>

namespace indem
{
#define SLAM_LOOSE_ID 1
const static int32_t DATA_LEFT = 0x0001;
const static int32_t DATA_RIGHT = 0x0002;

const static int32_t MSG_IMU = 0x0004;
const static int32_t MSG_IMAGE = 0x0008;

const static int32_t SRC_HMD = 0x000100;
const static int32_t SRC_USB = 0x002000;

enum RESOLUTION
{
    RES_640X400,
    RES_1280X800,
    RES_DEFAULT = RES_640X400
};

enum DepthCalMode
{
    HIGH_SPEED,
    HIGH_ACCURACY
};

typedef struct
{
    double time;
    float acc[3];
    float gyr[3];
    bool COME_FLAG;
    bool flag;
} IMU_HANDLE;

typedef struct
{
    double time;
    float acc[3];
    float gyr[3];
    bool COME_FLAG;
} IMU_HEADSET;

typedef struct
{
    double time;
    float p[3];
    float q[4];
    float vel[3];
    bool COME_FLAG;
    bool flag;
} SLAM_LOOSE_OUT;

typedef struct
{
    double time;
    float p[3];
    float q[4];
    float vel[3];
    float rms[9];
    bool COME_FLAG;
    bool flag;
    bool LI_flag;
} SLAM_PURE_OUT;

typedef struct
{
    SLAM_PURE_OUT salm_pure_out;
    SLAM_LOOSE_OUT slam_loose_out;
} HEADSET_OUT;

typedef struct
{
    float time;
    float p[3];
    float q[4];
    bool COME_FLAG;
    bool flag;
    int pattern;
} HANDLE_LOOSE_OUT;

typedef struct
{
    float time;
    float p[3];
    float q[4];
    bool COME_FLAG = false;
    bool flag = false;
    int mPointNum = -1;
    double mAveError = 1.7976931348623158e+308;
    int pattern;
} HANDLE_PURE_OUT;

typedef struct
{
    float time;
    float p[3];
    float q[4];
    bool COME_FLAG;
    bool flag;
} HANDLE_LOOSE_PREDICT;

struct ImuData
{
    double timestamp;
    float accel[3];
    float gyro[3];
    ImuData()
    {
    }
    ImuData(double time, float accel_x, float accel_y, float accel_z, float gyro_x, float gyro_y, float gyro_z)
    {
        timestamp = time;
        accel[0] = accel_x;
        accel[1] = accel_y;
        accel[2] = accel_z;
        gyro[0] = gyro_x;
        gyro[1] = gyro_y;
        gyro[2] = gyro_z;
    }
};

struct CameraParameter
{
    double _TSC[16];
    int _width;
    int _height;
    double _focal_length[2];
    double _principal_point[2];
    double _R[9];
    double _P[12];
    double _K[9];
    double _D[4];
    CameraParameter resize(double ratio) const
    {
        CameraParameter parameter;
        parameter = *this;
        parameter._width = static_cast<int>(this->_width * ratio);
        parameter._height = static_cast<int>(this->_height * ratio);
        for (int m = 0; m < 2; m++)
        {
            parameter._focal_length[m] = ratio * this->_focal_length[m];
        }
        for (int n = 0; n < 2; n++)
        {
            parameter._principal_point[n] = ratio * this->_principal_point[n];
        }
        for (int i = 0; i < 12; i++)
        {
            if (i != 10)
                parameter._P[i] = ratio * this->_P[i];
        }
        for (int j = 0; j < 9; j++)
        {
            if (j != 8)
                parameter._K[j] = ratio * this->_K[j];
        }
        return parameter;
    }
    void printInfo()
    {
        std::cout << "_width: " << _width << ", height: " << _height << std::endl;
        for (int i = 0; i < 2; i++)
        {
            std::cout << "_focal_length[" << i << "]: " << _focal_length[i] << std::endl;
            std::cout << "_principal_point[" << i << "]: " << _principal_point[i] << std::endl;
        }
        std::cout << "_focal_length: { ";
        for (int i = 0; i < 2; i++)
        {
            std::cout << _focal_length[i] << " ";
        }
        std::cout << "} " << std::endl;
        std::cout << "_principal_point: { ";
        for (int i = 0; i < 2; i++)
        {
            std::cout << _principal_point[i] << " ";
        }
        std::cout << "} " << std::endl;
        std::cout << "_TSC: { ";
        for (int i = 0; i < 16; i++)
        {
            std::cout << _TSC[i] << " ";
        }
        std::cout << "} " << std::endl;
        std::cout << "_R: { ";
        for (int i = 0; i < 9; i++)
        {
            std::cout << _R[i] << " ";
        }
        std::cout << "} " << std::endl;
        std::cout << "_P: { ";
        for (int i = 0; i < 12; i++)
        {
            std::cout << _P[i] << " ";
        }
        std::cout << "} " << std::endl;
        std::cout << "_K: { ";
        for (int i = 0; i < 9; i++)
        {
            std::cout << _K[i] << " ";
        }
        std::cout << "} " << std::endl;
        std::cout << "_D: { ";
        for (int i = 0; i < 4; i++)
        {
            std::cout << _D[i] << " ";
        }
        std::cout << "} " << std::endl;
    }
};
struct IMUParameter
{
    double _a_max;
    double _g_max;
    double _sigma_g_c;
    double _sigma_a_c;
    double _sigma_bg;
    double _sigma_ba;
    double _sigma_gw_c;
    double _sigma_aw_c;
    double _tau;
    double _g;
    double _a0[4];
    double _T_BS[16];
    double _Acc[12];
    double _Gyr[12];
    void printInfo()
    {
        std::cout << "_a_max: " << _a_max << std::endl
                  << "_g_max: " << _g_max << std::endl
                  << "_sigma_g_c: " << _sigma_g_c << std::endl
                  << "_sigma_a_c: " << _sigma_a_c << std::endl
                  << "_sigma_bg: " << _sigma_bg << std::endl
                  << "_sigma_ba: " << _sigma_ba << std::endl
                  << "_sigma_gw_c: " << _sigma_gw_c << std::endl
                  << "_sigma_aw_c: " << _sigma_aw_c << std::endl
                  << "_tau: " << _tau << std::endl
                  << "_g: " << _g << std::endl;
        std::cout << "_a0: { ";
        for (int i = 0; i < 4; i++)
        {
            std::cout << _a0[i] << " ";
        }
        std::cout << "} " << std::endl;
        std::cout << "_T_BS: { ";
        for (int i = 0; i < 16; i++)
        {
            std::cout << _T_BS[i] << " ";
        }
        std::cout << "} " << std::endl;
        std::cout << "_Acc: { ";
        for (int i = 0; i < 12; i++)
        {
            std::cout << _Acc[i] << " ";
        }
        std::cout << "} " << std::endl;
        std::cout << "_Gyr: { ";
        for (int i = 0; i < 12; i++)
        {
            std::cout << _Gyr[i] << " ";
        }
        std::cout << "} " << std::endl;
    }
};

struct ModuleInfo
{
    char _id[32];
    char _designer[32];
    char _fireware_version[32];
    char _hardware_version[32];
    char _lens[32];
    char _imu[32];
    char _viewing_angle[32];
    char _baseline[32];
    void printInfo()
    {
        std::cout << "id: ";
        for (int i = 0; i < 32; i++)
        {
            std::cout << _id[i];
        }
        std::cout << std::endl << "designer: ";
        for (int i = 0; i < 32; i++)
        {
            std::cout << _designer[i];
        }
        std::cout << std::endl << "fireware_version: ";
        for (int i = 0; i < 32; i++)
        {
            std::cout << _fireware_version[i];
        }
        std::cout << std::endl << "hardware_version: ";
        for (int i = 0; i < 32; i++)
        {
            std::cout << _hardware_version[i];
        }
        std::cout << std::endl << "lens: ";
        for (int i = 0; i < 32; i++)
        {
            std::cout << _lens[i];
        }
        std::cout << std::endl << "imu: ";
        for (int i = 0; i < 32; i++)
        {
            std::cout << _imu[i];
        }
        std::cout << std::endl << "viewing_angle: ";
        for (int i = 0; i < 32; i++)
        {
            std::cout << _viewing_angle[i];
        }
        std::cout << std::endl << "_baseline: ";
        for (int i = 0; i < 32; i++)
        {
            std::cout << _baseline[i];
        }
        std::cout << std::endl;
    }
};

struct SlamParameter
{
    int _numKeyframes;
    int _numImuFrames;
    int _ceres_minIterations;
    int _ceres_maxIterations;
    double _ceres_timeLimit;
    int detection_threshold;
    int detection_octaves;
    int detection_maxNoKeypoints;
    bool displayImages;
};
struct MoudleAllParam
{
    std::map<RESOLUTION, CameraParameter> _left_camera;
    std::map<RESOLUTION, CameraParameter> _right_camera;
    int _camera_channel = 1;
    double _baseline = 0.12;
    IMUParameter _imu;
    ModuleInfo _device;
    SlamParameter _slam;
};

struct ModuleParameters
{
    CameraParameter _camera[2];
    IMUParameter _imu;
    ModuleInfo _device;
    SlamParameter _slam;
};
enum CLASS_NAME
{
    BG = 0,
    PERSON,
    PET_CAT,
    PET_DOG,
    SOFA,
    TABLE,
    BED,
    EXCREMENT,
    WIRE,
    KEY,
};

struct BoxInfo
{
    cv::Rect box;
    float score;
    float cx;
    float cy;
    float width;
    float height;
    CLASS_NAME class_name;
    int index;
};

struct InstanceInfo
{
    CLASS_NAME class_name;
    int instance_id;
    float location[3];
    float location_cam[3];
    float time;
    float scale;
    cv::Rect box;
    int _count;
    float front_face_points[3][3];
    float rear_face_points[3][3];
    bool visible;
    int class_count = 0;
    bool valid;
    float cx;
    float cy;
    float depth;
};

struct DetectorInfo
{
    double timestamp;
    cv::Mat img;
    std::vector<BoxInfo> finalBoxInfo;
};

#define MODULE_SIZE sizeof(ModuleParameters)

template <int VERSION_NUM> struct ModuleParamInFlash
{
};
template <> struct ModuleParamInFlash<0>
{
    ModuleParameters _parent;
};
template <> struct ModuleParamInFlash<1>
{
    ModuleParameters _parent;
    int _imgFrequency;
    int _imgResolution;
    int _imuFrequency;
};
#define FLASH_PARAMETER_SIZE_1 sizeof(ModuleParamInFlash<1>)
enum ModuleType
{
    NORMAL_MODULE,
    AR_MODULE,
    MR_MODULE,
};
template <> struct ModuleParamInFlash<2>
{
    ModuleParamInFlash<1> _parent;
    ModuleType _type;
};
#define FLASH_PARAMETER_SIZE_2 sizeof(ModuleParamInFlash<2>)
static_assert(std::is_pod<ModuleParamInFlash<1>>::value == true, "ModuleParamInFlash<1> is not POD");
static_assert(std::is_pod<ModuleParamInFlash<2>>::value == true, "ModuleParamInFlash<2> is not POD");
static_assert(std::is_pod<ModuleParamInFlash<3>>::value == true, "ModuleParamInFlash<3> is not POD");
static_assert(std::is_pod<ModuleParamInFlash<4>>::value == true, "ModuleParamInFlash<4> is not POD");
static_assert(std::is_pod<ModuleParamInFlash<5>>::value == true, "ModuleParamInFlash<5> is not POD");

#define FLASH_MAX_SIZE FLASH_PARAMETER_SIZE_2

};
#endif
