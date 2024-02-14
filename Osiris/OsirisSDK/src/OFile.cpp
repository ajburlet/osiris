#include <stdio.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include "OsirisSDK/OString.hpp"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OFile.h"

// ----------------------------------------------------------------------------
// Internal implementation
// ----------------------------------------------------------------------------
struct OFile::Impl : public OMemoryManagedObject<OSystemMemoryAllocator<OMemoryManager::Scope::Default>> 
{
	void copyFrom(const Impl& aOther);

	FILE*		fp		= nullptr;
	OString		filename;
	Mode		mode;
};

void OFile::Impl::copyFrom(const OFile::Impl& aOther) 
{
#if WIN32
	auto fd = _fileno(aOther.fp);
#else
	auto fd = fileno(aOther.fp);
#endif
	if (fd >= 0) {
#ifdef WIN32
		auto newfd = _dup(fd);
#else
		auto newfd = dup(fd);
#endif
#ifdef WIN32
		if (newfd < 0 || (fp = _fdopen(newfd, "r")) == nullptr) {
#else
		if (newfd < 0 || (fp = fdopen(newfd, "r")) == nullptr) {
#endif
			throw OException("Unable to duplicate file descriptor.");
		}
	}
	filename = aOther.filename;
	mode = aOther.mode;
}

// ----------------------------------------------------------------------------
// OFile 
// ----------------------------------------------------------------------------
OFile::OFile(const OString& aFilename, Mode aMode)
{
	OExceptionPointerCheck(_impl = new Impl);
	_impl->filename = aFilename;
	_impl->mode = aMode;

	open();
}

OFile::OFile(const OFile & aOther)
{
	OExceptionPointerCheck(_impl = new Impl);
	_impl->copyFrom(*aOther._impl);
}

OFile::~OFile()
{
	if (_impl != nullptr) {
		close();
		delete _impl;
	}
}

OFile & OFile::operator=(const OFile & aOther)
{
	if (_impl != nullptr) {
		close();
	}
	_impl->copyFrom(*aOther._impl);
	return *this;
}

OFile & OFile::operator=(OFile && aOther)
{
	if (_impl != nullptr) {
		close();
		delete _impl;
	}
 	_impl = aOther._impl;
	aOther._impl = nullptr;
	return *this;
}

void OFile::open()
{
	if (_impl->fp != nullptr) return;

	OString modestr;
	switch (_impl->mode) {
	case Mode::Read:	modestr = "rb";		break;
	case Mode::Write:	modestr = "wb";		break;
	case Mode::ReadWrite:	modestr = "wb+";	break;
	case Mode::Append:	modestr = "ab";		break;
	case Mode::ReadAppend:	modestr = "ab+";	break;
	}

#ifdef WIN32
	fopen_s(&_impl->fp, _impl->filename.cString(), modestr.cString());
#else
	_impl->fp = fopen(_impl->filename.cString(), modestr.cString());
#endif
	if (_impl->fp == nullptr) {
		auto errStr = OString::Fmt("Error opening file: %s", _impl->filename.cString());
		throw OException(errStr.cString());
	}
}

void OFile::close()
{
	if (_impl->fp == nullptr) return;
	fclose(_impl->fp);
	_impl->fp = nullptr;
}

OFile::Mode OFile::mode() const
{
	return _impl->mode;
}

bool OFile::eof() const
{
	return feof(_impl->fp);
}

const OString& OFile::filename() const
{
	return _impl->filename;
}

uint32_t OFile::read(char * aBuffer, uint32_t aBufferSize)
{
	switch (_impl->mode) {
	case Mode::Read:
	case Mode::ReadWrite:
	case Mode::ReadAppend:
		break;
	default:
		throw OException("Wrong file mode.");
	}
	return fread(aBuffer, aBufferSize, 1, _impl->fp);
}

uint32_t OFile::write(char * aBuffer, uint32_t aSize)
{
	switch (_impl->mode) {
	case Mode::Write:
	case Mode::Append:
		break;
	default:
		throw OException("Wrong file mode.");
	}
	return fwrite(aBuffer, aSize, 1, _impl->fp);
}

uint32_t OFile::readLine(char * aBuffer, uint32_t aBufferSize)
{
	switch (_impl->mode) {
	case Mode::Read:
	case Mode::ReadWrite:
	case Mode::ReadAppend:
		break;
	default:
		throw OException("Wrong file mode.");
	}
	if (fgets(aBuffer, aBufferSize, _impl->fp) == nullptr) {
		if (!eof()) throw OException("Unable to read line.");
		return 0;
	}
	return strlen(aBuffer);
}

void OFile::seekTo(int32_t aOffset, SeekReference aRef)
{
	int whence;
	switch (aRef) {
	case SeekReference::Start:	whence = SEEK_SET;	break;
	case SeekReference::Current:	whence = SEEK_CUR;	break;
	case SeekReference::End:	whence = SEEK_END;	break;
	}
	if (fseek(_impl->fp, aOffset, whence) != 0) {
		throw OException("Failed to move the file position indicator to the given value.");
	}
}

uint32_t OFile::currentPosition() const
{
	auto rv = ftell(_impl->fp);
	if (rv == -1) throw OException("Failed to determine current file position.");
	return static_cast<uint32_t>(rv);
}

