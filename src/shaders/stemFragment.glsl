#version 330 core

in vec3 vNormal;
out vec4 FragColor;

void main() {

	vec3 lightDir = normalize(vec3(0.0, -1.0, 1.0));
	float ambient = 0.3;
	float diffuse = 0.6 * clamp(dot(normalize(vNormal), lightDir), 0.0, 1.0);

	float light = ambient + diffuse;

	FragColor = vec4(light * vec3(0.0, 1.0, 1.0), 1.0);
}