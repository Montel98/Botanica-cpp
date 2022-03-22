#include "Scene.h"

void Scene::addEntity(Object3D& object3D/*EntityId entity*/) {
	entities.push_back(/*entity*/&object3D);
}

void Scene::removeEntity(Object3D& object3D/*EntityId entity*/) {
	for (int i = 0; i < entities.size(); i++) {

		if (entities[i] == /*entity*/&object3D) {
			entities.erase(entities.begin() + i);
			break;
		}
	}
}

Scene::Scene(const Camera& worldCamera) : camera(worldCamera) {};