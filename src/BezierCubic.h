#pragma once

#include <glm/glm.hpp>
#include <cmath>

class BezierCubic {
private:
	glm::vec3 p0, p1, p2, p3;
public:
	BezierCubic(glm::vec3 &c0, glm::vec3 &c1, glm::vec3 &c2, glm::vec3 &c3);
	glm::vec3 eval(float t) const;
	glm::vec3 derivative(float t) const;
};