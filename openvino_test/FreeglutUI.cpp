//
//  @ Project : HumanPose3D
//  @ File Name : FreeglutUI.cpp
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#include "FreeglutUI.h"
#include "CameraParameter.h"
#include "HumanLib.h"


#define BODY_NUM 24

const int win_w = 1000, win_h = 1000;	// 窗口大小
GLfloat default_matrix[16];
GLfloat modelview_matrix[16];
GLfloat modelview_z_dis;


static int bodyInfo[BODY_NUM][2] = { { 0,15 },{ 0,16 },{ 0,1 },{ 1,2 },{ 1,5 },
{ 1,8 },{ 2,3 },{ 3,4 },{ 5,6 },{ 6,7 },
{ 8,9 },{ 8,12 },{ 9,10 },{ 10,11 },{ 11,22 },
{ 11,24 },{ 12,13 },{ 13,14 },{ 14,19 },{ 14,21 },
{ 15,17 },{ 16,18 },{ 19,20 },{ 22,23 } };

static float bodyColor[BODY_NUM][3] =
{
	{ 153 / 255.0, 0 / 255.0,102 / 255.0 },				// 0 15
	{ 102.0 / 255.0, 0.0 / 255.0, 153.0 / 255.0 },		// 0 16
	{ 153 / 255.0,0 / 255.0,51 / 255.0 },				// 0 1
	{ 153 / 255.0,51 / 255.0,0 / 255.0 },				// 1 2
	{ 102 / 255.0,153 / 255.0, 0 / 255.0 },				// 1 5

	{ 153 / 255.0,0 / 255.0,0 / 255.0 },				// 1 8
	{ 153 / 255.0, 102 / 255.0, 0 / 255.0 },			// 2 3
	{ 153 / 255.0,153 / 255.0, 0 / 255.0 },				// 3 4
	{ 51 / 255.0, 153 / 255.0,0 / 255.0 },				// 5 6
	{ 0 / 255.0, 153 / 255.0, 0 / 255.0 },				// 6 7

	{ 0 / 255.0,153 / 255.0, 51 / 255.0 },				// 8 9
	{ 0 / 255.0,102 / 255.0,153 / 255.0 },				// 8 12
	{ 0 / 255.0, 153 / 255.0, 102 / 255.0 },			// 9 10
	{ 0 / 255.0,153 / 255.0,153 / 255.0 },				// 10 11
	{ 0 / 255.0, 153 / 255.0, 153 / 255.0 },			// 11 22

	{ 0 / 255.0, 153 / 255.0,153 / 255.0 },				// 11 24
	{ 0 / 255.0, 0 / 255.0, 153 / 255.0 },				// 12 13
	{ 0 / 255.0,0 / 255.0, 153 / 255.0 },				// 13 14
	{ 0 / 255.0, 0 / 255.0, 153 / 255.0 },				// 14 19
	{ 0 / 255.0,0 / 255.0, 153 / 255.0 },				// 14 21

	{ 153.0 / 255.0, .0, 153.0 / 255.0 },				// 15 17
	{ 51 / 255.0, 0 / 255.0, 153 / 255.0 },				// 16 18
	{ 0 / 255.0, 0 / 255.0, 153 / 255.0 },				// 19 20
	{ 0 / 255.0,153 / 255.0, 153 / 255.0 },				// 22 23

};

void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void drawSphere(Eigen::Vector3f point, Eigen::Vector3f color, float radius)
{
	glPushMatrix();
	glTranslatef(point(0), point(1), point(2));
	glColor3f(color(0), color(1), color(2));
	glutSolidSphere(radius, 20, 20);
	glPopMatrix();
}

void draw_floor()
{
	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);

	float width = 60;

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, width * 12, 0);
	glEnd();

	for (int i = 0; i < 8; i++) {
		glBegin(GL_LINES);
		glVertex3f((i + 0.5)*width, 0, 0);
		glVertex3f((i + 0.5)*width, width * 12, 0);
		glEnd();
	}
	for (int i = 0; i < 12; i++) {
		glBegin(GL_LINES);
		glVertex3f(0, i*width, 0);
		glVertex3f(width*7.5, i*width, 0);
		glEnd();
	}
	glPopMatrix();
}

