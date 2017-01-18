#include "OsirisSDK/OEvent.h"

// ***********************************************************************
// OEvent
// ***********************************************************************
/**
 \brief Class constructor.
 \param type Event type.
 */
OEvent::OEvent(OEvent::EventType type) :
	_type(type)
{
}

/**
 \brief Class destructor.
 */
OEvent::~OEvent()
{
}

/**
 \brief Returns event type.
 */
OEvent::EventType OEvent::type() const
{
	return _type;
}

// ***********************************************************************
// OKeyboardPressEvent
// ***********************************************************************
/**
 \brief Class constructor.
 \param code Key code.
 \param mouse_x Mouse position on the window on the X-axis.
 \param mouse_y Mouse position on the window on the Y-axis.
 */
OKeyboardPressEvent::OKeyboardPressEvent(KeyCode code, int mouse_x, int mouse_y) :
	OEvent(OEvent::KeyboardPressEvent),
	_code(code),
	_mouse_x(mouse_x),
	_mouse_y(mouse_y)
{
}

/**
 \brief Class destructor.
 */
OKeyboardPressEvent::~OKeyboardPressEvent()
{

}

/**
 \brief Key code.
 */
OKeyboardPressEvent::KeyCode OKeyboardPressEvent::code() const
{
	return _code;
}

/**
 \brief Mouse position on the window on the X-axis.
 */
int OKeyboardPressEvent::mouseX() const
{
	return _mouse_x;
}

/**
 \brief Mouse position on the window on the Y-axis.
 */
int OKeyboardPressEvent::mouseY() const
{
	return _mouse_y;
}

// ***********************************************************************
// OMouseClickEvent
// ***********************************************************************
/**
 \brief Class constructor.
 \param btn Mouse button that generated the event.
 \param status Status of the mouse button: released or pressed.
 \param x Window X-axis component in pixels.
 \param y Window Y-axis component in pixels.
 */
OMouseClickEvent::OMouseClickEvent(OMouseClickEvent::MouseButton btn, OMouseClickEvent::MouseStatus status, int x, int y) :
	OEvent(OEvent::MouseClickEvent),
	_btn(btn),
	_status(status),
	_x(x),
	_y(y)
{
}

/**
 \brief Class destructor.
 */
OMouseClickEvent::~OMouseClickEvent()
{
}

/**
 \brief Mouse button that generated the event.
 */
OMouseClickEvent::MouseButton OMouseClickEvent::button() const
{
	return _btn;
}

/**
 \brief Mouse button status: pressed or released.
 */
OMouseClickEvent::MouseStatus OMouseClickEvent::status() const
{
	return _status;
}

/**
 \brief Window X-axis component in pixels.
 */
int OMouseClickEvent::x() const
{
	return _x;
}

/**
 \brief Window Y-axis component in pixels.
 */
int OMouseClickEvent::y() const
{
	return _y;
}

// ***********************************************************************
// OMouseClickEvent
// ***********************************************************************
/**
 \brief Class constructor.
 \param width Window width in pixels.
 \param height Window height in pixels.
 */
OResizeEvent::OResizeEvent(int width, int height) :
	OEvent(OEvent::ResizeEvent),
	_width(width),
	_height(height)
{
}

/**
 \brief Class destructor.
 */
OResizeEvent::~OResizeEvent()
{
}

/**
 \brief Window width in pixels.
 */
int OResizeEvent::width() const
{
	return _width;
}

/**
 \brief Window height in pixels.
 */
int OResizeEvent::height() const
{
	return _height;
}
