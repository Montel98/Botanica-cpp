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

	std::map<std::string, BufferAttribute<glm::vec1>> attributesVec1;
	std::map<std::string, BufferAttribute<glm::vec2>> attributesVec2;
	std::map<std::string, BufferAttribute<glm::vec3>> attributesVec3;
	std::map<std::string, BufferAttribute<glm::vec4>> attributesVec4;
	std::map<std::string, BufferAttribute<glm::mat4>> attributesMat4;

	std::set<std::string> attributeNames;

	bool isAttributeNameUsed(const std::string &name) const;

public:

	// Creates a new empty buffer attribute of specified type with given name
	void addBufferAttributeVec1(const std::string &name);
	void addBufferAttributeVec2(const std::string &name);
	void addBufferAttributeVec3(const std::string &name);
	void addBufferAttributeVec4(const std::string &name);
	void addBufferAttributeMat4(const std::string &name);


	// Sets the buffer data of given name and type, if it exists
	void setBufferAttributeDataVec1(const std::string &name, const std::vector<glm::vec1> &newBufferData);
	void setBufferAttributeDataVec2(const std::string &name, const std::vector<glm::vec2> &newBufferData);
	void setBufferAttributeDataVec3(const std::string &name, const std::vector<glm::vec3> &newBufferData);
	void setBufferAttributeDataVec4(const std::string &name, const std::vector<glm::vec4> &newBufferData);
	void setBufferAttributeDataMat4(const std::string &name, const std::vector<glm::mat4> &newBufferData);


	// Gets the buffer attribute of given name and type
	// If it doesn't exist, a new empty buffer attribute is created
	BufferAttribute<glm::vec1>& getBufferAttributeVec1(const std::string &name);
	BufferAttribute<glm::vec2>& getBufferAttributeVec2(const std::string &name);
	BufferAttribute<glm::vec3>& getBufferAttributeVec3(const std::string &name);
	BufferAttribute<glm::vec4>& getBufferAttributeVec4(const std::string &name);
	BufferAttribute<glm::mat4>& getBufferAttributeMat4(const std::string &name); 

	void removeBufferAttribute(const std::string &name);

	int getStride();
};