#version 430

layout (location = 0) in vec4 aPosition;

uniform mat4 uMvpTransform;

void main()
{
	gl_Position = uMvpTransform * aPosition;
}

