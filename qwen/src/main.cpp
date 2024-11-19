#include <iostream>
#include "raytracer.h"
#include "logger.h"

int main(int argc, char* argv[]) {
	bool enableLogging = false;

	// Check for logging parameter
	for (int i = 1; i < argc; ++i) {
		if (std::string(argv[i]) == "--log")
			enableLogging = true;
	}

	Logger::setLogging(enableLogging);

	// Define a list of spheres
	std::vector<Sphere> sceneSpheres;

	// Add some spheres with different colors and positions
	/// We add two spheres to the scene, one purple and one yellowish.
	sceneSpheres.push_back(Sphere(Vec3(-2.0, 0.0, -4.0), 1.0, Vec3(0.5, 0.1, 0.9))); // Purple sphere
	sceneSpheres.push_back(Sphere(Vec3(2.0, 0.0, -8.0), 1.0, Vec3(0.7, 0.6, 0.4))); // Yellowish sphere

	// Render an image with the defined spheres
	int width = 500;
	int height = 300;

	RayTracer tracer(width, height, sceneSpheres);
	tracer.RenderImage();

	return 0;
}