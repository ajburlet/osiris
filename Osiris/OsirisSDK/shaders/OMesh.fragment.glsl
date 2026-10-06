#version 430

struct Material {
	vec3 color;
};

layout(std430, binding = 0) readonly buffer MaterialBuffer {
	Material materials[];
};

layout(std430, binding = 1) readonly buffer TriangleMaterialIndexBuffer {
	uint materialIndices[];
};

out vec4 oColor;

void main()
{
	oColor = vec4(materials[materialIndices[gl_PrimitiveID]].color, 1.0);
}

