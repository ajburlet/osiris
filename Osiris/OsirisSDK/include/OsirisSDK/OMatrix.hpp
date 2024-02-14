#pragma once

#include <glm/detail/qualifier.hpp>
#include <glm/detail/type_mat4x4.hpp>

#include "OsirisSDK/OMathPrimitive.hpp"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OMatrixDefs.h"

/**
 @brief Base template matrix class.
 */
template <uint8_t L, uint8_t C, typename T, glm::qualifier Q>
class OMatrix : public OMathPrimitive<glm::mat<L, C, T, Q>>
{
private:
	using Super = OMathPrimitive<glm::mat<L, C, T, Q>>;

public:
	using Super::GLMType;

public:
	/**
	 @brief Default class constructor.
	 */
	OMatrix() = default;
	
	/**
	 @brief Copy constructor for the OMathPrimitive superclass.
	 */
	OMatrix(const Super& aOther);
	
	/**
	 @brief Constructor for the base GLM type.
	 */
	OMatrix(const typename Super::GLMType& aGLM);

	/**
	 @brief Class destructor.
	 */
	virtual ~OMatrix() = default;
	
	/**
	 @brief Set the value of a specific matrix element.
	 @param aRow Row index number.
	 @param aCol Column index number.
	 @param aValue New element value.
	*/
	virtual void setValue(uint8_t aRow, uint8_t aCol, T aValue) = 0;
	
	/**
	 @brief Returns the value of a specific matrix element.
	 @param aRow Row index number.
	 @param aCol Column index number.
	 @return Element value.
	*/
	virtual T value(uint8_t aRow, uint8_t aCol) const = 0;
};

template<uint8_t L, uint8_t C, typename T, glm::qualifier Q>
inline OMatrix<L, C, T, Q>::OMatrix(const Super & aOther) :
	Super(aOther)
{
}

template<uint8_t L, uint8_t C, typename T, glm::qualifier Q>
inline OMatrix<L, C, T, Q>::OMatrix(const typename Super::GLMType & aGLM) :
	Super(aGLM)
{
}

/**
 @brief Represents 4x4 matrices.
*/
template <typename T, glm::qualifier Q>
class OMatrix4x4 : public OMatrix<4, 4, T, Q> 
{
private:
	using Super = OMatrix<4, 4, T, Q>; 

public:
	/**
	 @brief Class constructor.
	*/
	OMatrix4x4() = default;
	
	/**
	 @brief Class copy constructor.
	*/
	OMatrix4x4(const OMatrix4x4& aOther);
	
	/**
	 @brief Constructor for the base GLM type.
	 */
	OMatrix4x4(const typename Super::GLMType& aGLM);
	
	/**
	 @brief Class constructor that creates a diagonal matrix with values defined by the input parameter.
	 @param aIdentValue Values of the matrix main diagonal elements.
	*/
	OMatrix4x4(T aIdentValue);

	/**
	 @brief Class destructor.
	*/
	virtual ~OMatrix4x4() = default;

	/**
	 @brief Matrix product operator for four-dimentional vectors.
	 */
	OVector4<T, Q> operator*(const OVector4<T, Q>& aOther) const;

	//OMatrix4x4 operator*(const OMatrix4x4& in) const { return Super::operator*(in);  }

	/**
	 @copydoc OMatrix::setValue(uint8_t, uint8_t, T)
	*/
	void setValue(uint8_t aRow, uint8_t aCol, T aValue) override;
	
	/**
	 @copydoc OMatrix::setValue(uint8_t, uint8_t)
	*/
	T value(uint8_t aRow, uint8_t aCol) const override;
};

template<typename T, glm::qualifier Q>
inline OMatrix4x4<T, Q>::OMatrix4x4(const OMatrix4x4 & aOther) :
	OMatrix(aOther)
{
}

template<typename T, glm::qualifier Q>
inline OMatrix4x4<T, Q>::OMatrix4x4(T aIdentValue) :
	OMatrix(GLMType(aIdentValue))
{
}

template<typename T, glm::qualifier Q>
inline OMatrix4x4<T, Q>::OMatrix4x4(const typename Super::GLMType& aGLM) :
	Super(aGLM)
{

}

template<typename T, glm::qualifier Q>
inline OVector4<T, Q> OMatrix4x4<T, Q>::operator*(const OVector4<T, Q>& aOther) const
{
	return Super(_glmInternal * aOther._glmInternal);
}

template<typename T, glm::qualifier Q>
inline void OMatrix4x4<T, Q>::setValue(uint8_t aRow, uint8_t aCol, T aValue)
{
	if (aCol < 0 || aCol > 3) throw OException("Invalid column index for 4x4 matrix.");

	switch (aRow) {
	case 0: _glmInternal[aCol].x = aValue; break;
	case 1: _glmInternal[aCol].y = aValue; break;
	case 2: _glmInternal[aCol].z = aValue; break;
	case 3: _glmInternal[aCol].w = aValue; break;
	default: throw OException("Invalid row index for 4x4 matrix.");
	}
}

template<typename T, glm::qualifier Q>
inline T OMatrix4x4<T, Q>::value(uint8_t aRow, uint8_t aCol) const
{
	if (aCol < 0 || aCol > 3) throw OException("Invalid column index for 4x4 matrix.");

	switch (aRow) {
	case 0: return _glmInternal[aCol].x;
	case 1: return _glmInternal[aCol].y;
	case 2: return _glmInternal[aCol].z;
	case 3: return _glmInternal[aCol].w;
	default: throw OException("Invalid row index for 4x4 matrix.");
	}
}
