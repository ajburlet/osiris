#include "OsirisSDK/OApplication.h"
#include "OsirisSDK/OTrashBin.h"

#include "OsirisSDK/OObject.h"

OObject::OObject()
{
}

OObject::~OObject()
{
}

void OObject::scheduleDelete()
{
	OApplication::activeInstance()->trashBin().trash(this);
}

void OObject::processEvent(const OEvent * evt)
{
	switch (evt->type()) {
	case OEventType::KeyboardPressEvent:	onKeyboardPress((OKeyboardPressEvent*)evt);	break;
	case OEventType::KeyboardReleaseEvent:	onKeyboardRelease((OKeyboardPressEvent*)evt);	break;
	case OEventType::MouseClickEvent:	onMouseClick((OMouseClickEvent*)evt);		break;
	case OEventType::MouseActiveMoveEvent:
	case OEventType::MousePassiveMoveEvent:	onMouseMove((OMouseMoveEvent*)evt);		break;
	case OEventType::ResizeEvent:		onScreenResize((OResizeEvent*)evt);		break;
	}
}

void OObject::onKeyboardPress(const OKeyboardPressEvent * evt)
{
}

void OObject::onKeyboardRelease(const OKeyboardPressEvent * evt)
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
