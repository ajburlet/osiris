#pragma once

#include "defs.h"
#include "GLdefs.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class OAPI OVector3
{
public:
	OVector3();
	OVector3(const OVector3& in);
	OVector3(float val);
	OVector3(float vx, float vy, float vz);
	virtual ~OVector3();

	OVector3& operator=(const OVector3& in);
	OVector3 operator*(const OVector3& in);
	OVector3& operator*=(const OVector3& in);

#ifdef OSIRISSDK_EXPORTS
	const glm::vec3 & glm() const;
#endif

	OVector3 cross(const OVector3& in) const;
	static OVector3 cross(const OVector3& a, const OVector3& b);

	void setX(float val);
	void setY(float val);
	void setZ(float val);

	float x() const;
	float y() const;
	float z() const;

	const GLfloat* glArea() const;

private:
	glm::vec3 _glmInternal;
};

class OAPI OMatrix4x4
{
public:
	OMatrix4x4();
	OMatrix4x4(float identValue);
	OMatrix4x4(const OMatrix4x4& in);
	virtual ~OMatrix4x4();
	
	OMatrix4x4& operator=(const OMatrix4x4& in);
	OMatrix4x4& operator=(float identValue);
	OMatrix4x4 operator*(const OMatrix4x4& in);
	OMatrix4x4& operator*=(const OMatrix4x4& in);
#ifdef OSIRISSDK_EXPORTS
	OMatrix4x4 operator*(const glm::mat4x4& in);
	OMatrix4x4& operator*=(const glm::mat4x4& in);
#endif

#ifdef OSIRISSDK_EXPORTS
	const glm::mat4x4 & glm() const;
#endif

	void setValue(int row, int col, float value);
	float value(int row, int col) const;

	const GLfloat* glArea() const;

private:
	glm::mat4x4 _glmInternal;
};

