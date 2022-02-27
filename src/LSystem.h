#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "LSystemOps.h"
#include "LSystemRules.h"

class LSystem {
private:
	LSystemRules rules;
public:
	LSystem() = default;
	std::vector<OpCode> buildString(std::vector<OpCode> startString, int depth);
};