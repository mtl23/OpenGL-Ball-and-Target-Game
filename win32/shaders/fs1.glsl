#version 400
// Interpolated values from the vertex shaders
in vec2 UV;


// Ouput data
out vec3 color;

uniform sampler2D myTextureSampler;

void main(){
	// Output color = color of the texture at the specified UV
	color = texture( myTextureSampler, UV ).rgb;
};