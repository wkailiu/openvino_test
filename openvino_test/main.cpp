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

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);

	CameraManager manager;
	StereoCamera stereo("stereo_zed.yaml");
	manager.addStereoCamera(&stereo);

	FreeglutUI::initWindows("HumanPose3D", 800, 600);
	FreeglutUI::setCameraView(*stereo.getLeftCameraPtr());

	HumanLib oldHumanLib, newHumanLib;
	double inferenceTime = 0.0;
	int ttt = 0;
	while (true) {
		ttt++;
		double t1 = static_cast<double>(cv::getTickCount());
		manager.readAllFrame();						//读取图像
		//manager.undistort();						//畸变矫正
		newHumanLib = manager.mergeMultiCamera();	//多视图融合
		newHumanLib.merge(oldHumanLib);				//与上一帧融合
		FreeglutUI::showHuman(newHumanLib);			//绘画三维骨架图
		oldHumanLib = newHumanLib;					//保存为上一帧
		if(ttt%10==0)
			std::cout << "frameId=" << manager.getFrameId() << "\tframeRate=" << manager.getFrameRate() << std::endl;;
		manager.imshow();
		cv::waitKey(5);

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
