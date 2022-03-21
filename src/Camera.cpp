#include "Camera.h"
#include <iostream>
#define PI 3.1415f

Camera::Camera(const glm::vec3& eye, const glm::vec3& centre, const glm::vec3& up) : origin(glm::normalize(eye)), target(glm::normalize(centre)), vertical(glm::normalize(up)) {};

glm::mat4 Camera::getCameraMatrix() const {
	//std::cout << "origin: " << origin.x << "," << origin.y << "," << origin.z << "\n";
	//std::cout << "target: " << target.x << "," << target.y << "," << target.z << "\n";
	//std::cout << "vertical: " << vertical.x << "," << vertical.y << "," << vertical.z << "\n";
	//return glm::lookAt(origin, target, vertical);
	return glm::lookAt(glm::vec3(-0.02, -1.0, 0.15), glm::vec3(-0.02, 0.0, 0.15),  glm::vec3(0.0, 0.0, 1.0));
}

glm::mat4 Camera::getPerspectiveMatrix(double aspectRatio) const {
	return glm::perspective(PI * 0.25, 1.0, /*zNear, zFar*/0.1, 100.0);
}