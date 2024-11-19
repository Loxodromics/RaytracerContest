#pragma once

#include <cmath>

class Vec3 {
public:
	double x, y, z;

	Vec3();
	Vec3(double _x, double _y, double _z);

	Vec3 operator+(const Vec3& v) const;
	Vec3 operator-(const Vec3& v) const;
	Vec3 operator*(double s) const;
	double dot(const Vec3& v) const;
	double length() const;
	Vec3 normalize() const;
};