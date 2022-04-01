#include "Leaves.h"

#define PI 3.14159265

Leaves::Leaves() : Entity(Object3D(generatePlane())) {
	Geometry& geometry = *worldObject.getMesh()._geometry;
	geometry.useInstancing();
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
		{"aPose", BufferAttributeElement(pose)}
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
}

Mesh Leaves::generatePlane() const {

	GeometryConstraints constraints{0.0, 0.015, -0.003, 0.003, 4, 4};

	PlaneSurface surface;
	ParametricGeometry<PlaneSurface> planeGeometry(surface, constraints);
	planeGeometry.generateGeometry();

	Material material;

	Mesh mesh(material, std::make_unique<Geometry>(planeGeometry));

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