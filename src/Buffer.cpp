#include "Buffer.h"
#include <utility>

Buffer::Buffer(int vbo, int ibo, int vao) : _vbo(vbo), _ibo(ibo), _vao(vao) {
}

bool Buffer::isEmpty() {
	return entities.empty();
}

void Buffer::addEntityBuffer(Geometry* geometry) {
	BufferRange bufferRange{0,0,0,0};
	entities[geometry] = bufferRange;
}

void Buffer::removeEntityBuffer(Geometry* geometry) {
	entities.erase(geometry);
}

BufferRange Buffer::getBufferInfo(Geometry* geometry) {
	return BufferRange(entities[geometry]);
}

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
	return buffers[id];
}

Buffer& BufferManager::getBufferByName(std::string alias) {
	return buffers[aliases[alias]];
}