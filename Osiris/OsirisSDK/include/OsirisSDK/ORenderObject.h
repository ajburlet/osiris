#pragma once

#include "defs.h"

class OMatrixStack;

/**
 @brief Common interface for objects capable of rendering.
 */
class OAPI ORenderObject
{
public:
	/**
	 @brief Class constructor.
	 */
	ORenderObject();

	/**
	 @brief Class destructor.
	 */
	virtual ~ORenderObject();

	/**
	 @brief Enables rendering capability.
	 */
	void show();

	/**
	 @brief Disables rendering capability.
	 */
	void hide();

	/**
	 @brief Returns true if rendering capability is disabled.
	 */
	bool isHidden() const;

	virtual void render(OMatrixStack& stack) = 0;

private:
	bool _hidden;
};

