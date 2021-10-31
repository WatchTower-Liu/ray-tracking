#pragma once
#include <Eigen\Dense>
#include "Ray.hpp"

using namespace Eigen;

class Camera
{
public:
	Camera();
	Camera(const Vector3f& _eye, const Vector3f& _front, const Vector3f& _refUp, Vector2i _imgsize, float _fov);
	~Camera();
	Ray clcdircetion(Vector2f in);

private:
	Vector3f eye;
	Vector3f front;
	Vector3f refUp;
	Vector3f right;
	Vector3f up;
	Vector2i imgsize;
	float rfov;
	float ufov;
};
