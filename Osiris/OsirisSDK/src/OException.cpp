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

const char* OException::what() const
{
	char strLineNbr[16];
	string msg;

	snprintf(strLineNbr, 16, "%d", _line);

	msg = "[" + _file + ": " + strLineNbr + "] " + _what;

	return msg.c_str();
}
