cmake_minimum_required(VERSION 3.10)
project(raytracer)

# Set the standard to C++17 or higher
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
add_executable(raytracer 
	src/main.cpp 
	src/raytracer.h 
	src/raytracer.cpp 
	src/sphere.h 
	src/sphere.cpp 
	src/vec3.h 
	src/vec3.cpp 
	src/logger.h 
	src/logger.cpp
)

# Target properties to ensure all files are in the same directory
target_include_directories(raytracer PRIVATE ${CMAKE_SOURCE_DIR}/src)