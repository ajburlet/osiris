#pragma once

#include <glm/detail/qualifier.hpp>

template <typename T, glm::qualifier Q> class OBaseQuaternion;

using OQuaternion = OBaseQuaternion<float, glm::defaultp>;	/**< Default quaternion representation. */
