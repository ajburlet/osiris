#include "OsirisSDK/ORenderObject.h"


ORenderObject::ORenderObject() :
	_hidden(false)
{
}

ORenderObject::~ORenderObject()
{
}

void ORenderObject::show()
{
	_hidden = false;
}

void ORenderObject::hide()
{
	_hidden = true;
}

bool ORenderObject::isHidden() const
{
	return _hidden;
}
