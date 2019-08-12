//
//  @ Project : HumanPose3D
//  @ File Name : Human.h
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#if !defined(_HUMAN_H)
#define _HUMAN_H

#include <iostream>
#include <Eigen\Dense>
#include <vector>
#include "HumanPose.h"
#include "define.h"

class Human {
public:
	Human(HumanPose humanPose);
    
	Eigen::Vector3f color;

	void setColor(Eigen::Vector3f c);
	void setName(std::string n);
	std::string getName() { return name; };
    
    Eigen::Vector3f getCurrNodePoint(int nodeId);
    std::vector<Eigen::Vector3f> getNodePointHistory(int nodeId);
    float getPartyLength(int nodeId1, int nodeId2);
    std::string postureRecognition();
	bool isFallDown();
	void mergeHuman(Human human);
private:
	std::vector<HumanPose> humanPoseVector;
	std::string name;
};

#endif  //_HUMAN_H
