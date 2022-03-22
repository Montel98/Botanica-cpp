#include "LSystem.h"
#include <iostream>

std::vector<OpCode> LSystem::buildString(std::vector<OpCode> startString, int depth, std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& gen) {
	std::vector<OpCode> finalString = startString;

	for (auto currentDepth = 0; currentDepth < depth; currentDepth++) {
		std::vector<OpCode> currentString;

		for (auto i = 0; i < finalString.size(); i++) {
			OpCode& currentOp = startString[i];

			if (rules.hasRule(currentOp.symbol)) {

				std::vector<OpCode> ruleSubString = rules.getRandomStringFromRule(currentOp.symbol, gen);
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

StemNode LSystem::buildTree(const std::vector<OpCode>& lString, const LSystemParams& params, int endIndex, EntityManager& entityManager) {

	if (endIndex >= lString.size()) {
		endIndex = lString.size() - 1;
	}

	StemNode root;
	root.prev = nullptr;

	LSystemStackFrame currentFrame = LSystemStackFrame{params, nullptr};
	std::vector<LSystemStackFrame> stackFrame;

	bool rootVisited = false;

	for (int index = params.stringIndex; index <= endIndex; index++) {

		const OpCode& currentOp = lString[index];

		if (currentOp.symbol == '0') {

			unsigned int stemId = entityManager.addEntity(std::make_unique<Stem>(
				Stem(entityManager, currentFrame.lParams, currentFrame.node))
			);
			StemNode newNode{stemId, currentFrame.node};

			if (rootVisited) {
				currentFrame.node->next.push_back(std::make_unique<StemNode>(std::move(newNode)));
				currentFrame.node->next.back()->prev = currentFrame.node;
				currentFrame.node = currentFrame.node->next.back().get();
			}
			else {
				currentFrame.node = &root;
				currentFrame.node->current = stemId;
				rootVisited = true;
			}
		}

		currentFrame = currentOp.params->operator()(currentFrame, stackFrame);
	}

	return root;
}