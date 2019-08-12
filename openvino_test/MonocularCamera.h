//
//  @ Project : HumanPose3D
//  @ File Name : MonocularCamera.h
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#if !defined(_MONOCULARCAMERA_H)
#define _MONOCULARCAMERA_H

#include <opencv2\opencv.hpp>

#include "CameraParameter.h"
#include "BaseCamera.h"

class MonocularCamera : public BaseCamera {
public:
    MonocularCamera(std::string title, int openId);
    void readFrame();
    void release();
private:
    cv::VideoCapture cap;
};

#endif  //_MONOCULARCAMERA_H
