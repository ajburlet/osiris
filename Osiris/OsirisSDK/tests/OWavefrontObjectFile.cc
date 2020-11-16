#include <string.h>

#include <OsirisSDK/OWavefrontObjectFile.h>
#include <OsirisSDK/OArray.hpp>

#include "OsirisTests.h"

OTEST_START(OWavefrontObjectFile, CubeSquaredFaces) {
	OWavefrontObjectFile file(OSIRISTEST_BASEDIR "/resources/cube.obj");
	OWavefrontObjectFile::RawData rawData;
	
	file.loadMesh("Cube", rawData);
	ASSERT_EQ(file.objectArray().size(), 1);
	ASSERT_EQ(strcmp(file.objectArray()[0], "Cube"), 0);
	ASSERT_EQ(rawData.hasNormals(), true);
	ASSERT_EQ(rawData.hasTexCoords(), true);
	ASSERT_EQ(rawData.hasIndices(), true);
	ASSERT_EQ(rawData.vertexComponents(), 3);
	ASSERT_EQ(rawData.textureComponents(), 2);


}
OTEST_END