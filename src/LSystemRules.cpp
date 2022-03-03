#include "LSystemRules.h"
#include "RandomLCG.h"
#include <utility>

#define PI 3.1415f

// Rule #1
// Later define in a text file for better flexibility + readability
std::vector<OpCode> BranchRule1::getRule(std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& gen) const {
	std::vector<OpCode> rule;

	rule.push_back(ROT_P(0.0f, PI / 16.0f));
	rule.push_back(GEN_STEM());
	rule.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	rule.push_back(GEN_STEM());
	rule.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	rule.push_back(GEN_STEM());
	rule.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	rule.push_back(GEN_STEM());

	rule.push_back(S_PUSH(false));
	rule.push_back(ROT_P(PI / 6.0f, 0.0f));
	rule.push_back(BRANCH_1());
	rule.push_back(S_POP());

	rule.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	rule.push_back(GEN_STEM());
	rule.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f ,PI / 16.0f, gen)));
	rule.push_back(GEN_STEM());

	rule.push_back(S_PUSH(false));
	rule.push_back(ROT_P(PI / 6.0f, 0.0f));
	rule.push_back(BRANCH_1());
	rule.push_back(S_POP());

	rule.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	rule.push_back(GEN_STEM());

	rule.push_back(S_PUSH(true));
	rule.push_back(ROT_H(((PI / 5.0f) + 2.0 * randomFloat(gen) * (PI / 8.0f)) - (PI / 4.0f)));
	rule.push_back(GEN_STEM());
	rule.push_back(S_POP());

	rule.push_back(S_PUSH(true));
	rule.push_back(ROT_H(-((PI / 5.0f) + 2.0 * randomFloat(gen) * (PI / 8.0f)) - (PI / 4.0f)));
	rule.push_back(GEN_STEM());
	rule.push_back(S_POP());

	return rule;
}

// Rule #2
std::vector<OpCode> BranchRule2::getRule(std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& gen) const {
	std::vector<OpCode> rule;

	rule.push_back(S_PUSH(true));
	rule.push_back(ROT_P(PI / 6.0f, 0.0f));
	rule.push_back(BRANCH_1());
	rule.push_back(S_POP());

	rule.push_back(S_PUSH(true));
	rule.push_back(ROT_P(-PI / 6.0f, 0.0f));
	rule.push_back(BRANCH_1());
	rule.push_back(S_POP());

	return rule;
}

LSystemRules::LSystemRules() {
	addRule('1', std::make_unique<BranchRule1>());
	addRule('2', std::make_unique<BranchRule2>());
}

void LSystemRules::addRule(char symbol, std::unique_ptr<LSystemRule> newRule) {
	rules.emplace(std::make_pair(symbol, std::move(newRule)));
}

bool LSystemRules::hasRule(char symbol) {
	return rules.find(symbol) != rules.end();
}

std::vector<OpCode> LSystemRules::getRandomStringFromRule(char symbol, std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& gen) {
	return rules[symbol]->getRule(gen);
}