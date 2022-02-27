#include "LSystem.h"

std::vector<OpCode> LSystem::buildString(std::vector<OpCode> startString, int depth) {

	std::vector<OpCode> finalString = startString;

	for (auto currentDepth = 0; currentDepth < depth; currentDepth++) {

		std::vector<OpCode> currentString;

		for (auto i = 0; i < finalString.size(); i++) {
			OpCode& currentOp = startString[i];

			if (rules.hasRule(currentOp.symbol)) {

				std::vector<OpCode> ruleSubString = rules.getRandomStringFromRule(currentOp.symbol);
				currentString.insert(currentString.end(), ruleSubString.begin(), ruleSubString.end());
			}
			else {
				currentString.push_back(currentOp);
			}
		}

		finalString = std::move(currentString);
	}

	return finalString;
}