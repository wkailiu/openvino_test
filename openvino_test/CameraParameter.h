//
//  @ Project : HumanPose3D
//  @ File Name : CameraParameter.h
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#if !defined(_CAMERAPARAMETER_H)
#define _CAMERAPARAMETER_H

#include <Eigen\Dense>

class CameraParameter {
public:
    void setCameraT(Eigen::Matrix4f t);
	void setCameraR(Eigen::Matrix3f r);
	void setCamera_t(float x, float y, float z);
    Eigen::Vector3f pixelToCamera(float px, float py);
    Eigen::Vector3f cameraToWorld(Eigen::Vector3f pc);
    Eigen::Vector3f pixelToWorld(float px, float py);
	Eigen::Vector3f getCenter();
	CameraParameter();
	Eigen::Matrix3f getK() { return K; }
private:
    Eigen::Matrix4f T;
protected:
	Eigen::Matrix3f K;
};

#endif  //_CAMERAPARAMETER_H
