#include "OsirisSDK/OEvent.h"

#include "OsirisSDK/OBehavior.h"

void OBehavior::processEvent(OParameterList ** attribute, ODoubleBuffer<OState>* state, const OEvent * event)
{
	switch (event->type()) {
	case OEvent::KeyboardPressEvent:	onKeyboardPress(attribute, state, (const OKeyboardPressEvent*)event);	break;
	case OEvent::KeyboardReleaseEvent:	onKeyboardRelease(attribute, state, (const OKeyboardPressEvent*)event);	break;
	case OEvent::MouseClickEvent:		onMouseClick(attribute, state, (const OMouseClickEvent*)event);		break;
	case OEvent::MouseActiveMoveEvent:
	case OEvent::MousePassiveMoveEvent:	onMouseMove(attribute, state, (const OMouseMoveEvent*)event);		break;
	case OEvent::ResizeEvent:		onScreenResize(attribute, state, (const OResizeEvent*)event);		break;
	}
}

void OBehavior::onKeyboardPress(OParameterList ** attribute, ODoubleBuffer<OState>* state, const OKeyboardPressEvent * evt)
{
	/* by default do nothing */
}

void OBehavior::onKeyboardRelease(OParameterList ** attribute, ODoubleBuffer<OState>* state, const OKeyboardPressEvent * evt)
{
	/* by default do nothing */
}

void OBehavior::onMouseClick(OParameterList ** attribute, ODoubleBuffer<OState>* state, const OMouseClickEvent * evt)
{
	/* by default do nothing */
}

void OBehavior::onMouseMove(OParameterList ** attribute, ODoubleBuffer<OState>* state, const OMouseMoveEvent * evt)
{
	/* by default do nothing */
}

void OBehavior::onScreenResize(OParameterList ** attribute, ODoubleBuffer<OState>* state, const OResizeEvent * evt)
{
	/* by default do nothing */
}
