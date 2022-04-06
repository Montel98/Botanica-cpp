#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aStart;
layout(location = 4) in float aAge;
layout(location = 5) in mat4 aPose;

out vec3 vNormal;
out vec2 vTexCoord;

uniform mat4 world;
uniform mat4 camera;
uniform mat4 perspective;

void main() {

	vec3 currentPosition = aStart + aAge * (aVertexPosition - aStart);
	gl_Position = perspective * camera * aPose * vec4(currentPosition, 1.0);
	vTexCoord = aTexCoord;
	vNormal = (aPose * vec4(aNormal, 1.0)).xyz;
}