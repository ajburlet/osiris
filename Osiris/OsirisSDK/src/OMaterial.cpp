#include <string>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OMaterial.h"

struct OMaterial::Impl : public OMemoryManagedObject<Allocator> {
	OVector3F	ka;	// ambient
	OVector3F	kd;	// diffuse
	OVector3F	ke;	// emmited
	OVector3F	ks;	// specular
	float		ns;	// specular exponent
	float		d;	// dissolve
	float		ni;	// optical density
	IllumModel	illum = OMaterial::IllumModel::NotSet;
};

OMaterial::OMaterial(OString&& aName)
	: OResource(std::move(aName))
	, _impl(std::make_unique<OMaterial::Impl>())
{}

OMaterial::OMaterial(const OMaterial& aOther)
	: OResource(OString(aOther.name()))
	, _impl(std::make_unique<OMaterial::Impl>(*aOther._impl))
{}

OMaterial::OMaterial(OMaterial&& aOther)
	: OResource(std::move(aOther))
	, _impl(std::move(aOther)._impl)
{}

OMaterial::~OMaterial() = default;

OMaterial & OMaterial::operator=(OMaterial&& aOther)
{
	Super::operator=(std::move(aOther));
	_impl = std::move(aOther)._impl;
	return *this;
}

OMaterial& OMaterial::operator=(const OMaterial& aOther)
{
	if (this != &aOther) {
		_impl = std::make_unique<OMaterial::Impl>(*aOther._impl);
	}
	return *this;
}

void OMaterial::setAmbientColor(const OVector3F & aColor)
{
	_impl->ka = aColor;
}

const OVector3F & OMaterial::ambientColor() const
{
	return _impl->ka;
}

void OMaterial::setDiffuseColor(const OVector3F & aColor)
{
	_impl->kd = aColor;
}

const OVector3F & OMaterial::diffuseColor() const
{
	return _impl->kd;
}

void OMaterial::setEmitedColor(const OVector3F & aColor)
{
	_impl->ke = aColor;
}

const OVector3F & OMaterial::emitedColor() const
{
	return _impl->ke;
}

void OMaterial::setSpectralColor(const OVector3F & aColor)
{
	_impl->ks = aColor;
}

const OVector3F & OMaterial::spectralColor() const
{
	return _impl->ks;
}

void OMaterial::setSpectralExponent(float aValue)
{
	_impl->ns = aValue;
}

float OMaterial::spectralExponent() const
{
	return _impl->ns;
}

void OMaterial::setDissolve(float aValue)
{
	_impl->d = aValue;
}

float OMaterial::dissolve() const
{
	return _impl->d;
}

void OMaterial::setOpticalDensity(float aValue)
{
	_impl->ni = aValue;
}

float OMaterial::opticalDensity() const
{
	return _impl->ni;
}

void OMaterial::setIllumModel(IllumModel aModel)
{
	_impl->illum = aModel;
}

OMaterial::IllumModel OMaterial::illumModel() const
{
	return _impl->illum;
}
