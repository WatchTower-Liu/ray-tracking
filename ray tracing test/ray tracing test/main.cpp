#include <opencv2\opencv.hpp>
#include <iostream>
#include <Eigen\Dense>
#include <memory>
#include <vector>
#include "camera.hpp"
#include "object.hpp"
#include "Ray.hpp"
#include "material.hpp"
#include "somemath.hpp"

using namespace std;
using namespace cv;
using namespace Eigen;

#define W   500
#define H   500
#define oneframetime   20


void drawpixel(Ray& ray, vector<pair<shared_ptr<Object>, string> >& object, vector<Material>& materialtest, vector<Vector3f>& colorv, int tracnum, int lastminnum)
{
	vector<Ray> objectray;
	if (tracnum > 5)
		return;
	int mindisnum = -1;
	float mindis = INF;
	for (int ob = 0; ob < object.size(); ob++)
	{
		//if (ob == lastminnum)
		//	continue;
		Ray thisone = object[ob].first->intersect(ray);
		objectray.push_back(thisone);
		if (thisone.whetherhit())
		{
			if (thisone.getdistance() < mindis )
			{
				mindis = thisone.getdistance();
				mindisnum = ob;
			}
		}
	}
	if (mindisnum >= 0)
	{
		//(1 - tracnum * 0.3) *
		Vector3f pixcolor = ((1 - tracnum * 0.3) *(materialtest[mindisnum].*materialtest[mindisnum].materialfun[int(materialtest[mindisnum].getreftype())])(object[mindisnum], objectray[mindisnum], 0.5));
		Vector3f c(pixcolor[0] > 0 ? pixcolor[0] : 0, pixcolor[1] > 0 ? pixcolor[1] : 0, pixcolor[2] > 0 ? pixcolor[2] : 0);
		colorv.push_back(c);//在前面被击中的材质，需push到像素渲染堆中

		Ray reflectray = object[mindisnum].first->reflect(objectray[mindisnum], int(materialtest[mindisnum].getreftype()), materialtest[mindisnum].getreflevel());
		tracnum++;
		drawpixel(reflectray, object, materialtest, colorv, tracnum, mindisnum);
	}
}

