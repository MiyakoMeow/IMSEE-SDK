#ifndef _IMRDATA_H_
#define _IMRDATA_H_
struct ImrPose
{
    double time;
    float _position[3];
    float _rotation[4];
    float _oula[3];
};
struct ImrModulePose
{
    ImrPose _pose;
    double _cov[36];
};
struct ImrImage
{
    double _time;
    int _width;
    int _height;
    unsigned char *_image;
};
struct MrPoint
{
    float _x;
    float _y;
    float _z;
    float _flag;
};
struct CameraCalibrationParameter
{
    int _width;
    int _height;
    int _channel;

    double _Kl[9];
    double _Kr[9];
    double _Dl[4];
    double _Dr[4];
    double _Pl[12];
    double _Pr[12];
    double _Rl[9];
    double _Rr[9];
    double _TSCl[16];
    double _TSCr[16];
    double _focal_length_l[2];
    double _focal_length_r[2];
    double _principal_point_l[2];
    double _principal_point_r[2];
    double _baseline;
    double _AMax;
    double _GMax;
    double _SigmaGC;
    double _SigmaAC;
    double _SigmaBg;
    double _SigmaBa;
    double _SigmaGwC;
    double _SigmaAwC;
    double _Acc[12];
    double _Gyr[12];
};
#endif