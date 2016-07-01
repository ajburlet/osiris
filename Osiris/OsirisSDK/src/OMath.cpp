#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OsirisSDK/OMath.h"
#include "OsirisSDK/OException.h"

/**
 \brief Class constructor
*/
OVector3::OVector3()
{
}

/**
 \brief Class copy constructor.
*/
OVector3::OVector3(const OVector3 & in) :
	OMathPrimitive<glm::vec3>(in)
{
}

/**
 \brief Class constructor.
 
 Creates a OVector3 with all components with the same value.
 
 \param val The value to be used by all of the three components.
*/
OVector3::OVector3(float val)
{
	_glmInternal = glm::vec3(val);
}

/**
 \brief Class constructor.

 Creates a OVector3 taking in the values for each component. 
*/
OVector3::OVector3(float vx, float vy, float vz)
{
	_glmInternal = glm::vec3(vx, vy, vz);
}

/**
 \brief Class destructor.
*/
OVector3::~OVector3()
{
}

/**
 \brief Vector cross product.
 \param in Right value in the cross operation.
 \return Resulting vector of the cross product.
*/
OVector3 OVector3::cross(const OVector3 & in) const
{
	OVector3 res;

	res._glmInternal = glm::cross(this->_glmInternal, in._glmInternal);

	return res;
}

/**
 \brief Vector cross product.
 \param a Left value of the operation.
 \param b Right value of the operation.
 \return Resulting vector of the cross product.
*/
OVector3 OVector3::cross(const OVector3 & a, const OVector3 & b)
{
	return a.cross(b);
}

/**
 \brief Set the value for the X axis component.
*/
void OVector3::setX(float val)
{
	_glmInternal.x = val;
}

/**
 \brief Set the value for the Y axis component.
*/
void OVector3::setY(float val)
{
	_glmInternal.y = val;
}

/**
 \brief Set the value for the Z axis component.
*/
void OVector3::setZ(float val)
{
	_glmInternal.z = val;
}

/**
 \brief Returns the value of the X axis component.
*/
float OVector3::x() const
{
	return _glmInternal.x;
}

/**
 \brief Returns the value of the Y axis component.
*/
float OVector3::y() const
{
	return _glmInternal.y;
}

/**
 \brief Returns the value of the Z axis component.
*/
float OVector3::z() const
{
	return _glmInternal.z;
}

/**
 \brief Class constructor.
*/
OMatrix4x4::OMatrix4x4()
{
}

/**
 \brief Class constructor that creates a diagonal matrix with values defined by the input parameter.
 \param identValue Values of the matrix main diagonal elements.
*/
OMatrix4x4::OMatrix4x4(float identValue)
{
	_glmInternal = glm::mat4x4(identValue);
}

/**
 \brief Class copy constructor.
*/
OMatrix4x4::OMatrix4x4(const OMatrix4x4 & in) :
	OMathPrimitive<glm::mat4x4>(in)
{
}

/**
 \brief Class destructor.
*/
OMatrix4x4::~OMatrix4x4()
{
}

/**
 \brief Set the value of a specific matrix element.
 \param row Row index number.
 \param col Column index number.
 \param value New element value.
*/
void OMatrix4x4::setValue(int row, int col, float value)
{
	if (col < 0 || col > 3) throw OException("Invalid column index for 4x4 matrix.");

	switch (row) {
	case 0: _glmInternal[col].x = value; break;
	case 1: _glmInternal[col].y = value; break;
	case 2: _glmInternal[col].z = value; break;
	case 3: _glmInternal[col].w = value; break;
	default: throw OException("Invalid row index for 4x4 matrix.");
	}
}

/**
 \brief Returns the value of a specific matrix element.
 \param row Row index number.
 \param col Column index number.
 \return Element value.
*/
float OMatrix4x4::value(int row, int col) const
{
	if (col < 0 || col > 3) throw OException("Invalid column index for 4x4 matrix.");

	switch (row) {
	case 0: return _glmInternal[col].x;
	case 1: return _glmInternal[col].y;
	case 2: return _glmInternal[col].z;
	case 3: return _glmInternal[col].w;
	default: throw OException("Invalid row index for 4x4 matrix.");
	}
}
