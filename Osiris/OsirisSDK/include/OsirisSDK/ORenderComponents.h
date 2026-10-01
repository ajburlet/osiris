#pragma once

#include <stdint.h>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"
#include "OsirisSDK/OGPUObject.h"
#include "OsirisSDK/ORefCountObject.hpp"
#include "OsirisSDK/OGeometry.h"
#include "OsirisSDK/OTexture.h"
#include "OsirisSDK/OMaterial.h"
#include "OsirisSDK/OMaterialSet.h"

class OVertexBuffer;
class OIndexBuffer;
class OShaderProgram;
class OShaderUniformArgument;

/**
 @brief Contains all data pertaining to GPU rendering of a renderable object.
 */
class OAPI ORenderComponents : public OGPUObject
{
public:
	/**
	 @brief Default class constructor.
	 */
	ORenderComponents();

	/**
	 @brief Class destructor.
	 */
	virtual ~ORenderComponents();

	/**
	 @brief Returns the render mode.
	 */
	ORenderMode renderMode() const;

	/**
	 * @brief Returns the associated geometry.
	 */
	OGeometry* geometry();

	/**
	 @brief Returns the texture.
	 */
	OTexture* texture();

	/**
	 * @brief Returns the material set.
	 */
	OMaterialSet* materialSet();

	/**
	 @brief Returns the shader program.
	 */
	OShaderProgram* shaderProgram();

	/**
	 @brief Returns true if color blending is enabled.
	 */
	bool colorBlendingEnabled() const;

	/**
	 @brief Returns the color blending source factor.
	 */
	OBlendFactor colorBlendingSourceFactor() const;

	/**
	 @brief Returns the color blenduing destination factor.
	 */
	OBlendFactor colorBlendingDestinationFactor() const;

	/**
	 @brief Returns true if face culling is enabled.
	 */
	bool faceCullingEnabled() const;

	/**
	 @brief Returns the face that must be drawn.
	 */
	OCullFace cullingFace() const;

	/**
	 @brief Returns the definition of the front face by polygon vertex winding order.
	 */
	OCullFront cullingFrontFace() const;

	/**
	 @brief Returns true if the depth testing is enabled.
	 */
	bool depthTestingEnabled() const;

	/**
	 @brief Returns true if depth buffer writing is enabled.
	 */
	bool depthBufferWriteEnabled() const;

	/**
	 @brief Returns true if all related components are already loaded to the GPU. 
	 */
	bool componentsLoaded() const;

	/**
	 @brief Sets the render mode.
	 */
	void setRenderMode(ORenderMode aRenderMode);

	/**
	 * @brief Sets the geometry.
	 */
	void setGeometry(OGeometry& aGeometry);

	/**
	 * @brief Sets the material set.
	 */
	void setMaterialSet(OMaterialSet& aMaterialSet);

	/**
	 @brief Sets the texture.
	 @param aTexture The texture to be set.
	 */
	void setTexture(OTexture& aTexture);

	/**
	 @brief Sets the shader program.
	 @param aShaderProgram The shader program to be set.
	 */
	void setShaderProgram(OShaderProgram* aShaderProgram);

	/**
	 @brief Sets color blending preferences.
	 @param aEnabled Enables/disables color blending.
	 @param aSourceFactor Source blend factor.
	 @param aDestinationFactor Destination blend factor.
	 */
	void setColorBlending(bool aEnabled, OBlendFactor aSourceFactor = OBlendFactor::One,
			      OBlendFactor aDestinationFactor = OBlendFactor::Zero)
	{
		_blendingEnabled = aEnabled;
		_blendSourceFactor = aSourceFactor;
		_blendDestinationFactor = aDestinationFactor;
	}

	/**
	 @brief Sets face culling preferences.
	 @param aEnabled Enables/disables face culling.
	 @param aFace Defines which face is drawn.
	 @param aFrontFace Defines which is the front face by polygon vertex winding order.
	 */
	void setFaceCulling(bool aEnabled, OCullFace aFace = OCullFace::Undefined, 
			    OCullFront aFrontFace = OCullFront::Undefined);

	/**
	 @brief Sets depth testing settings.
	 @param aTestEnabled Depth test enabled/disabled.
	 @param aBufferWrite Depth buffer write.
	 */
	void setDepthTesting(bool aTestEnabled, bool aBufferWrite);

private:
	ORenderMode					_renderMode				= ORenderMode::Undefined;
	ORefCountPtr<OGeometry>		_geometry;
	ORefCountPtr<OMaterialSet>	_materialSet;
	ORefCountPtr<OTexture>		_texture				= nullptr;
	OShaderProgram*				_shaderProgram			= nullptr;
	bool						_blendingEnabled		= false;
	OBlendFactor				_blendSourceFactor		= OBlendFactor::One;
	OBlendFactor				_blendDestinationFactor = OBlendFactor::Zero;
	bool						_faceCullingEnabled		= false;
	OCullFace					_cullFace				= OCullFace::Undefined;
	OCullFront					_cullFront				= OCullFront::Undefined;
	bool						_depthTestEnabled		= false;
	bool						_depthBufferWrite		= false;
};

inline OGeometry* ORenderComponents::geometry()
{
	return _geometry.getPtr();
}

inline OTexture * ORenderComponents::texture()
{
	return _texture.getPtr();
}

inline OMaterialSet* ORenderComponents::materialSet()
{
	return _materialSet.getPtr();
}

inline OShaderProgram * ORenderComponents::shaderProgram()
{
	return _shaderProgram;
}

inline bool ORenderComponents::colorBlendingEnabled() const
{
	return _blendingEnabled;
}

inline OBlendFactor ORenderComponents::colorBlendingSourceFactor() const
{
	return _blendSourceFactor;
}

inline OBlendFactor ORenderComponents::colorBlendingDestinationFactor() const
{
	return _blendDestinationFactor;
}

inline bool ORenderComponents::faceCullingEnabled() const
{
	return _faceCullingEnabled;
}

inline OCullFace ORenderComponents::cullingFace() const
{
	return _cullFace;
}

inline OCullFront ORenderComponents::cullingFrontFace() const
{
	return _cullFront;
}

inline bool ORenderComponents::depthTestingEnabled() const
{
	return _depthTestEnabled;
}

inline bool ORenderComponents::depthBufferWriteEnabled() const
{
	return _depthBufferWrite;
}

inline void ORenderComponents::setRenderMode(ORenderMode aRenderMode)
{
	_renderMode = aRenderMode;
}

inline void ORenderComponents::setTexture(OTexture& aTexture)
{
	_texture = &aTexture;
}

inline void ORenderComponents::setMaterialSet(OMaterialSet& aMaterialSet)
{
	_materialSet = &aMaterialSet;
}

inline void ORenderComponents::setShaderProgram(OShaderProgram * aShaderProgram)
{
	_shaderProgram = aShaderProgram;
}

inline void ORenderComponents::setFaceCulling(bool aEnabled, OCullFace aFace, OCullFront aFrontFace)
{
	_faceCullingEnabled = aEnabled;
	_cullFace = aFace;
	_cullFront = aFrontFace;
}

inline void ORenderComponents::setDepthTesting(bool aTestEnabled, bool aBufferWrite)
{
	_depthTestEnabled = aTestEnabled;
	_depthBufferWrite = aBufferWrite;
}

