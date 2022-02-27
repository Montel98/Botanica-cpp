#pragma once

#include <glm/glm.hpp>

class Axis {
public:
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 left;
	
	void rotateFrameHorizontal(float angle);
	void rotateFrameVertical(float angle);
	void rotateFrameRoll(float angle);
};