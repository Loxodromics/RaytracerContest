#pragma once

#include "vec3.h"

class Sphere {
public:
	Vec3 center;
	double radius;
	Vec3 color;

	Sphere(Vec3 _center, double _radius, Vec3 _color);
};