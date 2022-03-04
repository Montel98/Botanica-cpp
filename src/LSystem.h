#pragma once

#include <string>
#include <vector>
#include <memory>
#include <random>
#include <glm/glm.hpp>
#include "LSystemOps.h"
#include "LSystemRules.h"
#include "LSystemParams.h"
#include "EntityManager.h"
#include "Stem.h"

class LSystem {
private:
	LSystemRules rules;
public:
	LSystem() = default;
	std::vector<OpCode> buildString(std::vector<OpCode> startString, int depth, std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& gen);
	StemNode buildTree(const std::vector<OpCode>& lString, const LSystemParams& params, int endIndex, EntityManager& entityManager);
};