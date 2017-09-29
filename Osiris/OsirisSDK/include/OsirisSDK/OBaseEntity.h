#pragma once

#include "defs.h"
#include "OObject.h"
#include "OTimeIndex.h"
#include "ORenderObject.h"

/**
 @brief Base entity definition.
 */
class OAPI OBaseEntity : public OObject, public ORenderObject {
public:
	/**
	 @brief Class constructor.
	 */
	OBaseEntity();

	/**
	 @brief Class destructor.
	 */
	virtual ~OBaseEntity();

	/**
	 @brief Update method. 

	 This is meant to be called by OApplication class objects. This method should not me overriden, since
	 this must be handled by the behavior object.

	 @param timeIndex Time index.
	 @param step_us Simulation step in microseconds.
	 */
	virtual void update(const OTimeIndex& timeIndex, int step_us) = 0;

	/**
	 @brief Equalize state buffers.
	 */
	virtual void equalizeState() = 0;
	
	/**
	 @brief Executes final update call on entity state and swaps it.
	 @param timeIndex Time index.
	 @param step_us Simulation step in microseconds.
	 */
	virtual void swapState(const OTimeIndex& timeIndex, int step_us) = 0;

	/**
	 @brief Enables entity processing for each update call.
	 */
	void enable();

	/**
	 @brief Disables entity processing for each update call.
	 */
	void disable();

	/**
	 @brief Returns true if object is disabled to process update calls.
	 */
	bool isDisabled() const;

private:
	bool _disabled;
};

