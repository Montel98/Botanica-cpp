#pragma once

#include "Entity.h"
#include <string>

struct DrawPassState {
	unsigned int frameBuffer;
	std::string shaderName;
}

class Renderer {
	renderEntity(std::unique_ptr<Entity>& entity, Scene scene, DrawPassState drawState)
}