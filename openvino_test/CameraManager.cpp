//
//  @ Project : HumanPose3D
//  @ File Name : CameraManager.cpp
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#include "CameraManager.h"
#include "MonocularCamera.h"
#include "StereoCamera.h"
#include "HumanLib.h"

void CameraManager::addMonocularCamera(MonocularCamera* camera) {
	monocularCameraVector.push_back(camera);
	allCamera.push_back(camera);
}

void CameraManager::addStereoCamera(StereoCamera* camera) {
	stereoCameraVector.push_back(camera);
	allCamera.push_back(camera->getLeftCameraPtr());
	allCamera.push_back(camera->getRightCameraPtr());
}

void CameraManager::readAllFrame() {
	if (frameId == 0) startTime = clock();
	for (auto camera : monocularCameraVector)camera->readFrame();
	for (auto camera : stereoCameraVector)camera->readFrame();
	frameId++;
}

void CameraManager::undistort() {
	for (auto camera : monocularCameraVector)camera->undistort();
	for (auto camera : stereoCameraVector) camera->undistort();
}

float CameraManager::getFrameRate() {
	return frameId*1000.0f / (clock() - startTime);
}

HumanLib CameraManager::mergeMultiCamera() {
	std::vector<HumanPose> allHumanPose;
	std::vector<std::vector<BodyNode>> bodyNodeLib;

	for (int i = 0; i < allCamera.size(); i++) {
		bodyNodeLib = ThirdPartyUtil::LightweightOpenPose(allCamera[i]->frame, true);
		for (auto bodyNode25 : bodyNodeLib) {
			HumanPose humanPose(i);
			for (int nodeId = 0; nodeId < bodyNode25.size(); nodeId++) {
				humanPose.setBodyNode(nodeId, bodyNode25[nodeId]);
			}
			allHumanPose.push_back(humanPose);
		}
	}

	HumanLib humanLib;
	std::vector<bool> visited(allHumanPose.size(), false);

	std::cout << "----------------------------------------------" << std::endl;
	std::cout << "----------------------------------------------" << std::endl;
	std::cout << "----------------------------------------------" << std::endl;
	
	for (int i = 0; i < allHumanPose.size(); i++) {
		//if (visited[i]) {
		//	std::cout << "["<<i<<", ] selected" << std::endl;
		//	continue;
		//}
		int cameraId1 = allHumanPose[i].getCameraId();
		for (int j = i + 1; j < allHumanPose.size(); j++) {
			//if (visited[j]) {
			//	std::cout << "["<<i<<","<<j<<"] selected" << std::endl;
			//	continue;
			//}

			int cameraId2 = allHumanPose[j].getCameraId();
			if (cameraId1 == cameraId2) {
				std::cout << "[" << i << "," << j << "]\tsame" << std::endl;
				continue;
			}
			

			

			std::cout << std::endl;
			std::cout << "[" << i << "," << j <<"]"<< std::endl;
			HumanPose humanPose = allHumanPose[i].mergeTwoHumanPose(allCamera[cameraId1], allCamera[cameraId2], allHumanPose[j]);
			if (humanPose.getGoodNodeNum() >= 5) {
				std::cout << "select" << std::endl;
				//visited[i] = true;
				visited[j] = true;
				humanLib.addHuman(Human(humanPose));
				break;
			}
		}
	}
	return humanLib;
}

void CameraManager::imshow() {
	for(auto camera:allCamera){
		camera->imshow();
	}
}


void CameraManager::imwrite(std::string path) {
	for (auto camera : allCamera) {
		camera->imwrite(path);
	}
}
