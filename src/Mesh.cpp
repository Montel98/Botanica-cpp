#include "Mesh.h"

Mesh::Mesh(const Material &material, std::unique_ptr<Geometry> geometry) : _material(material), _geometry(std::move(geometry)) {};

Mesh::Mesh(const Mesh &other) {
	this->_geometry = std::unique_ptr<Geometry>(other._geometry->clone());
	this->_material = other._material;
}

bool Mesh::usesProgram(const std::string programName) {
	return shaderPrograms.find(programName) != shaderPrograms.end();
}