int main()
{
	Mat mapimg = imread("1.jpg");

	Mat img = Mat::zeros(Size(H, W), CV_8UC3);
	Vector3f origin(10.0, 10.0, 10.0);

	Vector3f Panc(0.0, -5.0, 5.0);
	Vector3f Pann(0.0, 1, 0);
	Vector3f Panup(0.0, 0, 1.0);

	Vector3f Panc2(-5.0, 0.0, 5.0);
	Vector3f Pann2(1.0, 0, 0);
	Vector3f Panup2(0.0, 0, 1.0);

	Vector3f Panc1(0.0, 0.0, 0.0);
	Vector3f Pann1(0, 0.0, 1.0);
	Vector3f Panup1(0, 1.0, 0.0);

	Vector3f ballc1(2.0, 0.0, 3.0);
	float radius1 = 1.5;
	Vector3f ballc2(-2.0, -1.0, 3.0);
	float radius2 = 1.0;
	Vector3f ballc3(-1.0, 0.0, 1.5);
	float radius3 = 0.90;
	Vector3f ballc4(2.0, 3.0, 2.0);
	float radius4 = 1.1;


	Camera cam(origin, Vector3f(-1 / sqrt(3), -1 / sqrt(3), -1 / sqrt(3)),
		Vector3f(-1 / sqrt(3), -1 / sqrt(3), 1 / sqrt(3)), Vector2i(H, W), 50);

	vector<pair<shared_ptr<Object> , string> > object;
	vector<Material> materialtest;

	shared_ptr<Ball> ball1 = make_shared<Ball>(ballc1, radius1);
	object.push_back(pair<shared_ptr<Object>, string>(ball1,"ball"));
	materialtest.push_back(Material(reflecttype::testball, 1, refractiontype::test, 1, Vector3f(128, 128, 50)));  //加载测试材质

	shared_ptr<Ball> ball2 = make_shared<Ball>(ballc2, radius2);
	object.push_back(pair<shared_ptr<Object>, string>(ball2, "ball"));
	materialtest.push_back(Material(reflecttype::testball, 1, refractiontype::test, 1, Vector3f(128, 255, 255)));  //加载测试材质

	shared_ptr<Ball> ball3 = make_shared<Ball>(ballc3, radius3);
	object.push_back(pair<shared_ptr<Object>, string>(ball3, "ball"));
	materialtest.push_back(Material(reflecttype::testball, 0.99, refractiontype::test, 1, Vector3f(255, 255, 255)));  //加载测试材质

	shared_ptr<Ball> ball4 = make_shared<Ball>(ballc4, radius4);
	object.push_back(pair<shared_ptr<Object>, string>(ball4, "ball"));
	materialtest.push_back(Material(reflecttype::testball, 1, refractiontype::test, 1, Vector3f(255, 128, 255)));  //加载测试材质

	shared_ptr<Panel> panel1 = make_shared<Panel>(Panc1, Pann1, Panup1, Vector2f(20, 20));
	object.push_back(pair<shared_ptr<Object>, string>(panel1, "panel"));
	materialtest.push_back(Material(reflecttype::testpanel, 1, refractiontype::test, 1, Vector3f(255, 255, 255)));

	shared_ptr<Panel> panel2 = make_shared<Panel>(Panc2, Pann2, Panup2, Vector2f(10, 20));
	object.push_back(pair<shared_ptr<Object>, string>(panel2, "panel"));
	materialtest.push_back(Material(reflecttype::testball, 1, refractiontype::test, 1, Vector3f(255, 255, 200)));

	shared_ptr<Panel> panel = make_shared<Panel>(Panc, Pann, Panup, Vector2f(10, 20));
	object.push_back(pair<shared_ptr<Object>, string>(panel, "panel"));
	Material mapmaterial(reflecttype::map, 0.6, refractiontype::test, 1, Vector3f(0, 255, 255));
	mapmaterial.setmap(mapimg);
	materialtest.push_back(mapmaterial);


	namedWindow("img", WINDOW_NORMAL);
	Mat3f thisframecolor = Mat::zeros(Size(H, W), CV_8UC3);
	for (int ccin = 0; ccin <= oneframetime; ccin++)
	{

		for (int i = 0; i < img.rows; i++)
		{
			unsigned char *data = img.ptr<unsigned char>(i);
			float *framedata = thisframecolor.ptr<float>(i);
			float sy = 1 - 1 / float(img.rows) * i;
			for (int j = 0; j < img.cols; j++)
			{
				float sx = 1 / float(img.cols)  * j;
				Ray ray = cam.clcdircetion(Vector2f(sx, sy));
				vector<Vector3f> drawcolor;
				int drawnum = 0; int mmm = -1;
				drawpixel(ray, object, materialtest, drawcolor, drawnum, mmm);
				Vector3f pix(0, 0, 0);
				for (int hitnum = 0; hitnum < drawcolor.size(); hitnum++)
				{
					pix += drawcolor[hitnum];
				}
				if (drawcolor.size() > 0)
				{
					framedata[j * 3 + 0] += int((pix[0]) / (drawcolor.size()) / (oneframetime + 1));
					framedata[j * 3 + 1] += int((pix[1]) / (drawcolor.size()) / (oneframetime + 1));
					framedata[j * 3 + 2] += int((pix[2]) / (drawcolor.size()) / (oneframetime + 1));

				}

			}
		}
		Mat display = thisframecolor.clone();
		display.convertTo(img, CV_8UC3);
		imshow("img", img);
		waitKey(1);
	}
	//namedWindow("img", WINDOW_NORMAL);
	imshow("img", img);
	//imwrite("光追.jpg", img);
	waitKey(0);
	return 0;
}