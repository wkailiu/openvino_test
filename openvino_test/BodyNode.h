//
//  @ Project : HumanPose3D
//  @ File Name : BodyNode.h
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#if !defined(_BODYNODE_H)
#define _BODYNODE_H
#include <Eigen\Dense>
#include "define.h"

class BodyNode {
public:
    float px;
    float py;
    float rate;
    float x;
    float y;
    float z;
    int frameId;
    BodyNode();
    BodyNode(float px0, float py0, float rate0);
    bool isGood2D();
    bool isGood3D();
    Eigen::Vector2f getPixel();
    Eigen::Vector3f getPoint();
    float getRate();
};

#endif  //_BODYNODE_H
