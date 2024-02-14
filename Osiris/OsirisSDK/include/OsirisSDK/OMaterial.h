#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OVectorDefs.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OGraphicsAllocators.h"

/**
 @brief Mesh material definition class.
 */
class OAPI OMaterial : public OMemoryManagedObject<OGraphicsAllocators::Default>
{
public:
	using Allocator = OGraphicsAllocators::Default;

	/**
	 @brief Class constructor.
	 */
	OMaterial(const char* aName);

	/**
	 @brief Copy constructor.
	 */
	OMaterial(const OMaterial& aOther);

	/**
	 @brief Move constructor.
	 */
	OMaterial(OMaterial&& aOther);

	/**
	 @brief Class destructor.
	 */
	~OMaterial();

	/**
	 @brief Copy assignment operator.
	 */
	OMaterial& operator=(const OMaterial& aOther);

	/**
	 @brief Move assignment operator.
	 */
	OMaterial& operator=(OMaterial&& aOther);

	/**
	 @brief Returns material name.
	 */
	const char* name() const;

	/**
	 @brief Sets the ambient color.
	 */
	void setAmbientColor(const OVector3F& aColor);

	/**
	 @brief Returns the ambient color.
	 */
	const OVector3F& ambientColor() const;

	/**
	 @brief Sets the diffuse color.
	 */
	void setDiffuseColor(const OVector3F& aColor);

	/**
	 @brief Returns the diffuse color.
	 */
	const OVector3F& diffuseColor() const;

	/**
	 @brief Sets the emited color.
	 */
	void setEmitedColor(const OVector3F& aColor);

	/**
	 @brief Returns the emited color.
	 */
	const OVector3F& emitedColor() const;

	/**
	 @brief Sets the spectral color.
	 */
	void setSpectralColor(const OVector3F& aColor);

	/**
	 @brief Returns the spectral color.
	 */
	const OVector3F& spectralColor() const;

	/**
	 @brief Sets the spectral exponent.
	 */
	void setSpectralExponent(float aValue);

	/**
	 @brief Returns the spectral exponent.
	 */
	float spectralExponent() const;

	/**
	 @brief Sets the dissolve (transparency) factor.
	 @note Value of 1.0 means opaque, 0.0 transparent.
	 */
	void setDissolve(float aValue);

	/**
	 @brief Returns the dissolve (transparency) factor.
	 */
	float dissolve() const;

	/**
	 @brief Sets the optical density (or index of refraction).
	 */
	void setOpticalDensity(float aValue);

	/**
	 @brief Returns the optical density (or index of refraction).
	 */
	float opticalDensity() const;

	/**
	 @brief Illumination models.
	 */
	enum class IllumModel {
		NotSet,					/**< Not set or supported. */
		ColorOnAmbientOff,			/**< Color on and Ambient off */
		ColorOnAmbientOn,			/**< Color on and Ambient on */
		Highlight,				/**< Highlight on */
		ReflectionOnRayTraceOn,			/**< Reflection on and Ray trace on */
		TranspGlassReflectRayTrace,		/**< Transparency: Glass on, Reflection: Ray trace on */
		ReflectFresnelRayTrace,			/**< Reflection: Fresnel on and Ray trace on */
		TranspRefractionReflectRayTrace,	/**< Transparency: Refraction on, Reflection: Fresnel off and Ray trace on */
		TranspRefractionReflectFresnelRayTrace,	/**< Transparency: Refraction on, Reflection: Fresnel on and Ray trace on */
		ReflectionOnRayTraceOff,		/**< Reflection on and Ray trace off */
		TranspGlassReflectionRayTraceOff	/**< Transparency: Glass on, Reflection: Ray trace off */
	};

	/**
	 @brief Sets the illumination model.
	 */
	void setIllumModel(IllumModel aModel);

	/**
	 @brief Returns the illumination model.
	 */
	IllumModel illumModel() const;

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