#pragma once

#include "Object3D.h"
#include "Mesh.h"
#include "WorldTime.h"
#include <variant>

class Entity {
public:
	//std::vector<Behaviour> behaviours;
	Object3D worldObject;
	Entity(Object3D object3D);
	virtual void act(const WorldTime& worldTime) = 0;
};