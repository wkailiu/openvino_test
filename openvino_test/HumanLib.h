//
//  @ Project : HumanPose3D
//  @ File Name : HumanLib.h
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#if !defined(_HUMANLIB_H)
#define _HUMANLIB_H
#include <vector>
#include "Human.h"

class HumanLib {
public:
	void addHuman(Human human);
    void merge(HumanLib humanLib);
	int getHumanNum() { return humanVector.size(); }
	std::vector<Human> getHumanVector() { return humanVector; }
private:
	std::vector<Human> humanVector;
};

#endif  //_HUMANLIB_H
