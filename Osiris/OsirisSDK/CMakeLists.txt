
set(SRC_DIR src)
set(HDR_DIR include/OsirisSDK)
set(SHADER_DIR shaders)

set (BASE_SOURCES
	${SRC_DIR}/OApplication.cpp
	${HDR_DIR}/OApplication.h
	${SRC_DIR}/OError.cpp
	${HDR_DIR}/OError.h
	${SRC_DIR}/OEvent.cpp
	${HDR_DIR}/OEvent.h
	${HDR_DIR}/OEventDefs.h
	${SRC_DIR}/OFile.cpp
	${HDR_DIR}/OFile.h
	${HDR_DIR}/ONonCopiable.h
	${SRC_DIR}/OObject.cpp
	${HDR_DIR}/OObject.h
	${HDR_DIR}/ORefCountObject.hpp
	${HDR_DIR}/OSingleton.hpp
	${SRC_DIR}/OStat.cpp
	${HDR_DIR}/OStat.h
	${HDR_DIR}/OString.hpp
    ${HDR_DIR}/OStringDefs.h
	${SRC_DIR}/OTimeIndex.cpp
	${HDR_DIR}/OTimeIndex.h
	${HDR_DIR}/defs.h
)
source_group("Base" FILES ${BASE_SOURCES})

set (CONTAINER_SOURCES
	${HDR_DIR}/OArray.hpp
	${HDR_DIR}/OList.hpp
	${HDR_DIR}/OMap.hpp
	${HDR_DIR}/OSet.hpp
	${HDR_DIR}/OStack.hpp
)
source_group("Container" FILES ${CONTAINER_SOURCES})

set (GRAPHICS_SOURCES
	${SRC_DIR}/OCamera.cpp
	${HDR_DIR}/OCamera.h
	${SRC_DIR}/OCameraController.cpp
	${HDR_DIR}/OCameraController.h
	${SRC_DIR}/OFont.cpp
	${HDR_DIR}/OFont.h
	${SRC_DIR}/OGlyph.cpp
	${HDR_DIR}/OGlyph.h
	${HDR_DIR}/OGPUObject.h
	${HDR_DIR}/OGraphicsAllocators.h
	${HDR_DIR}/OGraphicsDefinitions.h
	${SRC_DIR}/OIndexBuffer.cpp
	${HDR_DIR}/OIndexBuffer.h
	${SRC_DIR}/OMaterial.cpp
	${HDR_DIR}/OMaterial.h
	${SRC_DIR}/OMaterialSet.cpp
	${HDR_DIR}/OMaterialSet.h
	${SRC_DIR}/OMesh.cpp
	${HDR_DIR}/OMesh.h
	${HDR_DIR}/OMeshFile.h
	${HDR_DIR}/OMeshRawData.h
	${SRC_DIR}/OMeshBuilder.cpp
	${HDR_DIR}/OMeshBuilder.h
	${HDR_DIR}/OMeshGeometry.h
	${HDR_DIR}/OGeometry.h
	${HDR_DIR}/OObjMeshFile.h
	${SRC_DIR}/OObjMeshFile.cpp
	${SRC_DIR}/OObjMeshFileTokenizer.h
	${SRC_DIR}/OObjMeshFileTokenizer.cpp
	${SRC_DIR}/ORenderable.cpp
	${HDR_DIR}/ORenderable.h
	${SRC_DIR}/ORenderComponents.cpp
	${HDR_DIR}/ORenderComponents.h
	${SRC_DIR}/ORenderingEngine.cpp
	${HDR_DIR}/ORenderingEngine.h
	${HDR_DIR}/OResource.h
	${HDR_DIR}/OResourceFactory.h
	${SRC_DIR}/OResourceFactory.cpp
	${HDR_DIR}/OResourceManager.h
	${HDR_DIR}/OResourceManagerDefs.h
	${SRC_DIR}/OShaderArgument.cpp
	${HDR_DIR}/OShaderArgument.h
	${HDR_DIR}/OShaderArrayBuffer.hpp
	${SRC_DIR}/OShaderObject.cpp
	${HDR_DIR}/OShaderObject.h
	${SRC_DIR}/OShaderProgram.cpp
	${HDR_DIR}/OShaderProgram.h
	${HDR_DIR}/OShaderStorageBuffer.h
	${SRC_DIR}/OText2D.cpp
	${HDR_DIR}/OText2D.h
	${SRC_DIR}/OTexture.cpp
	${HDR_DIR}/OTexture.h
	${SRC_DIR}/OVertexBuffer.cpp
	${HDR_DIR}/OVertexBuffer.h
	${HDR_DIR}/OVisualObject.h
	${SRC_DIR}/OWavefrontMaterialFile.cpp
	${HDR_DIR}/OWavefrontMaterialFile.h
	${SRC_DIR}/OWavefrontParser.cpp
	${HDR_DIR}/OWavefrontParser.h
)
source_group("Graphics" FILES ${GRAPHICS_SOURCES})

set (GRAPHICS_SHADERS_SOURCES
	${SHADER_DIR}/OMesh.fragment.glsl
	${SHADER_DIR}/OMesh.vertex.glsl
	${SHADER_DIR}/OGlyph.fragment.glsl
	${SHADER_DIR}/OGlyph.vertex.glsl
	${SHADER_DIR}/OShaderID.h
	${SHADER_DIR}/shaders.h
	${SHADER_DIR}/shader_conversion.py

)
source_group("Graphics\\Shaders" FILES ${GRAPHICS_SHADERS_SOURCES})


