#include "BezierLinear.h"
#include "Stem.h"
#include "Geometry.h"
#include "Material.h"
#include <memory>
#include <math.h>

Stem::Stem() : worldObject(generateMesh(NULL)) {}

Mesh Stem::generateMesh(Stem* prevStem) {
	//return prevStem->worldObject.getMesh();
	//Geometry geometry;
	Material material;

	return Mesh(material, std::make_unique<Geometry>(Geometry()));
}

void Stem::generateGeometry(const StemBuilder::LSystemParams &lParams, Stem* prevStem) {

	ParametricGeometry<StemBuilder::StemSurface> stemBody = StemBuilder::generateStemBody(lParams);
	ParametricGeometry<StemBuilder::StemSurface> stemTip = StemBuilder::generateStemTip(lParams);

	if (lParams.connectParent && prevStem != NULL) {
		// LOGIC REDEFINE HERE
	}
}