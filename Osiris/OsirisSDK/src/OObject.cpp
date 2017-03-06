#include "OsirisSDK/OObject.h"
#include "OsirisSDK/OApplication.h"

OObject::OObject()
{
}

OObject::~OObject()
{
}

void OObject::scheduleDelete()
{
	OApplication::activeInstance()->scheduleDelete(this);
}

void OObject::processEvent(const OEvent * evt)
{
	switch (evt->type()) {
	case OEvent::KeyboardPressEvent:	onKeyboardPress((OKeyboardPressEvent*)evt);	break;
	case OEvent::MouseClickEvent:		onMouseClickEvent((OMouseClickEvent*)evt);	break;
	case OEvent::MouseActiveMoveEvent:
	case OEvent::MousePassiveMoveEvent:	onMouseMoveEvent((OMouseMoveEvent*)evt);	break;
	case OEvent::ResizeEvent:		onScreenResize((OResizeEvent*)evt);		break;
	}
}

void OObject::onKeyboardPress(const OKeyboardPressEvent * evt)
{
}

void OObject::onMouseClickEvent(const OMouseClickEvent * evt)
{
}

void OObject::onMouseMoveEvent(const OMouseMoveEvent * evt)
{
}

void OObject::onScreenResize(const OResizeEvent * evt)
{
}
