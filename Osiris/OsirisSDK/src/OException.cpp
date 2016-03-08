#include <stdlib.h>

#include "OsirisSDK/OException.h"

using namespace std;

#undef OException

OException::OException(const string& file, int line, const string& what) :
	_file(file),
	_line(line),
	_what(what)
{
}

OException::~OException()
{
}

string OException::what() const
{
	char strLineNbr[16];

	snprintf(strLineNbr, 16, "%d", _line);

	return "[" + _file + ": " + strLineNbr + "] " + _what;
}
