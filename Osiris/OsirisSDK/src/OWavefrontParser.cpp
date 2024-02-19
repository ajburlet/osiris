#include <string.h>
#include <stdlib.h>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OFile.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OGraphicsAllocators.h"
#include "OsirisSDK/OWavefrontParser.h"

#ifndef OWAVEFRONTPARSER_LINEBUFFER
#define OWAVEFRONTPARSER_LINEBUFFER		128
#endif

#ifndef OWAVEFRONTPARSER_WORDBUFFER
#define OWAVEFRONTPARSER_WORDBUFFER		64
#endif

struct OWavefrontParser::Impl : public OMemoryManagedObject<OGraphicsAllocators::Default> {
	Impl(OFile* aFile) : file(aFile) {}

	OFile*		file		= nullptr;
	uint32_t	currLine	= 0;
	char*		wordptr		= nullptr;
	bool		endOfLine	= false;
	char lineBuffer[OWAVEFRONTPARSER_LINEBUFFER];
	char wordBuffer[OWAVEFRONTPARSER_WORDBUFFER];
};

OWavefrontParser::OWavefrontParser(OFile * aFile)
{
	OExPointerCheck(_impl = new Impl(aFile));
	memset(_impl->lineBuffer, 0, OWAVEFRONTPARSER_LINEBUFFER);
}

OWavefrontParser::~OWavefrontParser()
{
	if (_impl != nullptr) delete _impl;
}

int OWavefrontParser::readNextLine()
{
	if (_impl->file->readLine(_impl->lineBuffer, OWAVEFRONTPARSER_LINEBUFFER) == 0) {
		_impl->wordptr = NULL;
		return 1;
	}
	_impl->wordptr = _impl->lineBuffer;
	_impl->endOfLine = false;
	_impl->currLine++;
	return 0;
}

char * OWavefrontParser::readNextWord()
{
	char *p = _impl->wordptr;

	/* if end of line has been reached, point to the beginning of the line and return NULL. */
	if (_impl->endOfLine) {
		_impl->wordptr = _impl->lineBuffer;
		_impl->endOfLine = false;
		return NULL;
	}

	/* if no space is found or end of line chars, means end of line */
	if ((p = strchr(_impl->wordptr, ' ')) == NULL && ((p = strchr(_impl->wordptr, '\n')) != NULL || 
	    (p = strchr(_impl->wordptr, '\r')) != NULL || (p = strchr(_impl->wordptr, '\0')) != NULL) ) {
		_impl->endOfLine = true;
	}

	*_impl->wordBuffer = '\0';

#ifdef WIN32
	strncpy_s(_impl->wordBuffer, (rsize_t)OWAVEFRONTPARSER_WORDBUFFER, _impl->wordptr, (size_t)(p-_impl->wordptr));
#else
	strncpy(_impl->wordBuffer, _impl->wordptr, (size_t)(p - _impl->wordptr));
#endif
	if (!_impl->endOfLine) _impl->wordptr = p + 1;
	return _impl->wordBuffer;
}

float OWavefrontParser::readNextFloat()
{
	auto word = readNextWord();
	if (word == nullptr) throw OEx("Expected float to be read not found.");
	return static_cast<float>(atof(word));
}

int32_t OWavefrontParser::readNextInt()
{
	auto word = readNextWord();
	if (word == nullptr) throw OEx("Expected integer to be read not found.");
	return atoi(word);
}

uint32_t OWavefrontParser::readNextUint()
{
	auto word = readNextWord();
	if (word == nullptr) throw OEx("Expected integer to be read not found.");
	return static_cast<uint32_t>(atoi(word));
}

bool OWavefrontParser::endOfLine() const
{
	return _impl->endOfLine;
}

unsigned int OWavefrontParser::currLine() const
{
	return _impl->currLine;
}

