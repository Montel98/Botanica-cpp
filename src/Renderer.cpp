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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_EQUAL);
}

void Renderer::renderEntity(Object3D& entityObj, const Scene& scene, const DrawPassState& drawState) {
	Mesh& mesh = entityObj.getMesh();
	Geometry& geometry = *mesh._geometry;

	GLuint program = bindShaderProgram(mesh.shaderPrograms[drawState.shaderName]);
	updateUniforms(scene.camera, program, entityObj);
	setBuffersAndAttributes(program, entityObj);

	Buffer& buffer = bufferManager.getBufferById(geometry.bufferId);
	glBindVertexArray(buffer._vao);
	BufferRange bufferInfo = buffer.getBufferInfo(&geometry);

	glDrawElements(
		GL_TRIANGLES, 
		bufferInfo.indexBufferLength, 
		GL_UNSIGNED_INT, 
		/*(void*) (bufferInfo.indexBufferStart * sizeof(GL_UNSIGNED_INT))*/0
	);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Renderer::renderScene(const Scene& scene, const std::vector<std::reference_wrapper<Object3D>>& entityObjects) {

	for (int state = 0; state < drawPassStates.size(); state++) {
		DrawPassState& drawState = drawPassStates[state];
		glBindFramebuffer(GL_FRAMEBUFFER, drawState.fbo);

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
		//clear(drawState.fbo);
	}
}

void Renderer::initBufferAttributes(GLuint program, BufferAttributes& bufferAttributes) {

	for (const std::string& name : bufferAttributes.getAttributeNames()) {

		const GLint location = glGetAttribLocation(program, name.c_str());

		std::cout << name << "," << location << "\n";

		const int stride = bufferAttributes.getStride();

		std::visit([location, stride](auto& bufferAttribute) {

			glVertexAttribPointer(
				location,
				bufferAttribute.attribLength,
				GL_FLOAT,
				false,
				stride * sizeof(GL_FLOAT),
				(void*) (bufferAttribute.offset * sizeof(GL_FLOAT))
			);

		}, bufferAttributes.getBufferAttributeGeneric(name));

		glEnableVertexAttribArray(location);

		if (bufferAttributes.isInstanceBuffer) {
			glVertexAttribDivisor(location, 1);
		}
	}
}

void Renderer::updateUniforms(const Camera& camera, GLuint program, Object3D& object3D) {

	GLuint worldLoc = glGetUniformLocation(program, "world");
	GLuint cameraLoc = glGetUniformLocation(program, "camera");
	GLuint perspectiveLoc = glGetUniformLocation(program, "perspective");

	glm::mat4 cameraMatrix = camera.getCameraMatrix();
	glm::mat4 perspectiveMatrix = camera.getPerspectiveMatrix(1.0);

	glUniformMatrix4fv(worldLoc, 1, false, glm::value_ptr(object3D.worldMatrix));
	glUniformMatrix4fv(cameraLoc, 1, false, glm::value_ptr(cameraMatrix));
	glUniformMatrix4fv(perspectiveLoc, 1, false, glm::value_ptr(perspectiveMatrix));

	updateModelUniforms(program, object3D);
}

void Renderer::updateModelUniforms(GLuint program, Object3D& object3D) {

	for (const std::pair<std::string, Shader>& shader : object3D.getMesh().shaderPrograms) {
		const std::string& shaderName = shader.first;

		for (const std::pair<std::string, UniformType>& uniform : object3D.getMesh().shaderPrograms[shaderName].uniforms) {
			const std::string &uniformName = uniform.first;
			std::visit(overload{
				[program, &uniformName](const glm::vec1& uniformData) {
					glUniform1fv(glGetAttribLocation(program, uniformName.c_str()), 1, &uniformData[0]);
				},
				[program, &uniformName](const glm::vec2& uniformData) {
					glUniform2fv(glGetAttribLocation(program, uniformName.c_str()), 1, glm::value_ptr(uniformData));
				},
				[program, &uniformName](const glm::vec3& uniformData) {
					glUniform3fv(glGetAttribLocation(program, uniformName.c_str()), 1, glm::value_ptr(uniformData));
				},
				[program, &uniformName](const glm::vec4& uniformData) {
					glUniform4fv(glGetAttribLocation(program, uniformName.c_str()), 1, glm::value_ptr(uniformData));
				},
				[program, &uniformName](const glm::mat4& uniformData) {
					glUniformMatrix4fv(glGetAttribLocation(program, uniformName.c_str()), 1, false, glm::value_ptr(uniformData));
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
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	return shaderProgram;
}

GLuint Renderer::loadShader(GLenum shaderType, const std::string& source) {
	GLuint shader = glCreateShader(shaderType);
	const GLchar* cSource = source.c_str();
	glShaderSource(shader, 1, &cSource, NULL);
	glCompileShader(shader);

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
	glBindVertexArray(VAO);

	GLuint VBO = initBuffer<float>(geometry.bufferAttributes, false);
	initBufferAttributes(program, geometry.bufferAttributes);

	GLuint IBO = initBuffer<int>(geometry.indexBuffer, true);

	int bufferId = bufferManager.addBuffer(VBO, IBO, VAO);
	Buffer &buffer = bufferManager.getBufferById(bufferId);
	buffer.addBufferGeometry(&geometry);

	glBindVertexArray(0);

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
	bindTexture(program, object3D.getMesh()._material);

	if (geometry.bufferId == -1) {

		if (geometry.bufferName != "") {

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

	if (!buffer.geometryInBuffer(&geometry)) {
		buffer.addBufferGeometry(&geometry);
	}

	glBindVertexArray(buffer._vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer._vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer._ibo);

	// Make a temp copy of index buffer data and add real buffer base position to offset
	std::vector<int> newIndexBuffer = geometry.indexBuffer.mergeAttributes<int>();
	
	BufferRange bufferInfo = buffer.getBufferInfo(&geometry);

	for (int i = 0; i < newIndexBuffer.size(); i++) {
		newIndexBuffer[i] += bufferInfo.indexBufferStart;
	}

	// Update indices
	glBufferSubData(
		GL_ELEMENT_ARRAY_BUFFER, 
		bufferInfo.indexBufferStart, 
		newIndexBuffer.size() * sizeof(GL_UNSIGNED_INT), 
		&newIndexBuffer
	);

	std::vector<float> newVertexBuffer = geometry.bufferAttributes.mergeAttributes<float>();

	// Update vertices
	glBufferSubData(
		GL_ELEMENT_ARRAY_BUFFER, 
		bufferInfo.vertexBufferStart, 
		newVertexBuffer.size() * sizeof(GL_UNSIGNED_INT), 
		&newVertexBuffer
	);

	glBindVertexArray(0);
}

void Renderer::clear(GLuint fbo) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}