#pragma once
#include <Eigen\Dense>
#include <vector>
#include <iostream>
#include <memory>
#include <opencv2\opencv.hpp>
#include "Ray.hpp"
#include "somemath.hpp"
#include "object.hpp"

using namespace Eigen;
using namespace std;
using namespace cv;

enum class reflecttype { testball = 0, testpanel ,map};
enum class refractiontype { test = 0 };


class Material
{
	typedef Vector3f(Material::*_F)(pair<shared_ptr<Object>, string>& obj, Ray& position, float scale);
public:

	Vector3f sample(pair<shared_ptr<Object>, string>& obj, Ray& position, float scale);   //∆Â≈Ã≤‚ ‘

	Vector3f sample1(pair<shared_ptr<Object>, string>& obj, Ray& position, float scale);

	Vector3f matmap(pair<shared_ptr<Object>, string>& obj, Ray& position, float scale);

	_F materialfun[3];
	
	Material();
	Material(reflecttype _reflecttype, float _reflectlevel,
		refractiontype _refractiontype, float _refractionlevel, Vector3f _color);
		
	~Material() {};
	void setmap(Mat _map) { map = _map.clone(); }
	reflecttype getreftype() { return rltype; }
	float getreflevel() { return reflectlevel; }


private:
	reflecttype rltype;
	refractiontype rctype;
	float reflectlevel;
	float refractionlevel;
	Vector3f color;
	Mat map;
};
