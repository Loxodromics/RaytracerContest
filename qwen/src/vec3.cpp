#include "vec3.h"
#include <iostream>

Vec3::Vec3() : x(0.0), y(0.0), z(0.0) {}

Vec3::Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

Vec3 Vec3::operator+(const Vec3& v) const {
	return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator-(const Vec3& v) const {
	return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator*(double s) const {
	return Vec3(x * s, y * s, z * s);
}

double Vec3::dot(const Vec3& v) const {
	return x * v.x + y * v.y + z * v.z;
}

double Vec3::length() const {
	return std::sqrt(dot(*this));
}

Vec3 Vec3::normalize() const {
	double len = length();
	if (len == 0.0)
		return *this;
	else
		return *this * (1.0 / len);
}