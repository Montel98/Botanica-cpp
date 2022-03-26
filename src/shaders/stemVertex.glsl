#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aMatureStart;
layout(location = 2) in vec3 aImmatureStart;
layout(location = 3) in vec3 aImmatureEnd;

uniform mat4 world;
uniform mat4 camera;
uniform mat4 perspective;
uniform float stemLength;
uniform float stemGirth;

void main() {
	vec3 immatureGirth = aImmatureStart + stemGirth * (aImmatureEnd - aImmatureStart);
	vec3 matureGirth = aMatureStart + stemGirth * (aVertexPosition - aMatureStart);

	gl_Position = perspective * camera * vec4(immatureGirth + stemLength * (matureGirth - immatureGirth), 1.0);
}