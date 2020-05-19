#pragma once

namespace OMath {
	constexpr float PI = 3.1415f; /**< PI approximate value. */

	/**
	 \brief Convert angle from degrees to radians.
	 */
	float deg2rad(float deg);

	/**
	 \brief Convert angle from radians to degrees.
	 */
	float rad2deg(float rad);

	/**
	 \brief Reverse sign of a float.
	 */
	float reverseSign(float in);
}

