#pragma once

#include <map>

#include "defs.h"
#include "OEvent.h"

/**
 \brief Base OsirisSDK class.

 Objects of this class are tied to the running application. They can be subscribed to listen to
 events. Their deletion can be handled by the application.

 */
class OAPI OObject
{
public:
	OObject();
	virtual ~OObject();

	void scheduleDelete();
	virtual void processEvent(const OEvent* evt);

	virtual void onKeyboardPress(const OKeyboardPressEvent* evt);
	virtual void onMouseClickEvent(const OMouseClickEvent* evt);
	virtual void onResizeEvent(const OResizeEvent* evt);

};
