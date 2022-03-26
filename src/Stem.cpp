#include "BezierLinear.h"
#include "Stem.h"
#include "Geometry.h"
#include "Material.h"
#include "StemBuilder.h"
#include <memory>
#include <map>
#include <math.h>
#include <glm/glm.hpp>
#include <iostream>

#define PI 3.1415

Stem::Stem(EntityManager& manager, const LSystemParams& lSystemParams, const StemNode* prevStem) 
: Entity(Object3D(generateMesh(lSystemParams, prevStem))), entityManager(manager), stemLength(0.0f), 
stemGirth(0.0f), growthRate(0.3f), lParams(lSystemParams) {};

Mesh Stem::generateMesh(const LSystemParams& lParams, const StemNode* prevStem) {
	Material material;

	Mesh mesh(material, std::make_unique<Geometry>(generateGeometry(lParams, prevStem)));

	mesh.shaderPrograms.emplace(
		std::make_pair("Default", Shader("Test", "./src/shaders/stemVertex.glsl", "./src/shaders/stemFragment.glsl"))
	);
	mesh.shaderPrograms.at("Default").addUniform<glm::vec1>("stemLength", glm::vec1(0.0f));
	mesh.shaderPrograms.at("Default").addUniform<glm::vec1>("stemGirth", glm::vec1(0.0f));

	return mesh;
}

Geometry Stem::generateGeometry(const LSystemParams &lParams, const StemNode* prevStem) {

	int uStepsBody = 2;
	int uStepsTip = 3;
	int vSteps = 16;

	int offset = uStepsBody - 1;

	GeometryConstraints stemBodyConstraints = {0.0, 1.0, 0.0, 2.0 * PI, uStepsBody, vSteps};
	GeometryConstraints stemTipConstraints = {0.0, 1.0, 0.0, 2.0 * PI, uStepsTip, vSteps};

	ParametricGeometry<StemBuilder::StemSurface> stemBody = StemBuilder::generateStemBody(lParams, stemBodyConstraints);
	ParametricGeometry<StemBuilder::StemSurface> stemTip = StemBuilder::generateStemTip(lParams, stemTipConstraints);

	// Manually connect the top-row vertices of the previous stem body with the bottom-row vertices of the new stem
	/*if (lParams.connectParent && prevStem) {

		BufferAttributes& body = stemBody.bufferAttributes;
		BufferAttributes& tip = stemTip.bufferAttributes;
		BufferAttributes& bodyPrev = entityManager.getEntityById(prevStem->current)->worldObject.getMesh()._geometry->bufferAttributes;

		for (int i = 0; i < vSteps; i++) {

			int prevIndex = (uStepsBody * vSteps) + (uStepsTip * i) + offset;

			body.getBufferAttribute<glm::vec3>("aVertexPosition").bufferData[uStepsBody*i] = bodyPrev.getBufferAttribute<glm::vec3>("aVertexPosition").bufferData[prevIndex + offset];
			//body.getBufferAttribute<glm::vec3>("aNormal").bufferData[uStepsBody*i] = bodyPrev.getBufferAttribute<glm::vec3>("aNormal").bufferData[prevIndex + offset];
			body.getBufferAttribute<glm::vec3>("aMatureStart").bufferData[uStepsBody*i] = bodyPrev.getBufferAttribute<glm::vec3>("aMatureStart").bufferData[prevIndex + offset];

			tip.getBufferAttribute<glm::vec3>("aImmatureEnd").bufferData[uStepsTip*i] = bodyPrev.getBufferAttribute<glm::vec3>("aImmatureEnd").bufferData[prevIndex + offset];
			//tip.getBufferAttribute<glm::vec3>("aNormalImmatureEnd").bufferData[uStepsTip*i] = bodyPrev.getBufferAttribute<glm::vec3>("aNormalImmatureEnd").bufferData[prevIndex + offset];
			tip.getBufferAttribute<glm::vec3>("aImmatureStart").bufferData[uStepsTip*i] = bodyPrev.getBufferAttribute<glm::vec3>("aImmatureStart").bufferData[prevIndex + offset];
		}
	}*/
	//std::vector<std::reference_wrapper<Geometry>>geometries{std::ref(stemBody)/*, std::ref(stemTip)*/};
	std::vector<std::reference_wrapper<Geometry>>geometries{std::ref(stemBody), std::ref(stemTip)};

	Geometry mergedGeometry = mergeGeometry(geometries);
	mergedGeometry.bufferName = "stemBuffer";
	mergedGeometry.bufferAttributes.sizeBytes = 1048576;//524288;
	mergedGeometry.indexBuffer.sizeBytes = 1048576;//524288;

	return mergedGeometry;
}

void Stem::act(const WorldTime& worldTime) {
	stemLength = grow(worldTime);
	worldObject.getMesh().shaderPrograms.at("Default").setUniform<glm::vec1>("stemLength", glm::vec1(stemLength));
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