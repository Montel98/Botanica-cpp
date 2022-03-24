#pragma once

#include <map>
#include <set>
#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <variant>
#include <vector>

template<class... Ts> 
struct overload : Ts... {
	using Ts::operator()...;
};

template<class... Ts>
overload(Ts...) -> overload<Ts...>;

class BufferAttributeException : public std::exception {
public:
	const char* what() const noexcept;
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
	BufferAttribute<glm::ivec1>,
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

	int length; // Total number of components in merged buffer
	int stride; // Total length (in vector units) of one line of attributes
public:

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
	void appendBufferAttributeData(const std::string& name, const std::vector<T>& newBufferData);

	template<typename T>
	BufferAttribute<T>& getBufferAttribute(const std::string& name);

	BufferAttributeVec& getBufferAttributeGeneric(const std::string& name);

	template<typename T>
	void removeBufferAttribute(const std::string &name);

	template<typename T>
	std::vector<T> mergeAttributes(unsigned int indexStart, unsigned int indexLength);

	void sliceBufferAttributes(unsigned int start, unsigned int length);

	int getLength() const;

	int getStride() const;

	int getNoElements() const;
};

template<typename T>
void BufferAttributes::addBufferAttribute(const std::string& name) {

	if (!isAttributeNameUsed(name)) {
		BufferAttribute<T> newAttribute{T::length(), stride, ++indexesUsed};
		attributes.insert(std::make_pair(name, BufferAttributeVec{newAttribute}));
		stride += T::length();
	}
}

template<typename T>
void BufferAttributes::setBufferAttributeData(const std::string& name, std::vector<T> newBufferData) {
	BufferAttribute<T>& bufferAttribute = std::get<BufferAttribute<T>>(attributes[name]);
	length += T::length() * (newBufferData.size() - bufferAttribute.bufferData.size());
	bufferAttribute.bufferData = std::move(newBufferData);
}

template<typename T>
void BufferAttributes::appendBufferAttributeData(const std::string &name, const std::vector<T>& newBufferData) {
	BufferAttribute<T>& bufferAttribute = std::get<BufferAttribute<T>>(attributes[name]);
	length += T::length() * newBufferData.size();
	bufferAttribute.bufferData.insert(bufferAttribute.bufferData.end(), newBufferData.begin(), newBufferData.end());
}

template<typename T>
BufferAttribute<T>& BufferAttributes::getBufferAttribute(const std::string& name) {
	return std::get<BufferAttribute<T>>(attributes[name]);
}

template<typename T>
void BufferAttributes::removeBufferAttribute(const std::string &name) {
	length -= std::get<BufferAttribute<T>>(attributes[name]).bufferData.size();
	stride -= T::length();
	attributes.erase(name);
}

template<typename T>
std::vector<T> BufferAttributes::mergeAttributes(unsigned int indexStart, unsigned int indexLength) {

	//std::vector<T> buffer(getLength());

	int bufferStride = getStride();

	std::vector<T> buffer(bufferStride * indexLength);

	for (const std::string& name : getAttributeNames()) {

		std::visit(overload{
			[&buffer, &bufferStride, &indexStart, &indexLength](BufferAttribute<glm::vec1>& bufferAttribute) {

				for(/*int i = 0; i < bufferAttribute.bufferData.size();*/int i = indexStart; i < indexStart + indexLength; i++) {
					buffer[(bufferStride * i) + bufferAttribute.offset] = bufferAttribute.bufferData[i][0];
				}
			},
			[&buffer, &bufferStride, &indexStart, &indexLength](BufferAttribute<glm::ivec1>& bufferAttribute) {

				for(/*int i = 0; i < bufferAttribute.bufferData.size();*/int i = indexStart; i < indexStart + indexLength; i++) {
					buffer[(bufferStride * i) + bufferAttribute.offset] = bufferAttribute.bufferData[i][0];
				}
			},
			[&buffer, &bufferStride, &indexStart, &indexLength](auto& bufferAttribute) {

				for(/*int i = 0; i < bufferAttribute.bufferData.size();*/int i = indexStart; i < indexStart + indexLength; i++) {
					for (int j = 0; j < bufferAttribute.attribLength; j++) {
						buffer[(bufferStride * i) + bufferAttribute.offset + j] = glm::value_ptr(bufferAttribute.bufferData[i])[j];
					}
				}
			}
		}, attributes[name]);
	}

	return buffer;
}