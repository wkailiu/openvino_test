//
//  @ Project : HumanPose3D
//  @ File Name : StereoCamera.cpp
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#include "StereoCamera.h"

StereoCamera::StereoCamera(std::string strSettingsFile) {
	cv::FileStorage fsSettings(strSettingsFile.c_str(), cv::FileStorage::READ);
	if (!fsSettings.isOpened()) {
		std::cerr << "Failed to open settings file at: " << strSettingsFile << std::endl;
		exit(-1);
	}
	
	std::string title = fsSettings["title"];
	int width = fsSettings["Stereo.width"];
	int height = fsSettings["Stereo.height"];
	int openId = fsSettings["Stereo.openId"];
	
	if (cap.open(openId)) {
		std::cout << "open " << title << " sucessfully!" << std::endl;
	}
	else {
		std::cout << "open " << title << " failed!" << std::endl;
	}
	left = new MonocularCamera(title + "_left", -1);
	right = new MonocularCamera(title + "_right", -1);

	cv::Mat left_K, left_T, right_K, right_T;
	cv::Mat left_distortion, right_distortion;
	fsSettings["Stereo.LEFT.K"] >> left_K;
	fsSettings["Stereo.LEFT.T"] >> left_T;
	fsSettings["Stereo.LEFT.Distortion"] >> left_distortion;
	
	fsSettings["Stereo.RIGHT.K"] >> right_K;
	fsSettings["Stereo.RIGHT.T"] >> right_T;
	fsSettings["Stereo.RIGHT.Distortion"] >> right_distortion;

	left->setCameraK(ThirdPartyUtil::matToMatrix(left_K));
	left->setCameraT(ThirdPartyUtil::matToMatrix(left_T));
	left->setDistortion(left_distortion);

	right->setCameraK(ThirdPartyUtil::matToMatrix(right_K));
	right->setCameraT(ThirdPartyUtil::matToMatrix(right_T));
	right->setDistortion(right_distortion);
	
	cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
}

void StereoCamera::readFrame() {
	cap >> frame;
	left->frame = frame(cv::Rect(0, 0, frame.cols / 2, frame.rows));
	right->frame = frame(cv::Rect(frame.cols / 2, 0, frame.cols / 2, frame.rows));
	left->frameId++;
	right->frameId++;
}

void StereoCamera::undistort() {
	left->undistort();
	right->undistort();
}
void StereoCamera::imshow() {
	left->imshow();
	right->imshow();
}
void StereoCamera::release() {
	cap.release();
	delete left;
	delete right;
}

MonocularCamera* StereoCamera::getLeftCameraPtr() {
	return left;
}

MonocularCamera* StereoCamera::getRightCameraPtr() {
	return right;
}

