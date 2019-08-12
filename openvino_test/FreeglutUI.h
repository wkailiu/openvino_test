//
//  @ Project : HumanPose3D
//  @ File Name : FreeglutUI.h
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#if !defined(_FREEGLUTUI_H)
#define _FREEGLUTUI_H

#include <Eigen\Dense>
#include <stdio.h>
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "GL/freeglut_std.h"
#include "GL/glut.h"
#include "CameraParameter.h"
#include "HumanLib.h"
#include "MonocularCamera.h"

class FreeglutUI {
public:
	static void initWindows(std::string windowsName, int w,int h);
    static void showHuman(HumanLib humanLib);
	static void setCameraView(MonocularCamera camera);
private:
    
    void drawFloor();
};

#endif  //_FREEGLUTUI_H
