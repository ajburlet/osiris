#include <cstring>
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
	descriptor.addAttribute(OShaderVertexArgument(OVarType::UnsignedInt, 3));

	OMeshBuilder builder(factory.geometryManager(), factory.materialManager(),
		factory.materialSetManager(), descriptor);
	const auto result = builder.build("CubeMultiMaterial", "CubeMultiMaterial", rawData);

	ASSERT_FALSE(result.geometry.isNull());
	ASSERT_FALSE(result.materials.isNull());
	ASSERT_EQ(result.materials->size(), 6u);
	ASSERT_EQ(result.geometry->vertexBuffer().vertexCount(), 24u);
	ASSERT_EQ(result.geometry->indexBuffer().faceCount(), 12u);
}
OTEST_END

OTEST_START(MeshBuilder, ProvokingVertexMaterial) {
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
	descriptor.addAttribute(OShaderVertexArgument(OVarType::UnsignedInt, 1));

	OMeshBuilder builder(factory.geometryManager(), factory.materialManager(),
		factory.materialSetManager(), descriptor);
	const auto duplicated = builder.build("DuplicatedMaterial", "DuplicatedMaterial", rawData);

	OMeshBuilder::Options options;
	options.vertexMaterialMode = OMeshBuilder::Options::VertexMaterialMode::ProvokingVertex;
	const auto provoking = builder.build("ProvokingMaterial", "ProvokingMaterial", rawData, options);

	ASSERT_EQ(duplicated.geometry->vertexBuffer().vertexCount(), 9u);
	ASSERT_EQ(duplicated.geometry->indexBuffer().faceCount(), 3u);
	ASSERT_EQ(provoking.materials->size(), 2u);
	ASSERT_EQ(provoking.geometry->vertexBuffer().vertexCount(), 5u);
	ASSERT_EQ(provoking.geometry->indexBuffer().faceCount(), 3u);

	auto& vertexBuffer = provoking.geometry->vertexBuffer();
	const auto& indexBuffer = provoking.geometry->indexBuffer();
	const auto* indices = static_cast<const uint32_t*>(indexBuffer.buffer());
	const auto* vertexData = static_cast<const uint8_t*>(vertexBuffer.buffer());
	const auto materialAt = [&](uint32_t aVertexIndex) {
		uint32_t materialIndex = 0;
		std::memcpy(&materialIndex,
			vertexData + aVertexIndex * vertexBuffer.descriptor().stride() + vertexBuffer.descriptor().offset(1),
			sizeof(materialIndex));
		return materialIndex;
	};

	ASSERT_EQ(materialAt(indices[2]), 0u);
	ASSERT_EQ(materialAt(indices[5]), 1u);
	ASSERT_EQ(materialAt(indices[8]), 1u);
}
OTEST_END