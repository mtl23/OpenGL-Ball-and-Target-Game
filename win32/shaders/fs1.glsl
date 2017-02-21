#version 400
// Interpolated values from the vertex shaders
in vec3 fragmentColor;
// Ouput data
out vec3 color;
void main(){
		color = fragmentColor;
};