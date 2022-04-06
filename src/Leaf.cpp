#include "Leaf.h"
#include "FourierSeries.h"
#include "ParametricGeometry.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

Leaf::Leaf(const glm::mat4& leafPose, const glm::vec3& growthDirection) : leafAge(0.0f), stemGirth(0.0f), 
pose(nextPose()), basePose(leafPose), direction(growthDirection), leafHeight(0.0f) {};

void Leaf::updateStemGirth(float newGirth) {
	stemGirth = newGirth;
}

void Leaf::updateLeafHeight(float newHeight) {
	leafHeight = newHeight;
}

float Leaf::grow(const WorldTime& worldTime) {
	float newAge = leafAge + Leaf::growthRate * (float)(worldTime.dt()) / 1000.0f;

	if (newAge > Leaf::maxAge) {
		newAge = Leaf::maxAge;
	}
	return newAge;
}

void Leaf::act(const WorldTime& worldTime) {
	leafAge = grow(worldTime);
	pose = nextPose();
}

glm::mat4 Leaf::nextPose() {
	glm::mat4 scaleMatrix = glm::scale(glm::vec3(0.1f + 0.9f * leafAge));
	glm::mat4 girthMatrix = glm::translate(glm::vec3(stemGirth, 0, 0));
	glm::mat4 translateMatrix = glm::translate(leafHeight * direction);
	pose = translateMatrix * basePose * girthMatrix * scaleMatrix;
	return pose;
}

glm::mat4 Leaf::getPose() const {
	return pose;
}

float Leaf::getAge() const {
	return leafAge;
}