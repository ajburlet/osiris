#include <stdio.h>

#include "OsirisSDK/OException.h"

using namespace std;

#undef OException

const char* OException::what()
{
	char strLineNbr[16];

	snprintf(strLineNbr, 16, "%d", _line);

	_fullmsg = string("[") + _file + ": " + strLineNbr + "] " + _what;

	return _fullmsg.c_str();
}
