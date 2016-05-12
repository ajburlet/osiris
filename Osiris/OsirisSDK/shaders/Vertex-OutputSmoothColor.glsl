#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

smooth out vec4 smoothColor;

uniform mat4 perspectiveMtx;
uniform mat4 modelMtx;
uniform mat4 cameraMtx;

void main()
{
	gl_Position = perspectiveMtx * (cameraMtx * (modelMtx*position));
	smoothColor = color;
}

