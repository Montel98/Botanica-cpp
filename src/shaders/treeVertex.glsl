#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aMatureStart;

uniform mat4 world;
uniform mat4 camera;
uniform mat4 perspective;

void main() {
	vec3 matureGirth = aMatureStart + 1.0 * (aVertexPosition - aMatureStart);

	gl_Position = perspective * camera * vec4(matureGirth, 1.0);
}