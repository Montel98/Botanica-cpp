#pragma once

#include "Object3D.h"
#include "Mesh.h"
#include "BezierLinear.h"
#include "glm/glm.hpp"
#include "ParametricGeometry.h"
#include "LSystemParams.h"
#include "EntityManager.h"
#include "StemBuilder.h"
#include "Leaves.h"
#include "Leaf.h"
#include <random>
#include <memory>
#include <vector>

class Stem : public Entity {
private:
	EntityManager& entityManager;

	Mesh generateMesh(const LSystemParams& lParams, const StemNode* prevStem);
	Geometry generateGeometry(const LSystemParams& lParams, const StemNode* prevStem);
	float grow(const WorldTime& worldTime) const;

	std::vector<int> leafIndices;
	float stemLength;
	float stemGirth;
	float growthRate;
public:
	static constexpr float maxLength = 1.0f;
	static constexpr float maxGirth = 1.0f;

	//LSystemParams lParams;
	Stem(EntityManager &manager, const LSystemParams &lSystemParams, const StemNode* prevStem);
	void act(const WorldTime& worldTime) override;
	void addLeaves(Leaves& leaves, int count);
	void updateGirth(float newGirth);
	void updateLeaves(Leaves& leaves);
	bool isMaxLength() const;

	LSystemParams lParams;
	StemBuilder::StemBodyRadius radiusFunc;
};