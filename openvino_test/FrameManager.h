//
//  @ Project : HumanPose3D
//  @ File Name : FrameManager.h
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#if !defined(_FRAMEMANAGER_H)
#define _FRAMEMANAGER_H
#include <opencv2\opencv.hpp>
#include <vector>

class FrameManager {
public:
    std::vector<int> frameIdLib();
    void addFrame(cv::Mat frame);
};

#endif  //_FRAMEMANAGER_H
