#include <cstring>
#include <cstdint>
#include <vector>

#include <OsirisSDK/OMaterial.h>
#include <OsirisSDK/OMeshBuilder.h>
#include <OsirisSDK/OMeshRawData.h>
#include <OsirisSDK/OResourceFactory.h>
#include <OsirisSDK/OShaderArgument.h>
#include <OsirisSDK/OVector.hpp>

#ifdef Bind
#undef Bind
#endif

#include "OsirisTests.h"

OTEST_START(MeshBuilder, CubeFacePerMaterial) {
	OResourceFactory factory;
	OMeshRawData rawData;
	rawData.setPositionComponents(3);
	rawData.setTextureComponents(2);

	const std::vector<OMeshRawData::Position> positions = {
		{ 1.0f, 1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, -1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, -1.0f, 1.0f }, { -1.0f, -1.0f, -1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f, 1.0f }, { -1.0f, -1.0f, 1.0f, 1.0f }
	};
	const std::vector<OMeshRawData::Normal> normals = {
		{ 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }
	};
	const std::vector<OMeshRawData::TexCoord> texCoords = {
		{ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }
	};

	for (const auto& position : positions) rawData.addPosition(position);
	for (const auto& normal : normals) rawData.addNormal(normal);
	for (const auto& texCoord : texCoords) rawData.addTexCoordinate(texCoord);

	for (uint32_t face = 0; face < 6; ++face) {
		const OString materialName = OString::Fmt("Face%u", face);
		OMaterial material{ OString(materialName) };
		material.setDiffuseColor(OVector3F(float(face + 1) / 6.0f, 0.25f, 0.5f));
		rawData.addMaterial(std::move(material));
	}

	const OMeshRawData::Index cubeFaces[6][4] = {
		{{ 0, 0, 0 }, { 4, 0, 1 }, { 6, 0, 2 }, { 2, 0, 3 }},
		{{ 3, 1, 0 }, { 2, 1, 1 }, { 6, 1, 2 }, { 7, 1, 3 }},
		{{ 7, 2, 0 }, { 6, 2, 1 }, { 4, 2, 2 }, { 5, 2, 3 }},
		{{ 5, 3, 0 }, { 1, 3, 1 }, { 3, 3, 2 }, { 7, 3, 3 }},
		{{ 1, 4, 0 }, { 0, 4, 1 }, { 2, 4, 2 }, { 3, 4, 3 }},
		{{ 5, 5, 0 }, { 4, 5, 1 }, { 0, 5, 2 }, { 1, 5, 3 }}
	};

	for (uint32_t face = 0; face < 6; ++face) {
		rawData.useMaterial(OString::Fmt("Face%u", face));
		OMeshRawData::Face polygon;
		for (const auto& corner : cubeFaces[face]) polygon.corners.push_back(corner);
		rawData.addFace(std::move(polygon));
	}

	OVertexBufferDescriptor descriptor;
	descriptor.addAttribute(OShaderVertexArgument(OVarType::Float3, 0));
	descriptor.addAttribute(OShaderVertexArgument(OVarType::Float3, 1));
	descriptor.addAttribute(OShaderVertexArgument(OVarType::Float2, 2));

	OMeshBuilder builder(factory.geometryManager(), factory.materialManager(),
		factory.materialSetManager(), descriptor);
	const auto result = builder.build("CubeMultiMaterial", "CubeMultiMaterial", rawData);

	ASSERT_FALSE(result.geometry.isNull());
	ASSERT_FALSE(result.materials.isNull());
	ASSERT_EQ(result.materials->count(), 6u);
	ASSERT_EQ(result.geometry->vertexBuffer().vertexCount(), 24u);
	ASSERT_EQ(result.geometry->indexBuffer().faceCount(), 12u);
	ASSERT_EQ(result.geometry->indexBuffer().indexCount(), 36u);
	ASSERT_EQ(result.geometry->materialIndexBuffer().size(), 12u);

	const auto& vertexBuffer = result.geometry->vertexBuffer();
	const auto* vertexData = static_cast<const uint8_t*>(vertexBuffer.buffer());
	const auto& indexBuffer = result.geometry->indexBuffer();
	const auto* indices = static_cast<const uint32_t*>(indexBuffer.buffer());
	const auto* materialIndices = result.geometry->materialIndexBuffer().data();
	for (uint32_t face = 0; face < 6; ++face) {
		const auto color = result.materials->at(face)->diffuseColor();
		ASSERT_EQ(color.x(), float(face + 1) / 6.0f);
		ASSERT_EQ(color.y(), 0.25f);
		ASSERT_EQ(color.z(), 0.5f);
		const uint32_t baseVertex = face * 4;
		const uint32_t expectedIndices[] = {
			baseVertex, baseVertex + 1, baseVertex + 2,
			baseVertex, baseVertex + 2, baseVertex + 3
		};
		for (uint32_t index = 0; index < 6; ++index) {
			ASSERT_EQ(indices[face * 6 + index], expectedIndices[index]);
		}
		ASSERT_EQ(materialIndices[face * 2], face);
		ASSERT_EQ(materialIndices[face * 2 + 1], face);

		for (uint32_t corner = 0; corner < 4; ++corner) {
			const uint32_t vertex = baseVertex + corner;
			const auto& source = cubeFaces[face][corner];
			float position[3]{};
			float normal[3]{};
			float texCoord[2]{};
			const uint8_t* data = vertexData + vertex * vertexBuffer.descriptor().stride();
			std::memcpy(position, data + vertexBuffer.descriptor().offset(0), sizeof(position));
			std::memcpy(normal, data + vertexBuffer.descriptor().offset(1), sizeof(normal));
			std::memcpy(texCoord, data + vertexBuffer.descriptor().offset(2), sizeof(texCoord));
			ASSERT_EQ(position[0], positions[source.vert].x);
			ASSERT_EQ(position[1], positions[source.vert].y);
			ASSERT_EQ(position[2], positions[source.vert].z);
			ASSERT_EQ(normal[0], normals[source.norm].x);
			ASSERT_EQ(normal[1], normals[source.norm].y);
			ASSERT_EQ(normal[2], normals[source.norm].z);
			ASSERT_EQ(texCoord[0], texCoords[source.tex].u);
			ASSERT_EQ(texCoord[1], texCoords[source.tex].v);
		}
	}
}
OTEST_END

