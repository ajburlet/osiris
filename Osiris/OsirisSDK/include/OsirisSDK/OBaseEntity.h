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
	 */
	virtual void update(const OTimeIndex& timeIndex) = 0;

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

