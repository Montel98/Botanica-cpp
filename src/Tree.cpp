#include "Tree.h"
#include "Geometry.h"
#include "Object3D.h"
#include "LSystemOps.h"
#include "LSystem.h"
#include <memory>
#include <vector>

Tree::Tree(EntityManager& entityManager) 
: Entity(Object3D(initMesh())), root(buildTree(manager)), manager(entityManager), age(0.0f), growthRate(0.1f) {
	terminalStems.push_back(TerminalStem{&root, false});
}

Mesh Tree::initMesh() {
	Material material;
	std::unique_ptr<Geometry> treeGeometry = std::make_unique<Geometry>();
	treeGeometry->indexBuffer.sizeBytes = 65536;
	treeGeometry->bufferAttributes.sizeBytes = 65536;

	return Mesh(material, std::move(treeGeometry));
}

StemNode Tree::buildTree(EntityManager& manager) {

    std::vector<OpCode> startString;

    for (int i = 0; i < 2; i++) {
        startString.push_back(GEN_STEM());
    }

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
        0.1,
        0.1
    };

    LSystem lSystem;
    StemNode root = lSystem.buildTree(startString, lParams, startString.size(), manager);
    //manager.getEntityById(root.current).worldObject.hide();

    manager.addEntityToScene(root.current);

    return root;
}

void Tree::generateNewStems(EntityManager& manager) {

	for (int i = 0; i < terminalStems.size(); i++) {

		Stem& stem = *manager.getEntityById<Stem>(terminalStems[i].node->current);

		if (stem.isMaxLength() && terminalStems[i].node->next.size() > 0) {

			if (!terminalStems[i].isVisited) {
				terminalStems[i].isVisited = true;

				if (isEndStem(terminalStems[i].node)) {
					std::cout << "yesss" << std::endl;
					//mergeToGeometry(stem);
					stem.worldObject.hide();
					terminalStems.erase(terminalStems.begin() + i);
				}

				for (int nextStem = 0; nextStem < terminalStems[i].node->next.size(); nextStem++) {
					manager.addEntityToScene(terminalStems[i].node->next[nextStem]->current);
				}
			}
		}
	}
}

void Tree::mergeToGeometry(Stem& stem) {
	Geometry postGrowthGeometry = stem.worldObject.getMesh()._geometry->sliceGeometry(0, 8);
	worldObject.getMesh()._geometry->addGeometry(postGrowthGeometry);
}

void Tree::act(const WorldTime& worldTime) {
	generateNewStems(manager);
}

bool Tree::isEndStem(StemNode* node) {

	/*for (int i = 0;  i < node.nextStems.size(); i++) {
	}*/

	return true;
}