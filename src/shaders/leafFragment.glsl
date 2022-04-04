#version 330 core

in vec2 aTexCoord;
out vec4 FragColor;

uniform sampler2D uSampler;

void main() {

	FragColor = texture2D(uSampler, aTexCoord);
}