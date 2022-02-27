#include "LSystemRules.h"
#include <utility>

#define PI 3.1415f

// Rule #1
// Later define in a text file for better flexibility + readability
std::vector<OpCode> BranchRule1::getRule() const {
	std::vector<OpCode> rule;

	rule.push_back(ROT_P(0.0f, PI / 16.0f));
	rule.push_back(GEN_STEM());
	rule.push_back(ROT_P(PI / 16.0f, PI / 16.0f));
	rule.push_back(GEN_STEM());
	rule.push_back(ROT_P(PI / 16.0f, PI / 16.0f));
	rule.push_back(GEN_STEM());
	rule.push_back(ROT_P(PI / 16.0f, PI / 16.0f));
	rule.push_back(GEN_STEM());

	rule.push_back(S_PUSH(false));
	rule.push_back(ROT_P(PI / 6.0f, 0.0f));
	rule.push_back(BRANCH_1());
	rule.push_back(S_POP());

	rule.push_back(ROT_P(PI / 16.0f, PI / 16.0f));
	rule.push_back(GEN_STEM());
	rule.push_back(ROT_P(PI / 16.0f, PI / 16.0f));
	rule.push_back(GEN_STEM());

	rule.push_back(S_PUSH(false));
	rule.push_back(ROT_P(PI / 6.0f, 0.0f));
	rule.push_back(BRANCH_1());
	rule.push_back(S_POP());

	rule.push_back(ROT_P(PI / 16.0f, PI / 16.0f));
	rule.push_back(GEN_STEM());

	rule.push_back(S_PUSH(true));
	rule.push_back(ROT_H(PI / 5.0f));
	rule.push_back(GEN_STEM());
	rule.push_back(S_POP());

	rule.push_back(S_PUSH(true));
	rule.push_back(ROT_H(PI / 5.0f));
	rule.push_back(GEN_STEM());
	rule.push_back(S_POP());

	return rule;
}

// Rule #2
std::vector<OpCode> BranchRule2::getRule() const {
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

std::vector<OpCode> LSystemRules::getRandomStringFromRule(char symbol) {
	return rules[symbol]->getRule();
}