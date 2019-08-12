//
//  @ Project : HumanPose3D
//  @ File Name : CameraManager.h
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#if !defined(_CAMERAMANAGER_H)
#define _CAMERAMANAGER_H
#include <ctime>
#include "thirdPartyUtil.h"
#include "MonocularCamera.h"
#include "StereoCamera.h"
#include "HumanLib.h"

class CameraManager {
public:
    void addMonocularCamera(MonocularCamera* camera);
    void addStereoCamera(StereoCamera* camera);
	void readAllFrame();
	void undistort();
    HumanLib mergeMultiCamera();
    void imshow();
	void imwrite(std::string path);
	int getFrameId(){return frameId;}
	float getFrameRate();
private:
	int frameId = 0;
	clock_t startTime;
    std::vector<MonocularCamera*> monocularCameraVector;
    std::vector<StereoCamera*> stereoCameraVector;

	std::vector<MonocularCamera*> allCamera;
	
};

#endif  //_CAMERAMANAGER_H
