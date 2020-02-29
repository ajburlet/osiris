#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

smooth out vec4 smoothColor;

uniform mat4 transformMtx;

void main()
{
	gl_Position = transformMtx * position;
	smoothColor = color;
}

