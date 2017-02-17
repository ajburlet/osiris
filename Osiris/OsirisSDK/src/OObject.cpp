#include "OsirisSDK/OObject.h"
#include "OsirisSDK/OApplication.h"

/**
 \brief Class constructor.
 */
OObject::OObject()
{
}

/**
 \brief Class destructor.
 */
OObject::~OObject()
{
}

/**
 \brief Schedule this object for deletion at the end of an application loop.
 */
void OObject::scheduleDelete()
{
	OApplication::activeInstance()->scheduleDelete(this);
}

/**
 \brief Main event handle.

 This is meant to be called by OApplication class objects. From here the specific event handlers are called. But can
 be overriden if necessary.

 \param evt Event class object.
 */
void OObject::processEvent(const OEvent * evt)
{
	switch (evt->type()) {
	case OEvent::KeyboardPressEvent:	onKeyboardPress((OKeyboardPressEvent*)evt);	break;
	case OEvent::MouseClickEvent:		onMouseClickEvent((OMouseClickEvent*)evt);	break;
	case OEvent::ResizeEvent:		onScreenResize((OResizeEvent*)evt);		break;
	}
}

/**
 \brief Keyboard press event handler.

 This handler is only called if the object is set as an event recipient. By default does nothing.

 \param evt Keyboard event object.
 */
void OObject::onKeyboardPress(const OKeyboardPressEvent * evt)
{
}

/**
 \brief Mouse click event handler.

 This handler is only called if the object is set as an event recipient. By default does nothing.

 \param evt Mouse click event object.
 */
void OObject::onMouseClickEvent(const OMouseClickEvent * evt)
{
}

/**
 \brief Screen resize event handler.

 This handler is only called if the object is set as an event recipient. By default does nothing.

 \param evt Screen resize event object.
 */
void OObject::onScreenResize(const OResizeEvent * evt)
{
}
