#include "Tree.h"
#include "Geometry.h"
#include "Object3D.h"
#include "LSystemOps.h"
#include "LSystem.h"
#include <memory>
#include <vector>

Tree::Tree(EntityManager& manager) : Entity(Object3D(initMesh())), root(buildTree(manager)) {};

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

    manager.addEntityToScene(root.current);
    manager.addEntityToScene(root.next[0]->current);

    return root;
}

void Tree::act(const WorldTime& worldTime) {};