#include <OsirisSDK/ODoubleBuffer.hpp>
#include <OsirisSDK/OState.h>
#include <OsirisSDK/OParameterList.h>

#include "PieceBehavior.h"



PieceBehavior::PieceBehavior()
{
}


PieceBehavior::~PieceBehavior()
{
}

void PieceBehavior::update(OParameterList ** attribute, ODoubleBuffer<OState>* state, OMesh ** meshPtr, 
			   const OTimeIndex & timeIndex, int step_us)
{
	if (state->curr()->position().x() <= (**attribute)[attrMinX].floatVal())
		state->next()->motionComponent(1).setX(fabs(state->curr()->motionComponent(1).x()));
	if (state->curr()->position().x() >= (**attribute)[attrMaxX].floatVal())
		state->next()->motionComponent(1).setX(-fabs(state->curr()->motionComponent(1).x()));
	if (state->curr()->position().z() <= (**attribute)[attrMinZ].floatVal())
		state->next()->motionComponent(1).setZ(fabs(state->curr()->motionComponent(1).z()));
	if (state->curr()->position().z() >= (**attribute)[attrMaxZ].floatVal())
		state->next()->motionComponent(1).setZ(-fabs(state->curr()->motionComponent(1).z()));
}
