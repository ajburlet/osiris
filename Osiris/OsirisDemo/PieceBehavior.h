#pragma once

#include <OsirisSDK/OBehavior.h>

class PieceBehavior : public OBehavior
{
public:
	PieceBehavior();
	virtual ~PieceBehavior();

	enum {
		attrMinX=0,
		attrMaxX,
		attrMinZ,
		attrMaxZ
	};

	virtual void update(OParameterList ** attribute, 
			    ODoubleBuffer<OState>* state, 
			    OMesh ** meshPtr, 
			    const OTimeIndex & timeIndex, 
			    int step_us) override;
};

