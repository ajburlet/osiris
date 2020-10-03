#pragma once

#include "GLdefs.h"
#include "defs.h"

#include "OsirisSDK/ORenderable.h"
#include "OsirisSDK/OVisualObject.h"
#include "OsirisSDK/OVectorDefs.h"
#include "OsirisSDK/OMeshBuffer.hpp"
#include "OsirisSDK/OCamera.h"

class OVertexIndex;
class OVertexBuffer;
class OMatrixStack;
class ORenderingEngine;

/**
 @brief Base class that represents a group of vertices that together make a geometrical shape.

 Meshes are first defined by entering vertex data and indices. The mesh object can then be initialized
 and then redered.
*/
class OAPI OMesh : public ORenderable, public OVisualObject
{
public:
	/**
	 @brief Class constructor.
	 @param program Pointer to the shader program that will be used to render the object.
	*/
	OMesh() = default;

	/**
	 @brief Class destructor.
	*/
	virtual ~OMesh() = default;

	/**
	 @brief Sets the pointer to the matrix stack. 
	 */
	void setMatrixStack(OMatrixStack* aMatrixStack);

	/**
	 @brief Returns the matrix stack.
	 */
	OMatrixStack* matrixStack() const;

	/**
	 @brief Starts the rendering process for the object.
	 @param aRenderingEngine The rendering engine.
	 @param aMatrixStack Pointer to the matrix stack that contains all the transformations.
	*/
	virtual void render(ORenderingEngine* aRenderingEngine, OMatrixStack *aMatrixStack = nullptr) override;

private:
	OMatrixStack* _matrixStack = nullptr;
};

inline void OMesh::setMatrixStack(OMatrixStack* aMatrixStack)
{
	_matrixStack = aMatrixStack;
}

inline OMatrixStack* OMesh::matrixStack() const
{
	return _matrixStack;
}

