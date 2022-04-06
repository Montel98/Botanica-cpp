#version 330 core

in vec3 vNormal;
in vec2 vTexCoord;

out vec4 FragColour;

uniform sampler2D uStemTexture;

void main() {

	vec3 lightDir = normalize(vec3(0.0, -1.0, 1.0));
	float ambient = 0.3;
	float diffuse = 0.6 * clamp(dot(normalize(vNormal), lightDir), 0.0, 1.0);

	float light = ambient + diffuse;

	vec4 colour = texture(uStemTexture, vTexCoord);

	FragColour = light * colour;
}