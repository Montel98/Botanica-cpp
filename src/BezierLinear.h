#pragma once

#include <glm/vec3.hpp>

class BezierLinear {
private:
	glm::vec3 p0;
	glm::vec3 p1;
	glm::vec3 gradient;

public:
	BezierLinear(const glm::vec3 &c0, const glm::vec3 &c1);
	glm::vec3 eval(const float &t) const;
	glm::vec3 derivative(const float &t) const;
};