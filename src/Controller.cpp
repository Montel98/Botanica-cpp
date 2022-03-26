#include "Controller.h"
#include <glm/glm.hpp>
#include <deque>

void Controller::updateStates(WorldTime& worldTime) {
	//std::vector<EntityId> entityStack = scene.entities;
	std::vector<unsigned int>& entities = entityManager.getActiveEntities();
	std::vector<std::reference_wrapper<Object3D>> visibleObjects;

	worldTime.updateTime();

	for (int i = 0; i < entities.size(); i++) {

		Entity& entity = entityManager.getEntityById(entities[i]);
		entity.act(worldTime);

		if (!entity.worldObject.isHidden) {
			visibleObjects.push_back(std::ref(entity.worldObject));
		}
	}

	std::deque<Object3D*> worldObjects(scene.entities.begin(), scene.entities.end());

	// Breadth first updating of object positions

	int i = 0;
	while (worldObjects.size() > 0) {

		//std::cout << i << "\n";

		Object3D* current = worldObjects.front();

		if (current->parent) {
			current->worldMatrix = current->localMatrix * current->parent->worldMatrix;
		}
		else {
			current->worldMatrix = current->localMatrix;
		}

		for (int i = 0; i < current->children.size(); i++) {
			worldObjects.push_back(current->children[i]);
		}

		worldObjects.pop_front();
		//i++;
	}

	renderer.renderScene(scene, visibleObjects);
}

Controller::Controller(EntityManager& manager, Renderer& worldRenderer, Scene& worldScene) :
entityManager(manager), renderer(worldRenderer), scene(worldScene) {};