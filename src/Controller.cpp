#include "Controller.h"

void Controller::updateStates() {
	std::vector<EntityId> entityStack = scene.entities;
	std::vector<std::reference_wrapper<Object3D>> visibleObjects;

	while(entityStack.size() > 0) {
		Entity& entity = *entityManager.getEntityById(entityStack.back());
		entity.act();

		if (!entity.worldObject.isHidden) {
			visibleObjects.push_back(std::ref(entity.worldObject));
		}
	}

	renderer.renderScene(scene, visibleObjects);
}