#include "Axis.h"
#include <cmath>

Axis::Axis(const glm::vec3& forwardDir, const glm::vec3& upDir, const glm::vec3& leftDir)
: forward(forwardDir), up(upDir), left(leftDir) {};

void Axis::rotateFrameHorizontal(float angle) {
	glm::vec3 forwardTemp = forward;
	float w = angle + 0.5 * M_PI;
	forward = glm::normalize(forwardTemp * std::cos(angle) + left * std::sin(angle));
	left = glm::normalize(forwardTemp * std::cos(w) + left * std::sin(w));
}

void Axis::rotateFrameVertical(float angle) {
	glm::vec3 forwardTemp = forward;
	float w = angle + 0.5 * M_PI;
	forward = glm::normalize(forwardTemp * std::cos(angle) + up * std::sin(angle));
	up = glm::normalize(forwardTemp * std::cos(w) + up * std::sin(w));
}

void Axis::rotateFrameRoll(float angle) {
	glm::vec3 upTemp = up;
	float w = angle + 0.5 * M_PI;
	up = glm::normalize(left * std::cos(angle) + upTemp * std::sin(angle));
	left = glm::normalize(left * std::cos(w) + upTemp * std::sin(w));
}