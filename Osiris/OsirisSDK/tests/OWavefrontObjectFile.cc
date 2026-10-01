#include <string.h>

#include <OsirisSDK/OWavefrontObjectFile.h>
#include <OsirisSDK/OObjMeshFile.h>
#include <OsirisSDK/OArray.hpp>
#include <OsirisSDK/OString.hpp>
#include <OsirisSDK/OIndexBuffer.h>
#include <OsirisSDK/OMeshBuilder.h>
#include <OsirisSDK/OResourceFactory.h>

#include <vector>

#include "OsirisTests.h"

OTEST_START(OWavefrontObjectFile, CubeSquaredFaces) {
	OWavefrontObjectFile file(OSIRISTEST_BASEDIR "/resources/cube.obj");
	OWavefrontObjectFile::RawData rawData;
	
	file.loadMesh("Cube", rawData);
	ASSERT_EQ(rawData.hasNormals(), true);
	ASSERT_EQ(rawData.hasTexCoords(), true);
	ASSERT_EQ(rawData.hasIndices(), true);
	ASSERT_EQ(rawData.positionComponents(), 3);
	ASSERT_EQ(rawData.textureComponents(), 2);
}
OTEST_END

OTEST_START(OObjMeshFile, CubeSquaredFacesAndMaterial) {
	OObjMeshFile file(OSIRISTEST_BASEDIR "/resources/cube.obj");
	OObjMeshFile::RawData rawData;

	file.loadMesh("Cube", rawData);
	ASSERT_TRUE(rawData.hasNormals());
	ASSERT_TRUE(rawData.hasTexCoords());
	ASSERT_TRUE(rawData.hasIndices());
	ASSERT_STREQ(rawData.currentMaterial().cString(), "Material");
	ASSERT_EQ(rawData.faceCount(), 6);
}
OTEST_END

OTEST_START(OMeshBuilder, CubeFacePerMaterial) {
	OResourceFactory factory;
	OMeshRawData rawData;
	rawData.setPositionComponents(3);
	rawData.setTextureComponents(2);

	using Position = OMeshRawData::Position;
	using Normal = OMeshRawData::Normal;
	using TexCoord = OMeshRawData::TexCoord;

	const std::vector<Position> positions = {
		{ 1.0f, 1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, -1.0f, 1.0f }, { -1.0f, -1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, 1.0f, 1.0f }, { -1.0f, -1.0f, 1.0f, 1.0f }
	};
	const std::vector<Normal> normals = {
		{ 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }
	};
	const std::vector<TexCoord> texCoords = {
		{ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }
	};

	for (const auto& position : positions) rawData.addPosition(position);
	for (const auto& normal : normals) rawData.addNormal(normal);
	for (const auto& tex : texCoords) rawData.addTexCoordinate(tex);

	for (uint32_t i = 0; i < 6; ++i) {
		OMaterial material(OString::Fmt("Face%u", i));
		material.setDiffuseColor(OVector3F(float(i + 1) / 6.0f, 0.25f * (i % 3), 0.5f * (i % 2)));
		rawData.addMaterial(material);
	}

	const auto faceIndices = {
		OMeshRawData::Index{0, 0, 0}, OMeshRawData::Index{4, 0, 1}, OMeshRawData::Index{6, 0, 2}, OMeshRawData::Index{2, 0, 3},
		OMeshRawData::Index{3, 1, 0}, OMeshRawData::Index{2, 1, 1}, OMeshRawData::Index{6, 1, 2}, OMeshRawData::Index{7, 1, 3},
		OMeshRawData::Index{7, 2, 0}, OMeshRawData::Index{6, 2, 1}, OMeshRawData::Index{5, 2, 2}, OMeshRawData::Index{8, 2, 3},
		OMeshRawData::Index{6, 3, 0}, OMeshRawData::Index{2, 3, 1}, OMeshRawData::Index{4, 3, 2}, OMeshRawData::Index{8, 3, 3},
		OMeshRawData::Index{1, 4, 0}, OMeshRawData::Index{0, 4, 1}, OMeshRawData::Index{2, 4, 2}, OMeshRawData::Index{3, 4, 3},
		OMeshRawData::Index{5, 5, 0}, OMeshRawData::Index{1, 5, 1}, OMeshRawData::Index{0, 5, 2}, OMeshRawData::Index{4, 5, 3}
	};

	for (uint32_t face = 0; face < 6; ++face) {
		OMeshRawData::Face faceData;
		faceData.materialIndex = face;
		for (uint32_t corner = 0; corner < 4; ++corner) {
			faceData.corners.push_back(OMeshRawData::Index{static_cast<uint32_t>((face * 4 + corner) % 8), 0u, 0u});
		}
		rawData.addFace(faceData);
	}

	OVertexBufferDescriptor descriptor;
	descriptor.addAttribute(OShaderVertexArgument(OVarType::Float3, 0));
	descriptor.addAttribute(OShaderVertexArgument(OVarType::Float3, 1));
	descriptor.addAttribute(OShaderVertexArgument(OVarType::Float2, 2));
	descriptor.addAttribute(OShaderVertexArgument(OVarType::UnsignedInt, 3));

	OMeshBuilder builder(factory.geometryManager(), factory.materialManager(), factory.materialSetManager(), descriptor);
	const auto result = builder.build("CubeMultiMaterial", "CubeMultiMaterial", rawData);

	ASSERT_FALSE(result.geometry.isNull());
	ASSERT_FALSE(result.materials.isNull());
	ASSERT_EQ(result.materials->size(), 6u);
	ASSERT_EQ(result.geometry->vertexBuffer().vertexCount(), 24u);
	ASSERT_EQ(result.geometry->indexBuffer().faceCount(), 12u);
}
OTEST_END