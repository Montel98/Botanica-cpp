#include "Camera.h"
#include <iostream>
#define PI 3.1415f

Camera::Camera(const glm::vec3& eye, const glm::vec3& centre, const glm::vec3& up) : origin(eye), target(centre), vertical(glm::normalize(up)) {};

glm::mat4 Camera::getCameraMatrix() const {

	//return glm::lookAt(origin, target, vertical);
	return glm::lookAt(glm::vec3(-0.02, -1.5, 0.15), glm::vec3(-0.02, 0.0, 0.15),  glm::vec3(0.0, 0.0, 1.0));
}

glm::mat4 Camera::getPerspectiveMatrix(double aspectRatio) const {
	return glm::perspective(PI * 0.25, 1.0, /*zNear, zFar*/0.1, 100.0);
}

glm::vec3 Camera::getPosition() const {
	return origin;
}