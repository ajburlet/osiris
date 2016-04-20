#include <stdlib.h>

#include "OsirisSDK/OException.h"

using namespace std;

#undef OException

OException::OException(const char* file, int line, const char* what) :
	_file(file),
	_line(line),
	_what(what)
{
}

OException::~OException()
{
}

const char* OException::what()
{
	char strLineNbr[16];

	snprintf(strLineNbr, 16, "%d", _line);

	_fullmsg = string("[") + _file + ": " + strLineNbr + "] " + _what;

	return _fullmsg.c_str();
}
