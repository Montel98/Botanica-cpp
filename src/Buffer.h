#pragma once

#include <map>
#include "Geometry.h"

using BufferId = unsigned int;

struct BufferRange {
	int vertexBufferStart, vertexBufferLength;
	int indexBufferStart, indexBufferLength;
	int indexCount;
};

class Buffer {
private:
	int indexCount;
	int vertexBufferLength;
	int indexBufferLength;
	std::map<Geometry*, BufferRange> geometries;
public:
	int _vbo;
	int _ibo;
	int _vao;
	Buffer(int vbo, int ibo, int vao);
	Buffer() = default;
	bool isEmpty();
	void addBufferGeometry(Geometry* geometry);
	void updateBufferGeometry(Geometry* geometry);
	void removeBufferGeometry(Geometry* geometry);
	BufferRange getBufferInfo(Geometry* geometry);
	bool geometryInBuffer(Geometry* geometry);
	int getIndexCount();
};

class BufferManager {
private:
	int bufferIdCount;
	std::map<BufferId, Buffer> buffers;
	std::map<std::string, BufferId> aliases;

public:
	BufferManager();
	BufferId addBuffer(std::string name, int vbo, int ibo, int vao);
	BufferId addBuffer(int vbo, int ibo, int vao);
	Buffer& getBufferById(BufferId id);
	BufferId getBufferIdByName(const std::string& alias);
	void removeBuffer(BufferId id);
	bool bufferExists(const std::string& alias);
};