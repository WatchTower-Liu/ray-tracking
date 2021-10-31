#include <Eigen\Dense>
#include "camera.hpp"
#include "Ray.hpp"

using namespace Eigen;

Camera::Camera()
{

}
Camera::~Camera()
{

}
Camera::Camera(const Vector3f& _eye, const Vector3f& _front, const Vector3f& _refUp, Vector2i _imgsize, float _fov)
{
	eye = _eye;
	front = _front;
	refUp = _refUp;
	imgsize = _imgsize;
	ufov = (_fov / imgsize[1]) * 20;
	rfov = (_fov / imgsize[0]) * 20;
	right = front.cross(refUp); 
	up = right.cross(front);	
}
Ray Camera::clcdircetion(Vector2f in)
{
	//cout << fovScale << endl;
	Vector3f r = right * float(float(in[0] - 0.5f) * rfov);
	Vector3f u = up * float(float(in[1] - 0.5f) * ufov);
	Vector3f tmp = front + r + u;
	float Mo = sqrt(tmp[0] * tmp[0] + tmp[1] * tmp[1] + tmp[2] * tmp[2]);
	tmp[0] = tmp[0] / Mo; tmp[1] = tmp[1] / Mo; tmp[2] = tmp[2] / Mo;
	Ray out(eye, tmp);
	return out;
}