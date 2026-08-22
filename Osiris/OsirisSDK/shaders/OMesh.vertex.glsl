#version 330

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec4 aColor;

smooth out vec4 smoothColor;

uniform mat4 uMvpTransform;

void main()
{
	gl_Position = uMvpTransform * aPosition;
	smoothColor = aColor;
}

