#pragma once

#include "Geometry.h"
#include "Material.h"
#include <memory>

class Mesh {
public:
	Material _material;
	std::unique_ptr<Geometry> _geometry;

	Mesh(const Material &material, std::unique_ptr<Geometry> geometry);
	Mesh(const Mesh&);
};