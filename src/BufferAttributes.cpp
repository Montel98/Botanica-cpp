#include <map>
#include <set>
#include <exception>
#include "Geometry.h"

std::vector<std::string> BufferAttributes::getAttributeNames() const {
	std::vector<std::string> attributeNames;

	for (const std::pair<std::string, BufferAttributeVec>& attrib : attributes) {
		attributeNames.push_back(attrib.first);
	}

	return attributeNames;
}

bool BufferAttributes::isAttributeNameUsed(const std::string &name) const {
	return (attributes.find(name) != attributes.end());
}

const char * BufferAttributeException::what() const noexcept {
	return "Contradictory attribute properties";
}

class BufferTypeVisitor {
private:
	BufferAttributes& _bufferAttributes;
	std::string _name;
public:
	BufferTypeVisitor(BufferAttributes& bufferAttributes, const std::string& name) : _name(name), _bufferAttributes(bufferAttributes) {};

	template<template <typename> typename B, typename T>
	void operator()(const B<T>& b) {
		_bufferAttributes.appendBufferAttributeData<T>(_name, b.bufferData);
	}
};

// Appends source attributes to the destination attributes (i.e. the attributes of the current buffer)
// Only appends if attribute already exists and attribute types of the same name are equal
void BufferAttributes::mergeBufferAttributes(const BufferAttributes& other) {

	for (const std::pair<const std::string, BufferAttributeVec>& otherAttrib : other.attributes) {
		const std::string& name = otherAttrib.first;

		if (isAttributeNameUsed(name)) {
			std::visit(BufferTypeVisitor(*this, name), otherAttrib.second);
		}
	}
}


// Removes elements from every attribute within a specified interval
void BufferAttributes::sliceBufferAttributes(unsigned int start, unsigned int length) {
	int& noElements = this->length;

	for (std::pair<const std::string, BufferAttributeVec>& bufferAttribute : attributes) {
		std::visit([&noElements, start, length](auto& attrib) {
			attrib.bufferData.erase(attrib.bufferData.begin() + start, attrib.bufferData.begin() + start + length);
			noElements -= attrib.attribLength * length;
		}, bufferAttribute.second);
	}
}

BufferAttributeVec& BufferAttributes::getBufferAttributeGeneric(const std::string& name) {
	return attributes[name];
}

int BufferAttributes::getLength() const {
	return length;
}

int BufferAttributes::getStride() const {
	return stride;
}

int BufferAttributes::getNoElements() const {
	return length / stride;
}

BufferAttributes::BufferAttributes() : sizeBytes(0), length(0), stride(0), isInstanceBuffer(false) {};