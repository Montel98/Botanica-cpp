#include "RandomLCG.h"
#include <math.h>

#define PI 3.1415

float randomFloat(std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& generator) {
	return(generator() / 2147483647.0f);
}

float randomNormal(float mean, float variance, std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& generator) {
	float u = randomFloat(generator);
	float v = randomFloat(generator);

	return mean + variance * sqrt(-2.0 * log(u)) * cos(2.0 * PI * v);
}