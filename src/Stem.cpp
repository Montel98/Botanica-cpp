#include "BezierLinear.h"
#include "Stem.h"
#include "Geometry.h"
#include "Material.h"
#include "StemBuilder.h"
#include <memory>
#include <math.h>
#include <glm/glm.hpp>
#include <iostream>

#define PI 3.1415

Stem::Stem(EntityManager& manager, const LSystemParams& lSystemParams, const StemNode* prevStem) 
: Entity(Object3D(generateMesh(prevStem))), lParams(lSystemParams), entityManager(manager) {};

Mesh Stem::generateMesh(const StemNode* prevStem) {
	//return prevStem->worldObject.getMesh();
	//Geometry geometry;
	Material material;

	glm::vec3 forward(0.0f, 0.0f, 1.0f);
	glm::vec3 up(0.0f, -1.0f, 0.0f);
	glm::vec3 left(-1.0f, 0.0f, 0.0f);

	Axis axis(forward, up, left);
	LSystemParams lParams{
		glm::vec3(1.0f, 1.0f, 1.0f),
		axis,
		0,
		0,
		true,
		0,
		1.0,
		1.0
	};

	return Mesh(material, std::make_unique<Geometry>(generateGeometry(lParams, prevStem)));
}

Geometry Stem::generateGeometry(const LSystemParams &lParams, const StemNode* prevStem) {

	int uStepsBody = 2;
	int uStepsTip = 3;
	int vSteps = 4;

	int offset = 0;//uStepsBody - 1;

	GeometryConstraints stemBodyConstraints = {0.0, 1.0, 0.0, 2.0 * PI, uStepsBody, vSteps};
	GeometryConstraints stemTipConstraints = {0.0, 1.0, 0.0, 2.0 * PI, uStepsTip, vSteps};

	ParametricGeometry<StemBuilder::StemSurface> stemBody = StemBuilder::generateStemBody(lParams, stemBodyConstraints);
	ParametricGeometry<StemBuilder::StemSurface> stemTip = StemBuilder::generateStemTip(lParams, stemTipConstraints);

	// Manually connect the top-row vertices of the previous stem body with the bottom-row vertices of the new stem
	if (lParams.connectParent && prevStem) {

		BufferAttributes& body = stemBody.bufferAttributes;
		BufferAttributes& tip = stemTip.bufferAttributes;
		BufferAttributes& bodyPrev = entityManager.getEntityById(prevStem->current)->worldObject.getMesh()._geometry->bufferAttributes;
		//BufferAttributes& tipPrev = entityManager.getEntityById(prevStem->current)->worldObject.getMesh()._geometry->bufferAttributes;

		for (int i = 0; i < vSteps; i++) {

			int prevIndex = (uStepsBody * vSteps) + (uStepsTip * i) + offset;

			body.getBufferAttribute<glm::vec3>("aVertexPosition").bufferData[uStepsBody*i] = bodyPrev.getBufferAttribute<glm::vec3>("aVertexPosition").bufferData[prevIndex];
			body.getBufferAttribute<glm::vec3>("aNormal").bufferData[uStepsBody*i] = bodyPrev.getBufferAttribute<glm::vec3>("aNormal").bufferData[prevIndex];
			body.getBufferAttribute<glm::vec3>("aVertexMatureStart").bufferData[uStepsBody*i] = bodyPrev.getBufferAttribute<glm::vec3>("aVertexMatureStart").bufferData[prevIndex];

			tip.getBufferAttribute<glm::vec3>("aVertexImmatureEnd").bufferData[uStepsTip*i] = bodyPrev.getBufferAttribute<glm::vec3>("aVertexImmatureEnd").bufferData[prevIndex];
			tip.getBufferAttribute<glm::vec3>("aNormalImmatureEnd").bufferData[uStepsTip*i] = bodyPrev.getBufferAttribute<glm::vec3>("aNormalImmatureEnd").bufferData[prevIndex];
			tip.getBufferAttribute<glm::vec3>("aVertexImmatureStart").bufferData[uStepsTip*i] = bodyPrev.getBufferAttribute<glm::vec3>("aVertexImmatureStart").bufferData[prevIndex];
		}
	}
	std::vector<std::reference_wrapper<Geometry>>geometries{std::ref(stemBody), std::ref(stemTip)};

	return mergeGeometry(geometries);
}