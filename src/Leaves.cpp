#include "Leaves.h"

#define PI 3.14159265

Leaves::Leaves() : Entity(Object3D(generatePlane())) {
	Geometry& geometry = *worldObject.getMesh()._geometry;
	geometry.useInstancing();
	geometry.instanceBuffer->addInstanceBufferAttribute<glm::vec1>("aAge");
	geometry.instanceBuffer->instanceAttributes.sizeBytes = 131072;
}

void Leaves::act(const WorldTime& worldTime) {

	for (int i = 0; i < leaves.size(); i++) {
		leaves[i].act(worldTime);
		updateLeafAttributes(i);
	}
}

int Leaves::addLeaf(const glm::mat4& pose, const glm::vec3& direction) {
	std::map<std::string, BufferAttributeElement> newInstance = {
		{"aPose", BufferAttributeElement(pose)},
		{"aAge", glm::vec1(0.0f)}
	};
	Geometry& geometry = *worldObject.getMesh()._geometry;
	geometry.instanceBuffer->addInstance(newInstance);

	leaves.push_back(Leaf(pose, direction));

	return leaves.size() - 1;
}

std::vector<int> Leaves::addLeaves(const LSystemParams& lParams, int noLeaves) {

	std::vector<int> leafIndices;
	
	for (int i = 0; i < noLeaves; i++) {
		float zAngle = (i * 2.0 * PI) / noLeaves;
		glm::mat4 projectionMatrix = LeafBuilder::projectToLeafAxis(lParams.axis, lParams.position);
		glm::mat4 zRotationMatrix = glm::rotate(zAngle, glm::vec3(0.0, 0.0, 1.0));
		int leafIndex = addLeaf(projectionMatrix * zRotationMatrix, lParams.axis.forward);
		leafIndices.push_back(leafIndex);
	}
	return leafIndices;
}

Leaf& Leaves::getLeaf(int index) {
	return leaves[index];
}

void Leaves::updateLeafAttributes(int index) {
	Geometry& geometry = *worldObject.getMesh()._geometry;
	geometry.instanceBuffer->instanceAttributes.getBufferAttribute<glm::mat4>("aPose").bufferData[index] = leaves[index].getPose();
	geometry.instanceBuffer->instanceAttributes.getBufferAttribute<glm::vec1>("aAge").bufferData[index][0] = leaves[index].getAge();
}

Mesh Leaves::generatePlane() const {

	GeometryConstraints constraints{0.0, 0.03f*1.2f, -0.006*1.5f*1.2f, 0.006*1.5f*1.2f, 6, 6};

	PlaneSurface<LeafProfileEnd> surfaceEnd;
	ParametricGeometry<PlaneSurface<LeafProfileEnd>> planeGeometryEnd(surfaceEnd, constraints);
	planeGeometryEnd.useNormals(false);
	planeGeometryEnd.useSTs();
	planeGeometryEnd.setSTMap(STMapping{0.0f, 1.0f, 0.0f, 1.0f});
	planeGeometryEnd.generateGeometry();

	PlaneSurface<LeafProfileStart> surfaceStart;
	ParametricGeometry<PlaneSurface<LeafProfileStart>> planeGeometryStart(surfaceStart, constraints);
	planeGeometryStart.generateGeometry();
	//planeGeometryStart.useNormals(false);

	planeGeometryEnd.addMorphTarget(
		"aStart",
		std::move(planeGeometryStart.vertexBuffer.getBufferAttribute<glm::vec3>("aVertexPosition").bufferData)
		//PlaneGeometryStart.vertexBuffer.getBufferAttribute<glm::vec3>("aNormal").bufferData
	);

	Material material;
	material.textureHandle = "LeafTexture";

	Mesh mesh(material, std::make_unique<Geometry>(planeGeometryEnd));

	mesh.shaderPrograms.emplace(
		std::make_pair(
			"Default", 
			Shader("LeafShader", "./src/shaders/leafVertex.glsl", "./src/shaders/leafFragment.glsl")
		)
	);

	return mesh;
}

namespace LeafBuilder {

glm::mat4 projectToLeafAxis(const Axis& axis, const glm::vec3& position) {

	glm::vec3 newLeft = glm::normalize(glm::vec3(axis.left.x, axis.left.y, 0));
	glm::vec3 newForward = glm::normalize(glm::cross(newLeft, axis.up));
	float matrix[16] = {
		newLeft.x, newForward.x, axis.up.x, 0,
		newLeft.y, newForward.y, axis.up.y, 0,
		newLeft.z, newForward.z, axis.up.z, 0,
		position.x, position.y, position.z, 1,
	};

	return glm::make_mat4(matrix);
}

glm::mat4 projectToLeafAxis2(const Axis& axis, const glm::vec3& position) {

	//glm::vec3 newLeft = glm::normalize(glm::vec3(axis.left.x, axis.left.y, 0));
	//glm::vec3 newForward = glm::normalize(glm::cross(newLeft, axis.up));
	float matrix[16] = {
		axis.left.x, axis.left.y, axis.left.z, 0,
		axis.forward.x, axis.forward.y, axis.forward.z, 0,
		axis.up.x, axis.up.y, axis.up.z, 0,
		position.x, position.y, position.z, 1,
	};

	return glm::make_mat4(matrix);
}
}