void drawCylinder(Eigen::Vector3f p1, Eigen::Vector3f p2, float r, float g, float b)
{
	double x0 = p1(0);
	double x1 = p2(0);
	double y0 = p1(1);
	double y1 = p2(1);
	double z0 = p1(2);
	double z1 = p2(2);
	glColor3f(r, g, b);
	GLdouble  dir_x = x1 - x0;
	GLdouble  dir_y = y1 - y0;
	GLdouble  dir_z = z1 - z0;
	GLdouble  bone_length = sqrt(dir_x*dir_x + dir_y*dir_y + dir_z*dir_z);
	static GLUquadricObj *  quad_obj = NULL;
	if (quad_obj == NULL)
		quad_obj = gluNewQuadric();
	gluQuadricDrawStyle(quad_obj, GLU_FILL);
	gluQuadricNormals(quad_obj, GLU_SMOOTH);
	glPushMatrix();
	// 平移到起始点
	glTranslated(x0, y0, z0);
	// 计算长度
	double  length;
	length = sqrt(dir_x*dir_x + dir_y*dir_y + dir_z*dir_z);
	if (length < 0.0001) {
		dir_x = 0.0; dir_y = 0.0; dir_z = 1.0;  length = 1.0;
	}
	dir_x /= length;  dir_y /= length;  dir_z /= length;
	GLdouble  up_x, up_y, up_z;
	up_x = 0.0;
	up_y = 1.0;
	up_z = 0.0;
	double  side_x, side_y, side_z;
	side_x = up_y * dir_z - up_z * dir_y;
	side_y = up_z * dir_x - up_x * dir_z;
	side_z = up_x * dir_y - up_y * dir_x;
	length = sqrt(side_x*side_x + side_y*side_y + side_z*side_z);
	if (length < 0.0001) {
		side_x = 1.0; side_y = 0.0; side_z = 0.0;  length = 1.0;
	}
	side_x /= length;  side_y /= length;  side_z /= length;
	up_x = dir_y * side_z - dir_z * side_y;
	up_y = dir_z * side_x - dir_x * side_z;
	up_z = dir_x * side_y - dir_y * side_x;
	// 计算变换矩阵
	GLdouble  m[16] = { side_x, side_y, side_z, 0.0,
		up_x,   up_y,   up_z,   0.0,
		dir_x,  dir_y,  dir_z,  0.0,
		0.0,    0.0,    0.0,    1.0 };
	glMultMatrixd(m);
	// 圆柱体参数
	GLdouble radius = 2;		// 半径
	gluCylinder(quad_obj, radius, radius, bone_length, 20, 10);
	glPopMatrix();
}


void reshape_func(int w, int h)
{
	if (h == 0)h = 1;
	float ratio = 1.0* w / h;

	// 单位化投影矩阵。
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 设置视口大小为增个窗口大小
	glViewport(0, 0, w, h);

	// 设置正确的投影矩阵
	gluPerspective(45, ratio, 1, 1000);
	//下面是设置模型视图矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);

}

/* 以下三个函数对物体进行平移、旋转、缩放，他们均在视觉(绝对)坐标下进行
* 正常调用 glTranslate,glRotate,glScale 均是在局部坐标下进行(按正序看)
* 为了达到在视觉坐标下操作的效果，需要将矩阵左乘到当前矩阵
*/
void absolute_translate(GLfloat x, GLfloat y, GLfloat z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, z);
	glMultMatrixf(modelview_matrix);			// 使变换矩阵左乘到当前矩阵，这样才适合绝对坐标的考虑
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	glPopMatrix();
}
void absolute_rotate(GLfloat dgree, GLfloat vecx, GLfloat vecy, GLfloat vecz)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(.0, .0, -modelview_z_dis);		// 平移回去，注意该句和后两句要倒序来看
	glRotatef(dgree, vecx, vecy, vecz);			// 积累旋转量
	glTranslatef(.0, .0, modelview_z_dis);		// 先平移到原点
	glMultMatrixf(modelview_matrix);			// 使变换矩阵左乘到当前矩阵，这样才适合绝对坐标的考虑
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	glPopMatrix();
}
void absolute_scale(GLfloat factor)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(.0, .0, -modelview_z_dis);		// 平移回去，注意该句和后两句要倒序来看
	glScalef(factor, factor, factor);
	glTranslatef(.0, .0, modelview_z_dis);		// 先平移到原点
	glMultMatrixf(modelview_matrix);			// 使变换矩阵左乘到当前矩阵，这样才适合绝对坐标的考虑
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	glPopMatrix();
}
void absolute_default()
{
	memcpy(modelview_matrix, default_matrix, sizeof(default_matrix));
}

// 鼠标状态变量，用来在鼠标点击事件和拖动事件之间通信
static bool l_button_down = false, r_button_down = false, mid_button_down = false;
static int last_x = -1, last_y = -1;
#define  GLUT_WHEEL_UP		3					// 滚轮操作  
#define  GLUT_WHEEL_DOWN	4
/* 鼠标点击和移动，左键拖动旋转，中键拖动平移，右键回到最初视图
*/
void mouse_click_func(int button, int state, int x, int y)
{
	y = win_h - y;
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			l_button_down = true;
			last_x = x; last_y = y;
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		}
		else {
			l_button_down = false;
			last_x = -1; last_y = -1;
			glutSetCursor(GLUT_CURSOR_INHERIT);
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN) {
			mid_button_down = true;
			last_x = x; last_y = y;
			glutSetCursor(GLUT_CURSOR_CYCLE);

		}
		else {
			mid_button_down = false;
			last_x = -1; last_y = -1;
			glutSetCursor(GLUT_CURSOR_INHERIT);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			r_button_down = true;
			absolute_default();
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			glutPostRedisplay();
		}
		else {
			r_button_down = false;
			glutSetCursor(GLUT_CURSOR_INHERIT);
		}
		break;
	case GLUT_WHEEL_UP:
		if (state == GLUT_UP) {
			absolute_scale(.9f);
			glutPostRedisplay();
		}
		break;
	case GLUT_WHEEL_DOWN:
		if (state == GLUT_UP) {
			absolute_scale(1.1f);
			glutPostRedisplay();
		}
		break;
	}
}
void mouse_move_func(int x, int y)
{
	y = win_h - y;
	if (last_x >= 0 && last_y >= 0 && (last_x != x || last_y != y)) {
		GLfloat deltax = GLfloat(x - last_x), deltay = GLfloat(y - last_y);
		if (mid_button_down) {
			absolute_translate(deltax * .1f, deltay * .1f, .0f);
			glutPostRedisplay();
		}
		else if (l_button_down) {
			GLfloat dis = sqrt(deltax*deltax + deltay * deltay);
			absolute_rotate(dis, -deltay / dis, deltax / dis, .0);
			glutPostRedisplay();
		}
	}
	last_x = x; last_y = y;
}


