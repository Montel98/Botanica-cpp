#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
	glm::vec3 origin;
	glm::vec3 target;
	glm::vec3 vertical;
	glm::vec3 left;
	glm::vec3 direction;

	double zNear, zFar;
public:
	Camera(const glm::vec3& eye, const glm::vec3& centre, const glm::vec3& up);
	glm::mat4 getCameraMatrix();
	glm::mat4 getPerspectiveMatrix();
};