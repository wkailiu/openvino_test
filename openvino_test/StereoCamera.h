//
//  @ Project : HumanPose3D
//  @ File Name : StereoCamera.h
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#if !defined(_STEREOCAMERA_H)
#define _STEREOCAMERA_H

#include "CameraParameter.h"
#include "MonocularCamera.h"

#include <opencv2\opencv.hpp>
#include "ThirdPartyUtil.h"

class StereoCamera : public CameraParameter {
public:
    StereoCamera(std::string strSettingsFile);
    void readFrame();
	void undistort();
    void release();
	void imshow();
	MonocularCamera* getLeftCameraPtr();
	MonocularCamera* getRightCameraPtr();

private:
    cv::VideoCapture cap;
    MonocularCamera* left;
    MonocularCamera* right;
	cv::Mat frame;
};

#endif  //_STEREOCAMERA_H
