
set(SRC_DIR src)
set(HDR_DIR include/OsirisSDK)
set(SHADER_DIR shaders)

set (BASE_SOURCES
	${HDR_DIR}/GLdefs.h
	${SRC_DIR}/OApplication.cpp
	${HDR_DIR}/OApplication.h
	${SRC_DIR}/OEvent.cpp
	${HDR_DIR}/OEvent.h
	${SRC_DIR}/OObject.cpp
	${HDR_DIR}/OObject.h
	${HDR_DIR}/OSingleton.hpp
	${SRC_DIR}/OTimeIndex.cpp
	${HDR_DIR}/OTimeIndex.h
	${HDR_DIR}/defs.h
)
source_group("Base" FILES ${BASE_SOURCES})

set (CONTAINER_SOURCES
	${HDR_DIR}/OArray.hpp
	${HDR_DIR}/OList.hpp
	${HDR_DIR}/OMap.hpp
)
source_group("Container" FILES ${CONTAINER_SOURCES})

set (GRAPHICS_SOURCES
	${SRC_DIR}/OCamera.cpp
	${HDR_DIR}/OCamera.h
	${SRC_DIR}/OCameraController.cpp
	${HDR_DIR}/OCameraController.h
	${SRC_DIR}/OFont.cpp
	${HDR_DIR}/OFont.h
	${SRC_DIR}/OMesh.cpp
	${HDR_DIR}/OMesh.h
	${HDR_DIR}/OMeshBuffer.hpp
	${SRC_DIR}/ORenderObject.cpp
	${HDR_DIR}/ORenderObject.h
	${SRC_DIR}/OShaderObject.cpp
	${HDR_DIR}/OShaderObject.h
	${SRC_DIR}/OShaderProgram.cpp
	${HDR_DIR}/OShaderProgram.h
	${SRC_DIR}/OText2D.cpp
	${HDR_DIR}/OText2D.h
	${SRC_DIR}/OVertexColorMesh.cpp
	${HDR_DIR}/OVertexColorMesh.h
	${SRC_DIR}/OWavefrontObjectFile.cpp
	${HDR_DIR}/OWavefrontObjectFile.h
)
source_group("Graphics" FILES ${GRAPHICS_SOURCES})

set (GRAPHICS_SHADERS_SOURCES
	${SHADER_DIR}/Fragment-OText2D.glsl
	${SHADER_DIR}/Vertex-OText2D.glsl
	${SHADER_DIR}/Fragment-StandardColor.glsl
	${SHADER_DIR}/Vertex-OutputSmoothColor.glsl
)
source_group("Graphics\\Shaders" FILES ${GRAPHICS_SHADERS_SOURCES})


set (MATH_SOURCES
	${SRC_DIR}/OMath.cpp
	${HDR_DIR}/OMath.h
	${HDR_DIR}/OMathPrimitive.hpp	
	${SRC_DIR}/OMatrixStack.cpp
	${HDR_DIR}/OMatrixStack.h
	${HDR_DIR}/OStats.hpp
)
source_group("Math" FILES ${MATH_SOURCES})

set (MEMORY_SOURCES
	${HDR_DIR}/OAbstractMemoryAllocator.hpp
	${SRC_DIR}/OMemoryManager.cpp
	${HDR_DIR}/OMemoryManager.h
	${SRC_DIR}/OMemoryPool.cpp
	${HDR_DIR}/OMemoryPool.h
	${HDR_DIR}/OMemoryPoolObject.hpp
	${SRC_DIR}/OSystemMemoryAllocator.cpp
	${HDR_DIR}/OSystemMemoryAllocator.h
)
source_group("Memory" FILES ${MEMORY_SOURCES})

set (SIM_SOURCES
	${SRC_DIR}/OBehavior.cpp
	${HDR_DIR}/OBehavior.h
	${SRC_DIR}/OEntity.cpp
	${HDR_DIR}/OEntity.h
	${SRC_DIR}/OParameterList.cpp
	${HDR_DIR}/OParameterList.h
	${SRC_DIR}/OSimulation.cpp
	${HDR_DIR}/OSimulation.h
	${SRC_DIR}/OSpatialGrid.cpp
	${HDR_DIR}/OSpatialGrid.h
	${SRC_DIR}/OState.cpp
	${HDR_DIR}/OState.h
)
source_group("Simulation" FILES ${SIM_SOURCES})

set (UTIL_SOURCES
	${SRC_DIR}/OChronometer.cpp
	${HDR_DIR}/OChronometer.h
	${HDR_DIR}/OCollection.hpp
	${HDR_DIR}/ODoubleBuffer.hpp
	${SRC_DIR}/OException.cpp
	${HDR_DIR}/OException.h
)
source_group("Utilities" FILES ${UTIL_SOURCES})

set (RESOURCES
	include/resource.h
	OsirisSDK.rc
)
source_group("Resources" FILES ${RESOURCES})

set (SOURCES 
	${BASE_SOURCES}
	${CONTAINER_SOURCES}
	${GRAPHICS_SOURCES}
	${GRAPHICS_SHADERS_SOURCES}
	${MATH_SOURCES} 
	${MEMORY_SOURCES}
	${SIM_SOURCES}
	${UTIL_SOURCES}
	${RESOURCES}
)

include_directories(./include 
		    ${PROJECT_SOURCE_DIR}/dependencies/glload/include
		    ${PROJECT_SOURCE_DIR}/dependencies/FreeGLUT/freeglut/freeglut/include
		    ${PROJECT_SOURCE_DIR}/dependencies/freetype2/include
		    ${PROJECT_SOURCE_DIR}/dependencies/glm)

add_definitions(-DOSIRISSDK_EXPORTS -DOSIRIS_DLL_NAME="OsirisSDK.dll")

add_library(OsirisSDK SHARED ${SOURCES})

# freetype
if(CMAKE_BUILD_TYPE EQUAL "DEBUG")
	target_link_libraries(OsirisSDK freeglut_staticd freetyped glload)
else()
	target_link_libraries(OsirisSDK freeglut_static freetype glload)
endif()
