
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OShaderArgument.h"

// ****************************************************************************
// OShaderArgument
// ****************************************************************************
OShaderArgument::OShaderArgument(OVarType aType, OVarPrecision aPrecision, uint8_t aDim) :
	_type(aType),
	_precision(aPrecision),
	_dim(aDim)
{
	uint32_t unit_size = 0;
	uint32_t multiplier = -1;
	switch (_type) {
	case OVarType::Float:
		if (multiplier == -1) multiplier = 1;
	case OVarType::Float2x2:
		if (multiplier == -1) multiplier = 4;
	case OVarType::Float3x3:
		if (multiplier == -1) multiplier = 9;
	case OVarType::Float4x4:
		if (multiplier == -1) multiplier = 16;

		switch (_precision) {
		case OVarPrecision::Low:	unit_size = 2;	break;
		case OVarPrecision::Medium:	unit_size = 4;	break;
		case OVarPrecision::High:	unit_size = 8;	break;
		default: 
			throw OException("Invalid float vertex attribute precision.");
		}
		break;

	case OVarType::Int:
	case OVarType::UnsignedInt:
		if (multiplier == -1) multiplier = 1;
	case OVarType::Int2x2:
		if (multiplier == -1) multiplier = 4;
	case OVarType::Int3x3:
		if (multiplier == -1) multiplier = 9;
	case OVarType::Int4x4:
		if (multiplier == -1) multiplier = 16;

		switch (_precision) {
		case OVarPrecision::Low:	unit_size = 1;	break;
		case OVarPrecision::Medium:	unit_size = 2;	break;
		case OVarPrecision::High:	unit_size = 4;	break;
		default:
			throw OException("Invalid integer vertex attribute precision.");
		}
		break;

	default:
		throw OException("Invalid vertex attribute type.");
	}
	
	_size = unit_size * multiplier;
}


// ****************************************************************************
// OShaderArgumentInstance
// ****************************************************************************
using Allocator = OSystemMemoryAllocator<OMemoryManager::Scope::Graphics>;

OShaderArgumentInstance::OShaderArgumentInstance(OVarType aType, OVarPrecision aPrecision, uint8_t aDim) :
	OShaderArgument(aType, aPrecision, aDim)
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
