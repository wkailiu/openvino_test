//
//  @ Project : HumanPose3D
//  @ File Name : ThirdPartyUtil.cpp
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#include "ThirdPartyUtil.h"
//
//op::Wrapper opWrapper{ op::ThreadManagerMode::Asynchronous };
//

human_pose_estimation::HumanPoseEstimator estimator("human-pose-estimation.xml", "CPU", false);

std::vector<std::vector<BodyNode>> ThirdPartyUtil::LightweightOpenPose(cv::Mat &frame,bool drawpose) {
	//static human_pose_estimation::HumanPoseEstimator estimator("human-pose-estimation.xml", "CPU", false);
	//static bool haveInit = false;
	//if (!haveInit) {
	//	//opWrapper.start();
	//	haveInit = true;
	//	std::cout << "openpose have started!" << std::endl;
	//}
	
	std::vector<std::vector<BodyNode>> humanPoseVector;
	

	std::vector<human_pose_estimation::HumanPose> poses = estimator.estimate(frame);

	float rate;
	for (auto pose : poses) {
		std::vector<BodyNode> bodyNode25;
		for (auto const& keypoint : pose.keypoints) {
			//std::cout << "point="<<keypoint.x << "," << keypoint.y << " " << std::endl;
			rate = keypoint.x == -1 ? -1.0f : 1.0;
			bodyNode25.push_back(BodyNode(keypoint.x, keypoint.y, rate));
		}
		humanPoseVector.push_back(bodyNode25);
	}

	if (drawpose) {
		human_pose_estimation::renderHumanPose(poses, frame);
	}
	//auto data = opWrapper.emplaceAndPop(frame);

	//if (data != nullptr && !data->empty()) {
	//	if (drawpose) {
	//		frame = data->at(0)->cvOutputData;
	//	}

	//	const auto& poseKeypoints = data->at(0)->poseKeypoints;
	//	for (int k = 0; k < poseKeypoints.getSize(0); k++) {
	//		std::vector<BodyNode> bodyNode25;
	//		for (int i = 0; i < 25; i++) {
	//			float px = poseKeypoints[{k, i, 0}];
	//			float py = poseKeypoints[{k, i, 1}];
	//			float rate = poseKeypoints[{k, i, 2}];
	//			bodyNode25.push_back(BodyNode(px, py, rate));
	//		}
	//		humanPoseVector.push_back(bodyNode25);
	//	}
	//}
	return humanPoseVector;
}

Eigen::MatrixXf ThirdPartyUtil::matToMatrix(cv::Mat mat) {
	Eigen::MatrixXf ret(mat.rows, mat.cols);
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			ret(i, j) = mat.at<double>(i, j);
		}
	}
	return ret;
}

float ThirdPartyUtil::distance(Eigen::Vector3f p1, Eigen::Vector3f p2) {
	Eigen::Vector3f d = p1 -p2;
	return sqrt(d.dot(d));
}

float ThirdPartyUtil::angle(Eigen::Vector3f dir1, Eigen::Vector3f dir2) {
	float a = sqrt(dir1.dot(dir1));
	float b = sqrt(dir2.dot(dir2));
	return acos(dir1.dot(dir2) / (a*b));
}

void ThirdPartyUtil::sendMessage(std::string to, std::string who, std::string state) {

}

const double eps = 1e-8;

//三维空间点
struct Point
{
	double x, y, z;
	Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
	Point(Eigen::Vector3f p) {
		x = p(0);
		y = p(1);
		z = p(2);
	}
	Point(const Point& a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		return;
	}
	void Print()
	{
		printf("%lf %lf %lf\n", x, y, z);
	}

	Point operator + (Point &t)
	{
		return Point(x + t.x, y + t.y, z + t.z);
	}
};

//空间直线
struct Line
{
	Point a, b;
};

//空间平面
struct Plane
{
	Point a, b, c;

	Plane() {}
	Plane(Point a, Point b, Point c) :a(a), b(b), c(c) {}

	void showPlane()
	{
		a.Print();
		b.Print();
		c.Print();
		return;
	}
};

double dcmp(double t)
{
	if (fabs(t) < eps) return 0;
	return t < 0 ? -1 : 1;
}

//三维叉积
Point cross(Point u, Point v)
{
	Point ret;
	ret.x = u.y * v.z - v.y * u.z;
	ret.y = u.z * v.x - u.x * v.z;
	ret.z = u.x * v.y - u.y * v.x;
	return ret;
}

//三维点积
double multi(Point u, Point v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

//矢量差
Point sub(Point u, Point v)
{
	Point ret;
	ret.x = u.x - v.x;
	ret.y = u.y - v.y;
	ret.z = u.z - v.z;
	return ret;
}

//两点距离
double dist(Point p1, Point p2)
{
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) + (p1.z - p2.z)*(p1.z - p2.z));
}

//向量的模
double VectorLength(Point p)
{
	return sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
}

//空间直线距离
double LineToLine(Line u, Line v, Point &tmp)
{
	tmp = cross(sub(u.a, u.b), sub(v.a, v.b));
	return fabs(multi(sub(u.a, v.a), tmp)) / VectorLength(tmp);
}

//取平面法向量
Point normalVector(Plane s)
{
	return cross(sub(s.a, s.b), sub(s.b, s.c));
}

//空间平面与直线的交点
Point Intersection(Line l, Plane s)
{
	Point ret = normalVector(s);
	double t = (ret.x*(s.a.x - l.a.x) + ret.y*(s.a.y - l.a.y) + ret.z*(s.a.z - l.a.z)) / (ret.x*(l.b.x - l.a.x) + ret.y*(l.b.y - l.a.y) + ret.z*(l.b.z - l.a.z));
	ret.x = l.a.x + (l.b.x - l.a.x) * t;
	ret.y = l.a.y + (l.b.y - l.a.y) * t;
	ret.z = l.a.z + (l.b.z - l.a.z) * t;
	return ret;
}

Eigen::Vector4f ThirdPartyUtil::getDistancePoint(Eigen::Vector3f p1, Eigen::Vector3f p2,
	Eigen::Vector3f p3, Eigen::Vector3f p4)
{
	Line A, B;
	A.a = Point(p1);
	A.b = Point(p2);

	B.a = Point(p3);
	B.b = Point(p4);

	Eigen::Vector4f result;

	Point normal;
	double d = LineToLine(A, B, normal);
	result(3) = d/2;	//误差:两直线距离的一半

	Plane alpha = Plane(A.a, A.b, A.a + normal);
	Plane beta = Plane(B.a, B.b, B.a + normal);
	Point u = Intersection(B, alpha);
	Point v = Intersection(A, beta);
	//printf("%.6lf %.6lf %.6lf %.6lf %.6lf %.6lf\n", v.x, v.y, v.z, u.x, u.y, u.z);

	//中点坐标
	result(0) = (u.x + v.x) / 2.0f;
	result(1) = (u.y + v.y) / 2.0f;
	result(2) = (u.z + v.z) / 2.0f;
	return result;
}