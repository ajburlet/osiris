#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsAPI.h"


/**
 @brief OpenGL API implementation.
 */
class OAPI OOpenGL : public OGraphicsAPI
{
public:
	/**
	 @brief Class default constructor.
	 */
	OOpenGL();

	/**
	 @brief Class destructor.
	 */
	virtual ~OOpenGL() = default;


	virtual OGraphicsCommandQueue * createCommandQueue() override;

	virtual void compile(OShaderProgram * aProgram) override;
};