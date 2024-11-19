#include "raytracer.h"

RayTracer::RayTracer(int width, int height, std::vector<Sphere> spheres) :
	imageWidth(width), imageHeight(height), sceneSpheres(spheres) {}

void RayTracer::RenderImage() const {
	std::ofstream ofs("./image.ppm", std::ios::out | std::ios::binary);
	if (!ofs.is_open()) {
		Logger::log("Failed to open PPM file for writing.");
		return;
	}

	// Write the header of the ppm file
	ofs << "P6\n" << imageWidth << " " << imageHeight << "\n255\n";

	for (int j = 0; j < imageHeight; ++j) { // Vertical loop
		for (int i = 0; i < imageWidth; ++i) { // Horizontal loop
			double x = (2.0 * ((double)i / (double)imageWidth) - 1.0) * ((double)imageWidth / (double)imageHeight);
			double y = -(2.0 * ((double)j / (double)imageHeight) - 1.0);

			Vec3 rayOrigin(0, 0, 0); // Camera is at the origin
			Vec3 rayDirection = Vec3(x, y, -1).normalize();

			if (Logger::isLoggingEnabled()) {
				Logger::log("Tracing ray from: (" + std::to_string(rayOrigin.x) + ", " 
							+ std::to_string(rayOrigin.y) + ", " 
							+ std::to_string(rayOrigin.z) + ") to direction: (" 
							+ std::to_string(rayDirection.x) + ", " 
							+ std::to_string(rayDirection.y) + ", " 
							+ std::to_string(rayDirection.z) + ")");
			}

			// Trace the ray and get the color of the intersection point
			Vec3 hitColor = traceRay(rayOrigin, rayDirection);

			// Clamp RGB values to [0, 255]
			int r = (int)(std::max(0.0, std::min(hitColor.x * 255.0, 255.0)));
			int g = (int)(std::max(0.0, std::min(hitColor.y * 255.0, 255.0)));
			int b = (int)(std::max(0.0, std::min(hitColor.z * 255.0, 255.0)));

			if (Logger::isLoggingEnabled()) {
				Logger::log("Hit color: (" + std::to_string(hitColor.x) + ", " 
							+ std::to_string(hitColor.y) + ", " 
							+ std::to_string(hitColor.z) + ") clamped to: (" 
							+ std::to_string(r) + ", " 
							+ std::to_string(g) + ", " 
							+ std::to_string(b) + ")");
			}

			// Write RGB values to the ppm file
			ofs << (unsigned char)r << (unsigned char)g << (unsigned char)b;
		}
	}

	ofs.close();
}

Vec3 RayTracer::traceRay(const Vec3& rayOrigin, const Vec3& rayDirection) const {
	double nearestT = INFINITY;
	Sphere* nearestSphere = nullptr;

	// Check intersections with all spheres
	for (const Sphere& sphere : sceneSpheres) { 
		double t0, t1; 
		if (RayTracer::intersectSphere(rayOrigin, rayDirection, sphere, t0, t1)) {
			Logger::log("Intersection found with sphere at: (" + std::to_string(sphere.center.x) + ", " 
						+ std::to_string(sphere.center.y) + ", " 
						+ std::to_string(sphere.center.z) + "), radius: " 
						+ std::to_string(sphere.radius) 
						+ " at distances: t0 = " + std::to_string(t0) 
						+ ", t1 = " + std::to_string(t1));

			if (t0 < nearestT && t0 > 0.0)
				nearestT = t0;
			else if (t1 < nearestT && t1 > 0.0)
				nearestT = t1;

			nearestSphere = const_cast<Sphere*>(&sphere); 
		}
	}

	// If no intersection was found, return background color
	if (nearestSphere == nullptr) {
		Logger::log("No intersection found, returning background color.");
		return Vec3(0.2, 0.7, 0.8);
	}

	// Compute the point of intersection on the sphere surface
	Vec3 hitPoint = rayOrigin + rayDirection * nearestT;

	// Calculate the normal at the point of intersection
	Vec3 normalAtHit = (hitPoint - nearestSphere->center).normalize();

	// Simple lighting: Lambertian shading
	double lightIntensity = 0.5;
	Vec3 lightPosition(5, 5, -10);
	Vec3 lightVector = (lightPosition - hitPoint).normalize();
	if (normalAtHit.dot(lightVector) < 0)
		lightIntensity *= -normalAtHit.dot(lightVector);

	Logger::log("Nearest intersection at: (" + std::to_string(hitPoint.x) + ", " 
				+ std::to_string(hitPoint.y) + ", " 
				+ std::to_string(hitPoint.z) + ") with sphere color: (" 
				+ std::to_string(nearestSphere->color.x) + ", " 
				+ std::to_string(nearestSphere->color.y) + ", " 
				+ std::to_string(nearestSphere->color.z) + ") and light intensity: " 
				+ std::to_string(lightIntensity));

	// Return the color of the sphere adjusted by the lighting intensity
	return nearestSphere->color * lightIntensity;
}

bool RayTracer::intersectSphere(const Vec3& rayOrigin, const Vec3& rayDirection, const Sphere& sphere, double& t0, double& t1) {
	Vec3 L = sphere.center - rayOrigin;
	double tca = L.dot(rayDirection);
	if (tca < 0.0)
		return false;

	double d2 = L.dot(L) - tca * tca;
	if (d2 > sphere.radius * sphere.radius)
		return false;

	double thc = std::sqrt(sphere.radius * sphere.radius - d2);
	t0 = tca - thc;
	t1 = tca + thc;

	return true;
}