#include "LSystemRules.h"
#include "RandomLCG.h"

using Generator = std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>;

namespace TreeString {
	std::vector<OpCode> getLString(Generator& gen);
}