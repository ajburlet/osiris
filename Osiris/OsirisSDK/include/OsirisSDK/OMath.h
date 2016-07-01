#pragma once

#include "defs.h"
#include "GLdefs.h"
#include "OMathPrimitive.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

/**
 \brief Class the represents three-dimentional vectors.
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
 \brief Class that represents 4x4 matrices.
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

