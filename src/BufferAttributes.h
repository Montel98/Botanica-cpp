#pragma once

#include <map>
#include <set>
#include <exception>
#include <glm/glm.hpp>
#include <variant>

template<class... Ts> 
struct overload : Ts... {
	using Ts::operator()...;
};

template<class... Ts>
overload(Ts...) -> overload<Ts...>;

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

using BufferAttributeVec = std::variant
<
	BufferAttribute<glm::vec1>,
	BufferAttribute<glm::vec2>, 
	BufferAttribute<glm::vec3>,
	BufferAttribute<glm::vec4>,
	BufferAttribute<glm::mat4>
>;

class BufferAttributes {
private:
	int indexesUsed; // Number of buffer attribute locations used

	std::map<std::string, BufferAttributeVec> attributes;

	bool isAttributeNameUsed(const std::string &name) const;

	template<typename T>
	void addBufferAttribute(const std::string& name, std::map<std::string, BufferAttribute<T>>& attributes);

public:

	int stride; // Total length (in vector units) of one line of attributes
	bool isInstanceBuffer;
	int sizeBytes;

	BufferAttributes();

	std::vector<std::string> getAttributeNames() const;

	void mergeBufferAttributes(const BufferAttributes& other);

	template<typename T>
	void addBufferAttribute(const std::string& name);

	template<typename T>
	void setBufferAttributeData(const std::string& name, std::vector<T> newBufferData);

	template<typename T>
	BufferAttribute<T>& getBufferAttribute(const std::string& name);

	BufferAttributeVec& getBufferAttributeGeneric(const std::string& name);

	template<typename T>
	void removeBufferAttribute(const std::string &name);

	std::vector<float> mergeAttributes();
};

template<typename T>
void BufferAttributes::addBufferAttribute(const std::string& name) {

	BufferAttribute<T> newAttribute{T::length(), stride, ++indexesUsed};
	attributes[name] = BufferAttributeVec{newAttribute};
	stride += T::length();
}

template<typename T>
void BufferAttributes::setBufferAttributeData(const std::string& name, std::vector<T> newBufferData) {
	std::get<BufferAttribute<T>>(attributes[name]).bufferData = std::move(newBufferData);
}

template<typename T>
BufferAttribute<T>& BufferAttributes::getBufferAttribute(const std::string& name) {
	return std::get<BufferAttribute<T>>(attributes[name]);
}

template<typename T>
void BufferAttributes::removeBufferAttribute(const std::string &name) {
	stride -= T::length();
}