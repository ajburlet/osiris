#include "OsirisSDK/OIndexBuffer.h"
#include "OsirisSDK/OMaterial.h"
#include "OsirisSDK/OIndexedDrawInfo.h"

struct OIndexedDrawInfo::Impl : public OMemoryManagedObject<Allocator>
{
	Impl(const OMaterial* aMaterial) 
	{ 
		if (material != nullptr) {
			OExPointerCheck(material = new OMaterial(*aMaterial)); 
		}
	}
	~Impl() 
	{ 
		if (material != nullptr) {
			delete material;
		}
	}

	OIndexBuffer	indexBuffer;
	OMaterial*	material = nullptr;
};

OIndexedDrawInfo::OIndexedDrawInfo(const OMaterial * aMaterial)
{
	OExPointerCheck(_impl = new Impl(aMaterial));
}

OIndexedDrawInfo::OIndexedDrawInfo(OIndexedDrawInfo && aOther)
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}

OIndexedDrawInfo& OIndexedDrawInfo::operator=(OIndexedDrawInfo&& aOther)
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
	return *this;
}

OIndexedDrawInfo::~OIndexedDrawInfo()
{
	if (_impl != nullptr) delete _impl;
}

OIndexBuffer * OIndexedDrawInfo::indexBuffer()
{
	return &_impl->indexBuffer;
}

OMaterial * OIndexedDrawInfo::material()
{
	return _impl->material;
}
