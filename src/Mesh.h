#pragma once

#include "Geometry.h"
#include "Material.h"
#include "Shader.h"
#include <memory>
#include <string>
#include <optional>

class Mesh {
public:
	Material _material;
	std::unique_ptr<Geometry> _geometry;
	std::map<std::string, Shader> shaderPrograms;

	Mesh(const Material& material, std::unique_ptr<Geometry> geometry);
	Mesh(const Mesh&);

	bool usesProgram(const std::string programName);
};