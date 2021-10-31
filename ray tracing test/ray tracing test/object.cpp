#include <Eigen\Dense>
#include <iostream>
#include <random>
#include <time.h>
#include "object.hpp"
#include "camera.hpp"
#include "material.hpp"

using namespace Eigen;

Ball::Ball()
{

}
Ball::~Ball()
{

}
Ball::Ball(const Vector3f& _center, float _R)
{
	center = _center;
	R = _R;
}
Ray Ball::intersect(Ray inray)
{
	Vector3f v = inray.getstart() - center;
	float a0 = v.transpose()*v - R * R;
	Vector3f direction = inray.getdirection();
	float DdotV = direction.transpose()*v;
	if (DdotV <= 0.0)
	{
		float discr = DdotV * DdotV - a0;
		if (discr >= 0.0)
		{
			float distance = -DdotV - sqrt(discr);
			inray.setdistance(distance);
		}
	}
	return inray;
}

Ray Ball::reflect(Ray inray, int materialreftype, float reflevel)
{
	Ray outray;
	outray.setstart(inray.getend());
	Vector3f n = inray.getend() - center;
	Vector3f ballreflect = inray.getdirection() - (2 * float(inray.getdirection().transpose()*n))*n;
	// 材质处理.....
	outray.setdirection(ballreflect);
	return outray;
}

Panel::Panel()
{

}
Panel::Panel(const Vector3f& _center, const Vector3f& _N, const Vector3f& _nUp, Vector2f _size)
{
	center = _center;
	N = _N;
	D0 = N.transpose()*center;
	size = _size;
	nUp = _nUp;
	right = nUp.cross(N); 
	up = right.cross(N);	
}
Panel::~Panel()
{

}
Ray Panel::intersect(Ray inray)
{
	float a = (N.transpose()*inray.getdirection());
	if (a < 0)
	{
		float distance = -(N.transpose()*inray.getstart() - D0) / a;
		if (distance < INF)
		{
			Vector3f end = inray.clcend(distance);
			//std::cout << end << std::endl;
			float dd = sqrt(float((end - center).transpose()*(end - center)));
			Vector3f po = (end - center).normalized();
			float angle = acosf(float(up.transpose()*po) / float(sqrt(up.transpose()*up)*sqrt(po.transpose()*po))) ;
			float w = dd * cos(angle);
			float h = dd * sin(angle);
			if (((w <= size[0] / 2.0) && (w >= -size[0] / 2.0)) && ((h <= size[1] / 2.0) && (h >= -size[1] / 2.0)))
			{
				inray.setdistance(distance);
			}
		}
	}
	return inray;
}


default_random_engine e;

Ray Panel::reflect(Ray inray, int materialreftype,float reflevel)
{
	Ray outray;
	outray.setstart(inray.getend());
	Vector3f ballreflect = inray.getdirection() - (2.0 * float(inray.getdirection().transpose()*N))*N;
	
	uniform_real_distribution<double> u(0, 1 - reflevel);
	Vector3f rreflect = Vector3f(ballreflect[0] * (u(e) + 1), ballreflect[1] * (u(e) + 1), ballreflect[2] * (u(e) + 1)).normalized();
	// 材质处理.....
	outray.setdirection(rreflect);
	return outray;
}