#include "Controller.h"

void Controller::updateStates(WorldTime& worldTime) {
	std::vector<EntityId> entityStack = scene.entities;
	std::vector<std::reference_wrapper<Object3D>> visibleObjects;

	worldTime.updateTime();

	while(entityStack.size() > 0) {
		Entity& entity = *entityManager.getEntityById(entityStack.back());
		entity.act(worldTime);

		if (!entity.worldObject.isHidden) {
			visibleObjects.push_back(std::ref(entity.worldObject));
		}

		entityStack.pop_back();
	}

	renderer.renderScene(scene, visibleObjects);
}

Controller::Controller(EntityManager& manager, Renderer& worldRenderer, Scene& worldScene) :
entityManager(manager), renderer(worldRenderer), scene(worldScene) {};