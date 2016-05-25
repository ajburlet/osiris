#include <stdlib.h>

#include "OsirisSDK/OException.h"

using namespace std;

#undef OException

/**
 \brief Class constructor.
 \param file Source file name.
 \param line Source file line number.
 \param what Exception message.
*/
OException::OException(const char* file, int line, const char* what) :
	_file(file),
	_line(line),
	_what(what)
{
}

/**
 \brief Class destructor.
*/
OException::~OException()
{
}

/**
 \brief Returns the exception message.
 \return The exception message along with source file name and line number.
*/
const char* OException::what()
{
	char strLineNbr[16];

	snprintf(strLineNbr, 16, "%d", _line);

	_fullmsg = string("[") + _file + ": " + strLineNbr + "] " + _what;

	return _fullmsg.c_str();
}
