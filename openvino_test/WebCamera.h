//
//  @ Project : HumanPose3D
//  @ File Name : WebCamera.h
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#if !defined(_WEBCAMERA_H)
#define _WEBCAMERA_H

#include "CameraParameter.h"
#include "BaseCamera.h"

class WebCamera : public BaseCamera {
public:
    WebCamera(std::string ip, std::string title);
    void readFrame();
    void release();
private:
    std::string ip;
};

#endif  //_WEBCAMERA_H
