//
//  @ Project : HumanPose3D
//  @ File Name : ThirdPartyUtil.h
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#if !defined(_THIRDPARTYUTIL_H)
#define _THIRDPARTYUTIL_H
#include <Eigen\Dense>
#include <iostream>
#include <vector>

#include "BodyNode.h"

#include "human_pose_estimator.hpp"
#include "render_human_pose.hpp"
#include <opencv2\opencv.hpp>

static class ThirdPartyUtil {
public:
	static std::vector<std::vector<BodyNode>> LightweightOpenPose(cv::Mat &frame, bool drawpose);
    static float distance(Eigen::Vector3f p1, Eigen::Vector3f p2);
    float angle(Eigen::Vector3f dir1, Eigen::Vector3f dir2);
    void sendMessage(std::string to, std::string who, std::string state);
	static Eigen::Vector4f getDistancePoint(Eigen::Vector3f p1, Eigen::Vector3f p2,
		Eigen::Vector3f p3, Eigen::Vector3f p4);
	static Eigen::MatrixXf matToMatrix(cv::Mat mat);
};

#endif  //_THIRDPARTYUTIL_H
