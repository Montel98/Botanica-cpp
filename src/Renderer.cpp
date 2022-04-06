#include "Renderer.h"
#include "Shader.h"
#include "BufferAttributes.h"
#include "Overload.h"
#include <string>
#include <iostream>
#include <variant>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <iostream>

Renderer::Renderer(TextureManager& resourceManager) : textureManager(resourceManager) {
	drawPassStates.push_back(DrawPassState{0, "Default"});

	// Configure depth testing flag
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void Renderer::renderEntity(Object3D& entityObj, const Scene& scene, const DrawPassState& drawState) {

	Mesh& mesh = entityObj.getMesh();
	Geometry& geometry = *mesh._geometry;

	GLuint program = bindShaderProgram(mesh.shaderPrograms.at(drawState.shaderName));

	updateUniforms(scene.camera, program, entityObj);
	setBuffersAndAttributes(program, entityObj);

	Buffer& buffer = bufferManager.getBufferById(geometry.bufferId);
	glBindVertexArray(buffer._vao);
	BufferRange bufferInfo = buffer.getBufferInfo(&geometry);

	if (!geometry.instanceBuffer) {
		glDrawElements(
			GL_TRIANGLES, 
			bufferInfo.indexBufferLength, 
			GL_UNSIGNED_INT,
			(void*) (bufferInfo.indexBufferStart * sizeof(GL_UNSIGNED_INT))
		);
	}
	else {
		glDrawElementsInstanced(
			GL_TRIANGLES, 
			bufferInfo.indexBufferLength, 
			GL_UNSIGNED_INT,
			(void*) (bufferInfo.indexBufferStart * sizeof(GL_UNSIGNED_INT)),
			geometry.instanceBuffer->getInstanceCount()
		);
	}

	glBindVertexArray(0);
}

void Renderer::renderScene(const Scene& scene, const std::vector<std::reference_wrapper<Object3D>>& entityObjects) {

	clear(0);

	for (int state = 0; state < drawPassStates.size(); state++) {
		DrawPassState& drawState = drawPassStates[state];
		//glBindFramebuffer(GL_FRAMEBUFFER, drawState.fbo);

		for (int i = 0; i < entityObjects.size(); i++) {
			Object3D& worldObject = entityObjects[i].get();

			if(worldObject.getMesh().usesProgram(drawState.shaderName)) {
				renderEntity(worldObject, scene, drawState);
			}
		}
	}

	// Clear framebuffers for next frame
	/*for (int state = 0; state < drawPassStates.size(); state++) {
		DrawPassState& drawState = drawPassStates[state];
		clear(0);
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_EQUAL);
	}*/
}
void Renderer::initBufferAttributes(GLuint program, BufferAttributes& bufferAttributes, GLuint vao, GLuint vbo, bool isInstanced) {

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	auto e = glGetError();
	assert(e == GL_NO_ERROR);

	for (const std::string& name : bufferAttributes.getAttributeNames()) {

		GLint location = glGetAttribLocation(program, name.c_str());
		e = glGetError();
		assert(e == GL_NO_ERROR);

		std::cout << name << "," << location << "\n";

		int stride = bufferAttributes.getStride();

		std::visit(overload{
			[this, location, stride, isInstanced](auto& bufferAttribute) {
				initBufferAttribute(
					location, 
					bufferAttribute.attribLength, 
					stride * sizeof(GL_FLOAT), 
					bufferAttribute.offset * sizeof(GL_FLOAT), isInstanced);
			},
			[this, location, stride, isInstanced](BufferAttribute<glm::mat4>& bufferAttribute) {
				for (int i = 0; i < 4; i++) {
					initBufferAttribute(
						location + i, 
						4, 
						stride * sizeof(GL_FLOAT),
						(bufferAttribute.offset + (4 * i)) * sizeof(GL_FLOAT), isInstanced);
				}
			},

		}, bufferAttributes.getBufferAttributeGeneric(name));
	}

	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::initBufferAttribute(int location, int attributeLength, int stride, int offset, bool isInstanced) {

	glVertexAttribPointer(
		location,
		attributeLength,
		GL_FLOAT,
		GL_FALSE,
		stride,
		(void*) (offset)
	);
	auto e = glGetError();
	assert(e == GL_NO_ERROR);

	glEnableVertexAttribArray(location);

	if (isInstanced) {
		glVertexAttribDivisor(location, 1);
	}
}

void Renderer::updateUniforms(const Camera& camera, GLuint program, Object3D& object3D) {

	GLuint worldLoc = glGetUniformLocation(program, "world");
	//auto e = glGetError();
	//assert(e == GL_NO_ERROR);
	GLuint cameraLoc = glGetUniformLocation(program, "camera");
	//e = glGetError();
	//assert(e == GL_NO_ERROR);
	GLuint perspectiveLoc = glGetUniformLocation(program, "perspective");
	//e = glGetError();
	//assert(e == GL_NO_ERROR);

	glm::mat4 cameraMatrix = camera.getCameraMatrix();
	glm::mat4 perspectiveMatrix = camera.getPerspectiveMatrix(1.0);

	glUniformMatrix4fv(worldLoc, 1, false, glm::value_ptr(object3D.worldMatrix));
	//e = glGetError();
	//assert(e == GL_NO_ERROR);
	glUniformMatrix4fv(cameraLoc, 1, false, glm::value_ptr(cameraMatrix));
	//e = glGetError();
	//assert(e == GL_NO_ERROR);
	glUniformMatrix4fv(perspectiveLoc, 1, false, glm::value_ptr(perspectiveMatrix));
	//e = glGetError();
	//assert(e == GL_NO_ERROR);

	updateModelUniforms(program, object3D);
}

void Renderer::updateModelUniforms(GLuint program, Object3D& object3D) {

	for (const std::pair<const std::string, Shader>& shader : object3D.getMesh().shaderPrograms) {
		const std::string& shaderName = shader.first;

		for (const std::pair<const std::string, UniformType>& uniform : object3D.getMesh().shaderPrograms.at(shaderName).uniforms) {
			const std::string &uniformName = uniform.first;

			std::visit(overload{
				[program, &uniformName](const glm::vec1& uniformData) {
					glUniform1f(glGetUniformLocation(program, uniformName.c_str()), uniformData[0]);
				},
				[program, &uniformName](const glm::vec2& uniformData) {
					glUniform2fv(glGetUniformLocation(program, uniformName.c_str()), 1, glm::value_ptr(uniformData));
				},
				[program, &uniformName](const glm::vec3& uniformData) {
					glUniform3fv(glGetUniformLocation(program, uniformName.c_str()), 1, glm::value_ptr(uniformData));
				},
				[program, &uniformName](const glm::vec4& uniformData) {
					glUniform4fv(glGetUniformLocation(program, uniformName.c_str()), 1, glm::value_ptr(uniformData));
				},
				[program, &uniformName](const glm::mat4& uniformData) {
					glUniformMatrix4fv(glGetUniformLocation(program, uniformName.c_str()), 1, false, glm::value_ptr(uniformData));
				},
			}, uniform.second);
		}
	}
}

GLuint Renderer::initShaderProgram(const std::string& vsSource, const std::string& fsSource) {

	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vsSource);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fsSource);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	auto e = glGetError();
	assert(e == GL_NO_ERROR);
	glAttachShader(shaderProgram, fragmentShader);
	e = glGetError();
	assert(e == GL_NO_ERROR);
	glLinkProgram(shaderProgram);
	e = glGetError();
	assert(e == GL_NO_ERROR);

	int success;
    char infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

	glUseProgram(shaderProgram);
	e = glGetError();
	assert(e == GL_NO_ERROR);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

	std::cout << "shader: " << shaderProgram << "\n";

	return shaderProgram;
}

