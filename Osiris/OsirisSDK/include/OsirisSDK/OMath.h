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
	
	/**
	 \brief Class constructor
	*/
	OVector3();
	
	/**
	 \brief Class copy constructor.
	*/
	OVector3(const OVector3& in);
	
	/**
	 \brief Class constructor.
	 
	 Creates a OVector3 with all components with the same value.
	 
	 \param val The value to be used by all of the three components.
	*/
	OVector3(float val);
	
	/**
	 \brief Class constructor.

	 Creates a OVector3 taking in the values for each component. 
	*/
	OVector3(float vx, float vy, float vz);
	
	/**
	 \brief Class destructor.
	*/
	virtual ~OVector3();

	/**
	 \brief Vector cross product.
	 \param in Right value in the cross operation.
	 \return Resulting vector of the cross product.
	*/
	OVector3 cross(const OVector3& in) const;
	
	/**
	 \brief Vector cross product.
	 \param a Left value of the operation.
	 \param b Right value of the operation.
	 \return Resulting vector of the cross product.
	*/
	static OVector3 cross(const OVector3& a, const OVector3& b);

	/**
	 \brief Set the value for the X axis component.
	*/
	void setX(float val);
	
	/**
	 \brief Set the value for the Y axis component.
	*/
	void setY(float val);
	
	/**
	 \brief Set the value for the Z axis component.
	*/
	void setZ(float val);

	/**
	 \brief Returns the value of the X axis component.
	*/
	float x() const;
	
	/**
	 \brief Returns the value of the Y axis component.
	*/
	float y() const;
	
	/**
	 \brief Returns the value of the Z axis component.
	*/
	float z() const;
};

/**
 \brief Represents four-dimentional vectors.
*/
class OAPI OVector4 : public OMathPrimitive<glm::vec4>
{
public:
	
	/**
	 \brief Class constructor
	*/
	OVector4();
	
	/**
	 \brief Class copy constructor.
	*/
	OVector4(const OVector4& in);
	
	/**
	 \brief Class constructor.
	 
	 Creates a OVector4 with all components with the same value.
	 
	 \param val The value to be used by all of the three components.
	*/
	OVector4(float val);
	
	/**
	 \brief Class constructor.

	 Creates a OVector4 taking in the values for each component. 
	*/
	OVector4(float vx, float vy, float vz, float vw);
	
	/**
	 \brief Class constructor.
	 
	 Creates a OVector4 from a OVector3 object, in which the X,Y and Z components
	 will receive the values from the three-dimensional vectors.
	 */
	OVector4(const OVector3& vec3, float vw=0.0f);
	
	/**
	 \brief Class destructor.
	*/
	virtual ~OVector4();

	/**
	 \brief Set the value for the X axis component.
	*/
	void setX(float val);
	
	/**
	 \brief Set the value for the Y axis component.
	*/
	void setY(float val);
	
	/**
	 \brief Set the value for the Z axis component.
	*/
	void setZ(float val);
	
	/**
	 \brief Set the value for the W axis component.
	*/
	void setW(float val);

	/**
	 \brief Returns the value of the X axis component.
	*/
	float x() const;
	
	/**
	 \brief Returns the value of the Y axis component.
	*/
	float y() const;
	
	/**
	 \brief Returns the value of the Z axis component.
	*/
	float z() const;
	
	/**
	 \brief Returns the value of the W axis component.
	*/
	float w() const;
};

/**
 \brief Represents 4x4 matrices.
*/
class OAPI OMatrix4x4 : public OMathPrimitive<glm::mat4x4>
{
public:
	/**
	 \brief Class constructor.
	*/
	OMatrix4x4();
	
	/**
	 \brief Class constructor that creates a diagonal matrix with values defined by the input parameter.
	 \param identValue Values of the matrix main diagonal elements.
	*/
	OMatrix4x4(float identValue);
	
	/**
	 \brief Class copy constructor.
	*/
	OMatrix4x4(const OMatrix4x4& in);
	
	/**
	 \brief Class destructor.
	*/
	virtual ~OMatrix4x4();
	
	/**
	 \brief Set the value of a specific matrix element.
	 \param row Row index number.
	 \param col Column index number.
	 \param value New element value.
	*/
	void setValue(int row, int col, float value);
	
	/**
	 \brief Returns the value of a specific matrix element.
	 \param row Row index number.
	 \param col Column index number.
	 \return Element value.
	*/
	float value(int row, int col) const;
};

