#include "Object3D.h"

Object3D::Object3D(Mesh mesh) : localMatrix(1.0), worldMatrix(1.0), isHidden(false), _mesh(std::move(mesh)), parent(nullptr) {
}

void Object3D::addChild(Object3D* child) {
	children.push_back(child);	
}

// Linear search to remove child entity
void Object3D::removeChild(Object3D* child) {
	for (int i = 0; i < children.size(); i++) {

		if (children[i] == child) {
			children.erase(children.begin() + i);
			break;
		}
	}
}

void Object3D::hide() {
	isHidden = true;
}

void Object3D::show() {
	isHidden = false;
}

Mesh& Object3D::getMesh() {
	return _mesh;
}