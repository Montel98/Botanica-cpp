#pragma once

#include "Object3D.h"
#include "Mesh.h"
#include "WorldTime.h"

class Entity {
public:
	Object3D worldObject;
	Entity(Object3D object3D);
	virtual void act(const WorldTime& worldTime) = 0;
};