void FreeglutUI::initWindows(std::string windowsName, int w, int h) {
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_DEPTH);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(windowsName.c_str());
	//glutReshapeFunc(reshape_func);
	//glutMotionFunc(mouse_move_func);
	//glutDisplayFunc(displayFun);
	glEnable(GL_DEPTH_TEST);
}

void FreeglutUI::showHuman(HumanLib humanLib) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(modelview_matrix);

	//绘制地板
	draw_floor();

	for (auto person : humanLib.getHumanVector()) {
		//绘制轨迹
		for (auto p : person.getNodePointHistory(0)) {
			drawSphere(p, person.color, 1.0f);
		}

		//绘制名字
		auto p = person.getCurrNodePoint(1);
		glRasterPos3f(p(0), p(1), p(2) + 50);
		glColor3f(0.0f, 0.0f, 1.0f);
		auto personName = person.getName();
		const unsigned char* t = reinterpret_cast<const unsigned char *>(personName.c_str());
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, t);

		//摔倒报警
		if (person.isFallDown()) {
			glColor3f(1.0f, 0.0f, 0.0f);
			glRasterPos3f(p(0), p(1), p(2) + 30);
			std::string error = "Fall Down!";
			const unsigned char* t = reinterpret_cast<const unsigned char *>(error.c_str());
			glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, t);
		}

		//绘制圆柱
		for (int i = 0; i < BODY_NUM; i++) {
			auto p1 = person.getCurrNodePoint(bodyInfo[i][0]);
			auto p2 = person.getCurrNodePoint(bodyInfo[i][1]);
			if (p1(2) > 0 && p2(2) > 0) {
				drawCylinder(p1, p2, bodyColor[i][0], bodyColor[i][1], bodyColor[i][2]);
			}
		}

		//绘制节点球体
		for (int i = 0; i < 25; i++) {
			auto p = person.getCurrNodePoint(i);
			Eigen::Vector3f color(1.0, 0.0, 0.0);
			if (p(2) > 0) drawSphere(p, color, 2.5);
			//std::cout << i << "\t" << p(0) << "\t" << p(1) << "\t" << p(2) << std::endl;
		}

	}
	glutSwapBuffers();
}

void FreeglutUI::setCameraView(MonocularCamera camera) {
	// projection matrix init
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  // 加载单位矩阵

	// lwk 棱锥
	gluPerspective(100.0, 1.0, 10.0, 500.0);	// 创建一个对称的透视型视景体
											// default_matrix, modelview_matrix, modelview_z_dis init
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//前三个参数设置观察者的观察位置，中三个参数设置观察点的位置，后三个参数设置观察者的观察方向

	// 摄像头坐标
	Eigen::Vector3f p = camera.getCenter();

	// 画面中点地面坐标
	Eigen::Vector3f dir = camera.pixelToWorld(672/2, 376/2);
	Eigen::Vector3f dd = dir - p;

	float t = p(2) / dd(2);
	//Eigen::Vector3f seePoint = p - t * dd;
	Eigen::Vector3f seePoint = p + 500 * (dir - p);

	//左右两个点，叉积求摄像头头朝向
	Eigen::Vector3f left = camera.pixelToWorld(672 / 2 - 100, 376 / 2);
	Eigen::Vector3f right = camera.pixelToWorld(672 / 2 + 100, 376 / 2);
	Eigen::Vector3f head = left.cross(right);

	gluLookAt(0, 0, 0,
		0, 0, 1,
		0, -1, 0);

	//gluLookAt(p(0), p(1), p(2),	// 摄像头位置
	//	seePoint(0), seePoint(1), seePoint(2),// 所看物体的位置
	//	head(0), head(1), head(2));			// 头向上方向
	modelview_z_dis = 100.0f * sqrt(3.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, default_matrix);
	memcpy(modelview_matrix, default_matrix, sizeof(default_matrix));
	glLoadIdentity();

	// color and lighting
	glClearColor(.2f, .2f, .2f, 1.0f);
	glColor4f(.5f, .5f, .5f, 1.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}

void FreeglutUI::drawFloor() {

}

