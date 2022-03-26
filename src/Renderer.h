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

	void initBufferAttributes(GLuint program, BufferAttributes& bufferAttributes, Buffer& buffer);
	void updateUniforms(const Camera& camera, GLuint program, Object3D& object3D);
	void updateModelUniforms(GLuint program, Object3D& object3D);

	void bindTexture(GLuint program, Material& material);
	GLuint bindShaderProgram(Shader shader);

	GLuint initShaderProgram(const std::string& vsSource, const std::string& fsSource);
	GLuint loadShader(GLenum shaderType, const std::string& source);
	GLuint initTexture(const Texture& texture);

	template<typename T>
	GLuint initBuffer(BufferAttributes& bufferAttributes, bool isIndexBuffer);

	int initBuffers(Geometry& geometry, GLuint program);

	void setBuffersAndAttributes(GLuint program, Object3D& object3D);
	void updateBuffers(Geometry& geometry);

	void clear(GLuint fbo);

	BufferManager bufferManager;
	ShaderManager shaderManager;
};

template<typename T>
GLuint Renderer::initBuffer(BufferAttributes& bufferAttributes, bool isIndexBuffer) {
	GLuint bufferObject;

	GLenum target = isIndexBuffer ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER; 

	glGenBuffers(1, &bufferObject);
	auto e = glGetError();
	assert(e == GL_NO_ERROR);
	glBindBuffer(target, bufferObject);
	e = glGetError();
	assert(e == GL_NO_ERROR);

	std::vector<T> mergedAttributes = bufferAttributes.mergeAttributes<T>(0, bufferAttributes.getNoElements());

	/*for (auto i : mergedAttributes) {
		std::cout << i << std::endl;
	}*/

	int bufferSize = bufferAttributes.sizeBytes > 0 ? bufferAttributes.sizeBytes : mergedAttributes.size() * sizeof(T);

	//std::cout << "size::: " << bufferSize << ", " << bufferAttributes.getLength() << "\n";

	//std::cout << "size::: " << sizeof(T) << "," << bufferSize * sizeof(T) << "\n";

	//glBufferSubData(target, 0, bufferSize * sizeof(T), &mergedAttributes.front());
	glBufferData(target, bufferSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(target, 0, mergedAttributes.size() * sizeof(T), &mergedAttributes.front());
	e = glGetError();
	assert(e == GL_NO_ERROR);

	//glBindBuffer(target, 0);

	return bufferObject;
}