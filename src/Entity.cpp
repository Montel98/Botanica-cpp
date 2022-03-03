#include "Entity.h"

Entity::Entity(Object3D object3D) : worldObject(Object3D(std::move(object3D))) {};