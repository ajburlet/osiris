#pragma once

#include <string>

#include "defs.h"

#ifdef WIN32
#	pragma warning (disable : 4251) /* std::string is encapsulated inside the class */
#endif

/**
 @brief Osiris exception class.
*/
class OAPI OException
{
public:
	/**
	 @brief Class constructor.
	 @param aFile Source file name.
	 @param aLine Source file line number.
	 @param aWhat Exception message.
	*/
	OException(const char* aFile, int aLine, const char* aWhat) :
		_file(aFile),
		_line(aLine),
		_what(aWhat)
	{}
	
	/**
	 @brief Class destructor.
	*/
	virtual ~OException() = default;

	/**
	 @brief Returns the exception message.
	 @return The exception message along with source file name and line number.
	*/
	const char* what();

private:
	std::string _file;
	int _line;
	std::string _what;
	std::string _fullmsg;
};

/**
 @brief Simplified exception constructor macro.

 Using this macro dispeses the need to enter source file name and line number.

 @param what Exception message.
*/
#define OException(what) OException(__FILE__, __LINE__, what)

/**
 @brief Checks if pointer is null, if it is throws an exception.
 @param ptr The pointer to be checked.
 */
#define OExceptionPointerCheck(ptr) if ((ptr) == nullptr) throw OException("Null pointer exception.")

/**
 @brief Checks if pointer is null, if it is calls a callback function and throws an exception.
 @param ptr The pointer to be checked.
 @param cb The callback function to be called prior to the exception.
 */
#define OExceptionPointerCheckCb(ptr, cb) if ((ptr) == nullptr) {cb(); throw OException("Null pointer exception."); }

/**
 @brief Forwards exception.
 */
#define OExceptionForward(code_block) try {code_block} catch (OException& e) {throw e;}

/**
 @brief Fowards an exception and executes a callback error handler.
 */
#define OExceptionForwardCb(cb, code_block) try {code_block} catch (OException& e) {cb(); throw e; }
