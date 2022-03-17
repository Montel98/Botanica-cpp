#include "Mesh.h"

Mesh::Mesh(const Material &material, std::unique_ptr<Geometry> geometry) : _material(material), _geometry(std::move(geometry)) {};

Mesh::Mesh(const Mesh &other) : isInstanced(other.isInstanced), _material(other._material), shaderPrograms(other.shaderPrograms) {
	this->_geometry = std::unique_ptr<Geometry>(other._geometry->clone());
}

bool Mesh::usesProgram(const std::string programName) {
	return shaderPrograms.find(programName) != shaderPrograms.end();
}