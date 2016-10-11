#pragma once

#include "defs.h"
#include "GLdefs.h"
#include "OMathPrimitive.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

/**
 \brief Represents three-dimentional vectors.
*/
class OAPI OVector3 : public OMathPrimitive<glm::vec3>
{
public:
	OVector3();
	OVector3(const OVector3& in);
	OVector3(float val);
	OVector3(float vx, float vy, float vz);
	virtual ~OVector3();

	OVector3 cross(const OVector3& in) const;
	static OVector3 cross(const OVector3& a, const OVector3& b);

	void setX(float val);
	void setY(float val);
	void setZ(float val);

	float x() const;
	float y() const;
	float z() const;
};

/**
 \brief Represents four-dimentional vectors.
*/
class OAPI OVector4 : public OMathPrimitive<glm::vec4>
{
public:
	OVector4();
	OVector4(const OVector4& in);
	OVector4(float val);
	OVector4(float vx, float vy, float vz, float vw);
	OVector4(const OVector3& vec3, float vw=0.0f);
	virtual ~OVector4();

	void setX(float val);
	void setY(float val);
	void setZ(float val);
	void setW(float val);

	float x() const;
	float y() const;
	float z() const;
	float w() const;
};

/**
 \brief Represents 4x4 matrices.
*/
class OAPI OMatrix4x4 : public OMathPrimitive<glm::mat4x4>
{
public:
	OMatrix4x4();
	OMatrix4x4(float identValue);
	OMatrix4x4(const OMatrix4x4& in);
	virtual ~OMatrix4x4();
	
	void setValue(int row, int col, float value);
	float value(int row, int col) const;
};

