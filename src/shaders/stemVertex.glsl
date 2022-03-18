#version 330 core

layout(location = 0) in vec3 aVertexPosition;

uniform mat4 world;
uniform mat4 camera;
uniform mat4 perspective;

void main() {
	gl_Position = perspective * camera * world * vec4(aVertexPosition, 1.0);
}