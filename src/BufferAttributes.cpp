#include <map>
#include <set>
#include <exception>
#include "Geometry.h"

void BufferAttributes::addBufferAttributeVec1(const std::string &name) {

	if (isAttributeNameUsed(name)) {
		throw BufferAttributeException();
	}
	attributesVec1[name] = {1, stride, ++indexesUsed};
	stride += 1;
}

void BufferAttributes::addBufferAttributeVec2(const std::string &name) {

	if (isAttributeNameUsed(name)) {
		throw BufferAttributeException();
	}
	attributesVec2[name] = {2, stride, ++indexesUsed};
	stride += 2;
}

void BufferAttributes::addBufferAttributeVec3(const std::string &name) {

	if (isAttributeNameUsed(name)) {
		throw BufferAttributeException();
	}
	attributesVec3[name] = {3, stride, ++indexesUsed};
	stride += 3;
}

void BufferAttributes::addBufferAttributeVec4(const std::string &name) {

	if (isAttributeNameUsed(name)) {
		throw BufferAttributeException();
	}
	attributesVec4[name] = {4, stride, ++indexesUsed};
	stride += 4;
}

void BufferAttributes::addBufferAttributeMat4(const std::string &name) {

	if (isAttributeNameUsed(name)) {
		throw BufferAttributeException();
	}
	attributesMat4[name] = {16, stride, ++indexesUsed};
	stride += 16;
}

void BufferAttributes::setBufferAttributeDataVec1(const std::string &name, const std::vector<glm::vec1> &newBufferData) {
	attributesVec1[name].bufferData = newBufferData;
}

void BufferAttributes::setBufferAttributeDataVec2(const std::string &name, const std::vector<glm::vec2> &newBufferData) {
	attributesVec2[name].bufferData = newBufferData;
}

void BufferAttributes::setBufferAttributeDataVec3(const std::string &name, const std::vector<glm::vec3> &newBufferData) {
	attributesVec3[name].bufferData = newBufferData;
}

void BufferAttributes::setBufferAttributeDataVec4(const std::string &name, const std::vector<glm::vec4> &newBufferData) {
	attributesVec4[name].bufferData = newBufferData;
}

void BufferAttributes::setBufferAttributeDataMat4(const std::string &name, const std::vector<glm::mat4> &newBufferData) {
	attributesMat4[name].bufferData = newBufferData;
}

BufferAttribute<glm::vec1>& BufferAttributes::getBufferAttributeVec1(const std::string &name) {
	return attributesVec1[name];
}

BufferAttribute<glm::vec2>& BufferAttributes::getBufferAttributeVec2(const std::string &name) {
	return attributesVec2[name];
}

BufferAttribute<glm::vec3>& BufferAttributes::getBufferAttributeVec3(const std::string &name) {
	return attributesVec3[name];
}

BufferAttribute<glm::vec4>& BufferAttributes::getBufferAttributeVec4(const std::string &name) {
	return (attributesVec4[name]);
}

BufferAttribute<glm::mat4>& BufferAttributes::getBufferAttributeMat4(const std::string &name) {
	return attributesMat4[name];
}

// Removes attribute from the vertex attributes by name, if it exists
void BufferAttributes::removeBufferAttribute(const std::string &name) {

	if (attributesVec1.count(name) > 0) {
		attributesVec1.erase(name);
		stride -= 1;
	}
	else if (attributesVec2.count(name) > 0) {
		attributesVec2.erase(name);
		stride -= 2;
	}
	else if (attributesVec3.count(name) > 0) {
		attributesVec3.erase(name);
		stride -= 3;
	}
	else if (attributesVec4.count(name) > 0) {
		attributesVec4.erase(name);
		stride -= 4;
	}
	else if (attributesMat4.count(name) > 0) {
		attributesMat4.erase(name);
		stride -= 16;
	}
}

// The total length of one line of vertex attributes
int BufferAttributes::getStride() {
	return stride;
}

bool BufferAttributes::isAttributeNameUsed(const std::string &name) const {
	return (attributeNames.find(name) != attributeNames.end());
}

const char * BufferAttributeException::what() const noexcept {
	return "Attribute name already exists";
}