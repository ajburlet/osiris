#pragma once

#include <glm/detail/qualifier.hpp>

template <typename, glm::qualifier> class OMatrix4x4;


using OMatrix4x4I8 = OMatrix4x4<int8_t, glm::lowp>;		/**< Matrix 4x4 of signed 8-bit integers. */
using OMatrix4x4U8 = OMatrix4x4<uint8_t, glm::lowp>;		/**< Matrix 4x4 of unsigned 8-bit integers. */
using OMatrix4x4I16 = OMatrix4x4<int16_t, glm::mediump>;	/**< Matrix 4x4 of signed 32-bit integers. */
using OMatrix4x4U16 = OMatrix4x4<uint16_t, glm::mediump>;	/**< Matrix 4x4 of unsigned 32-bit integers. */
using OMatrix4x4I32 = OMatrix4x4<int32_t, glm::highp>;		/**< Matrix 4x4 of signed 32-bit integers. */
using OMatrix4x4U32 = OMatrix4x4<uint32_t, glm::highp>;		/**< Matrix 4x4 of unsigned 32-bit integers. */
using OMatrix4x4FL = OMatrix4x4<float, glm::lowp>;		/**< Matrix 4x4 of low precision floats. */
using OMatrix4x4FM = OMatrix4x4<float, glm::mediump>;		/**< Matrix 4x4 of medium precision floats. */
using OMatrix4x4FH = OMatrix4x4<float, glm::highp>;		/**< Matrix 4x4 of high floats. */
using OMatrix4x4F = OMatrix4x4<float, glm::defaultp>;		/**< Matrix 4x4 of floats. */
using OMatrix4x4D = OMatrix4x4<double, glm::defaultp>;		/**< Matrix 4x4 of double precision floats. */
