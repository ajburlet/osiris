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

void OTexture::setContent(uint32_t aMipmapLevel, uint32_t aRows, uint32_t aLines, uint8_t* aData, uint32_t aSize)
{
	auto& entry = _impl->mipmap[aMipmapLevel];
	entry.data = reinterpret_cast<uint8_t*>(Allocator().allocate(aSize));
	entry.size = aSize;
	entry.lines = aLines;
	entry.rows = aRows;

	memcpy(entry.data, aData, aSize);
}
