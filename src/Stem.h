#pragma once

#include "Object3D.h"
#include "Mesh.h"
#include "BezierLinear.h"
#include "glm/glm.hpp"
#include "ParametricGeometry.h"
#include "LSystemParams.h"
#include "EntityManager.h"
#include <random>
#include <memory>

class Stem : public Entity {
private:
	EntityManager& entityManager;
public:
	//Object3D worldObject;
	LSystemParams lParams;
	Stem(EntityManager &manager, const LSystemParams &lSystemParams, const StemNode* prevStem);
	Mesh generateMesh(const StemNode* prevStem);
	Geometry generateGeometry(const LSystemParams &lParams, const StemNode* prevStem);
};