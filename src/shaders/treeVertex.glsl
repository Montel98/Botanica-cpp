#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aMatureStart;

uniform mat4 world;
uniform mat4 camera;
uniform mat4 perspective;

uniform float treeGirth;

void main() {
	vec3 matureGirth = aMatureStart + treeGirth * (aVertexPosition - aMatureStart);

	gl_Position = perspective * camera * vec4(matureGirth, 1.0);
}