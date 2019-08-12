//
//  @ Project : HumanPose3D
//  @ File Name : BodyNode.cpp
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#include "BodyNode.h"


BodyNode::BodyNode() {
	px = py = rate = BAD_FLOAT; 
	x = y = z = BAD_FLOAT;
}

BodyNode::BodyNode(float px0, float py0, float r) :px(px0), py(py0), rate(r) {
	x = y = z = BAD_FLOAT;
}

bool BodyNode::isGood2D() {
	return rate>0.1;
}

bool BodyNode::isGood3D() {
	return x != BAD_FLOAT;
}


Eigen::Vector2f BodyNode::getPixel() {
	return Eigen::Vector2f(px,py);
}

Eigen::Vector3f BodyNode::getPoint() {
	return Eigen::Vector3f(x,y,z);
}

float BodyNode::getRate() {
	return rate;
}

