#version 330 

layout (location = 0) in vec4 aPosition;
varying mediump vec2 oTexcoord;
uniform vec2 uPosOffset;
uniform vec2 uScale;

void main(void) 
{
	gl_Position = vec4(aPosition.xy * uScale.xy + uPosOffset.xy, 0.0f, 1.0f);
	oTexcoord = aPosition.zw;
}