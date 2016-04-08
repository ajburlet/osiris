#pragma once

#include <OsirisSDK/OApplication.h>

class DemoApplication :	public OApplication
{
public:
	DemoApplication();
	virtual ~DemoApplication();

	// Inherited via OApplication
	virtual void init() override;
	virtual void update(int deltaTime_ms) override;
};

