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
	/**
	 \brief Class constructor.
	 */
	OObject();
	
	/**
	 \brief Class destructor.
	 */
	virtual ~OObject();

	/**
	 \brief Schedule this object for deletion at the end of an application loop.
	 */
	void scheduleDelete();
	
	/**
	 \brief Main event handle.

	 This is meant to be called by OApplication class objects. From here the specific event handlers are called. But can
	 be overriden if necessary.

	 \param evt Event class object.
	 */
	virtual void processEvent(const OEvent* evt);

	/**
	 \brief Keyboard press event handler.

	 This handler is only called if the object is set as an event recipient. By default does nothing.

	 \param evt Keyboard event object.
	 */
	virtual void onKeyboardPress(const OKeyboardPressEvent* evt);
	
	/**
	 \brief Mouse click event handler.

	 This handler is only called if the object is set as an event recipient. By default does nothing.

	 \param evt Mouse click event object.
	 */
	virtual void onMouseClickEvent(const OMouseClickEvent* evt);
	
	/**
	 \brief Screen resize event handler.

	 This handler is only called if the object is set as an event recipient. By default does nothing.

	 \param evt Screen resize event object.
	 */
	virtual void onScreenResize(const OResizeEvent* evt);

};
