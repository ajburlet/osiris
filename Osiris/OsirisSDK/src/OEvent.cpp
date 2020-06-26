#include "OsirisSDK/OEvent.h"

// ***********************************************************************
// OEvent
// ***********************************************************************
OEvent::OEvent(OEventType type) :
	_type(type)
{
}

OEvent::~OEvent()
{
}

OEventType OEvent::type() const
{
	return _type;
}

// ***********************************************************************
// OMemoryPoolEvent
// ***********************************************************************
OMemoryPoolEvent::OMemoryPoolEvent(OEventType type) : OEvent(type)
{
}

OMemoryPoolEvent::~OMemoryPoolEvent()
{
}


// ***********************************************************************
// OKeyboardPressEvent
// ***********************************************************************
OKeyboardPressEvent::OKeyboardPressEvent(OKeyCode aCode, int aMouseX, int aMouseY, bool aKeyPressed) :
	OMemoryPoolEvent((aKeyPressed) ? OEventType::KeyboardPressEvent : OEventType::KeyboardReleaseEvent),
	_code(aCode),
	_mouse_x(aMouseX),
	_mouse_y(aMouseY)
{
}

OKeyboardPressEvent::~OKeyboardPressEvent()
{

}

OKeyCode OKeyboardPressEvent::code() const
{
	return _code;
}

int OKeyboardPressEvent::mouseX() const
{
	return _mouse_x;
}

int OKeyboardPressEvent::mouseY() const
{
	return _mouse_y;
}

// ***********************************************************************
// OMouseClickEvent
// ***********************************************************************
OMouseClickEvent::OMouseClickEvent(OMouseButton aBtn, OMouseButtonStatus aStatus, int aX, int aY) :
	OMemoryPoolEvent(OEventType::MouseClickEvent),
	_btn(aBtn),
	_status(aStatus),
	_x(aX),
	_y(aY)
{
}

OMouseClickEvent::~OMouseClickEvent()
{
}

OMouseButton OMouseClickEvent::button() const
{
	return _btn;
}

OMouseButtonStatus OMouseClickEvent::status() const
{
	return _status;
}

int OMouseClickEvent::x() const
{
	return _x;
}

int OMouseClickEvent::y() const
{
	return _y;
}

// ***********************************************************************
// OMouseMoveEvent 
// ***********************************************************************
OMouseMoveEvent::OMouseMoveEvent(OMouseMovementType aType, int aX, int aY) :
	OMemoryPoolEvent((aType == OMouseMovementType::Active) ? OEventType::MouseActiveMoveEvent : OEventType::MousePassiveMoveEvent),
	_x(aX),
	_y(aY)
{
}

OMouseMoveEvent::~OMouseMoveEvent()
{
}

OMouseMovementType OMouseMoveEvent::movementType() const
{
	return _type;
}

int OMouseMoveEvent::x() const
{
	return _x;
}

int OMouseMoveEvent::y() const
{
	return _y;
}

// ***********************************************************************
// OResizeEvent
// ***********************************************************************
OResizeEvent::OResizeEvent(int aWidth, int aHeight) :
	OMemoryPoolEvent(OEventType::ResizeEvent),
	_width(aWidth),
	_height(aHeight)
{
}

OResizeEvent::~OResizeEvent()
{
}

int OResizeEvent::width() const
{
	return _width;
}

int OResizeEvent::height() const
{
	return _height;
}

