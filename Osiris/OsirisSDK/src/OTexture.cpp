#include "OsirisSDK/OException.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OTexture.h"

using Allocator = OSystemMemoryAllocator<OMemoryManager::Scope::Graphics>;

struct OTexture::Impl {
	struct MipmapEntry {
		uint32_t	width	= 0;
		uint32_t	height	= 0;
		uint32_t	size	= 0;
		uint8_t*	data	= nullptr;
	};
	using MipmapArray = ODynArray<MipmapEntry, Allocator>;

	MipmapArray	mipmap;
	FilterType	minFilter		= FilterType::Default;
	FilterType	magFilter		= FilterType::Default;
	WrapMode	wrapTypeS		= WrapMode::Default;
	WrapMode	wrapTypeT		= WrapMode::Default;
	WrapMode	wrapTypeR		= WrapMode::Default;
	PixelFormat	srcPixelFormat		= PixelFormat::Undefined;
	PixelFormat	dstPixelFormat		= PixelFormat::Undefined;
	PixelDataType	pixelDataType		= PixelDataType::Undefined;
	RowAlignment	packAlignment		= RowAlignment::Default;
	RowAlignment	unpackAlignment		= RowAlignment::Default;
};

OTexture::OTexture()
{
	OExceptionPointerCheck(_impl = new Impl);
}

OTexture::~OTexture()
{
	for (auto entry : _impl->mipmap) {
		if (entry.data != nullptr) Allocator().deallocate(entry.data, entry.size);
	}
	if (_impl) delete _impl;
}

OTexture & OTexture::operator=(OTexture && aOther)
{
	if (_impl != nullptr) delete _impl;
	_impl = aOther._impl;
	aOther._impl = nullptr;
	return *this;
}

void OTexture::setMipmapLevelCount(uint32_t aMipmapLevelCount)
{
	_impl->mipmap.resize(aMipmapLevelCount);
}

uint32_t OTexture::mipmapLevelCount() const
{
	return _impl->mipmap.size();
}

void OTexture::setMinFilter(OTexture::FilterType aFilter)
{
	_impl->minFilter = aFilter;
}

void OTexture::setMagFilter(OTexture::FilterType aFilter)
{
	_impl->magFilter = aFilter;
}

OTexture::FilterType OTexture::minFilter() const
{
	return _impl->minFilter;
}

OTexture::FilterType OTexture::magFilter() const
{
	return _impl->magFilter;
}

void OTexture::setWrapType(Coordinate aCoordinate, OTexture::WrapMode aWrapType)
{
	switch (aCoordinate) {
	case Coordinate::S:	_impl->wrapTypeS = aWrapType;		break;
	case Coordinate::R:	_impl->wrapTypeR = aWrapType;		break;
	case Coordinate::T:	_impl->wrapTypeT = aWrapType;		break;
	default:		throw OException("Invalid coordinate.");
	}
}

OTexture::WrapMode OTexture::wrapType(Coordinate aCoordinate) const
{
	switch (aCoordinate) {
	case Coordinate::S:	return _impl->wrapTypeS;
	case Coordinate::R:	return _impl->wrapTypeR;
	case Coordinate::T:	return _impl->wrapTypeT;
	default:		throw OException("Invalid coordinate.");
	}
}

void OTexture::setPixelFormat(OTexture::PixelFormat aSrcPixelFormat, OTexture::PixelDataType aPixelDataType,
			      OTexture::PixelFormat aDstPixelFormat)
{
	_impl->srcPixelFormat = aSrcPixelFormat;
	_impl->pixelDataType = aPixelDataType;
	_impl->dstPixelFormat = (aDstPixelFormat == PixelFormat::Undefined) ? aSrcPixelFormat : aDstPixelFormat;
}

OTexture::PixelFormat OTexture::sourcePixelFormat() const
{
	return _impl->srcPixelFormat;
}

OTexture::PixelFormat OTexture::destinationPixelFormat() const
{
	return _impl->dstPixelFormat;
}

OTexture::PixelDataType OTexture::pixelDataType() const
{
	return _impl->pixelDataType;
}

void OTexture::setPackAlignment(OTexture::RowAlignment aAlignment)
{
	_impl->packAlignment = aAlignment;
}

void OTexture::setUnpackAlignment(RowAlignment aAlignment)
{
	_impl->unpackAlignment = aAlignment;
}

OTexture::RowAlignment OTexture::packAlignment() const
{
	return _impl->packAlignment;
}

OTexture::RowAlignment OTexture::unpackAlignment() const
{
	return _impl->unpackAlignment;
}

void OTexture::setContent(uint32_t aMipmapLevel, uint32_t aWidth, uint32_t aHeight, uint8_t* aData, uint32_t aSize)
{
	auto& entry = _impl->mipmap[aMipmapLevel];
	entry.data = reinterpret_cast<uint8_t*>(Allocator().allocate(aSize));
	entry.size = aSize;
	entry.width = aWidth;
	entry.height = aHeight;

	memcpy(entry.data, aData, aSize);
}

uint8_t * OTexture::content(uint32_t aMipmapLevel, uint32_t & aWidth, uint32_t & aHeight, uint32_t& aSize) const
{
	if (aMipmapLevel >= _impl->mipmap.size()) throw OException("Invalid mipmap level.");
	auto& mipmap = _impl->mipmap[aMipmapLevel];
	aWidth = mipmap.width;
	aHeight = mipmap.height;
	aSize = mipmap.size;
	return mipmap.data;
}
