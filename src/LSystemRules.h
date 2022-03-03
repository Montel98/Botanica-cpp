#pragma once

#include "LSystemOps.h"
#include <map>
#include <memory>
#include <random>

class LSystemRule {
public:
	virtual std::vector<OpCode> getRule(std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& gen) const = 0;
};

class BranchRule1 : public LSystemRule {
public:
	std::vector<OpCode> getRule(std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& gen) const override;
};

class BranchRule2 : public LSystemRule {
public:
	std::vector<OpCode> getRule(std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& gen) const override;
};

class LSystemRules {
private:
	std::map<char, std::unique_ptr<LSystemRule>> rules;
public:
	LSystemRules();
	void addRule(char symbol, std::unique_ptr<LSystemRule> newRule);
	bool hasRule(char symbol);
	std::vector<OpCode> getRandomStringFromRule(char symbol, std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& gen);
};