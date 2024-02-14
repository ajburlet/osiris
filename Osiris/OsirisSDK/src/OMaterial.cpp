#include <string>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OMaterial.h"

struct OMaterial::Impl : public OMemoryManagedObject<Allocator> {
	std::string	name;
	OVector3F	ka;	// ambient
	OVector3F	kd;	// diffuse
	OVector3F	ke;	// emmited
	OVector3F	ks;	// specular
	float		ns;	// specular exponent
	float		d;	// dissolve
	float		ni;	// optical density
	IllumModel	illum = OMaterial::IllumModel::NotSet;
};

OMaterial::OMaterial(const char* aName)
{
	OExceptionPointerCheck(_impl = new Impl);
	_impl->name = aName;
}

OMaterial::OMaterial(const OMaterial & aOther)
{
	OExceptionPointerCheck(_impl = new Impl);
	*_impl = *aOther._impl;
}

OMaterial::OMaterial(OMaterial && aOther)
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}

OMaterial::~OMaterial()
{
	if (_impl != nullptr) delete _impl;
}

OMaterial & OMaterial::operator=(OMaterial && aOther)
{
	if (_impl != nullptr) delete _impl;
	_impl = aOther._impl;
	aOther._impl = nullptr;
	return *this;
}

const char * OMaterial::name() const
{
	return _impl->name.c_str();
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