GLuint Renderer::loadShader(GLenum shaderType, const std::string& source) {
	GLuint shader = glCreateShader(shaderType);
	auto e = glGetError();
	assert(e == GL_NO_ERROR);
	const GLchar* cSource = source.c_str();
	glShaderSource(shader, 1, &cSource, NULL);
	e = glGetError();
	assert(e == GL_NO_ERROR);
	glCompileShader(shader);
	e = glGetError();
	assert(e == GL_NO_ERROR);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR: Shader compilation failed." << infoLog << std::endl;
	}

	return shader;
}

GLuint Renderer::initTexture(const Texture& texture) {
	GLuint textureHandle;
	int level = 0;
	int border = 0;

	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	auto e = glGetError();
	assert(e == GL_NO_ERROR);
	glTexImage2D(
		GL_TEXTURE_2D, 
		level, 
		GL_RGBA, 
		texture.width, 
		texture.height, 
		border, 
		GL_RGBA, 
		GL_UNSIGNED_BYTE, 
		(void*)(texture.bufferData.data())
	);
	glGenerateMipmap(GL_TEXTURE_2D);
	e = glGetError();
	assert(e == GL_NO_ERROR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	e = glGetError();
	assert(e == GL_NO_ERROR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureHandle;
}

int Renderer::initBuffers(Geometry& geometry, GLuint program) {

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	auto e = glGetError();
	assert(e == GL_NO_ERROR);
	glBindVertexArray(VAO);
	e = glGetError();
	assert(e == GL_NO_ERROR);

	GLuint VBO = initBuffer<float>(geometry.vertexBuffer, false);
	GLuint EBO = initBuffer<unsigned int>(geometry.indexBuffer, true);

	int bufferId;

	if (geometry.bufferName != "") {
		bufferId = bufferManager.addBuffer(geometry.bufferName, VBO, EBO, VAO);
	}
	else {
		bufferId = bufferManager.addBuffer(VBO, EBO, VAO);
	}
	Buffer &buffer = bufferManager.getBufferById(bufferId);
	buffer.addBufferGeometry(&geometry);

	initBufferAttributes(program, geometry.vertexBuffer, buffer._vao, buffer._vbo, false);

	if (geometry.instanceBuffer) {
		GLuint IBO = initBuffer<float>(geometry.instanceBuffer->instanceAttributes, false);
		geometry.instanceBuffer->ibo = IBO;
		initBufferAttributes(program, geometry.instanceBuffer->instanceAttributes, buffer._vao, IBO, true);
	}

	glBindVertexArray(0);
	e = glGetError();
	assert(e == GL_NO_ERROR);

	return bufferId;
}

void Renderer::bindTexture(GLuint program, Material& material) {

	Texture& texture = textureManager.getTextureById(material.textureHandle);

	if (texture.textureId == -1) {
		texture.textureId = initTexture(texture);
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.textureId);
	auto e = glGetError();
	assert(e == GL_NO_ERROR);
	glUniform1i(glGetUniformLocation(program, texture.name.c_str()), 0);
	e = glGetError();
	assert(e == GL_NO_ERROR);
}

GLuint Renderer::bindShaderProgram(Shader shader) {

	if (!shaderManager.shaderExists(shader.name)) {
		shaderManager.addCustomShader(shader);
	}
	ShaderInfo &shaderInfo = shaderManager.getShaderInfo(shader.name);

	if (shaderInfo.programId == -1) {
		shaderInfo.programId = initShaderProgram(shaderInfo.vertexSource, shaderInfo.fragmentSource);
	}
	glUseProgram(shaderInfo.programId);
	return shaderInfo.programId;
}

void Renderer::setBuffersAndAttributes(GLuint program, Object3D& object3D) {

	Geometry& geometry = *(object3D.getMesh()._geometry);

	bindTexture(program, object3D.getMesh()._material);

	if (geometry.bufferId == -1) {
		geometry.modificationEvents.clear();

		if (geometry.bufferName != "" && bufferManager.bufferExists(geometry.bufferName)) {
			geometry.bufferId = bufferManager.getBufferIdByName(geometry.bufferName);

			geometry.addGeometryEvent(
				0, 
				geometry.vertexBuffer.getNoElements(),
				0,
				geometry.indexBuffer.getNoElements()
			);
		}
		else {
			geometry.bufferId = initBuffers(geometry, program);
		}
	}

	if (geometry.modificationEvents.size() > 0) {
		updateBuffers(geometry);
	}

	if (geometry.instanceBuffer) {
		updateInstanceBuffer(*(geometry.instanceBuffer));
	}
}

void Renderer::updateBuffers(Geometry& geometry) {

	Buffer& buffer = bufferManager.getBufferById(geometry.bufferId);

	glBindVertexArray(buffer._vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer._vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer._ibo);

	GeometryEvent& event = geometry.modificationEvents.back();

	if (!buffer.geometryInBuffer(&geometry)) {
		buffer.addBufferGeometry(&geometry);
	}
	else {
		buffer.updateBufferGeometry(&geometry);
	}

	BufferRange bufferInfo = buffer.getBufferInfo(&geometry);

	// Make a temp copy of index buffer data
	std::vector<unsigned int> newIndexBuffer = geometry.indexBuffer.mergeAttributes<unsigned int>(
		event.indexStart, event.indexLength
	);

	// Shift index buffer positions by total index count if mulitple geometries exist
	if (buffer.getGeometryCount() > 1) {
		int indexCount = buffer.getIndexCount() - bufferInfo.indexCount;

		for (int i = 0; i < newIndexBuffer.size(); i++) {
			newIndexBuffer[i] += indexCount;
		}
	}

	// Update indices
	glBufferSubData(
		GL_ELEMENT_ARRAY_BUFFER,
		(bufferInfo.indexBufferStart + (event.indexStart * geometry.indexBuffer.getStride())) * sizeof(GL_UNSIGNED_INT), 
		newIndexBuffer.size() * sizeof(GL_UNSIGNED_INT), 
		&newIndexBuffer.front()
	);

	std::vector<float> newVertexBuffer = geometry.vertexBuffer.mergeAttributes<float>(
		event.vertexStart, event.vertexLength
	);

	// Update vertices
	glBufferSubData(
		GL_ARRAY_BUFFER,
		(bufferInfo.vertexBufferStart + (event.vertexStart * geometry.vertexBuffer.getStride())) * sizeof(GL_FLOAT), 
		newVertexBuffer.size() * sizeof(GL_FLOAT), 
		&newVertexBuffer.front()
	);

	geometry.modificationEvents.pop_back();

	glBindVertexArray(0);
}

void Renderer::updateInstanceBuffer(InstanceBuffer& instanceBuffer) {

	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer.ibo);
	auto e = glGetError();
	//assert(e == GL_NO_ERROR);

	BufferAttributes& instanceBufferAttributes = instanceBuffer.instanceAttributes;

	std::vector<float> newInstanceBuffer = instanceBufferAttributes.mergeAttributes<float>(
		0, instanceBufferAttributes.getNoElements()
	);

	glBufferSubData(
		GL_ARRAY_BUFFER,
		0,
		newInstanceBuffer.size() * sizeof(GL_FLOAT),
		&newInstanceBuffer.front()
	);
	e = glGetError();
	//assert(e == GL_NO_ERROR);
}

void Renderer::clear(GLuint fbo) {
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}