set (GRAPHICS_ENGINE_SOURCES
	${HDR_DIR}/OGraphicsCommandBuffer.h
	${HDR_DIR}/OGraphicsCommandEncoder.h
	${HDR_DIR}/OGraphicsCommandQueue.h
	${HDR_DIR}/OGraphicsAPI.h
	${HDR_DIR}/OGraphicsRenderCommandEncoder.h
	${HDR_DIR}/OGraphicsResourceCommandEncoder.h
)
source_group("Graphics\\Engine" FILES ${GRAPHICS_ENGINE_SOURCES})

set (GRAPHICS_ENGINE_OPENGL_SOURCES
	${HDR_DIR}/GLdefs.h
	${SRC_DIR}/OOpenGL.cpp
	${HDR_DIR}/OOpenGL.h
	${SRC_DIR}/OOpenGLCommandBuffer.cpp
	${HDR_DIR}/OOpenGLCommandBuffer.h
	${HDR_DIR}/OOPenglCommandEncoder.h
	${SRC_DIR}/OOpenGLCommandQueue.cpp
	${HDR_DIR}/OOpenGLCommandQueue.h
	${SRC_DIR}/OOpenGLRenderCommandEncoder.cpp
	${HDR_DIR}/OOpenGLRenderCommandEncoder.h
	${SRC_DIR}/OOpenGLResourceCommandEncoder.cpp
	${HDR_DIR}/OOpenGLResourceCommandEncoder.h
	${HDR_DIR}/OOpenGLVertexBufferHandle.h
)
source_group("Graphics\\Engine\\OpenGL" FILES ${GRAPHICS_ENGINE_OPENGL_SOURCES})

set (MATH_SOURCES
	${SRC_DIR}/OMath.cpp
	${HDR_DIR}/OMath.h
	${HDR_DIR}/OMathPrimitive.hpp	
	${HDR_DIR}/OMatrix.hpp
	${HDR_DIR}/OMatrixDefs.h
	${SRC_DIR}/OMatrixStack.cpp
	${HDR_DIR}/OMatrixStack.h
	${HDR_DIR}/OQuaternion.hpp
	${HDR_DIR}/OQuaternionDefs.h
	${HDR_DIR}/OStats.hpp
	${HDR_DIR}/OVector.hpp
	${HDR_DIR}/OVectorDefs.h
)
source_group("Math" FILES ${MATH_SOURCES})

set (MEMORY_SOURCES
	${HDR_DIR}/OAbstractMemoryAllocator.hpp
	${HDR_DIR}/OMemoryManagedObject.h
	${SRC_DIR}/OMemoryManager.cpp
	${HDR_DIR}/OMemoryManager.h
	${HDR_DIR}/OMemoryManagerScope.h
	${SRC_DIR}/OMemoryPool.cpp
	${HDR_DIR}/OMemoryPool.h
	${HDR_DIR}/OMemoryPoolObject.hpp
	${SRC_DIR}/OSystemMemoryAllocator.cpp
	${HDR_DIR}/OSystemMemoryAllocator.h
	${SRC_DIR}/OTrashBin.cpp
	${HDR_DIR}/OTrashBin.h
	${SRC_DIR}/OTrashBinOwner.cpp
	${HDR_DIR}/OTrashBinOwner.h
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
	${SRC_DIR}/OState.cpp
	${HDR_DIR}/OState.h
)
source_group("Simulation" FILES ${SIM_SOURCES})

set (UTIL_SOURCES
	${SRC_DIR}/OChronometer.cpp
	${HDR_DIR}/OChronometer.h
	${HDR_DIR}/ODoubleBuffer.hpp
	${SRC_DIR}/OException.cpp
	${HDR_DIR}/OException.h
)
source_group("Utilities" FILES ${UTIL_SOURCES})

set (SOURCES 
	${BASE_SOURCES}
	${CONTAINER_SOURCES}
	${GRAPHICS_SOURCES}
	${GRAPHICS_SHADERS_SOURCES}
	${GRAPHICS_ENGINE_SOURCES}
	${GRAPHICS_ENGINE_OPENGL_SOURCES}
	${MATH_SOURCES} 
	${MEMORY_SOURCES}
	${SIM_SOURCES}
	${UTIL_SOURCES}
)

include_directories(./include
		    ${SHADER_DIR}
		    ${PROJECT_SOURCE_DIR}/dependencies/glload/include
		    ${PROJECT_SOURCE_DIR}/dependencies/FreeGLUT/freeglut/freeglut/include
		    ${PROJECT_SOURCE_DIR}/dependencies/freetype2/include
		    ${PROJECT_SOURCE_DIR}/dependencies/glm)

add_definitions(-DOSIRISSDK_EXPORTS -DOSIRIS_DLL_NAME="OsirisSDK.dll")

add_library(OsirisSDK SHARED ${SOURCES})
target_compile_features(OsirisSDK PRIVATE cxx_std_23)
#set_property(TARGET OsirisSDK PROPERTY CXX_STANDARD 23)

# debug/release
if(CMAKE_BUILD_TYPE EQUAL "DEBUG")
	target_link_libraries(OsirisSDK freeglut_staticd freetyped glload)
else()
	target_link_libraries(OsirisSDK freeglut_static freetype glload)
endif()

add_subdirectory(tests)
