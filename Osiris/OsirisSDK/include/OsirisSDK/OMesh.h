#pragma once

#include <memory>

#include "GLdefs.h"
#include "defs.h"

#include "OsirisSDK/ORenderable.h"
#include "OsirisSDK/OVisualObject.h"
#include "OsirisSDK/OVectorDefs.h"
#include "OsirisSDK/OMatrixDefs.h"
#include "OsirisSDK/OCamera.h"

class ORenderingEngine;
class ORenderComponents;

/**
 @brief Base class that represents a group of vertices that together make a geometrical shape.

 Meshes are first defined by entering vertex data and indices. The mesh object can then be initialized
 and then redered.
*/
class OAPI OMesh : public ORenderable, public OVisualObject
{
private:
	using Super = ORenderable;

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
	 @brief Returns the mvp matrix. 
	 */
	const OMatrix4x4F& mvp() const;

	/**
	 * @brief Returns the components used to render this mesh.
	 */
	ORenderComponents& renderComponents();
	const ORenderComponents& renderComponents() const;


	/**
	 @brief Starts the rendering process for the object.
	 @param aRenderingEngine The rendering engine.
	 @param aMatrixStack Pointer to the matrix stack that contains all the transformations.
	*/
	virtual void render(ORenderingEngine& aRenderingEngine, OMatrixStack *aMatrixStack = nullptr) override;

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	std::unique_ptr<Impl> _impl;
	/**
	 @endcond
	 */

};
