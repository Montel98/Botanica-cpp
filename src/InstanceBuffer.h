#pragma once

#include "BufferAttributes.h"
#include <map>
#include <string>
#include <glm/glm.hpp>

class InstanceBuffer {
public:
	BufferAttributes instanceAttributes; // Stores the buffer attribute data
	int ibo; // Handle to instance buffer

	InstanceBuffer();

	/**
	 * @brief      Inserts a new instance with all specified attributes and returns its index.
	 * Throws exception if any attributes defined in instanceAttributes are not in attribs.
	 * Extra attributes in attribs are ignored.
	 * @param[in]  attribs  Instance Attributes to add
	 *
	 * @return     Index of instance
	 */
	int addInstance(const std::map<std::string, BufferAttributeElement>& attribs);
	void removeInstance(int index);


	/**
	 * @brief      Adds a named generic instance buffer attribute to the buffer store.
	 */
	template<typename T>
	void addInstanceBufferAttribute(const std::string& name) {
		instanceAttributes.addBufferAttribute<T>(name);
	}

	/**
	 * @brief      Gets number of instances currently in the instance buffer.
	 */
	int getInstanceCount();
};