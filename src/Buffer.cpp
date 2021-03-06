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

	int geometryVertexLength = geometry->vertexBuffer.getLength();
	int geometryIndexLength = geometry->indexBuffer.getLength();

	BufferRange bufferRange{vertexBufferLength, 0, indexBufferLength, 0, 0};
	geometries.emplace(std::make_pair(geometry, bufferRange));
	updateBufferGeometry(geometry);
}

void Buffer::updateBufferGeometry(Geometry* geometry) {
	BufferRange& bufferRange = geometries.at(geometry);

	this->vertexBufferLength += geometry->vertexBuffer.getLength() - bufferRange.vertexBufferLength;
	this->indexBufferLength += geometry->indexBuffer.getLength() - bufferRange.indexBufferLength;
	this->indexCount += geometry->vertexBuffer.getNoElements() - bufferRange.indexCount;

	bufferRange.vertexBufferLength = geometry->vertexBuffer.getLength();
	bufferRange.indexBufferLength = geometry->indexBuffer.getLength();
	bufferRange.indexCount = geometry->vertexBuffer.getNoElements();
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

int Buffer::getIndexCount() {
	return indexCount;
}

int Buffer::getGeometryCount() {
	return geometries.size();
}