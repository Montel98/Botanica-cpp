#include "BezierLinear.h"
#include "Stem.h"
#include "Geometry.h"
#include "Material.h"
#include "StemBuilder.h"
#include <memory>
#include <math.h>

Stem::Stem() : worldObject(generateMesh(NULL)) {}

Mesh Stem::generateMesh(Stem* prevStem) {
	//return prevStem->worldObject.getMesh();
	//Geometry geometry;
	Material material;

	return Mesh(material, std::make_unique<Geometry>(Geometry()));
}

void Stem::generateGeometry(const LSystemParams &lParams, Stem* prevStem) {

	ParametricGeometry<StemBuilder::StemSurface> stemBody = StemBuilder::generateStemBody(lParams);
	ParametricGeometry<StemBuilder::StemSurface> stemTip = StemBuilder::generateStemTip(lParams);

	if (lParams.connectParent && prevStem != NULL) {
		// LOGIC REDEFINE HERE
	}
}

float randomNormal(std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647>& generator) {
	float val = generator();
	return val / 2147483647.0f;
}