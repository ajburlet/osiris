#pragma once

#include "OsirisSDK/defs.h"

/**
 @brief Sets which face will be rendered when face culling is available.
*/
enum class OCullFace {
	Undefined=-1,	/**< Undefined face. */
	Front,		/**< Front face. */
	Back		/**< Back face. */
};


/**
 @brief In order to be able to ascertain which face is the front when dealing with
	face culling, we use the order in which the vertices are laid on each triangle.
*/
enum class OCullFront {
	Undefined=-1,	/**< Undefined front face. */
	CW,		/**< Front face is defined by clockwise vertex order. */
	CCW		/**< Front face is defined by counter-clockwise vertex order. */
};
	
/**
 @brief Variable type.
 */
enum class OVarType {
	Undefined,
	Float,
	Int,
	UnsignedInt,
	Float2x2,
	Float3x3,
	Float4x4,
	Int2x2,
	Int3x3,
	Int4x4
};

/**
 @brief Variable precision.
 */
enum class OVarPrecision {
	Undefined,
	Low,		/**< Low precision, 16-bit floats or 8-bit integers. */
	Medium,		/**< Medium precision, 32-bit floats or 16-bit integers. */
	High		/**< High precision, 64-bit floats or 32-bit integers. */
};

/**
 @brief Rendering type, combination of the face primitives and how they are built.
 */
enum class ORenderMode {
	Undefined,
	Triangle,		/**< Sets of three vertices that makes a triangle. */
	TriangleStrip,		/**< Each successive sequence of three vertices makes a triangle. */
	IndexedTriangle		/**< Each triangle is defined by three indexes referring to vertices on the vertex buffer. */
};
