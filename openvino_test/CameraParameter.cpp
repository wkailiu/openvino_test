//
//  @ Project : HumanPose3D
//  @ File Name : CameraParameter.cpp
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#include "CameraParameter.h"

CameraParameter::CameraParameter() {
	T << 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;
}


void CameraParameter::setCameraT(Eigen::Matrix4f t) {
	T = t;
}


void CameraParameter::setCameraR(Eigen::Matrix3f r) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			T(i, j) = r(i, j);
		}
	}
}

void CameraParameter::setCamera_t(float x,float y,float z) {
	T(0, 3) = x;
	T(1, 3) = y;
	T(2, 3) = z;
}

Eigen::Vector3f CameraParameter::pixelToCamera(float px, float py) {
	Eigen::Vector3f new_p(px, py, 1.0f);
	Eigen::Vector3f camera = K.inverse()*new_p;
	//转化为单位向量
	camera /= sqrt(camera.squaredNorm());
	return camera;
}

Eigen::Vector3f CameraParameter::cameraToWorld(Eigen::Vector3f pc) {
	Eigen::Vector4f c;
	c << pc, 1.0f;
	//Eigen::Vector4f world = T.inverse()*c;
	Eigen::Vector4f world = T*c;
	return world.head(3);
}

Eigen::Vector3f CameraParameter::pixelToWorld(float px, float py) {
	return cameraToWorld(pixelToCamera(px, py));
}

Eigen::Vector3f CameraParameter::getCenter() {
	Eigen::Vector3f p(0, 0, 0);
	return cameraToWorld(p);
}