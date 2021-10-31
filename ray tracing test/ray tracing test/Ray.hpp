#pragma once
#include <Eigen\Dense>
#include "somemath.hpp"

using namespace Eigen;

class Ray
{
public:
	Ray()
	{
		distance = INF;
		whethertouch = false;
	}
	Ray(const Vector3f& _start)
	{
		start = _start;
		distance = INF;
		whethertouch = false;
	}
	Ray(const Vector3f& _start, const Vector3f& _directon)
	{
		start = _start;
		directon = _directon;
		distance = INF;
		whethertouch = false;
	}
	~Ray(){}
	Vector3f clcend(float _distance)
	{
		return Vector3f(start + _distance * directon);
	}
	//void clcdirection(const Vector3f& _eye, const Vector3f& _front, const Vector3f& _refUp, float _fov, Vector2f in);
	void setdirection(Vector3f _directon)
	{
		directon = _directon;
	}
	void setstart(Vector3f _start)
	{
		start = _start;
	}
	void setdistance(float _distance)
	{ 
		distance = _distance; 
		whethertouch = true;
		end = start + distance * directon;
	}
	Vector3f getend() { return end; }
	Vector3f getstart() { return start; }
	Vector3f getdirection() { return directon; }
	float getdistance() { return distance; }
	bool whetherhit() { return whethertouch; }

private:
	Vector3f start;
	Vector3f directon;
	Vector3f end;
	float distance;
	bool whethertouch;
};
