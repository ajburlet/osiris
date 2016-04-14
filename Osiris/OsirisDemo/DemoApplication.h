#pragma once

#include <OsirisSDK/OApplication.h>
#include <OsirisSDK/OMesh.h>

class DemoApplication :	public OApplication
{
public:
	DemoApplication(int argc=0, char **argv=NULL);
	virtual ~DemoApplication();

	// Inherited via OApplication
	virtual void init() override;
	virtual void update(int deltaTime_ms) override;

private:
	OMesh* _cube;
	OMatrixStack _mtx;
};

