#include "Object3D.h"
#include "Mesh.h"
#include "BezierLinear.h"
#include "glm/glm.hpp"
#include "ParametricGeometry.h"
#include "LSystem.h"
#include <random>

class Stem {
public:
	Object3D worldObject;
	LSystemParams lParams;
	Stem();
	Mesh generateMesh(Stem* prevStem);
	void generateGeometry(const LSystemParams &lParams, Stem* prevStem);
};

float randomNormal(std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& generator);