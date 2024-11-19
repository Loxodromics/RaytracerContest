#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// Define a 3D vector class
class Vec3 {
public:
    double x, y, z;
    
    Vec3() : x(0.0), y(0.0), z(0.0) {}
    Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

    // Vector addition
    Vec3 operator+(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }

    // Vector subtraction
    Vec3 operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }

    // Scalar multiplication
    Vec3 operator*(double s) const {
        return Vec3(x * s, y * s, z * s);
    }

    // Dot product
    double dot(const Vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    // Vector length (magnitude)
    double length() const {
        return std::sqrt(dot(*this));
    }

    // Normalize the vector to have a magnitude of 1
    Vec3 normalize() const {
        double len = length();
        if(len == 0.0) 
            return *this;
        else 
            return *this * (1.0 / len);
    }
};

// Define a sphere class
class Sphere {
public:
    Vec3 center;
    double radius;
    Vec3 color;

    Sphere(Vec3 _center, double _radius, Vec3 _color) : center(_center), radius(_radius), color(_color) {}
};

// Function to check for intersection of a ray with a sphere
bool intersectSphere(const Vec3& rayOrigin, const Vec3& rayDirection, const Sphere& sphere, double& t0, double& t1) {
    Vec3 L = sphere.center - rayOrigin;
    double tca = L.dot(rayDirection);
    if(tca < 0.0)
        return false;

    double d2 = L.dot(L) - tca * tca;
    if(d2 > sphere.radius*sphere.radius)
        return false;

    double thc = std::sqrt(sphere.radius*sphere.radius - d2);
    t0 = tca - thc;
    t1 = tca + thc;

    return true;
}

// Function to trace a ray
Vec3 traceRay(const Vec3& rayOrigin, const Vec3& rayDirection, const std::vector<Sphere>& spheres) {
    double nearestT = INFINITY;
    Sphere* nearestSphere = nullptr;

    // Check intersections with all spheres
    for(const Sphere& sphere : spheres) { // Use const reference here
        double t0, t1; // Distances from ray origin to intersection points
        if(intersectSphere(rayOrigin, rayDirection, sphere, t0, t1)) {
            std::cout << "Intersection found with sphere at: (" 
                      << sphere.center.x << ", " << sphere.center.y << ", " 
                      << sphere.center.z << "), radius: " << sphere.radius 
                      << " at distances: t0 = " << t0 << ", t1 = " << t1 
                      << std::endl;

            if(t0 < nearestT && t0 > 0.0)
                nearestT = t0;
            else if(t1 < nearestT && t1 > 0.0)
                nearestT = t1;

            nearestSphere = const_cast<Sphere*>(&sphere); // Cast away const qualifier
        }
    }

    // If no intersection was found, return background color
    if(nearestSphere == nullptr) {
        std::cout << "No intersection found, returning background color." << std::endl;
        return Vec3(0.2, 0.7, 0.8); // Sky blue background
    }

    // Compute the point of intersection on the sphere surface
    Vec3 hitPoint = rayOrigin + rayDirection * nearestT;

    // Calculate the normal at the point of intersection
    Vec3 normalAtHit = (hitPoint - nearestSphere->center).normalize();

    // Simple lighting: Lambertian shading
    double lightIntensity = 0.5;
    Vec3 lightPosition(5, 5, -10);
    Vec3 lightVector = (lightPosition - hitPoint).normalize();
    if(normalAtHit.dot(lightVector) < 0)
        lightIntensity *= -normalAtHit.dot(lightVector);

    std::cout << "Nearest intersection at: (" 
              << hitPoint.x << ", " << hitPoint.y << ", " 
              << hitPoint.z << ") with sphere color: (" 
              << nearestSphere->color.x << ", " << nearestSphere->color.y << ", " 
              << nearestSphere->color.z << ") and light intensity: " 
              << lightIntensity << std::endl;

    // Return the color of the sphere adjusted by the lighting intensity
    return nearestSphere->color * lightIntensity;
}

// Function to render an image in PPM format with debugging output
void renderImage(const std::vector<Sphere>& spheres, int width, int height) {
    std::ofstream ofs("./image.ppm", std::ios::out | std::ios::binary);
    if(!ofs.is_open()) {
        std::cerr << "Failed to open PPM file for writing." << std::endl;
        return;
    }

    // Write the header of the ppm file
    ofs << "P6\n" << width << " " << height << "\n255\n";

    for(int j = 0; j < height; ++j) { // Vertical loop
        for(int i = 0; i < width; ++i) { // Horizontal loop
            double x = (2.0 * ((double)i / (double)width) - 1.0) * ((double)width / (double)height);
            double y = -(2.0 * ((double)j / (double)height) - 1.0);
            
            Vec3 rayOrigin(0, 0, 0); // Camera is at the origin
            // Vec3 rayDirection(x, y, -1).normalize();
            Vec3 rayDirection = Vec3(x, y, -1).normalize();
            
            std::cout << "Tracing ray from: (" 
                      << rayOrigin.x << ", " << rayOrigin.y << ", " 
                      << rayOrigin.z << ") to direction: (" 
                      << rayDirection.x << ", " << rayDirection.y << ", " 
                      << rayDirection.z << ")" << std::endl;

            // Trace the ray and get the color of the intersection point
            Vec3 hitColor = traceRay(rayOrigin, rayDirection, spheres);

            // Clamp RGB values to [0, 255]
            int r = (int)(std::max(0.0, std::min(hitColor.x * 255.0, 255.0)));
            int g = (int)(std::max(0.0, std::min(hitColor.y * 255.0, 255.0)));
            int b = (int)(std::max(0.0, std::min(hitColor.z * 255.0, 255.0)));

            std::cout << "Hit color: (" 
                      << hitColor.x << ", " << hitColor.y << ", " 
                      << hitColor.z << ") clamped to: (" 
                      << r << ", " << g << ", " << b << ")" << std::endl;

            // Write RGB values to the ppm file
            ofs << (unsigned char)r << (unsigned char)g << (unsigned char)b;
        }
    }

    ofs.close();
}

int main() {
    // Define a list of spheres
    std::vector<Sphere> spheres;

    // Add some spheres with different colors and positions
    spheres.push_back(Sphere(Vec3(-2, 0, -4), 1.0, Vec3(0.5, 0.1, 0.9))); // Purple sphere
    spheres.push_back(Sphere(Vec3(2, 0, -8), 1.0, Vec3(0.7, 0.6, 0.4))); // Yellowish sphere

    // Render an image with the defined spheres
    int width = 500;
    int height = 300;

    renderImage(spheres, width, height);

    return 0;
}