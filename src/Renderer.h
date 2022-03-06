#pragma once

#include "Object3D.h"
#include "Scene.h"
#include "Camera.h"
#include "Object3D.h"
#include "Mesh.h"
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
	void updateUniforms(const Camera& camera, GLuint program, Object3D& object3D);
	void updateModelUniforms(GLuint program, Object3D& object3D);
	GLuint initShaderProgram(const std::string& vsSource, const std::string& fsSource);
	GLuint loadShader(GLenum shaderType, const std::string& source);
};