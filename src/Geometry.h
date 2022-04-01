#pragma once

#include <vector>
#include <string>
#include <memory>

#include "BufferAttributes.h"
#include "InstanceBuffer.h"

struct GeometryEvent {
	int vertexStart, vertexLength;
	int indexStart, indexLength;
};

class Geometry {
private:
	bool useST, useNormal;
	bool invertedNormals;
public:
	Geometry();

	/**
	 * @brief      Adds a generic morph target as a buffer attribute.
	 */
	void addMorphTarget(const std::string &targetName, std::vector<glm::vec3> morphVertices);
	void addMorphTarget(
		const std::string &targetName, 
		std::vector<glm::vec3> morphVertices, 
		std::vector<glm::vec3> morphNormals
	);

	/**
	 * @brief      Adds new geometry to the current geometry.
	 * Index and vertex buffers are merged with the current geometry.
	 * Only attributes matching with those defined in current geometry are merged.
	 */
	void addGeometry(Geometry& other);

	/**
	 * @brief      Generates a new subset of the geometry from the specified start attribute index.
	 * Start + length spanning beyond the attribute range will slice the geometry from [start, end].
	 */
	Geometry sliceGeometry(unsigned int start, unsigned int length);

	bool usesNormals() const;
	bool normalsInverted() const;
	bool usesSTs() const;

	/**
	 * @brief      Add normal vec3 attribute and specify whether directions should be inverted
	 */
	void useNormals(bool inverted);
	void useSTs();
	void useInstancing();

	// Buffer Attributes
	BufferAttributes vertexBuffer;
	BufferAttributes indexBuffer;
	std::optional<InstanceBuffer> instanceBuffer;

	// Alias for internal buffer managed by renderer
	// Useful for geometries sharing a buffer
	std::string bufferName;

	int bufferId; // Handle to buffer object managed by renderer

	std::vector<GeometryEvent> modificationEvents; // Messages read by renderer when geometry updates


	/**
	 * @brief      Signal to renderer that geometry needs to be updated in the internal buffers.
	 *
	 * @param[in]  vertexStart   Start attribute index of vertex buffer
	 * @param[in]  vertexLength  Number of attributes to update in vertex buffer
	 * @param[in]  indexStart    Start attribute index of index buffer
	 * @param[in]  indexLength   Number of attributes to update in index buffer
	 */
	void addGeometryEvent(int vertexStart, int vertexLength, int indexStart, int indexLength);

	virtual Geometry* clone() const;
	virtual ~Geometry();
};

/**
 * @brief      Merges a collection of geometries to a new geometry object.
 * Merges any type of geometry provided they share the same buffer attributes.
 * A generic geometry container is returned, any geometry specific info is lost.
 */
Geometry mergeGeometry(std::vector<std::reference_wrapper<Geometry>>& geometries);