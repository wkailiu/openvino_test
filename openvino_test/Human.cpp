//
//  @ Project : HumanPose3D
//  @ File Name : Human.cpp
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#include "Human.h"

Human::Human(HumanPose humanPose) {
	name = "???";
	humanPoseVector.push_back(humanPose);
}

void Human::mergeHuman(Human human) {
	//头部为最新数据
	for (int i = human.humanPoseVector.size() - 1; i >= 0; i--) {
		humanPoseVector.push_back(human.humanPoseVector[i]);
	}
}

void Human::setColor(Eigen::Vector3f c){
	color = c;
}

void Human::setName(std::string n) {
	name = n;
}

Eigen::Vector3f Human::getCurrNodePoint(int nodeId) {
	if (humanPoseVector.size() > 0) {
		return humanPoseVector[0].getNodePoint(nodeId);
	}
	return Eigen::Vector3f(BAD_FLOAT, BAD_FLOAT, BAD_FLOAT);
}

std::vector<Eigen::Vector3f> Human::getNodePointHistory(int nodeId) {
	std::vector<Eigen::Vector3f> points;
	for(auto humanPose: humanPoseVector){
		points.push_back(humanPose.getNodePoint(nodeId));
	}
	return points;
}

float Human::getPartyLength(int nodeId1, int nodeId2) {
	return 1.0f;
}

std::string Human::postureRecognition() {
	return "正常";
}

bool Human::isFallDown() {
	//int ids[] = { 0,15,16,17,18,1,2,5,8,9,12 };
	//for (auto id : ids) {
	//	auto p = getCurrNodePoint(id);
	//	if (p(2)!= BAD_FLOAT && p(2) < 15.0f)
	//		return true;
	//}
	return false;
}