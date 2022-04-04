#include "BezierLinear.h"
#include "Stem.h"
#include "Geometry.h"
#include "Material.h"
#include "TextureBuilder.h"
#include <memory>
#include <map>
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>

Stem::Stem(EntityManager& manager, const LSystemParams& lSystemParams, const StemNode* prevStem) 
: Entity(Object3D(generateMesh(lSystemParams, prevStem))), entityManager(manager), stemLength(0.0f), 
stemGirth(0.0f), growthRate(0.3f), lParams(lSystemParams),
radiusFunc(
	StemBuilder::StemBodyRadius(
		lParams.radiusStart,
		lParams.radiusEnd,
		lParams.branchLength,
		lParams.count
	)
) {};

Mesh Stem::generateMesh(const LSystemParams& lParams, const StemNode* prevStem) {
	Material material;
	material.textureHandle = "StemTexture";

	Mesh mesh(material, std::make_unique<Geometry>(generateGeometry(lParams, prevStem)));

	mesh.shaderPrograms.emplace(
		std::make_pair("Default", Shader("Test", "./src/shaders/stemVertex.glsl", "./src/shaders/stemFragment.glsl"))
	);
	mesh.shaderPrograms.at("Default").addUniform<glm::vec1>("stemLength", glm::vec1(0.0f));
	mesh.shaderPrograms.at("Default").addUniform<glm::vec1>("stemGirth", glm::vec1(0.0f));

	return mesh;
}

Geometry Stem::generateGeometry(const LSystemParams& lParams, const StemNode* prevStem) {

	int uStepsBody = 2;
	int uStepsTip = 3;
	int vSteps = 16;

	GeometryConstraints stemBodyConstraints = {0.0, 1.0, 0.0, 2.0 * M_PI, uStepsBody, vSteps};
	GeometryConstraints stemTipConstraints = {0.0, 1.0, 0.0, 2.0 * M_PI, uStepsTip, vSteps};

	ParametricGeometry<StemBuilder::StemSurface> stemBody = (lParams.connectParent && prevStem) ? 
		StemBuilder::generateStemBody(
			lParams, 
			stemBodyConstraints, 
			*entityManager.getEntityById(prevStem->current).worldObject.getMesh()._geometry
		) :
		StemBuilder::generateStemBody(lParams, stemBodyConstraints);

	ParametricGeometry<StemBuilder::StemSurface> stemTip = StemBuilder::generateStemTip(lParams, stemTipConstraints);

	std::vector<std::reference_wrapper<Geometry>>geometries{std::ref(stemBody), std::ref(stemTip)};

	Geometry mergedGeometry = mergeGeometry(geometries);
	mergedGeometry.bufferName = "stemBuffer";
	mergedGeometry.vertexBuffer.sizeBytes = 3*1048576;//524288;
	mergedGeometry.indexBuffer.sizeBytes = 1048576;//524288;

	return mergedGeometry;
}

void Stem::act(const WorldTime& worldTime) {
	stemLength = grow(worldTime);
	worldObject.getMesh().shaderPrograms.at("Default").setUniform<glm::vec1>("stemLength", glm::vec1(stemLength));
}

void Stem::addLeaves(Leaves& leaves, int count) {
	leafIndices = leaves.addLeaves(lParams, count);
}

void Stem::updateLeaves(Leaves& leaves) {

	for (int i = 0; i < leafIndices.size(); i++) {
		Leaf& leaf = leaves.getLeaf(leafIndices[i]);
		leaf.updateStemGirth(radiusFunc(0.0f) * stemGirth);
		leaf.updateLeafHeight(0.03f * stemLength);
	}
}

float Stem::grow(const WorldTime& worldTime) const {
	float newLength = stemLength + growthRate * (float)(worldTime.dt()) / 1000.0f;

	if (newLength >= Stem::maxLength) {
		newLength = maxLength;
	}
	return newLength;
}

void Stem::updateGirth(float newGirth) {
	stemGirth = newGirth;
	worldObject.getMesh().shaderPrograms.at("Default").setUniform<glm::vec1>("stemGirth", glm::vec1(stemGirth));
}

bool Stem::isMaxLength() const {
	return stemLength == maxLength;
}