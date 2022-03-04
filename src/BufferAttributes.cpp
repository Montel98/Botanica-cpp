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

// The total length of one line of vertex attributes
int BufferAttributes::getStride() {
	return stride;
}

bool BufferAttributes::isAttributeNameUsed(const std::string &name) const {
	return (attributes.find(name) != attributes.end());
}

const char * BufferAttributeException::what() const noexcept {
	return "Contradictory attribute properties";
}

class BufferTypeVisitor {
public:
	template<typename T>
	void operator()(T& a, const T& b) {
		a.bufferData.insert(
			a.bufferData.end(), 
			b.bufferData.begin(), 
			b.bufferData.end()
		);
	}

	template<typename A, typename B>
	void operator()(A& a, B& b) {
		throw BufferAttributeException();
	}
};

// Appends source attributes to the destination attributes (i.e. the attributes of the current buffer)
// Only appends if attribute already exists and attribute types of the same name are equal
void BufferAttributes::mergeBufferAttributes(const BufferAttributes& other) {

	for (const std::pair<std::string, BufferAttributeVec>& otherAttrib : other.attributes) {
		const std::string& name = otherAttrib.first;

		if (isAttributeNameUsed(name)) {
			std::visit(BufferTypeVisitor{}, attributes[name], otherAttrib.second);
		}
	}
}