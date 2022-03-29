#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in mat4 aPose;

uniform mat4 world;
uniform mat4 camera;
uniform mat4 perspective;

void main() {

	gl_Position = perspective * camera * aPose * vec4(aVertexPosition, 1.0);
}