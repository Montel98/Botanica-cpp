#pragma once

#include <map>
#include "Geometry.h"

using BufferId = unsigned int;

struct BufferRange {
	int vertexBufferStart, vertexBufferLength;
	int indexBufferStart, indexBufferLength;
};

class Buffer {
private:
	int _vbo;
	int _ibo;
	int _vao;
	int indexCount;
	int vertexBufferLength;
	int indexBufferLength;
	std::map<Geometry*, BufferRange> entities;
public:
	Buffer(int vbo, int ibo, int vao);
	Buffer() = default;
	bool isEmpty();
	void addEntityBuffer(Geometry* geometry);
	void removeEntityBuffer(Geometry* geometry);
	BufferRange getBufferInfo(Geometry* geometry);
};

class BufferManager {
private:
	int bufferIdCount;
	std::map<BufferId, Buffer> buffers;
	std::map<std::string, BufferId> aliases;

public:
	BufferId addBuffer(std::string name, int vbo, int ibo, int vao);
	BufferId addBuffer(int vbo, int ibo, int vao);
	Buffer& getBufferById(BufferId id);
	BufferId getBufferIdByName(const std::string& alias);
	void removeBuffer(BufferId id);
	bool bufferExists(const std::string& alias);
};