#include "Renderer.h"
#include "Shader.h"
#include "BufferAttributes.h"
#include <string>
#include <iostream>
#include <variant>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

void Renderer::renderEntity(Object3D entityObj, const Scene& scene, const DrawPassState& drawState) {

}

void Renderer::initBufferAttributes(ShaderInfo shaderInfo, BufferAttributes& bufferAttributes) {

	for (const std::string& name : bufferAttributes.getAttributeNames()) {

		const GLint location = glGetAttribLocation(shaderInfo.programId, name.c_str());
		const int stride = bufferAttributes.stride;

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

GLuint* Renderer::initBuffer(BufferAttributes& bufferAttributes) {
	GLuint* bufferObject;

	glGenBuffers(1, bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[0]);

	glBufferData(GL_ARRAY_BUFFER, bufferAttributes.sizeBytes * sizeof(GL_FLOAT), NULL, GL_STATIC_DRAW);

	std::vector<float> mergedAttributes = bufferAttributes.mergeAttributes();

	int bufferSize = bufferAttributes.sizeBytes > 0 ? mergedAttributes.size() : bufferAttributes.sizeBytes;

	glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize * sizeof(GL_FLOAT), &mergedAttributes);

	return bufferObject;
}

void Renderer::updateUniforms(const Camera& camera, GLuint program, Object3D& object3D) {

	GLuint worldLoc = glGetUniformLocation(program, "world");
	GLuint cameraLoc = glGetUniformLocation(program, "camera");
	GLuint perspectiveLoc = glGetUniformLocation(program, "perspective");

	glm::mat4 cameraMatrix = camera.getCameraMatrix();
	glm::mat4 perspectiveMatrix = camera.getPerspectiveMatrix();

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