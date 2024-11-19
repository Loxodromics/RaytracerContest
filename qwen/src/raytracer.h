#pragma once

#include <vector>
#include <fstream>

#include "sphere.h"
#include "vec3.h"
#include "logger.h"

class RayTracer {
public:
	/// Constructor initializes the ray tracer with given dimensions and spheres.
	RayTracer(int width, int height, std::vector<Sphere> spheres);

	/// Renders an image based on the current setup of spheres.
	void RenderImage() const;

private:
	int imageWidth;
	int imageHeight;
	std::vector<Sphere> sceneSpheres;

	/// Traces a ray through the scene and returns the color at intersection point or background color.
	Vec3 traceRay(const Vec3& rayOrigin, const Vec3& rayDirection) const;

	/// Checks for intersection of a ray with a sphere using mathematical calculations.
	static bool intersectSphere(const Vec3& rayOrigin, const Vec3& rayDirection, const Sphere& sphere, double& t0, double& t1);
};