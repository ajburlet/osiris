#pragma once

#include <stdint.h>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OStringDefs.h"

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
	 @brief Default class constructor. 
	 */
	OException();

	/**
	 @brief Class constructor.
	 @param aFile Source file name.
	 @param aLine Source file line number.
	 @param aWhat Exception message.
	 @param aNested Nested exception.
	*/
	OException(const char* aFile, uint32_t aLine, const char* aWhat); 

	/**
	 @copydoc OException(const char*, uint32_t, const char*, OException*)
	 */
	OException(const char* aFile, uint32_t aLine, const OString& aWhat);

	/**
	 @brief Nested exception constructor. 
	 @param aFile Source file name.
	 @param aLine Source file line number.
	 @param aNested Nested exception.
	 */
	OException(const char* aFile, uint32_t aLine, const OException* aNested);

	/**
	 @brief Copy constructor.
	 */
	OException(const OException& aOther);
	
	/**
	 @brief Class destructor.
	*/
	virtual ~OException();

	
	/**
	 @brief Returns the exception message.
	 @return The exception message along with source file name and line number.
	*/
	const char* what() const;

	/**
	 @brief Nested exception, if any.
	 */
	OException* nested() const;

	/**
	 @brief Dump message to standard error output. 
	 */
	void toStdError() const;

private:
	OString* _content = nullptr;
	OException* _nested = nullptr;
};

inline OException* OException::nested() const
{
	return _nested;
}

/**
 @brief Simplified exception constructor macro.

 Using this macro dispeses the need to enter source file name and line number.

 @param what Exception message.
*/
#define OEx(what) OException(__FILE__, __LINE__, what)

/**
 @brief Checks if pointer is null, if it is throws an exception.
 @param ptr The pointer to be checked.
 */
#define OExPointerCheck(ptr) if ((ptr) == nullptr) throw OEx("Null pointer exception.")

/**
 @brief Checks if pointer is null, if it is calls a callback function and throws an exception.
 @param ptr The pointer to be checked.
 @param cb The callback function to be called prior to the exception.
 */
#define OExPointerCheckCb(ptr, cb) if ((ptr) == nullptr) {cb(); throw OEx("Null pointer exception."); }

/**
 @brief Forwards exception.
 */
#define OExForward(code_block) try {code_block} catch (OException& e) { throw OEx(&e); }

/**
 @brief Fowards an exception and executes a callback error handler.
 */
#define OExForwardCb(cb, code_block) try {code_block} catch (OException& e) {cb(); throw OEx(&e); }
