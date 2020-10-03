#include "OsirisSDK/OException.h"
#include "OsirisSDK/OVector.hpp"
#include "OsirisSDK/OMatrix.hpp"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/ORenderingEngine.h"
#include "OsirisSDK/OMatrixStack.h"

#include "OsirisSDK/OMesh.h"

#include <stdio.h>

inline void OMesh::render(ORenderingEngine * aRenderingEngine, OMatrixStack * aMatrixStack)
{
	setMatrixStack(aMatrixStack);
	aRenderingEngine->render(this);
}
