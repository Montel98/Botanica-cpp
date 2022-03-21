#pragma once

#include "EntityManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "WorldTime.h"

class Controller {
private:
	EntityManager& entityManager;
	Scene& scene;
	Renderer& renderer;
public:
	void updateStates(WorldTime& worldTime);
	Controller(EntityManager& manager, Renderer& worldRenderer, Scene& worldScene);
};