#include "Axis.h"
#include <math.h>

#define PI 3.1415

void Axis::rotateFrameHorizontal(float angle) {
	glm::vec3 forwardTemp = forward;
	float w = angle + 0.5 * PI;
	forward = glm::normalize(forwardTemp * cos(angle) + left * sin(angle));
	left = glm::normalize(forwardTemp * cos(w) + left * sin(w));
}

void Axis::rotateFrameVertical(float angle) {
	glm::vec3 forwardTemp = forward;
	float w = angle + 0.5 * PI;
	forward = glm::normalize(forwardTemp * cos(angle) + up * sin(angle));
	up = glm::normalize(forwardTemp * cos(w) + up * sin(w));
}

void Axis::rotateFrameRoll(float angle) {
	glm::vec3 upTemp = up;
	float w = angle + 0.5 * PI;
	up = glm::normalize(left * cos(angle) + up * sin(angle));
	left = glm::normalize(left * cos(w) + up * sin(w));
}