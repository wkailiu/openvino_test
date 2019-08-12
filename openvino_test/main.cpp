#include <vector>
#include <inference_engine.hpp>
#include <samples/ocv_common.hpp>

#include "human_pose_estimator.hpp"
#include "render_human_pose.hpp"

#include "MonocularCamera.h"
#include "StereoCamera.h"
#include "CameraManager.h"
#include "FreeglutUI.h"
using namespace InferenceEngine;
using namespace human_pose_estimation;
using namespace std;


//提供给unity的接口
#define DLL_API extern "C" __declspec (dllexport)

DLL_API void initHumanPose3D();
DLL_API void nextFrame(float* humanPoseData);


CameraManager manager;
HumanLib oldHumanLib, newHumanLib;



void initHumanPose3D() {
	StereoCamera stereo("stereo_zed.yaml");
	manager.addStereoCamera(&stereo);
}


void nextFrame(float* humanPoseData) {
	manager.readAllFrame();						//读取图像
	//manager.undistort();						//畸变矫正
	newHumanLib = manager.mergeMultiCamera();	//多视图融合
	newHumanLib.merge(oldHumanLib);				//与上一帧融合

	oldHumanLib = newHumanLib;					//保存为上一帧

	int k = 0;
	humanPoseData[0] = newHumanLib.getHumanNum();
	k++;
	for (auto human : newHumanLib.getHumanVector()) {
		for (int node = 0; node < 18; node++) {
			auto p = human.getCurrNodePoint(node);
			humanPoseData[k++] = p(0);	//x
			humanPoseData[k++] = p(1);	//y
			humanPoseData[k++] = p(2);	//z
		}
	}
}


int main(int argc, char* argv[]) {
	float humanPoseData[18 * 3 * 10 + 1];
	glutInit(&argc, argv);

	CameraManager manager;
	StereoCamera stereo("stereo_zed.yaml");
	manager.addStereoCamera(&stereo);


	FreeglutUI::initWindows("HumanPose3D", 800, 600);
	FreeglutUI::setCameraView(*stereo.getLeftCameraPtr());

	HumanLib oldHumanLib, newHumanLib;
	while (true) {
		manager.readAllFrame();						//读取图像
		//manager.undistort();						//畸变矫正
		newHumanLib = manager.mergeMultiCamera();	//多视图融合
		newHumanLib.merge(oldHumanLib);				//与上一帧融合

		FreeglutUI::showHuman(newHumanLib);			//绘画三维骨架图
		
		oldHumanLib = newHumanLib;					//保存为上一帧
		manager.imshow();
		cv::waitKey(5);


		int k = 0;
		humanPoseData[0] = newHumanLib.getHumanNum();
		k++;
		for (auto human : newHumanLib.getHumanVector()) {
			for (int node = 0; node < 18; node++) {
				auto p = human.getCurrNodePoint(node);
				humanPoseData[k++] = p(0);	//x
				humanPoseData[k++] = p(1);	//y
				humanPoseData[k++] = p(2);	//z
			}
		}
		//double t2 = static_cast<double>(cv::getTickCount());
		//if (inferenceTime == 0) {
		//	inferenceTime = (t2 - t1) / cv::getTickFrequency() * 1000;
		//}
		//else {
		//	inferenceTime = inferenceTime * 0.95 + 0.05 * (t2 - t1) / cv::getTickFrequency() * 1000;
		//}
		//cout << "FPS: " << int(1000.0f / inferenceTime * 100) / 100.0f << endl;;
	}

	return 0;
}
