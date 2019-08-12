//
//  @ Project : HumanPose3D
//  @ File Name : BaseCamera.cpp
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#include "BaseCamera.h"

BaseCamera::BaseCamera(std::string t):title(t) {
	frameId = 0;
}

void BaseCamera::setCameraK(Eigen::Matrix3f k) {
	K = k;
	cameraMatrix = (cv::Mat_<float>(3, 3) << k(0,0), 0, k(0,2), 0, k(1,1), k(1,2), 0, 0, 1);
}

void BaseCamera::setDistortion(cv::Mat distortion) {
	distCoeffs = distortion;
}

void BaseCamera::imshow() {
	cv::imshow(title, frame);
}

void BaseCamera::imwrite(std::string path) {
	cv::imwrite(path + title + "_" + std::to_string(frameId) + ".png", frame);
}

void BaseCamera::undistort() {
	cv::Mat undistortFrame;
	cv::undistort(frame, undistortFrame, cameraMatrix, distCoeffs);
	frame = undistortFrame;
}