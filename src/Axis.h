#pragma once

#include <glm/glm.hpp>

class Axis {
public:
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 left;

	Axis() = default;
	Axis(const glm::vec3& forwardDir, const glm::vec3& upDir, const glm::vec3& leftDir);
	
	void rotateFrameHorizontal(float angle);
	void rotateFrameVertical(float angle);
	void rotateFrameRoll(float angle);
};