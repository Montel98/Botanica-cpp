#include "InstanceBuffer.h"

InstanceBuffer::InstanceBuffer() : ibo(-1) {
	instanceAttributes.addBufferAttribute<glm::mat4>("aPose");
}

int InstanceBuffer::addInstance(const std::map<std::string, BufferAttributeElement>& attribs) {
	for (const std::string& name : instanceAttributes.getAttributeNames()) {

		std::visit([&name, this](auto& bufferAttributeData) {
			this->instanceAttributes.appendBufferAttributeData(name, bufferAttributeData);
		}, attribs.at(name));
	}

	return instanceAttributes.getNoElements() - 1;
}

void InstanceBuffer::removeInstance(int index) {
	instanceAttributes.sliceBufferAttributes(index, 1);
}

int InstanceBuffer::getInstanceCount() {
	return instanceAttributes.getNoElements();
}