#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsAPI.h"

/**
 @brief OpenGL API implementation.
 */
class OAPI OOpenGL : public OGraphicsAPI
{
public:
	virtual OGraphicsCommandQueue * createCommandQueue() override;

	virtual void compile(OShaderProgram * aProgram) override;
};