#pragma once

#include "Object3D.h"
#include "Scene.h"
#include <string>
#include <glad/glad.h>

struct DrawPassState {
	unsigned int frameBuffer;
	std::string shaderName;
};

class Renderer {
public:
	void renderEntity(Object3D entityObj, const Scene& scene, const DrawPassState& drawState);
	GLuint* initBuffer(BufferAttributes& bufferAttributes);
	void initBufferAttributes(ShaderInfo shaderInfo, BufferAttributes& bufferAttributes);
};