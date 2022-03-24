#include "TreeString.h"

#define PI 3.1415

// Defines the start L-String for specific bonsai appearance
std::vector<OpCode> TreeString::getLString(Generator& gen) {
	std::vector<OpCode> startString;

	startString.push_back(GEN_STEM());
	startString.push_back(ROT_P(randomNormal(0.0f, PI / 32.0f, gen), randomNormal(0.0f, PI / 32.0f, gen)));
	startString.push_back(GEN_STEM());
	startString.push_back(ROT_P(randomNormal(0.0f, PI / 32.0f, gen), randomNormal(0.0f, PI / 32.0f, gen)));
	startString.push_back(GEN_STEM());
	startString.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	startString.push_back(GEN_STEM());

	startString.push_back(S_PUSH(false));
	startString.push_back(ROT_P(randomFloat(gen) * 2.0f * PI, -PI / 2.0f));
	startString.push_back(BRANCH_1());
	startString.push_back(S_POP());

	startString.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	startString.push_back(GEN_STEM());
	startString.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	startString.push_back(GEN_STEM());
	startString.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	startString.push_back(GEN_STEM());
	startString.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	startString.push_back(GEN_STEM());

	startString.push_back(S_PUSH(false));
	startString.push_back(ROT_P(randomFloat(gen) * 2.0f * PI, -PI / 2.0f));
	startString.push_back(BRANCH_1());
	startString.push_back(S_POP());

	startString.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	startString.push_back(GEN_STEM());
	startString.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	startString.push_back(GEN_STEM());
	startString.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	startString.push_back(GEN_STEM());
	startString.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	startString.push_back(GEN_STEM());

	startString.push_back(S_PUSH(false));
	startString.push_back(ROT_P(randomFloat(gen) * 2.0f * PI, -PI / 2.0f));
	startString.push_back(BRANCH_1());
	startString.push_back(S_POP());

	startString.push_back(GEN_STEM());
	startString.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	startString.push_back(GEN_STEM());
	startString.push_back(ROT_P(randomNormal(0.0f, PI / 16.0f, gen), randomNormal(0.0f, PI / 16.0f, gen)));
	startString.push_back(BRANCH_2());

	return startString;
}