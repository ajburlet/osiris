#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OMemoryManagedObject.h"

/**
 @brief Base file class.
 */
class OAPI OFile : public OMemoryManagedObject<OSystemMemoryAllocator<OMemoryManager::Scope::Default>>
{
public:
	/**
	 @brief file operating modes.
	 */
	enum class Mode {
		Read,
		Write,
		ReadWrite,
		Append,
		ReadAppend
	};

	/**
	 @brief Class constructor.
	 @param aFilename File name.
	 @param aMode File operating mode. 
	 */
	OFile(const char* aFilename, Mode aMode);

	/**
	 @brief Copy constructor.
	 */
	OFile(const OFile& aOther);

	/**
	 @brief Move constructor.
	 */
	OFile(OFile&& aOther);
	
	/**
	 @brief Class destructor.
	 */
	virtual ~OFile();

	/**
	 @brief Copy assignment operator.
	 */
	OFile& operator=(const OFile& aOther);

	/**
	 @brief Move assignment operator.
	 */
	OFile& operator=(OFile&& aOther);

	/**
	 @brief Open the file.
	 */
	virtual void open();

	/**
	 @brief Close the file.
	 */
	virtual void close();

	/**
	 @brief Returns the file name.
	 */
	const char* filename() const;

	/**
	 @brief Returns the file operation mode.
	 */
	Mode mode() const;

	/**
	 @brief Returns true if at end of the file.
	 */
	bool eof() const;

	/**
	 @brief Reads from the file.
	 @param aBuffer Target buffer.
	 @param aBufferSize Target buffer size.
	 @return Number of bytes read.
	 */
	uint32_t read(char* aBuffer, uint32_t aBufferSize);

	/**
	 @brief Writes to the file.
	 @param aBuffer Target buffer.
	 @param aSize Amount of data to be written.
	 @return Number of bytes actually written.
	 */
	uint32_t write(char* aBuffer, uint32_t aSize);

	/**
	 @brief Reads a line, i.e. until buffer is depleted or a line break is found.
	 @param aBuffer Target buffer.
	 @param aBufferSize Target buffer size.
	 @return Number of bytes read.
	 */
	uint32_t readLine(char* aBuffer, uint32_t aBufferSize);

	/**
	 @brief Seek offset reference.
	 */
	enum class SeekReference {
		Start,		/**< Start of the file. */
		Current,	/**< Current position. */
		End		/**< End of the file. */
	};

	/**
	 @brief Changes the file position indicator.
	 */
	void seekTo(int32_t aOffset, SeekReference aRef = SeekReference::Start);

	/**
	 @brief Returns the current position in the file.
	 */
	uint32_t currentPosition() const;

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
};

inline OFile::OFile(OFile && aOther)
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}

