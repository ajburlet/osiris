#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "OsirisSDK/OMath.h"
#include "OsirisSDK/OException.h"

#define PI	3.1415f

// ****************************************************************************
// OMath functions 
// ****************************************************************************
float OMath::deg2rad(float deg)
{
	return 2 * PI * deg / 360.0f;
}

float OMath::rad2deg(float rad)
{ 
	return 360.0f * rad / (2 * PI); 
}

float OMath::reverseSign(float in)
{
	return -abs(in)/in;
}

// ****************************************************************************
// OVector3
// ****************************************************************************
OVector3::OVector3()
{
}

OVector3::OVector3(const OVector3 & in) :
	OMathPrimitive<glm::vec3>(in)
{
}

OVector3::OVector3(float val)
{
	_glmInternal = glm::vec3(val);
}

OVector3::OVector3(float vx, float vy, float vz)
{
	_glmInternal = glm::vec3(vx, vy, vz);
}

OVector3::OVector3(const OMathPrimitive<glm::vec3>& in) :
	OMathPrimitive<glm::vec3>(in)
{
}

OVector3::~OVector3()
{
}

float & OVector3::operator[](Axis axis)
{
	switch (axis) {
	case X:	return _glmInternal.x;
	case Y:	return _glmInternal.y;
	case Z:	return _glmInternal.z;
	default: throw OException("Invalid access to OVector3 component.");
	}
}

OVector3 OVector3::cross(const OVector3 & in) const
{
	OVector3 res;
	res._glmInternal = glm::cross(_glmInternal, in._glmInternal);
	return res;
}

OVector3 OVector3::cross(const OVector3 & a, const OVector3 & b)
{
	return a.cross(b);
}

float OVector3::dot(const OVector3 & in) const
{
	return glm::dot(_glmInternal, in._glmInternal);
}

float OVector3::dot(const OVector3 & a, const OVector3 & b)
{
	return a.dot(b);
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

void OVector3::setR(float val)
{
	_glmInternal.x = val;
}

void OVector3::setTheta(float val)
{
	_glmInternal.y = val;
}

void OVector3::setPhi(float val)
{
	_glmInternal.z = val;
}

float OVector3::r() const
{
	return _glmInternal.x;
}

float OVector3::theta() const
{
	return _glmInternal.y;
}

float OVector3::phi() const
{
	return _glmInternal.z;
}

OVector3 OVector3::toSpherical() const
{
	return OVector3(sqrtf(x()*x()+y()*y()+z()*z()), 
			atanf(OMath::deg2rad(z()/x())), 
			atanf(OMath::deg2rad(sqrtf(x()*x()+z()*z())/y()))
			);
}

OVector3 OVector3::toCartesian() const
{
	return OVector3(r()*cosf(OMath::deg2rad(theta()))*sinf(OMath::deg2rad(phi())), 
			r()*cosf(OMath::deg2rad(phi())),
			r()*sinf(OMath::deg2rad(theta()))*sinf(OMath::deg2rad(phi()))
			);
}

// ****************************************************************************
// OVector4
// ****************************************************************************
OVector4::OVector4()
{
}

OVector4::OVector4(const OVector4 & in) :
	OMathPrimitive<glm::vec4>(in)
{
}

OVector4::OVector4(float val)
{
	_glmInternal = glm::vec4(val);
}

OVector4::OVector4(float vx, float vy, float vz, float vw)
{
	_glmInternal = glm::vec4(vx, vy, vz, vw);
}

OVector4::OVector4(const OVector3 & vec3, float vw)
{
	_glmInternal.x = vec3.x();
	_glmInternal.y = vec3.y();
	_glmInternal.z = vec3.z();
	_glmInternal.w = vw;
}

OVector4::OVector4(const OMathPrimitive<glm::vec4>& in) :
	OMathPrimitive<glm::vec4>(in)
{
}

OVector4::~OVector4()
{
}

float & OVector4::operator[](Axis axis)
{
	switch (axis) {
	case X: return _glmInternal.x;
	case Y: return _glmInternal.y;
	case Z: return _glmInternal.z;
	case W: return _glmInternal.w;
	default: throw OException("Invalid access to OVector3 component.");
	}
}

void OVector4::setX(float val)
{
	_glmInternal.x = val;
}

void OVector4::setY(float val)
{
	_glmInternal.y = val;
}

void OVector4::setZ(float val)
{
	_glmInternal.z = val;
}

void OVector4::setW(float val)
{
	_glmInternal.w = val;
}

float OVector4::x() const
{
	return _glmInternal.x;
}

float OVector4::y() const
{
	return _glmInternal.y;
}

float OVector4::z() const
{
	return _glmInternal.z;
}

float OVector4::w() const
{
	return _glmInternal.w;
}

// ****************************************************************************
// OMatrix4x4
// ****************************************************************************
OMatrix4x4::OMatrix4x4()
{
}

OMatrix4x4::OMatrix4x4(float identValue)
{
	_glmInternal = glm::mat4x4(identValue);
}

OMatrix4x4::OMatrix4x4(const OMatrix4x4 & in) :
	OMathPrimitive<glm::mat4x4>(in)
{
}

OMatrix4x4::OMatrix4x4(const OMathPrimitive<glm::mat4x4>& in) :
	OMathPrimitive<glm::mat4x4>(in)
{
}

OMatrix4x4::~OMatrix4x4()
{
}

OVector4 OMatrix4x4::operator*(const OVector4 & in) const
{
	glm::vec4 newVec = _glmInternal * in.glm();
	OVector4 vec;
	vec.setGlm(newVec);
	return vec;
}

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

// ****************************************************************************
// OQuaternion
// ****************************************************************************
OQuaternion::OQuaternion()
{
}

OQuaternion::OQuaternion(const OQuaternion & in) :
	OMathPrimitive<glm::quat>(in)
{
}

OQuaternion::OQuaternion(float x, float y, float z, float w) 
{
	_glmInternal = glm::quat(x, y, z, w);
}

OQuaternion::OQuaternion(OVector3 rotationAxis, float angle)
{
	_glmInternal = glm::angleAxis(OMath::deg2rad(angle), rotationAxis.glm());
}

OQuaternion::OQuaternion(OVector3 eulerAngles)
{
	_glmInternal = glm::quat(eulerAngles.glm() * 2.0f * PI / 360.0f);
}

OQuaternion::OQuaternion(const OMathPrimitive<glm::quat>& in) :
	OMathPrimitive<glm::quat>(in)
{
}

OQuaternion::~OQuaternion()
{
}

OVector3 OQuaternion::operator*(const OVector3 & in) const
{
	OVector3 res;
	res.setGlm(_glmInternal * in.glm());
	return res;
}

OMatrix4x4 OQuaternion::toMatrix4() const
{
	OMatrix4x4 res;
	res.setGlm(glm::toMat4(_glmInternal));
	return res;
}

OVector3 OQuaternion::toEulerAngles() const
{
	OVector3 res;
	res.setGlm(glm::eulerAngles(_glmInternal));
	res *= 360 / (2 * PI);
	return res;
}

OQuaternion OQuaternion::inverse() const
{
	OQuaternion res;
	res.setGlm(glm::inverse(_glmInternal));
	return res;
}

