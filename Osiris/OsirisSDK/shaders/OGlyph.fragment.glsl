#version 330 

varying mediump vec2 aTexcoord;
out vec4 oOutColor;

uniform sampler2D uTexture;
uniform lowp vec4 uColor;

void main(void) 
{
	oOutColor = vec4(1, 1, 1, texture2D(uTexture, aTexcoord).r) * uColor;
}