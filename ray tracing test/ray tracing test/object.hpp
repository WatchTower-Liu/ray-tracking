#pragma once
#include <Eigen\Dense>
#include "Ray.hpp"
//#include "material.hpp"


using namespace Eigen;

class Object
{
public:
	Object(){}
	~Object(){}
	virtual Ray intersect(Ray inray) = 0;
	virtual Ray reflect(Ray inray, int materialreftype, float reflevel) = 0;
	virtual Vector3f getN() = 0;
	virtual Vector3f getcenter() = 0;
	virtual Vector3f getup() = 0;
	virtual Vector3f getright() = 0;
	virtual Vector2f getsize() = 0;

private:

};

class Ball : public Object
{
public:
	Ball();
	Ball(const Vector3f& _center, float _R);
	~Ball();
	Ray intersect(Ray inray);
	Ray reflect(Ray inray, int materialreftype, float reflevel);
	Vector3f getN() { return Vector3f(0,0,0); }
	Vector3f getcenter() { return center; }
	Vector3f getup() { return Vector3f(0, 0, 0); }
	Vector3f getright() { return Vector3f(0, 0, 0); }
	Vector2f getsize() { return Vector2f(0, 0); }
private:
	Vector3f center;
	float R;
};

class Panel : public Object
{
public:
	Panel();
	Panel(const Vector3f& _center, const Vector3f& _N, const Vector3f& _nUp, Vector2f _size);
	~Panel();
	Ray intersect(Ray inray);
	Ray reflect(Ray inray, int materialreftype, float reflevel);
	Vector3f getN() { return N; }
	Vector3f getcenter() { return center; }
	Vector3f getup() { return up; }
	Vector2f getsize() { return size; }
	Vector3f getright() { return right; }
private:
	Vector3f center;
	Vector3f N;
	Vector3f nUp;
	Vector2f size;
	Vector3f right;
	Vector3f up;
	float D0;
};
