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

	Mesh generateMesh(const LSystemParams& lParams, const StemNode* prevStem);
	Geometry generateGeometry(const LSystemParams& lParams, const StemNode* prevStem);
public:
	static constexpr float maxLength = 1.0f;
	static constexpr float maxGirth = 1.0f;

	//LSystemParams lParams;
	Stem(EntityManager &manager, const LSystemParams &lSystemParams, const StemNode* prevStem);
	void act(const WorldTime& worldTime) override;
	float grow(const WorldTime& worldTime) const;
	void updateGirth(float newGirth);
	bool isMaxLength() const;

	float stemLength;
	float stemGirth;
	float growthRate;
	LSystemParams lParams;
};