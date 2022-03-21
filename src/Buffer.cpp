#include "Buffer.h"
#include <utility>
#include <iostream>

Buffer::Buffer(int vbo, int ibo, int vao) : 
indexCount(0), vertexBufferLength(0), indexBufferLength(0), _vbo(vbo), _ibo(ibo), _vao(vao) {
}

bool Buffer::isEmpty() {
	return geometries.empty();
}

void Buffer::addBufferGeometry(Geometry* geometry) {

	int geometryVertexLength = geometry->bufferAttributes.getLength();
	int geometryIndexLength = geometry->indexBuffer.getLength();

	//std::cout << geometryVertexLength << " MMM " << geometryIndexLength << "\n";

	BufferRange bufferRange{
		vertexBufferLength, 
		geometryVertexLength, 
		indexBufferLength, 
		geometryIndexLength
	};

	vertexBufferLength += geometryVertexLength;
	indexBufferLength += geometryIndexLength;
	//indexCount += geometry->bufferAttributes.getBufferAttribute<glm::vec3>("aVertexPosition").bufferData.size();
	indexCount = geometry->bufferAttributes.getLength() / geometry->bufferAttributes.getStride();
	geometries.emplace(std::make_pair(geometry, bufferRange));
}

void Buffer::removeBufferGeometry(Geometry* geometry) {
	geometries.erase(geometry);
}

BufferRange Buffer::getBufferInfo(Geometry* geometry) {
	return BufferRange(geometries.at(geometry));
}

bool Buffer::geometryInBuffer(Geometry* geometry) {
	return geometries.find(geometry) != geometries.end();
}

BufferManager::BufferManager() : bufferIdCount(0) {}

BufferId BufferManager::addBuffer(std::string name, int vbo, int ibo, int vao) {
	bufferIdCount++;
	aliases[name] = bufferIdCount;
	buffers.emplace(std::make_pair(bufferIdCount, Buffer(vbo, ibo, vao)));
	return bufferIdCount;
}

BufferId BufferManager::addBuffer(int vbo, int ibo, int vao) {
	bufferIdCount++;
	buffers.emplace(std::make_pair(bufferIdCount, Buffer(vbo, ibo, vao)));
	return bufferIdCount;
}

Buffer& BufferManager::getBufferById(BufferId id) {
	return buffers.at(id);
}

BufferId BufferManager::getBufferIdByName(const std::string& alias) {
	return aliases[alias];
}

bool BufferManager::bufferExists(const std::string& alias) {
	return aliases.find(alias) != aliases.end();
}