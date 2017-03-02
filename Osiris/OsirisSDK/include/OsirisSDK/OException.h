#pragma once

#include <string>

#include "defs.h"

#ifdef WIN32
#	pragma warning (disable : 4251) /* std::string is encapsulated inside the class */
#endif

/**
 \brief Osiris exception class.
*/
class OAPI OException
{
public:
	/**
	 \brief Class constructor.
	 \param file Source file name.
	 \param line Source file line number.
	 \param what Exception message.
	*/
	OException(const char* file, int line, const char* what);
	
	/**
	 \brief Class destructor.
	*/
	virtual ~OException();

	/**
	 \brief Returns the exception message.
	 \return The exception message along with source file name and line number.
	*/
	const char* what();
private:
	std::string _file;
	int _line;
	std::string _what;
	std::string _fullmsg;
};

/**
 \brief Simplified exception constructor macro.

 Using this macro dispeses the need to enter source file name and line number.

 \param what Exception message.
*/
#define OException(what) OException(__FILE__, __LINE__, what)
