#pragma once

#include "BufferAttributes.h"
#include <map>
#include <string>
#include <glm/glm.hpp>

class InstanceBuffer {
public:
	BufferAttributes instanceAttributes;

	InstanceBuffer();
	/* Insert new instance with all specified attributes and returns its index
	   Throws exception if any attributes defined in instanceAttributes are not in attribs
	Extra attributes in attribs are ignored
	*/
	int addInstance(const std::map<std::string, BufferAttributeElement>& attribs);
	void removeInstance(int index);

	template<typename T>
	void addInstanceBufferAttribute(const std::string& name) {
		instanceAttributes.addBufferAttribute<T>(name);
	}

	// Get number of instances currently in the instance buffer
	int getInstanceCount();
};