//
//  @ Project : HumanPose3D
//  @ File Name : HumanLib.cpp
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#include "HumanLib.h"

void HumanLib::addHuman(Human human) {
	humanVector.push_back(human);
}
void HumanLib::merge(HumanLib humanLib) {
	auto humanVector2 = humanLib.getHumanVector();

	int n1 = humanVector.size();
	int n2 = humanVector2.size();
	if (n1 == 0 || n2 == 0)return;

	static float allDistance[20][20] = { 0 };
	if (n1 > 20 || n2 > 20) {
		std::cerr << "error at:[void HumanLib::merge(HumanLib humanLib)]" << std::endl;
	}

	for (int i = 0; i < n1; i++) {
		for (int j = 0; j < n2; j++) {
			float distance = 0;
			int cnt = 0;
			for (int k = 0; k < 25; k++) {
				auto p1 = humanVector[i].getCurrNodePoint(k);
				auto p2 = humanVector2[j].getCurrNodePoint(k);
				if (p1(0) == BAD_FLOAT || p2(0) == BAD_FLOAT)continue;
				distance += ThirdPartyUtil::distance(p1, p2);
				cnt++;
			}
			allDistance[i][j] = distance / cnt;
		}
	}

	for (int k = 0; k < n1; k++) {
		float minD = 1234567.0f;
		int x, y;
		for (int i = 0; i < n1; i++) {
			for (int j = 0; j < n2; j++) {
				if (allDistance[i][j] == -1)continue;
				if (allDistance[i][j] < minD) {
					minD = allDistance[i][j];
					x = i; y = j;
				}
			}
		}

		for (int i = 0; i < n1; i++) allDistance[i][y] = -1;
		for (int i = 0; i < n2; i++) allDistance[x][i] = -1;
		humanVector[x].mergeHuman(humanVector2[y]);
	}
}

