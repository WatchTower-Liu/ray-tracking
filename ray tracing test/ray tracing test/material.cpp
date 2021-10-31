#include <Eigen\Dense>
#include <iostream>
#include <memory>
#include "material.hpp"
#include "object.hpp"


Vector3f Material::sample(pair<shared_ptr<Object>, string>& obj, Ray& position, float scale)   //∆Â≈Ã≤‚ ‘
{
	float d = abs((floor(position.getend()[0] * scale) + floor(position.getend()[1] * scale)));
	d = fmod(d, 2);
	return  d < 1 ? Vector3f(0, 0, 0) : color;
}

Vector3f Material::sample1(pair<shared_ptr<Object>, string>& obj, Ray& position, float scale)   //≤ƒ÷ —’…´
{
	if (position.whetherhit())
		return color;
	return Vector3f(0,0,0);
}

Vector3f Material::matmap(pair<shared_ptr<Object>, string>& obj, Ray& position, float scale)
{
	float dd = sqrt(float((obj.first->getcenter() - position.getend()).transpose()*(obj.first->getcenter() - position.getend())));
	Vector3f po = (obj.first->getcenter() - position.getend()).normalized();
	float angle = acosf(float(obj.first->getright().transpose()*po) / sqrt(obj.first->getright().transpose()*obj.first->getright())*sqrt(po.transpose()*po));
	float angle34 = acosf(float(obj.first->getup().transpose()*po) / sqrt(obj.first->getup().transpose()*obj.first->getup())*sqrt(po.transpose()*po));
	float w = dd * cos(angle);
	float h;
	if (angle34 > PI / 2)
		h = -dd * sin(angle);
	else 
		h = dd * sin(angle);
	Vector2f size = obj.first->getsize();
	float panellocalx = 1 - (size[1] / 2 + w ) / size[1];
	float panellocaly = (size[0] / 2 - h) / size[0];
	Vector3f color;
	color[0] = map.at<Vec3b>(map.rows*panellocaly, map.cols*panellocalx)[0];
	color[1] = map.at<Vec3b>(map.rows*panellocaly, map.cols*panellocalx)[1];
	color[2] = map.at<Vec3b>(map.rows*panellocaly, map.cols*panellocalx)[2];
	return color;
}

Material::Material()
{
	materialfun[0] = (&Material::sample1);
	materialfun[1] = (&Material::sample);
	materialfun[2] = (&Material::matmap);
}

Material::Material(reflecttype _reflecttype, float _reflectlevel,
	refractiontype _refractiontype, float _refractionlevel, Vector3f _color)
	:rltype(_reflecttype), reflectlevel(_reflectlevel), rctype(_refractiontype),
	refractionlevel(_refractionlevel), color(_color)
{
	materialfun[0] = (&Material::sample1);
	materialfun[1] = (&Material::sample);
	materialfun[2] = (&Material::matmap);
}
