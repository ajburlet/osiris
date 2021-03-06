#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <stack>

#include "defs.h"
#include "OMath.h"

/**
 \brief Matrix stack class.

 This class represents a stack of matrices, useful for handling coordinate system transformations on
 multiple levels. There is a top matrix on the stack, where transformations can be applied. When the need
 comes to preserve the top matrix, it can be pushed onto the stack, so it can be retrieved at a later time
 by using the pop() method.
*/
class OAPI OMatrixStack
{
public:

	/**
	 \brief Class constructor.
	*/
	OMatrixStack();

	/**
	 \brief Class destructor.
	*/
	virtual ~OMatrixStack();

	/**
	 \brief Pushes the current matrix on the stack.
	*/
	void push();

	/**
	 \brief Discards the matrix on top of the stack, and brings up the next one.
	*/
	void pop();

	/**
	 \brief Retrieves the current (top) matrix.
	*/
	OMatrix4x4 top() const;

	/**
	 \brief Returns true if the current stack is empty.
	*/
	bool isEmpty() const;

	/**
	 \brief Clears the stack and resets top matrix to identity.
	*/
	void clear();

	/**
	 \brief Assignment operator.
	 */
	OMatrixStack& operator=(const OMatrixStack& in);
 
	/**
	 \see product(const OMatrixStack&)
	*/
	OMatrixStack& operator*=(const OMatrixStack& in);

	/**
	 \see product(const OMatrix4x4&)
	*/
	OMatrixStack& operator*=(const OMatrix4x4& in);
	
	/**
	 \see product(const OQuaternion&)
	*/
	OMatrixStack& operator*=(const OQuaternion& in);

	/**
	 \brief Product of the top matrix by a four-dimentional vector.
	 */
	OVector4 operator*(const OVector4& in);

	/**
	 \brief Multiply the top matrix by another stack top matrix.
	 \param in Stack matrix that contains the top matrix that is going to be used in this operation.
	*/
	void product(const OMatrixStack& in);

	/**
	 \brief Multiply the top matrix by another matrix.
	 \param in Right value matrix in this product.
	*/
	void product(const OMatrix4x4& in);

	/**
	 \brief Multiply the top matrix by a quaternion representing a rotation.
	 \param in Right value in this matrix product.
	 */
	void product(const OQuaternion& in);

	/**
	 \brief Applies the translation transformation to a given direction.
	 \param dir Displacement direction vector.
	*/
	void translate(const OVector3 &dir);

	/**
	 \brief Applies the translation transform to a given direction.
	 \param dx Displacement direction on the X axis.
	 \param dy Displacement direction on the Y axis.
	 \param dz Displacement direction on the Z axis.
	*/
	void translate(const float& dx, const float& dy, const float& dz);

	/**
	 \brief Applies the rotation transformation around a given axis.
	 \param axis Rotation axis vector.
	 \param angle Rotation angle in degrees.
	*/
	void rotate(const OVector3 & axis, const float& angle);

	/**
	 \brief Applies the rotation transformation around a given axis.
	 \param axisX Rotation axis vector component on the X axis.
	 \param axisY Rotation axis vector component on the Y axis.
	 \param axisZ Rotation axis vector component on the Z axis.
	 \param angle Rotation angle in degrees.
	*/
	void rotate(const float& axisX, const float& axisY, const float& axisZ, const float& angle);

	/**
	 \brief Applies the rotation transformation around the X axis.
	 \param angle Rotation angle in degrees.
	*/
	void rotateX(const float& angle);

	/**
	 \brief Applies the rotation transformation around the Y axis.
	 \param angle Rotation angle in degrees.
	*/
	void rotateY(const float& angle);

	/**
	 \brief Applies the rotation transformation around the Z axis.
	 \param angle Rotation angle in degrees.
	*/
	void rotateZ(const float& angle);

	/**
	 \brief Applies the scale transformation, which re-scales the coordinate system by given factors
	 in each component axis.
	 \param factorVec Scale factors for each axis component.
	*/
	void scale(const OVector3& factorVec);

	/**
	 \brief Applies the scale transformation by a uniform parameter for all space axes.
	 \param uniformFactor Scale factor for all space axes.
	*/
	void scale(const float& uniformFactor);

	/**
	 \brief Applies the perspective projection transformation.
	 \param fieldOfViewDeg Angle of the camera's field of view in degrees.
	 \param aspectRatio The aspect ration of the screen (width/height).
	 \param zNear Nearest camera depth.
	 \param zFar Farthest camera depth.
	*/
	void perspective(float fieldOfViewDeg, float aspectRatio, float zNear, float zFar);

	/**
	 \brief Applies the camera transformation, moving the coordinate system to account for the 
	 current camera position and looking direction.
	 \param position Current camera position coordinates.
	 \param direction Camera looking direction vector.
	 \param up Vertical reference vector (direction when the camera is facing "up").
	*/
	void camera(const OVector3 &position, const OVector3 &direction, const OVector3 &up = OVector3(0, 1, 0));

private:
	OMatrix4x4 _currMtx;
	std::stack<OMatrix4x4> _stack;
};

