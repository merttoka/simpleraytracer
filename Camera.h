#ifndef _CAMERA_H
#define _CAMERA_H

#include "Vector3D.hpp"

class Camera
{
public:
	Vector3D camera_position;
	Vector3D camera_lookat;
	Vector3D camera_top;

	Vector3D camera_direction;
	Vector3D camera_right;
	Vector3D camera_up;
public:
	Camera() {}

	Camera(const Vector3D& pos, const Vector3D& look, const Vector3D& top = Vector3D(0.0, 1.0, 0.0))
		:	camera_position(pos), camera_lookat(look), camera_top(top)
	{
		camera_direction = camera_lookat - camera_position;
		camera_direction.normalize();

		camera_right = camera_direction.cross(camera_top);
		camera_right.normalize();

		camera_up = camera_right.cross(camera_direction);
		camera_up.normalize();
	}

	Vector3D ray_direction(const double& i, const double& j) const
	{
		Vector3D image_point = camera_right * i + camera_up * j;
		return Vector3D(image_point - camera_position);
	}

};

#endif