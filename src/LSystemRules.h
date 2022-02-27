#pragma once

#include "LSystem.h"

class LSystemRule {
	virtual std::vector<OpCode> getRule() const = 0;
};

class BranchRule1 : LSystemRule {
	std::vector<OpCode> getRule() const override;
};

class BranchRule2 : LSystemRule {
	std::vector<OpCode> getRule() const override;
};