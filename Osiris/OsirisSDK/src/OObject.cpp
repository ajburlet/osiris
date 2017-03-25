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
	case OEvent::MouseClickEvent:		onMouseClick((OMouseClickEvent*)evt);		break;
	case OEvent::MouseActiveMoveEvent:
	case OEvent::MousePassiveMoveEvent:	onMouseMove((OMouseMoveEvent*)evt);		break;
	case OEvent::ResizeEvent:		onScreenResize((OResizeEvent*)evt);		break;
	}
}

void OObject::onKeyboardPress(const OKeyboardPressEvent * evt)
{
}

void OObject::onMouseClick(const OMouseClickEvent * evt)
{
}

void OObject::onMouseMove(const OMouseMoveEvent * evt)
{
}

void OObject::onScreenResize(const OResizeEvent * evt)
{
}
