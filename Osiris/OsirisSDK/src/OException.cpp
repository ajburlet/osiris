#include <stdio.h>
#include <inttypes.h>

#include "OsirisSDK/OString.hpp"
#include "OsirisSDK/OException.h"

using namespace std;

OException::OException()
= default;

OException::OException(const char* aFile, uint32_t aLine, const char* aWhat)
{
	_content = new OString(OString::Fmt("[%s:%" PRIu32 "] %s", aFile, aLine, aWhat));
}

OException::OException(const char* aFile, uint32_t aLine, const OString& aWhat)
{
	_content = new OString(OString::Fmt("[%s:%" PRIu32 "] %s", aFile, aLine, aWhat.cString()));
}

OException::OException(const char* aFile, uint32_t aLine, const OException* aNested)
{
	_content = new OString(OString::Fmt("[%s:%" PRIu32 "] Forwarded exception.", aFile, aLine));
	_nested = new OException(*aNested);
}

OException::OException(const OException& aOther)
{
	_content = new OString(*aOther._content);
	_nested = new OException(*aOther._nested);
}

OException::~OException()
{
	if (_content != nullptr) {
		delete _content;
	}
	if (_nested != nullptr) {
		delete _nested;
	}
}

const char* OException::what() const
{
	return _content->cString();
}

void OException::toStdError() const
{
	uint32_t i=1;
	fprintf(stderr, "[0] %s\n", what());
	for (auto e = _nested; e != nullptr; e = e->_nested) {
		fprintf(stderr, "[%" PRIu32 "] %s\n", i++, e->what());
	}
}