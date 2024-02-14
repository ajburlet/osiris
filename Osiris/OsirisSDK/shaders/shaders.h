// ********************************************************************************
// This file was generated by shader_conversion.py.
// ********************************************************************************

#include "OShaderID.h"

constexpr char glsl_OGlyph_fragment[] = R"(
#version 330 

varying mediump vec2 voTexcoord;
out lowp vec4 foOutColor;

uniform sampler2D uTexture;
uniform vec4 uColor;

void main(void) 
{
	foOutColor = vec4(1, 1, 1, texture2D(uTexture, voTexcoord).r) * uColor;
}
)";

constexpr char glsl_OGlyph_vertex[] = R"(
#version 330 

layout (location = 0) in vec4 aPosition;
varying mediump vec2 voTexcoord;
uniform vec2 uPosOffset;
uniform vec2 uScale;

void main(void) 
{
	gl_Position = vec4(aPosition.xy * uScale.xy + uPosOffset.xy, 0.0f, 1.0f);
	voTexcoord = aPosition.zw;
}
)";

constexpr char glsl_OMesh_fragment[] = R"(
#version 330

smooth in vec4 smoothColor;

out vec4 outputColor;

void main()
{
	outputColor = vec4(0.0f, 0.0f, 1.0f, 0.5f);
	//outputColor = smoothColor;
}


)";

constexpr char glsl_OMesh_vertex[] = R"(
#version 330

layout (location = 0) in vec4 position;
//layout (location = 1) in vec4 color;

smooth out vec4 smoothColor;

uniform mat4 uMvpTransform;

void main()
{
	gl_Position = uMvpTransform * position;
	//smoothColor = color;
}


)";

void createShaderTable(OShaderSourceTable& aShaderTable)
{
	aShaderTable[{OGraphicsAPI::Type::OpenGL,OShaderObject::Type::Fragment,ORenderable::Type::Glyph}] = glsl_OGlyph_fragment;
	aShaderTable[{OGraphicsAPI::Type::OpenGL,OShaderObject::Type::Vertex,ORenderable::Type::Glyph}] = glsl_OGlyph_vertex;
	aShaderTable[{OGraphicsAPI::Type::OpenGL,OShaderObject::Type::Fragment,ORenderable::Type::Mesh}] = glsl_OMesh_fragment;
	aShaderTable[{OGraphicsAPI::Type::OpenGL,OShaderObject::Type::Vertex,ORenderable::Type::Mesh}] = glsl_OMesh_vertex;
}

