#pragma once

#include "EntityManager.h"
#include "Renderer.h"
#include "Scene.h"

class Controller {
private:
	EntityManager entityManager;
	Scene scene;
	Renderer renderer;
public:
	void updateStates();
};