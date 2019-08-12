//
//  @ Project : HumanPose3D
//  @ File Name : HumanPose.cpp
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#include "HumanPose.h"

int HumanPose::getGoodNodeNum() {
	int cnt = 0;
	for (auto node : bodyNodeVector) {
		if (node.isGood3D())cnt++;
	}
	return cnt;
}

bool HumanPose::isGood2D(int nodeId) {
	return bodyNodeVector[nodeId].isGood2D();
}

bool HumanPose::isGood3D(int nodeId) {
	return bodyNodeVector[nodeId].isGood3D();
}

Eigen::Vector2f HumanPose::getNodePixel(int nodeId) {
	return bodyNodeVector[nodeId].getPixel();
}

Eigen::Vector3f HumanPose::getNodePoint(int nodeId) {
	return bodyNodeVector[nodeId].getPoint();
}

void HumanPose::isSameHuman(HumanPose pose) {

}

void HumanPose::setBodyNode(int nodeId, BodyNode bodyNode) {
	 bodyNodeVector[nodeId] = bodyNode;
}

HumanPose HumanPose::mergeTwoHumanPose(MonocularCamera* camera1, MonocularCamera* camera2, HumanPose humanPose2) {
	HumanPose humanPose(1);
	//std::cout << "---------------------------------" << std::endl;
	for (int i = 0; i < bodyNodeVector.size(); i++) {
		// 标记节点无效
		if (!(isGood2D(i) && humanPose2.isGood2D(i))) continue;

		Eigen::Vector3f p1 = camera1->getCenter();
		Eigen::Vector3f p2 = camera1->pixelToWorld(getPx(i), getPy(i));
		Eigen::Vector3f p3 = camera2->getCenter();
		Eigen::Vector3f p4 = camera2->pixelToWorld(humanPose2.getPx(i),humanPose2.getPy(i));
		Eigen::Vector4f ret = ThirdPartyUtil::getDistancePoint(p1, p2, p3, p4);

		std::cout << "node="<<i<<"\t"<<ret(0) << "\t" << ret(1) << "\t" << ret(2) << "\t\t" << ret(3) << std::endl;
		if (ret(2) < 30)continue;	// 至少离摄像头30cm
		if (ret(3) > 3.0f) continue;	//误差小于5cm

		BodyNode node;
		node.x = ret(0);
		node.y = ret(1);
		node.z = ret(2);
		humanPose.setBodyNode(i, node);
	}

	return humanPose;
}