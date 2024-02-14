#pragma once

#include "GLdefs.h"
#include "defs.h"

#include "OsirisSDK/ORenderable.h"
#include "OsirisSDK/OVisualObject.h"
#include "OsirisSDK/OVectorDefs.h"
#include "OsirisSDK/OCamera.h"

class OMatrixStack;
class ORenderingEngine;
template <class RefCountT> class ORefCountPtr;
class OMeshGeometry;
class OTexture;

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
	OMesh();

	/**
	 @brief Deleted copy constructor.
	 */
	OMesh(const OMesh& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OMesh(OMesh&& aOther);

	/**
	 @brief Class destructor.
	*/
	virtual ~OMesh();

	/**
	 @brief Deleted copy assignment operator.
	 */
	OMesh& operator=(const OMesh& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OMesh& operator=(OMesh&& aOther);

	/**
	 @brief Sets the pointer to the matrix stack. 
	 */
	void setMatrixStack(OMatrixStack* aMatrixStack);

	/**
	 @brief Returns the matrix stack.
	 */
	OMatrixStack* matrixStack() const;

	/**
	 @brief Sets the mesh geometry.
	 @param aGeometry The reference countable pointer to the mesh geometry.
	 @param aIndex The index of the couple material x index buffer to be used for this geometry.
	 */
	void setGeometry(ORefCountPtr<OMeshGeometry>& aGeometry, uint32_t aIndex=0);

	/**
	 @brief Sets the mesh texture.
	 */
	void setTexture(ORefCountPtr<OTexture>& aTexture);

	/**
	 @brief Starts the rendering process for the object.
	 @param aRenderingEngine The rendering engine.
	 @param aMatrixStack Pointer to the matrix stack that contains all the transformations.
	*/
	virtual void render(ORenderingEngine* aRenderingEngine, OMatrixStack *aMatrixStack = nullptr) override;

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */

};

inline OMesh::OMesh(OMesh && aOther) : ORenderable(std::move(aOther))
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}
