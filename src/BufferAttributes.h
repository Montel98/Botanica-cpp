#pragma once

#include <map>
#include <set>
#include <exception>
#include <glm/glm.hpp>

class BufferAttributeException : public std::exception {
public:
	const char * what() const noexcept;
};

template <typename T>
struct BufferAttribute {
	int attribLength;
	int offset;
	int index;
	std::vector<T> bufferData;
};

class BufferAttributes {
private:
	int stride; // Total length (in vector units) of one line of attributes
	int indexesUsed; // Number of buffer attribute locations used

	std::map<std::string, BufferAttribute<glm::ivec1>> attributesIVec1;
	std::map<std::string, BufferAttribute<glm::vec1>> attributesVec1;
	std::map<std::string, BufferAttribute<glm::vec2>> attributesVec2;
	std::map<std::string, BufferAttribute<glm::vec3>> attributesVec3;
	std::map<std::string, BufferAttribute<glm::vec4>> attributesVec4;
	std::map<std::string, BufferAttribute<glm::mat4>> attributesMat4;

	std::set<std::string> attributeNames;

	bool isAttributeNameUsed(const std::string &name) const;

	template<typename V>
	std::vector<std::string> getAttributeNames(const std::map<std::string, BufferAttribute<V>>& attributesVec) const;

	template<typename T>
	void addBufferAttribute(const std::string& name, std::map<std::string, BufferAttribute<T>>& attributes);

	template<typename T>
	void mergeBufferAttributes(
		std::map<std::string, BufferAttribute<T>>& destAttributes, 
		std::map<std::string, BufferAttribute<T>>& sourceAttributes
	);

public:

	// Creates a new empty buffer attribute of specified type with given name
	void addBufferAttributeVec1(const std::string &name);
	void addBufferAttributeVec2(const std::string &name);
	void addBufferAttributeVec3(const std::string &name);
	void addBufferAttributeVec4(const std::string &name);
	void addBufferAttributeMat4(const std::string &name);

	void mergeBufferAttributesVec1(BufferAttributes& other);
	void mergeBufferAttributesVec2(BufferAttributes& other);
	void mergeBufferAttributesVec3(BufferAttributes& other);
	void mergeBufferAttributesVec4(BufferAttributes& other);
	void mergeBufferAttributesMat4(BufferAttributes& other);

	// Sets the buffer data of given name and type, if it exists
	void setBufferAttributeDataVec1(const std::string &name, std::vector<glm::vec1> newBufferData);
	void setBufferAttributeDataVec2(const std::string &name, std::vector<glm::vec2> newBufferData);
	void setBufferAttributeDataVec3(const std::string &name, std::vector<glm::vec3> newBufferData);
	void setBufferAttributeDataVec4(const std::string &name, std::vector<glm::vec4> newBufferData);
	void setBufferAttributeDataMat4(const std::string &name, std::vector<glm::mat4> newBufferData);

	// Gets the buffer attribute of given name and type
	// If it doesn't exist, a new empty buffer attribute is created
	BufferAttribute<glm::vec1>& getBufferAttributeVec1(const std::string &name);
	BufferAttribute<glm::vec2>& getBufferAttributeVec2(const std::string &name);
	BufferAttribute<glm::vec3>& getBufferAttributeVec3(const std::string &name);
	BufferAttribute<glm::vec4>& getBufferAttributeVec4(const std::string &name);
	BufferAttribute<glm::mat4>& getBufferAttributeMat4(const std::string &name);

	// Get list of attribute names in use by type
	std::vector<std::string> getAttributeNamesVec1() const;
	std::vector<std::string> getAttributeNamesVec2() const;
	std::vector<std::string> getAttributeNamesVec3() const;
	std::vector<std::string> getAttributeNamesVec4() const;
	std::vector<std::string> getAttributeNamesMat4() const;

	void removeBufferAttribute(const std::string &name);

	int getStride();
};

template<typename T>
void BufferAttributes::addBufferAttribute(const std::string& name, std::map<std::string, BufferAttribute<T>>& attributes) {
	if (isAttributeNameUsed(name)) {
		throw BufferAttributeException();
	}
	attributeNames.insert(name);
	attributes[name] = {T::length(), stride, ++indexesUsed};
	stride += T::length();
}

// Appends source attributes to the destination attributes (i.e. the attributes of the current buffer)
// Only appends if attribute already exists
template<typename T>
void BufferAttributes::mergeBufferAttributes(
	std::map<std::string, BufferAttribute<T>>& sourceAttributes, 
	std::map<std::string, BufferAttribute<T>>& destAttributes) {

	for (std::pair<std::string, BufferAttribute<T>> sourceAttrib : sourceAttributes) {
		std::string& name = sourceAttrib.first;
		std::vector<T>& sourceBufferData = sourceAttrib.second.bufferData;

		if (isAttributeNameUsed(name)) {
			destAttributes[name].bufferData.insert(
				destAttributes[name].bufferData.end(), 
				sourceBufferData.begin(), 
				sourceBufferData.end()
			);
		}
	}
}