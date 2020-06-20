#include "OsirisSDK/OException.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OArray.hpp"
#include "OsirisSDK/OTexture.h"

using Allocator = OSystemMemoryAllocator<OMemoryManager::Scope::Graphics>;

struct OTexture::Impl {
	struct MipmapEntry {
		uint32_t	rows	= 0;
		uint32_t	lines	= 0;
		uint32_t	size	= 0;
		uint8_t*	data	= nullptr;
	};
	using MipmapArray = ODynArray<MipmapEntry, Allocator>;

	MipmapArray	mipmap;
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

void OTexture::setMipmapLevelCount(uint32_t aMipmapLevelCount)
{
	_impl->mipmap.resize(aMipmapLevelCount);
}

uint32_t OTexture::mipmapLevelCount() const
{
	return _impl->mipmap.size();
}

void OTexture::setContent(uint32_t aMipmapLevel, uint32_t aRows, uint32_t aLines, uint8_t* aData, uint32_t aSize)
{
	auto& entry = _impl->mipmap[aMipmapLevel];
	entry.data = reinterpret_cast<uint8_t*>(Allocator().allocate(aSize));
	entry.size = aSize;
	entry.lines = aLines;
	entry.rows = aRows;

	memcpy(entry.data, aData, aSize);
}

uint8_t * OTexture::content(uint32_t aMipmapLevel, uint32_t & aRows, uint32_t & aLines, uint32_t& aSize) const
{
	if (aMipmapLevel >= _impl->mipmap.size()) throw OException("Invalid mipmap level.");
	auto& mipmap = _impl->mipmap[aMipmapLevel];
	aRows = mipmap.rows;
	aLines = mipmap.lines;
	aSize = mipmap.size;
	return mipmap.data;
}
