
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
	switch (_type) {
	case OVarType::Float:
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
	
}

// ****************************************************************************
// OShaderArgument
// ****************************************************************************
using Allocator = OSystemMemoryAllocator<OMemoryManager::Scope::Graphics>;

OShaderArgumentInstance::OShaderArgumentInstance(OVarType aType, OVarPrecision aPrecision, uint8_t aDim) :
	OShaderArgument(aType, aPrecision, aDim)
{
	OExceptionPointerCheck(_buffer = Allocator().allocate(size()));
}

OShaderArgumentInstance::~OShaderArgumentInstance()
{
	if (_buffer) Allocator().deallocate(_buffer, size());
}
