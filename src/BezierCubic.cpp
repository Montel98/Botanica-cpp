#include "BezierCubic.h"

BezierCubic::BezierCubic(glm::vec3 &c0, glm::vec3 &c1, glm::vec3 &c2, glm::vec3 &c3) :
p0(c0), p1(c1), p2(c2), p3(c3) {};

glm::vec3 BezierCubic::eval(float t) const {
	glm::vec3 term1 = (float)pow((1 - t), 3) * p0;
	glm::vec3 term2 = (3 * t * (float)pow(1 - t, 2)) * p1;
	glm::vec3 term3 = (3 * (1 - t) * (float)pow(t, 2)) * p2;
	glm::vec3 term4 = (float)pow(t, 3) * p3;

	return term1 + term2 + term3 + term4;
}

glm::vec3 BezierCubic::derivative(float t) const {
	glm::vec3 term1 = (3 * (float)pow((1 - t), 2)) * (p1 - p0);
	glm::vec3 term2 = (6 * (1 - t) * t) * (p2 - p1);
	glm::vec3 term3 = (3 * (float)pow(t, 2)) * (p3 - p2);

	return term1 + term2 + term3;
}