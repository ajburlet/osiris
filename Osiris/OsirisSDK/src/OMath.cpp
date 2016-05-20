#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OsirisSDK/OMath.h"
#include "OsirisSDK/OException.h"

OVector3::OVector3()
{
}

OVector3::OVector3(const OVector3 & in) :
	_glmInternal(in._glmInternal)
{
}

OVector3::OVector3(float val) :
	_glmInternal(val)
{
}

OVector3::OVector3(float vx, float vy, float vz) :
	_glmInternal(vx, vy, vz)
{
}

OVector3::~OVector3()
{
}

OVector3 & OVector3::operator=(const OVector3 & in)
{
	_glmInternal = in._glmInternal;
	return *this;
}

OVector3 OVector3::operator*(const OVector3 & in)
{
	OVector3 res;

	res._glmInternal = this->_glmInternal * in._glmInternal;

	return res;
}

OVector3 & OVector3::operator*=(const OVector3 & in)
{
	this->_glmInternal *= in._glmInternal;

	return *this;
}

#ifdef OSIRISSDK_EXPORTS
const glm::vec3 & OVector3::glm() const
{
	return _glmInternal;
}
#endif

OVector3 OVector3::cross(const OVector3 & in) const
{
	OVector3 res;

	res._glmInternal = glm::cross(this->_glmInternal, in._glmInternal);

	return res;
}

OVector3 OVector3::cross(const OVector3 & a, const OVector3 & b)
{
	return a.cross(b);
}

void OVector3::setX(float val)
{
	_glmInternal.x = val;
}

void OVector3::setY(float val)
{
	_glmInternal.y = val;
}

void OVector3::setZ(float val)
{
	_glmInternal.z = val;
}

float OVector3::x() const
{
	return _glmInternal.x;
}

float OVector3::y() const
{
	return _glmInternal.y;
}

float OVector3::z() const
{
	return _glmInternal.z;
}

const GLfloat * OVector3::glArea() const
{
	return glm::value_ptr(_glmInternal);
}

OMatrix4x4::OMatrix4x4()
{
}

OMatrix4x4::OMatrix4x4(float identValue) :
	_glmInternal(identValue)
{
}

OMatrix4x4::OMatrix4x4(const OMatrix4x4 & in) :
	_glmInternal(in._glmInternal)
{
}

OMatrix4x4::~OMatrix4x4()
{
}

OMatrix4x4 & OMatrix4x4::operator=(const OMatrix4x4 & in)
{
	_glmInternal = in._glmInternal;
	return *this;
}

OMatrix4x4 & OMatrix4x4::operator=(float identValue)
{
	_glmInternal = glm::mat4x4(identValue);
	return *this;
}

OMatrix4x4 OMatrix4x4::operator*(const OMatrix4x4 & in)
{
	OMatrix4x4 result;
	result._glmInternal = _glmInternal * in._glmInternal;
	return result;

}

OMatrix4x4 & OMatrix4x4::operator*=(const OMatrix4x4 & in)
{
	_glmInternal *= in._glmInternal;
	return *this;
}

#ifdef OSIRISSDK_EXPORTS
OMatrix4x4 OMatrix4x4::operator*(const glm::mat4x4 & in)
{
	OMatrix4x4 res;
	res._glmInternal = _glmInternal * in;
	return res;
}

OMatrix4x4 & OMatrix4x4::operator*=(const glm::mat4x4 & in)
{
	_glmInternal *= in;
	return *this;
}
#endif

#ifdef OSIRISSDK_EXPORTS
const glm::mat4x4 & OMatrix4x4::glm() const
{
	return _glmInternal;
}
#endif

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

const GLfloat * OMatrix4x4::glArea() const
{
	return glm::value_ptr(_glmInternal);
}
