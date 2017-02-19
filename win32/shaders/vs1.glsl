#version 400
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 color;
smooth out vec4 theColor;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main()
{
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	theColor = color;
}
