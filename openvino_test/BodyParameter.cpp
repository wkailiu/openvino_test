//
//  @ Project : HumanPose3D
//  @ File Name : BodyParameter.cpp
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#include "BodyParameter.h"

void BodyParameter::setLength(int n1, int n2, float min, float max) {
	minLength[n1][n2] = min;
	maxLength[n1][n2] = max;
}

float BodyParameter::getCenterLength(int n1, int n2) {
	return (minLength[n1][n2] + maxLength[n1][n2]) / 2.0f;
}

float BodyParameter::getMinLength(int n1, int n2) {
	return minLength[n1][n2];
}

float BodyParameter::getMaxLength(int n1, int n2) {
	return maxLength[n1][n2];
}

