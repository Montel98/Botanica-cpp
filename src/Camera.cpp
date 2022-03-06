#include "Camera.h"

#define PI 3.1415f

Camera::Camera(const glm::vec3& eye, const glm::vec3& centre, const glm::vec3& up) : origin(eye), target(centre), vertical(up) {};

glm::mat4 Camera::getCameraMatrix() const {
	return glm::lookAt(origin, target, vertical);
}

glm::mat4 Camera::getPerspectiveMatrix() const {
	return glm::perspective(PI * 0.25, 0.0, zNear, zFar);
}