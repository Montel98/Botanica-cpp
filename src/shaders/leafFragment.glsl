#version 330 core

in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vFragPosition;

out vec4 FragColour;

uniform sampler2D uLeafTexture;
uniform vec3 eye;

void main() {

	vec4 colour = texture(uLeafTexture, vTexCoord);

	if (colour.a < 0.1) {
		discard;
	}
	
	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(vec3(0.0, -1.0, 1.0));
	float ambient = 0.2;
	float diffuse = 0.6 * clamp(dot(norm, lightDir), 0.0, 1.0);

	vec3 reflected = lightDir - 2.0 * dot(norm, lightDir) * norm;
	vec3 viewDirection = normalize(vFragPosition - eye);

	float specular = 1.0 * pow(clamp(dot(reflected, viewDirection), 0.0, 1.0), 2.0);

	float light = ambient + diffuse + specular;

	FragColour = light * colour;
}