#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aMatureStart;

uniform mat4 world;
uniform mat4 camera;
uniform mat4 perspective;

void main() {
	//gl_Position = perspective * camera * vec4(aVertexPosition + (0.001 * aMatureStart), 1.0);
	gl_Position = perspective * camera * vec4(aMatureStart + 0.2 * (aVertexPosition - aMatureStart), 1.0);
}