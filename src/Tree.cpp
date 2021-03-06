#include "Tree.h"
#include "TreeString.h"
#include "Geometry.h"
#include "Object3D.h"
#include "LSystemOps.h"
#include "LSystem.h"
#include "StemBuilder.h"
#include <memory>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Tree::Tree(EntityManager& entityManager, Generator& gen) 
: Entity(Object3D(initMesh())), root(buildTree(manager, gen)), 
manager(entityManager), age(0.0f), growthRate(0.01f) {

	terminalStems.push_back(TerminalStem{&root, false});
	leavesId = entityManager.addEntity(std::make_unique<Leaves>());
	entityManager.addEntityToScene(leavesId);
	stems = buildStemList(root);
}

Mesh Tree::initMesh() {
	std::unique_ptr<Geometry> treeGeometry = std::make_unique<Geometry>();
	treeGeometry->indexBuffer.sizeBytes = 2*1048576;
	treeGeometry->vertexBuffer.sizeBytes = 2*1048576;
	treeGeometry->useNormals(false);
	treeGeometry->useSTs();
	treeGeometry->vertexBuffer.addBufferAttribute<glm::vec3>("aMatureStart");
	
	Material material;
	material.textureHandle = "StemTexture";

	Mesh mesh(material, std::move(treeGeometry));

	mesh.shaderPrograms.emplace(
		std::make_pair(
			"Default", 
			Shader("TreeShader", "./src/shaders/treeVertex.glsl", "./src/shaders/treeFragment.glsl")
		)
	);

	mesh.shaderPrograms.at("Default").addUniform<glm::vec1>("treeGirth", glm::vec1(0.0f));
	
	return mesh;
}

StemNode Tree::buildTree(EntityManager& manager, Generator& gen) {

    LSystem lSystem;

    std::vector<OpCode> finalString = lSystem.buildString(
    	TreeString::getLString(gen),
    	4,
    	gen
    );

    int branchLength = LSystemOps::getNoSegmentsInRange(finalString, 0, finalString.size());

    glm::vec3 forward(0.0f, 0.0f, 1.0f);
    glm::vec3 up(0.0f, -1.0f, 0.0f);
    glm::vec3 left(-1.0f, 0.0f, 0.0f);

    Axis axis(forward, up, left);
    LSystemParams lParams{
        glm::vec3(0.0f, 0.0f, 0.0f),
        axis,
        0,
        0,
        true,
        0,
        0.15,
        0.01,
        branchLength,
        0
    };

    StemNode root = lSystem.buildTree(finalString, lParams, finalString.size(), manager);
    //manager.getEntityById(root.current).worldObject.hide();

    manager.addEntityToScene(root.current);
    //worldObject.addChild(manager.getEntityById(root.current).worldObject);

    return root;
}

void Tree::generateNewStems(EntityManager& manager) {

	std::vector<StemNode*> stemsToAdd;
	std::vector<int> stemsToRemove;

	for (int i = 0; i < terminalStems.size(); i++) {

		Stem& stem = *manager.getEntityById<Stem>(terminalStems[i].node->current);

		if (stem.isMaxLength() && terminalStems[i].node->next.size() > 0) {

			if (!terminalStems[i].isVisited) {

				terminalStems[i].isVisited = true;
				mergeToGeometry(terminalStems[i].node);
				stem.worldObject.hide();
				stemsToRemove.push_back(i);

				for (int nextStem = 0; nextStem < terminalStems[i].node->next.size(); nextStem++) {
					stemsToAdd.push_back(terminalStems[i].node->next[nextStem].get());
				}
			}
		}
	}

	// Add new stems
	for (int i = 0; i < stemsToAdd.size(); i++) {
		manager.addEntityToScene(stemsToAdd[i]->current);
		//worldObject.addChild(manager.getEntityById(stemsToAdd[i]->current).worldObject);
		terminalStems.push_back(TerminalStem{stemsToAdd[i], false});
		Leaves& leaves = *manager.getEntityById<Leaves>(leavesId);
		//Experimental
		Stem& stem = *manager.getEntityById<Stem>(stemsToAdd[i]->current);
		if (stem.lParams.level >= 2) {
			manager.getEntityById<Stem>(terminalStems.back().node->current)->addLeaves(leaves, 2);
		}
	}

	// Remove fully grown terminal stems
	for (int i = 0; i < stemsToRemove.size(); i++) {
		terminalStems.erase(terminalStems.begin() + stemsToRemove[i] - i);
	}
}

/* Update girth of stems and positions of leaves from top-down
* (Leaf and stems need not be concerned about how these states are updated) 
*/
void Tree::updateStems() {
	for (int i = 0; i < stems.size(); i++) {
		Stem& stem = *manager.getEntityById<Stem>(stems[i]);
		stem.updateGirth(age);
		stem.updateLeaves(*manager.getEntityById<Leaves>(leavesId));
	}
}

// Add geometry from mature terminal stem to geometry of tree
// Tree is drawn with 1 draw call
void Tree::mergeToGeometry(StemNode* stemNode) {
	int uStepsBody = 2;
	int vSteps = 16;

	Stem& stem = *manager.getEntityById<Stem>(stemNode->current);

	GeometryConstraints stemBodyConstraints = {0.0, 1.0, 0.0, 2.0 * M_PI, uStepsBody, vSteps};

	ParametricGeometry<StemBuilder::StemSurface> stemBody = (stem.lParams.connectParent && stemNode->prev) ? 
		StemBuilder::generateStemBody(
			stem.lParams, 
			stemBodyConstraints, 
			*manager.getEntityById(stemNode->prev->current).worldObject.getMesh()._geometry) :
		StemBuilder::generateStemBody(stem.lParams, stemBodyConstraints);

	stemBody.vertexBuffer.removeBufferAttribute<glm::vec3>("aImmatureStart");
	stemBody.vertexBuffer.removeBufferAttribute<glm::vec3>("aImmatureEnd");
	worldObject.getMesh()._geometry->addGeometry(stemBody);
}

void Tree::act(const WorldTime& worldTime) {
	age = grow(worldTime);
	worldObject.getMesh().shaderPrograms.at("Default").setUniform<glm::vec1>("treeGirth", glm::vec1(age));
	generateNewStems(manager);
	updateStems();
}

float Tree::grow(const WorldTime& worldTime) const {
	float newAge = age + growthRate * (float)(worldTime.dt()) / 1000.0f;

	if (newAge >= Tree::maxAge) {
		newAge = Tree::maxAge;
	}
	return newAge;
}

std::vector<EntityId> Tree::buildStemList(StemNode& root) {
	std::deque<StemNode*> stemNodes;
	std::vector<EntityId> stemList;
	stemNodes.push_back(&root);

	while(stemNodes.size() > 0) {
		StemNode* stemNode = stemNodes.front();

		for (int i = 0; i < stemNode->next.size(); i++) {
			stemNodes.push_back(stemNode->next[i].get());
		}
		stemList.push_back(stemNode->current);
		stemNodes.pop_front();
	}
	return stemList;
}