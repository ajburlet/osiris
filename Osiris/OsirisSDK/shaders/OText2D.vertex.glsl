#version 330 

layout (location = 0) in vec4 position;
varying vec2 texcoord;
uniform vec2 posOffset;
uniform vec2 scale;

void main(void) 
{
	gl_Position = vec4(position.xy * scale.xy + posOffset.xy, 0.0f, 1.0f);
	texcoord = position.zw;
}