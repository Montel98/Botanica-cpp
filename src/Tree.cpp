#include "Tree.h"
#include "Geometry.h"
#include "Object3D.h"
#include <memory>

Tree::Tree() : Entity(Object3D(initMesh())) {};

Mesh Tree::initMesh() {
	Material material;
	return Mesh(material, std::make_unique<Geometry>());
}

void Tree::act(const WorldTime& worldTime) {};