OTEST_START(MeshBuilder, SharedCornersAcrossMaterialsWithoutUVs) {
	OResourceFactory factory;
	OMeshRawData rawData;
	rawData.setPositionComponents(3);

	const std::vector<OMeshRawData::Position> positions = {
		{ 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }
	};
	for (const auto& position : positions) rawData.addPosition(position);

	rawData.addMaterial(OMaterial(OString("Material0")));
	rawData.addMaterial(OMaterial(OString("Material1")));

	rawData.useMaterial(OString("Material0"));
	rawData.addFace({ { { 0, 0, 0 }, { 1, 0, 0 }, { 2, 0, 0 } } });
	rawData.useMaterial(OString("Material1"));
	rawData.addFace({ { { 0, 0, 0 }, { 2, 0, 0 }, { 3, 0, 0 } } });
	rawData.addFace({ { { 0, 0, 0 }, { 1, 0, 0 }, { 2, 0, 0 } } });

	OVertexBufferDescriptor descriptor;
	descriptor.addAttribute(OShaderVertexArgument(OVarType::Float3, 0));

	OMeshBuilder builder(factory.geometryManager(), factory.materialManager(),
		factory.materialSetManager(), descriptor);
	const auto result = builder.build("SharedMaterial", "SharedMaterial", rawData);
	ASSERT_EQ(result.geometry->vertexBuffer().descriptor().attributeCount(), 1u);
	ASSERT_EQ(result.geometry->vertexBuffer().vertexCount(), 4u);
	ASSERT_EQ(result.geometry->indexBuffer().faceCount(), 3u);
	ASSERT_EQ(result.geometry->indexBuffer().indexCount(), 9u);
	ASSERT_EQ(result.materials->count(), 2u);

	const uint32_t expectedIndices[] = { 0, 1, 2, 0, 2, 3, 0, 1, 2 };
	const auto* indices = static_cast<const uint32_t*>(result.geometry->indexBuffer().buffer());
	for (uint32_t index = 0; index < 9; ++index) ASSERT_EQ(indices[index], expectedIndices[index]);
	const auto* materialIndices = result.geometry->materialIndexBuffer().data();
	ASSERT_EQ(result.geometry->materialIndexBuffer().size(), 3u);
	ASSERT_EQ(materialIndices[0], 0u);
	ASSERT_EQ(materialIndices[1], 1u);
	ASSERT_EQ(materialIndices[2], 1u);
}
OTEST_END