#pragma once

#include "Axis.h"

using EntityId = unsigned int;

struct StemNode {
	EntityId current;
	StemNode* prev;
	std::vector<std::unique_ptr<StemNode>> next;
};

struct LSystemParams {
	glm::vec3 position;
	Axis axis;
	int count;
	int depth;
	bool connectParent;
	int stringIndex;
	float radiusStart;
	float radiusEnd;
	int branchLength;
	int level;
};

struct LSystemStackFrame {
	LSystemParams lParams;
	StemNode* node;
};