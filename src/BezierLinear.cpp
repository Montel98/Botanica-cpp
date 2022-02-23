#include "BezierLinear.h"

BezierLinear::BezierLinear(const glm::vec3 &c0, const glm::vec3 &c1) {
	p0 = c0;
	p1 = c1;
	gradient = p1 - p0;
}

glm::vec3 BezierLinear::eval(const float &t) const {
	return ((1 - t) * p0) + (t * p1);
}

glm::vec3 BezierLinear::derivative(const float &t) const {
	return gradient;
}