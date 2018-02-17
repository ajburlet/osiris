#include <string.h>
#include "OsirisSDK/OException.h"

#include "OsirisSDK/OAttributeList.h"


// ****************************************************************************
// OAttributeList
// ****************************************************************************
OAttributeList::OAttributeList(int count) : 
	_attrCount(count)
{
	if (count > 0) _attrs = new Attribute[count];
	else _attrs = NULL;
}

OAttributeList::~OAttributeList()
{
	delete[] _attrs;
}

int OAttributeList::count() const
{
	return _attrCount;
}

OAttributeList::Attribute & OAttributeList::get(int idx)
{
	if (idx >= _attrCount) throw OException("Invalid access to attribute list.");
	return _attrs[idx];
}

OAttributeList::Attribute & OAttributeList::operator[](int idx)
{
	return get(idx);
}

// ****************************************************************************
// OAttributeList::Attribute
// ****************************************************************************
OAttributeList::Attribute::Attribute() : _type(Uninitialized)
{
}

OAttributeList::Attribute::Attribute(const Attribute & in) :
	_type(in._type),
	_value(in._value)
{
}

OAttributeList::Attribute::Attribute(bool val) :
	_type(Boolean)
{
	_value._bool = val;
}

OAttributeList::Attribute::Attribute(int val) :
	_type(Integer)
{
	_value._int = val;
}

OAttributeList::Attribute::Attribute(float val) :
	_type(Float)
{
	_value._float = val;
}

OAttributeList::Attribute::Attribute(const char * val) :
	_type(String)
{
	_value._str = _strdup(val);
}

OAttributeList::Attribute::~Attribute()
{
	if (_type == String && _value._str != NULL) delete _value._str;
}

OAttributeList::Attribute::Type OAttributeList::Attribute::type() const
{
	return _type;
}

bool OAttributeList::Attribute::boolVal() const
{
	if (_type != Boolean) throw OException("Attribute type mismatch.");
	return _value._bool;
}

int OAttributeList::Attribute::intVal() const
{
	if (_type != Integer) throw OException("Attribute type mismatch.");
	return _value._int;
}

float OAttributeList::Attribute::floatVal() const
{
	if (_type != Float) throw OException("Attribute type mismatch.");
	return _value._float;
}

const char * OAttributeList::Attribute::strVal() const
{
	if (_type != String) throw OException("Attribute type mismatch.");
	return _value._str;
}

void OAttributeList::Attribute::setVal(bool val)
{
	if (_type != Boolean) throw OException("Attribute type mismatch.");
	_value._bool = val;
}

void OAttributeList::Attribute::setVal(int val)
{
	if (_type != Integer) throw OException("Attribute type mismatch.");
	_value._int = val;
}

void OAttributeList::Attribute::setVal(float val)
{
	if (_type != Float) throw OException("Attribute type mismatch.");
	_value._float = val;
}

void OAttributeList::Attribute::setVal(const char * val)
{
	if (_type != String) throw OException("Attribute type mismatch.");
	if (_value._str != NULL) free(_value._str);
	_value._str = _strdup(val);
}

OAttributeList::Attribute & OAttributeList::Attribute::operator=(const Attribute & in)
{
	if (_type == Uninitialized) _type = in._type;
	else if (_type != in._type) throw OException("Attribute type mismatch.");
	_value = in._value;
	return *this;
}

OAttributeList::Attribute & OAttributeList::Attribute::operator=(bool in)
{
	setVal(in);
	return *this;
}

OAttributeList::Attribute & OAttributeList::Attribute::operator=(int in)
{
	setVal(in);
	return *this;
}

OAttributeList::Attribute & OAttributeList::Attribute::operator=(float in)
{
	setVal(in);
	return *this;
}

OAttributeList::Attribute & OAttributeList::Attribute::operator=(const char * in)
{
	setVal(in);
	return *this;
}
