//
//  @ Project : HumanPose3D
//  @ File Name : HumanPose.h
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#if !defined(_HUMANPOSE_H)
#define _HUMANPOSE_H


#include <Eigen\Dense>
#include <vector>
#include "BodyNode.h"
#include "MonocularCamera.h"
#include "ThirdPartyUtil.h"

class HumanPose {
public:
	HumanPose(int id) :cameraId(id) { 
		for (int i = 0; i < 25; i++)
			bodyNodeVector.push_back(BodyNode());
	}
	int getGoodNodeNum();
    bool isGood2D(int nodeId);
    bool isGood3D(int nodeId);
	float getPx(int nodeId) { return bodyNodeVector[nodeId].px; }
	float getPy(int nodeId) { return bodyNodeVector[nodeId].py; }
	float getRate(int nodeId) { return bodyNodeVector[nodeId].getRate(); }
    Eigen::Vector2f getNodePixel(int nodeId);
    Eigen::Vector3f getNodePoint(int nodeId);
    void isSameHuman(HumanPose pose);
	void setBodyNode(int nodeId,BodyNode bodyNode);
	int getCameraId() { return cameraId; }
	HumanPose mergeTwoHumanPose(MonocularCamera* camera1, MonocularCamera* camera2, HumanPose humanPose2);
private:
    std::vector<BodyNode> bodyNodeVector;
	int cameraId;
};

#endif  //_HUMANPOSE_H
