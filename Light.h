#ifndef _LIGHT_H
#define _LIGHT_H

#include "Vector3D.hpp"

class Light	{
public:
	Light()		{}
	Light(Vector3D posit, Vector3D col): position(posit), intensity(col)	{}
	virtual ~Light()	{}

	virtual Vector3D get_direction(ShadeRec& sr) = 0;
public:
	Vector3D position;
	Vector3D intensity;
};

class PointLight: public Light	
{
public:
	PointLight() {}
	PointLight(Vector3D posit, Vector3D col): Light(posit, col)	{}
	virtual ~PointLight()	{}

	virtual Vector3D get_direction(ShadeRec& sr)
	{
		Vector3D temp(position.x()-sr.hit_point.x(), position.y()-sr.hit_point.y(), position.z()-sr.hit_point.z());
		temp.normalize();
		return temp;
	}
};

class FakeAreaLight: public Light
{
public:
	FakeAreaLight()	{}
	FakeAreaLight(Vector3D posit, Vector3D col, const float& r = 3.0): Light(posit, col), radius(r)	{}
	virtual ~FakeAreaLight() {}
	
	virtual Vector3D get_direction(ShadeRec& sr)
	{
		Vector3D newLocation;
		newLocation.setX(position.x() + radius * (2.0 * rand_float() - 1.0));
		newLocation.setY(position.y() + radius * (2.0 * rand_float() - 1.0));
		newLocation.setZ(position.z() + radius * (2.0 * rand_float() - 1.0));
		Vector3D temp(newLocation - sr.hit_point);
		temp.normalize();
		return temp;
	}
public:
	float radius;
};

#endif