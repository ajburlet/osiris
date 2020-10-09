
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OShaderArgument.h"

// ****************************************************************************
// OShaderArgument
// ****************************************************************************
OShaderArgument::OShaderArgument(OVarType aType, uint8_t aDim) :
	_type(aType),
	_arrayLength(aDim)
{
	uint32_t unit_size = 0;
	uint32_t multiplier = -1;
	switch (_type) {
	case OVarType::Float:
		if (multiplier == -1) multiplier = 1;
	case OVarType::Float2:
		if (multiplier == -1) multiplier = 2;
	case OVarType::Float3:
		if (multiplier == -1) multiplier = 3;
	case OVarType::Float4:
		if (multiplier == -1) multiplier = 4;
	case OVarType::Float2x2:
		if (multiplier == -1) multiplier = 4;
	case OVarType::Float3x3:
		if (multiplier == -1) multiplier = 9;
	case OVarType::Float4x4:
		if (multiplier == -1) multiplier = 16;
	
		unit_size = sizeof(float);
		break;

	case OVarType::Int:
	case OVarType::UnsignedInt:
		if (multiplier == -1) multiplier = 1;
	case OVarType::Int2:
	case OVarType::UnsignedInt2:
		if (multiplier == -1) multiplier = 2;
	case OVarType::Int3:
	case OVarType::UnsignedInt3:
		if (multiplier == -1) multiplier = 3;
	case OVarType::Int4:
	case OVarType::UnsignedInt4:
		if (multiplier == -1) multiplier = 4;
	case OVarType::Int2x2:
		if (multiplier == -1) multiplier = 4;
	case OVarType::Int3x3:
		if (multiplier == -1) multiplier = 9;
	case OVarType::Int4x4:
		if (multiplier == -1) multiplier = 16;
		
		unit_size = 4;
		break;

	default:
		throw OException("Invalid vertex attribute type.");
	}
	
	_size = unit_size * multiplier * _arrayLength;
}


// ****************************************************************************
// OShaderArgumentInstance
// ****************************************************************************
using Allocator = OSystemMemoryAllocator<OMemoryManager::Scope::Graphics>;

OShaderArgumentInstance::OShaderArgumentInstance(OVarType aType, uint8_t aDim) :
	OShaderArgument(aType, aDim)
{
	OExceptionPointerCheck(_buffer = static_cast<uint8_t*>(Allocator().allocate(size())));
}

OShaderArgumentInstance::~OShaderArgumentInstance()
{
	if (_buffer) Allocator().deallocate(_buffer, size());
}

void OShaderArgumentInstance::copyFrom(const void* aSrc, uint32_t aOffset, uint32_t aBytes)
{
	uint32_t len = (aBytes == 0) ? _size - aOffset : aBytes;
	if (len > _size) throw new OException("Argument instance buffer overflow.");
	memcpy(_buffer + aOffset, aSrc, len);
}
