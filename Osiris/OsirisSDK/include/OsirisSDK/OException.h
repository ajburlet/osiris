#pragma once

#include <string>

class OException
{
public:
	OException(const std::string &file, int line, const std::string &what);
	virtual ~OException();

	std::string what() const;
private:
	std::string _file;
	int _line;
	std::string _what;
};

#define OException(what) OException(__FILE__, __LINE__, what)
