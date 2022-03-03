#pragma once

#include <random>

float randomFloat(std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& generator);
float randomNormal(float mean, float variance, std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& generator);