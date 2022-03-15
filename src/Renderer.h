#pragma once

#include "Object3D.h"
#include "Scene.h"
#include "Camera.h"
#include "Object3D.h"
#include "Mesh.h"
#include "Texture.h"
#include "Buffer.h"
#include <string>
#include <glad/glad.h>

struct DrawPassState {
	unsigned int fbo;
	std::string shaderName;
};

class Renderer {
private:
	std::vector<DrawPassState> drawPassStates;
public:
	Renderer();
	void renderEntity(Object3D& entityObj, const Scene& scene, const DrawPassState& drawState);
	void renderScene(const Scene& scene, const std::vector<std::reference_wrapper<Object3D>>& entityObjects);

	void initBufferAttributes(GLuint program, BufferAttributes& bufferAttributes);
	void updateUniforms(const Camera& camera, GLuint program, Object3D& object3D);
	void updateModelUniforms(GLuint program, Object3D& object3D);

	void bindTexture(GLuint program, Material& material);
	GLuint bindShaderProgram(Shader shader);

	GLuint initShaderProgram(const std::string& vsSource, const std::string& fsSource);
	GLuint loadShader(GLenum shaderType, const std::string& source);
	GLuint initTexture(const Texture& texture);

	template<typename T>
	GLuint initBuffer(BufferAttributes& bufferAttributes);

	int initBuffers(Geometry& geometry, GLuint program);

	void setBuffersAndAttributes(GLuint program, Object3D& object3D);
	void updateBuffers(Geometry& geometry);

	void clear(GLuint fbo);

	BufferManager bufferManager;
	ShaderManager shaderManager;
};

template<typename T>
GLuint Renderer::initBuffer(BufferAttributes& bufferAttributes) {
	GLuint bufferObject;

	glGenBuffers(1, &bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject);

	glBufferData(GL_ARRAY_BUFFER, bufferAttributes.sizeBytes * sizeof(T), NULL, GL_STATIC_DRAW);

	std::vector<T> mergedAttributes = bufferAttributes.mergeAttributes<T>();

	int bufferSize = bufferAttributes.sizeBytes > 0 ? mergedAttributes.size() : bufferAttributes.sizeBytes;

	glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize * sizeof(T), &mergedAttributes);

	return bufferObject;
}