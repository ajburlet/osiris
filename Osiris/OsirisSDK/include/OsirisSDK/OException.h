#pragma once

#include <string>

#include "defs.h"

#ifdef WIN32
#	pragma warning (disable : 4251) /* std::string is encapsulated inside the class */
#endif

class OAPI OException
{
public:
	OException(const char* file, int line, const char* what);
	virtual ~OException();

	const char* what() const;
private:
	std::string _file;
	int _line;
	std::string _what;
};

#define OException(what) OException(__FILE__, __LINE__, what)
