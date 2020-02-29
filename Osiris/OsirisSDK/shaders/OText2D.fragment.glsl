#version 330 

varying vec2 texcoord;
out vec4 outColor;

uniform sampler2D tex;
uniform vec4 color;

void main(void) 
{
	outColor = vec4(1, 1, 1, texture2D(tex, texcoord).r) * color;
}