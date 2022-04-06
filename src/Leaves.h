#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <math.h>
#include "ParametricGeometry.h"
#include "Object3D.h"
#include "Entity.h"
#include "WorldTime.h"
#include "LSystemParams.h"
#include "Leaf.h"
#include "LeafBuilder.h"

class Leaves : public Entity {
private:
	std::vector<Leaf> leaves;
public:
	Leaves();
	Mesh generatePlane() const;
	void act(const WorldTime& worldTime) override;
	int addLeaf(const glm::mat4& pose, const glm::vec3& direction);
	std::vector<int> addLeaves(const LSystemParams& lParams, int noLeaves);
	Leaf& getLeaf(int index);
	void updateLeafAttributes(int index);
};

namespace LeafBuilder {
glm::mat4 projectToLeafAxis(const Axis& axis, const glm::vec3& position);
glm::mat4 projectToLeafAxis2(const Axis& axis, const glm::vec3& position);
}