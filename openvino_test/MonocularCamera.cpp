//
//  @ Project : HumanPose3D
//  @ File Name : MonocularCamera.cpp
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#include "MonocularCamera.h"

MonocularCamera::MonocularCamera(std::string title, int openId): BaseCamera(title){
	if (openId >= 0) {
		if (cap.open(openId)) {
			std::cout << "open " << title << " sucessfully!" << std::endl;
		}
		else {
			std::cout << "open " << title << " failed!" << std::endl;
		}
	}
}

void MonocularCamera::readFrame() {
	cap>>frame;
	frameId++;
}

void MonocularCamera::release() {
	cap.release();
}

