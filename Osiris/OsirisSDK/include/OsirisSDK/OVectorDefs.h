#pragma once

#include <cstdint>
#include <glm/detail/qualifier.hpp>

template <typename, glm::qualifier> class OVector2;
template <typename, glm::qualifier> class OVector3;
template <typename, glm::qualifier> class OVector4;
	
/**
 @brief Axis index enumeration.
 */
enum class OVectorAxis {
	X=0,
	Y,
	Z,
	W,
	R=X,
	Theta=Y,
	Phi=Z,
	G=Y,
	B=Z,
	A=W
};


using OVector2I8 = OVector2<int8_t, glm::lowp>;		/**< Two-dimensional vector of signed 8-bit integers. */
using OVector2U8 = OVector2<uint8_t, glm::lowp>;	/**< Two-dimensional vector of unsigned 8-bit integers. */
using OVector2I16 = OVector2<int16_t, glm::mediump>;	/**< Two-dimensional vector of signed 32-bit integers. */
using OVector2U16 = OVector2<uint16_t, glm::mediump>;	/**< Two-dimensional vector of unsigned 32-bit integers. */
using OVector2I32 = OVector2<int32_t, glm::highp>;	/**< Two-dimensional vector of signed 32-bit integers. */
using OVector2U32 = OVector2<uint32_t, glm::highp>;	/**< Two-dimensional vector of unsigned 32-bit integers. */
using OVector2FL = OVector2<float, glm::lowp>;		/**< Two-dimensional vector of low precision floats. */
using OVector2FM = OVector2<float, glm::mediump>;	/**< Two-dimensional vector of medium precision floats. */
using OVector2FH = OVector2<float, glm::highp>;		/**< Two-dimensional vector of high floats. */
using OVector2F = OVector2<float, glm::defaultp>;	/**< Two-dimensional vector of floats. */
using OVector2D = OVector2<double, glm::defaultp>;	/**< Two-dimensional vector of double precision floats. */

using OVector3I8 = OVector3<int8_t, glm::lowp>;		/**< Three-dimensional vector of signed 8-bit integers. */
using OVector3U8 = OVector3<uint8_t, glm::lowp>;	/**< Three-dimensional vector of unsigned 8-bit integers. */
using OVector3I16 = OVector3<int16_t, glm::mediump>;	/**< Three-dimensional vector of signed 32-bit integers. */
using OVector3U16 = OVector3<uint16_t, glm::mediump>;	/**< Three-dimensional vector of unsigned 32-bit integers. */
using OVector3I32 = OVector3<int32_t, glm::highp>;	/**< Three-dimensional vector of signed 32-bit integers. */
using OVector3U32 = OVector3<uint32_t, glm::highp>;	/**< Three-dimensional vector of unsigned 32-bit integers. */
using OVector3FL = OVector3<float, glm::lowp>;		/**< Three-dimensional vector of low precision floats. */
using OVector3FM = OVector3<float, glm::mediump>;	/**< Three-dimensional vector of medium precision floats. */
using OVector3FH = OVector3<float, glm::highp>;		/**< Three-dimensional vector of high floats. */
using OVector3F = OVector3<float, glm::defaultp>;	/**< Three-dimensional vector of floats. */
using OVector3D = OVector3<double, glm::defaultp>;	/**< Three-dimensional vector of double precision floats. */

using OVector4I8 = OVector4<int8_t, glm::lowp>;		/**< Four-dimensional vector of signed 8-bit integers. */
using OVector4U8 = OVector4<uint8_t, glm::lowp>;	/**< Four-dimensional vector of unsigned 8-bit integers. */
using OVector4I16 = OVector4<int16_t, glm::mediump>;	/**< Four-dimensional vector of signed 32-bit integers. */
using OVector4U16 = OVector4<uint16_t, glm::mediump>;	/**< Four-dimensional vector of unsigned 32-bit integers. */
using OVector4I32 = OVector4<int32_t, glm::highp>;	/**< Four-dimensional vector of signed 32-bit integers. */
using OVector4U32 = OVector4<uint32_t, glm::highp>;	/**< Four-dimensional vector of unsigned 32-bit integers. */
using OVector4FL = OVector4<float, glm::lowp>;		/**< Four-dimensional vector of low precision floats. */
using OVector4FM = OVector4<float, glm::mediump>;	/**< Four-dimensional vector of medium precision floats. */
using OVector4FH = OVector4<float, glm::highp>;		/**< Four-dimensional vector of high floats. */
using OVector4F = OVector4<float, glm::defaultp>;	/**< Four-dimensional vector of floats. */
using OVector4D = OVector4<double, glm::defaultp>;	/**< Four-dimensional vector of double precision floats. */

