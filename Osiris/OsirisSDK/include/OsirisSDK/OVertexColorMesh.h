#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OMesh.h"
#include "OsirisSDK/OVectorDefs.h"

/**
 \brief Mesh class in which colors can be defined for each vertex.

 The surface color of each triangle will be an interpolation between the values set
 for each vertex.
*/
class OAPI OVertexColorMesh : public OMesh
{
public:
	/**
	 \brief Class constructor.
	*/
	OVertexColorMesh();

	/**
	 \brief Class destructor.
	*/
	virtual ~OVertexColorMesh();

	/**
	 \brief Add color definition values for each vertex. 
	 
	 The colors should be set after all vertex where defined.

	 \param r Red component value.
	 \param g Green component value.
	 \param b Blue component value.
	 \param alpha Alpha channel component value.
	*/
	void addVertexColorData(float r, float g, float b, float alpha);

	/**
	 \brief Add color definition values for each vertex. 
	 
	 The colors should be set after all vertex where defined.

	 \param colorVec Four-dimentional vector containing color information in the (r, g, b, alpha) format.
	 */
	void addVertexColorData(const OVector4F& colorVec);

protected:
	/* Inherited from OMesh */
	void setupAdditionalVertexArrays();

private:
	static OShaderProgram *_colorVertexProgram;
};

