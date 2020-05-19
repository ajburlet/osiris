#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "OsirisSDK/OMath.h"
#include "OsirisSDK/OException.h"

// ****************************************************************************
// OMath functions 
// ****************************************************************************
float OMath::deg2rad(float deg)
{
	return 2 * PI * deg / 360.0f;
}

float OMath::rad2deg(float rad)
{ 
	return 360.0f * rad / (2 * PI); 
}

float OMath::reverseSign(float in)
{
	return -abs(in)/in;
}

