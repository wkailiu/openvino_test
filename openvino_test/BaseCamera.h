//
//  @ Project : HumanPose3D
//  @ File Name : BaseCamera.h
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#if !defined(_BASECAMERA_H)
#define _BASECAMERA_H

#include <iostream>
#include <opencv2\opencv.hpp>
#include "CameraParameter.h"

class BaseCamera : public CameraParameter {
public:
    cv::Mat frame;
    int frameId;
    BaseCamera(std::string title);
    void imshow();
	void imwrite(std::string path);
	void undistort();
	void setCameraK(Eigen::Matrix3f k);
	void setDistortion(cv::Mat distortion);
private:
	std::string title;
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
};

#endif  //_BASECAMERA_H
