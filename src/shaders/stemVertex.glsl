in vec3 aVertexPosition;
//out vec3 aFragPosition;

void main() {
	gl_Position = vec4(aVertexPosition, 1.0);
	//aFragPosition = aVertexPosition;
}