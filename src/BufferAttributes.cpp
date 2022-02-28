#include <map>
#include <set>
#include <exception>
#include "Geometry.h"

void BufferAttributes::addBufferAttributeVec1(const std::string &name) {
	addBufferAttribute<glm::vec1>(name, attributesVec1);
}

void BufferAttributes::addBufferAttributeVec2(const std::string &name) {
	addBufferAttribute<glm::vec2>(name, attributesVec2);
}

void BufferAttributes::addBufferAttributeVec3(const std::string &name) {
	addBufferAttribute<glm::vec3>(name, attributesVec3);
}

void BufferAttributes::addBufferAttributeVec4(const std::string &name) {
	addBufferAttribute<glm::vec4>(name, attributesVec4);
}

void BufferAttributes::addBufferAttributeMat4(const std::string &name) {
	addBufferAttribute<glm::mat4>(name, attributesMat4);
}

void BufferAttributes::mergeBufferAttributesVec1(BufferAttributes& other) {
	mergeBufferAttributes<glm::vec1>(other.attributesVec1, attributesVec1);
}

void BufferAttributes::mergeBufferAttributesVec2(BufferAttributes& other) {
	mergeBufferAttributes<glm::vec2>(other.attributesVec2, attributesVec2);
}

void BufferAttributes::mergeBufferAttributesVec3(BufferAttributes& other) {
	mergeBufferAttributes<glm::vec3>(other.attributesVec3, attributesVec3);	
}

void BufferAttributes::mergeBufferAttributesVec4(BufferAttributes& other) {
	mergeBufferAttributes<glm::vec4>(other.attributesVec4, attributesVec4);
}

void BufferAttributes::mergeBufferAttributesMat4(BufferAttributes& other) {
	mergeBufferAttributes<glm::mat4>(other.attributesMat4, attributesMat4);
}

void BufferAttributes::setBufferAttributeDataVec1(const std::string &name, std::vector<glm::vec1> newBufferData) {
	attributesVec1[name].bufferData = std::move(newBufferData);
}

void BufferAttributes::setBufferAttributeDataVec2(const std::string &name, std::vector<glm::vec2> newBufferData) {
	attributesVec2[name].bufferData = std::move(newBufferData);
}

void BufferAttributes::setBufferAttributeDataVec3(const std::string &name, std::vector<glm::vec3> newBufferData) {
	attributesVec3[name].bufferData = std::move(newBufferData);
}

void BufferAttributes::setBufferAttributeDataVec4(const std::string &name, std::vector<glm::vec4> newBufferData) {
	attributesVec4[name].bufferData = std::move(newBufferData);
}

void BufferAttributes::setBufferAttributeDataMat4(const std::string &name, std::vector<glm::mat4> newBufferData) {
	attributesMat4[name].bufferData = std::move(newBufferData);
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

template <typename V>
std::vector<std::string> BufferAttributes::getAttributeNames(const std::map<std::string, BufferAttribute<V>>& attributesVec) const {
	std::vector<std::string> attributeNames;

	for (const std::pair<std::string, BufferAttribute<V>>& attrib : attributesVec) {
		attributeNames.push_back(attrib.first);
	}

	return attributeNames;
}

std::vector<std::string> BufferAttributes::getAttributeNamesVec1() const {
	return getAttributeNames<glm::vec1>(attributesVec1);
}

std::vector<std::string> BufferAttributes::getAttributeNamesVec2() const {
	return getAttributeNames<glm::vec2>(attributesVec2);
}

std::vector<std::string> BufferAttributes::getAttributeNamesVec3() const {
	return getAttributeNames<glm::vec3>(attributesVec3);
}

std::vector<std::string> BufferAttributes::getAttributeNamesVec4() const {
	return getAttributeNames<glm::vec4>(attributesVec4);
}

std::vector<std::string> BufferAttributes::getAttributeNamesMat4() const {
	return getAttributeNames<glm::mat4>(attributesMat4);
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