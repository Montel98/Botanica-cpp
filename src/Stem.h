#include "Object3D.h"
#include "Mesh.h"
#include "BezierLinear.h"
#include "glm/glm.hpp"
#include "ParametricGeometry.h"
#include "LSystem.h"

class Stem {
public:
	Object3D worldObject;
	LSystemParams lParams;
	Stem();
	Mesh generateMesh(Stem* prevStem);
	void generateGeometry(const LSystemParams &lParams, Stem* prevStem);
};