#include "OsirisSDK/OEvent.h"

// ***********************************************************************
// OEvent
// ***********************************************************************
OEvent::OEvent(OEvent::EventType type) :
	_type(type)
{
}

OEvent::~OEvent()
{
}

OEvent::EventType OEvent::type() const
{
	return _type;
}

// ***********************************************************************
// OMemoryPoolEvent
// ***********************************************************************
OMemoryPoolEvent::OMemoryPoolEvent(OEvent::EventType type) : OEvent(type)
{
}

OMemoryPoolEvent::~OMemoryPoolEvent()
{
}
// ***********************************************************************
// OKeyboardPressEvent
// ***********************************************************************
OKeyboardPressEvent::OKeyboardPressEvent(KeyCode code, int mouse_x, int mouse_y) :
	OMemoryPoolEvent(OEvent::KeyboardPressEvent),
	_code(code),
	_mouse_x(mouse_x),
	_mouse_y(mouse_y)
{
}

OKeyboardPressEvent::~OKeyboardPressEvent()
{

}

OKeyboardPressEvent::KeyCode OKeyboardPressEvent::code() const
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
OMouseClickEvent::OMouseClickEvent(OMouseClickEvent::MouseButton btn, OMouseClickEvent::MouseStatus status, int x, int y) :
	OMemoryPoolEvent(OEvent::MouseClickEvent),
	_btn(btn),
	_status(status),
	_x(x),
	_y(y)
{
}

OMouseClickEvent::~OMouseClickEvent()
{
}

OMouseClickEvent::MouseButton OMouseClickEvent::button() const
{
	return _btn;
}

OMouseClickEvent::MouseStatus OMouseClickEvent::status() const
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
// OMouseClickEvent
// ***********************************************************************
OResizeEvent::OResizeEvent(int width, int height) :
	OMemoryPoolEvent(OEvent::ResizeEvent),
	_width(width),
	_height(height)
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

