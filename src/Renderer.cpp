#include "Renderer.h"
#include "Shader.h"
#include "BufferAttributes.h"
#include <string>
#include <iostream>
#include <variant>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <iostream>

Renderer::Renderer() {
	drawPassStates.push_back(DrawPassState{0, "Default"});
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_EQUAL);
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

	//std::cout << "VAO: " << buffer._vao << " Len: " << bufferInfo.indexBufferLength << std::endl;

	float vertexData[bufferInfo.vertexBufferLength];

	glGetBufferSubData(GL_ARRAY_BUFFER, bufferInfo.vertexBufferStart * sizeof(float), bufferInfo.vertexBufferLength * sizeof(float), vertexData);

	unsigned int indexData[bufferInfo.indexBufferLength];

	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, bufferInfo.indexBufferStart * sizeof(unsigned int), bufferInfo.indexBufferLength * sizeof(unsigned int), indexData);

	glDrawElements(
		GL_TRIANGLES, 
		bufferInfo.indexBufferLength, 
		GL_UNSIGNED_INT,
		(void*) (bufferInfo.indexBufferStart * sizeof(GL_UNSIGNED_INT))
	);

	/*const auto e = glGetError();
	std::cout << "Error: " << e << std::endl;
	assert(e == GL_NO_ERROR);*/

	//glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Renderer::renderScene(const Scene& scene, const std::vector<std::reference_wrapper<Object3D>>& entityObjects) {

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
	for (int state = 0; state < drawPassStates.size(); state++) {
		DrawPassState& drawState = drawPassStates[state];
		//clear(/*drawState.fbo*/0);
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_EQUAL);
	}
}

void Renderer::initBufferAttributes(GLuint program, BufferAttributes& bufferAttributes, Buffer& buffer) {

	//glBindVertexArray(buffer._vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer._vbo);
	auto e = glGetError();
	assert(e == GL_NO_ERROR);

	//std::cout << "VAO: " << buffer._vao << "VBO: " << buffer._vbo << "\n";

	for (const std::string& name : bufferAttributes.getAttributeNames()) {

		GLint location = glGetAttribLocation(program, name.c_str());
		e = glGetError();
		assert(e == GL_NO_ERROR);

		//std::cout << name << "," << location << "\n";

		int stride = bufferAttributes.getStride();

		std::visit([location, stride](auto& bufferAttribute) {

			std::cout << "location" << location << "\n";
			std::cout << "length: " << bufferAttribute.attribLength << "\n";
			std::cout << "offset: " << bufferAttribute.offset << "\n";
			std::cout << "stride: " << stride << "\n";

			glVertexAttribPointer(
				location,
				bufferAttribute.attribLength,
				GL_FLOAT,
				GL_FALSE,
				stride * sizeof(GL_FLOAT),
				(void*) (bufferAttribute.offset * sizeof(GL_FLOAT))
			);
			auto e = glGetError();
			assert(e == GL_NO_ERROR);

		}, bufferAttributes.getBufferAttributeGeneric(name));

		glEnableVertexAttribArray(location);
		e = glGetError();
		assert(e == GL_NO_ERROR);

		/*if (bufferAttributes.isInstanceBuffer) {
			glVertexAttribDivisor(location, 1);
			glGetError();
			assert(e == GL_NO_ERROR);
		}*/
	}

	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

	GLuint VBO = initBuffer<float>(geometry.bufferAttributes, false);
	GLuint IBO = initBuffer<unsigned int>(geometry.indexBuffer, true);

	//std::cout << "first: vao: " << VAO << " vbo: " << VBO << "\n";

	int bufferId;

	if (geometry.bufferName != "") {
		bufferId = bufferManager.addBuffer(geometry.bufferName, VBO, IBO, VAO);
	}
	else {
		bufferId = bufferManager.addBuffer(VBO, IBO, VAO);
	}
	Buffer &buffer = bufferManager.getBufferById(bufferId);
	buffer.addBufferGeometry(&geometry);

	initBufferAttributes(program, geometry.bufferAttributes, buffer);

	glBindVertexArray(0);
	e = glGetError();
	assert(e == GL_NO_ERROR);

	return bufferId;
}

void Renderer::bindTexture(GLuint program, Material& material) {
	if (material.textureMap.textureId == -1) {
		material.textureMap.textureId = initTexture(material.textureMap);
	}
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, material.textureMap.textureId);
	glUniform1i(glGetUniformLocation(program, material.textureMap.name.c_str()), 1);
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
	//bindTexture(program, object3D.getMesh()._material);

	if (geometry.bufferId == -1) {

		if (geometry.bufferName != "" && bufferManager.bufferExists(geometry.bufferName)) {

			geometry.bufferId = bufferManager.getBufferIdByName(geometry.bufferName);
			updateBuffers(geometry);
		}
		else {
			geometry.bufferId = initBuffers(geometry, program);
		}
	}
}

void Renderer::updateBuffers(Geometry& geometry) {

	Buffer& buffer = bufferManager.getBufferById(geometry.bufferId);

	glBindVertexArray(buffer._vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer._vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer._ibo);

	// Make a temp copy of index buffer data and add real buffer base position to offset
	std::vector<unsigned int> newIndexBuffer = geometry.indexBuffer.mergeAttributes<unsigned int>();

	int indexCount = buffer.getIndexCount();

	for (int i = 0; i < newIndexBuffer.size(); i++) {
		newIndexBuffer[i] += indexCount;
	}

	if (!buffer.geometryInBuffer(&geometry)) {
		buffer.addBufferGeometry(&geometry);
	}

	BufferRange bufferInfo = buffer.getBufferInfo(&geometry);

	// Update indices
	glBufferSubData(
		GL_ELEMENT_ARRAY_BUFFER, 
		bufferInfo.indexBufferStart * sizeof(GL_UNSIGNED_INT), 
		newIndexBuffer.size() * sizeof(GL_UNSIGNED_INT), 
		&newIndexBuffer.front()
	);

	std::vector<float> newVertexBuffer = geometry.bufferAttributes.mergeAttributes<float>();

	// Update vertices
	glBufferSubData(
		GL_ARRAY_BUFFER, 
		bufferInfo.vertexBufferStart * sizeof(GL_FLOAT), 
		newVertexBuffer.size() * sizeof(GL_FLOAT), 
		&newVertexBuffer.front()
	);

	glBindVertexArray(0);
}

void Renderer::clear(GLuint fbo) {
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	/*
	auto e = glGetError();
	assert(e == GL_NO_ERROR);
	glClearDepth(1.0f);
	e = glGetError();
	assert(e == GL_NO_ERROR);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	e = glGetError();
	assert(e == GL_NO_ERROR);*/
}