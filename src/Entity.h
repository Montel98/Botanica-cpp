#pragma once

#include <memory>
#include <vector>

#pragma once

class Entity {
private:
	std::unique_ptr<Entity> parent;
	std::vector<Entity> children;
public:
	Entity();
};