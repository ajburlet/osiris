#include <string.h>
#include "OsirisSDK/OException.h"

#include "OsirisSDK/OParameterList.h"


// ****************************************************************************
// OParameterList
// ****************************************************************************
OParameterList::OParameterList(int count) : 
	_attrCount(count)
{
	if (count > 0) _attrs = new Parameter[count];
	else _attrs = NULL;
}

OParameterList::~OParameterList()
{
	delete[] _attrs;
}

int OParameterList::count() const
{
	return _attrCount;
}

OParameterList::Parameter & OParameterList::get(int idx)
{
	if (idx >= _attrCount) throw OException("Invalid access to parameter list.");
	return _attrs[idx];
}

OParameterList::Parameter & OParameterList::operator[](int idx)
{
	return get(idx);
}

// ****************************************************************************
// OParameterList::Parameter
// ****************************************************************************
OParameterList::Parameter::Parameter() : _type(Uninitialized)
{
}

OParameterList::Parameter::Parameter(const Parameter & in) :
	_type(in._type),
	_value(in._value)
{
}

OParameterList::Parameter::Parameter(bool val) :
	_type(Boolean)
{
	_value._bool = val;
}

OParameterList::Parameter::Parameter(int val) :
	_type(Integer)
{
	_value._int = val;
}

OParameterList::Parameter::Parameter(float val) :
	_type(Float)
{
	_value._float = val;
}

OParameterList::Parameter::Parameter(const char * val) :
	_type(String)
{
	_value._str = _strdup(val);
}

OParameterList::Parameter::~Parameter()
{
	if (_type == String && _value._str != NULL) delete _value._str;
}

OParameterList::Parameter::Type OParameterList::Parameter::type() const
{
	return _type;
}

bool OParameterList::Parameter::boolVal() const
{
	if (_type != Boolean) throw OException("Parameter type mismatch.");
	return _value._bool;
}

int OParameterList::Parameter::intVal() const
{
	if (_type != Integer) throw OException("Parameter type mismatch.");
	return _value._int;
}

float OParameterList::Parameter::floatVal() const
{
	if (_type != Float) throw OException("Parameter type mismatch.");
	return _value._float;
}

const char * OParameterList::Parameter::strVal() const
{
	if (_type != String) throw OException("Parameter type mismatch.");
	return _value._str;
}

void OParameterList::Parameter::setVal(bool val)
{
	if (_type != Boolean) throw OException("Parameter type mismatch.");
	_value._bool = val;
}

void OParameterList::Parameter::setVal(int val)
{
	if (_type != Integer) throw OException("Parameter type mismatch.");
	_value._int = val;
}

void OParameterList::Parameter::setVal(float val)
{
	if (_type != Float) throw OException("Parameter type mismatch.");
	_value._float = val;
}

void OParameterList::Parameter::setVal(const char * val)
{
	if (_type != String) throw OException("Parameter type mismatch.");
	if (_value._str != NULL) free(_value._str);
	_value._str = _strdup(val);
}

OParameterList::Parameter & OParameterList::Parameter::operator=(const Parameter & in)
{
	if (_type == Uninitialized) _type = in._type;
	else if (_type != in._type) throw OException("Parameter type mismatch.");
	_value = in._value;
	return *this;
}

OParameterList::Parameter & OParameterList::Parameter::operator=(bool in)
{
	setVal(in);
	return *this;
}

OParameterList::Parameter & OParameterList::Parameter::operator=(int in)
{
	setVal(in);
	return *this;
}

OParameterList::Parameter & OParameterList::Parameter::operator=(float in)
{
	setVal(in);
	return *this;
}

OParameterList::Parameter & OParameterList::Parameter::operator=(const char * in)
{
	setVal(in);
	return *this;
}
