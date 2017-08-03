#include "OsirisSDK/OBaseEntity.h"

OBaseEntity::OBaseEntity() :
	_disabled(false) 
{ 
}

OBaseEntity::~OBaseEntity()
{
}

void OBaseEntity::enable()
{
	_disabled = false;
}

void OBaseEntity::disable()
{
	_disabled = true;
}

bool OBaseEntity::isDisabled() const
{
	return _disabled;
}
