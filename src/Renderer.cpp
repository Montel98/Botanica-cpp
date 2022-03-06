#include "Renderer.h"
#include <string>

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