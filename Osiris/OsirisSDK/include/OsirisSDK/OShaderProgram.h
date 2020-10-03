#pragma once

#include <string>
#include <list>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OGPUObject.h"
#include "OsirisSDK/OShaderObject.h" /* replace by a forward declaration */

/* REPLACE BY A CONCEALED IMPLEMENTATION */
#ifdef WIN32
#	pragma warning (disable : 4251) /* STL is restricted for class internal usage only */
#endif

/**
 \brief Class that represents a shader program.
*/
class OAPI OShaderProgram : public OGPUObject
{
public:
	/**
	 @brief Class default constructor.
	 */
	OShaderProgram();

	/**
	 @brief Class destructor.
	*/
	virtual ~OShaderProgram();

	/**
	 @brief Adds a new shader object.
	 @param aObject The new object to be added.
	 */
	void addObject(OShaderObject* aObject);

	/**
	 @brief Adds a preprocessor macro to all objects in the program.
	 @param aName Macro name.
	 @param aValue Macro value.
	 */
	void addPreprocessorMacro(const char* aName, const char* aValue = nullptr);
	
	/**
	 @brief Shader object iterator callback.
	 */
	using ObjectIteratorCb = std::function<void(OShaderObject*)>;

	/**
	 @brief Shader object iterator.
	 @param aCallback The function called for each preprocessor value.
	 */
	void ForEachObject(ObjectIteratorCb aCallback);

private:
	/**
	 @cond HIDDEN
	 */
	struct Implementation;
	Implementation* _impl	= nullptr;
	/**
	 @endcond
	 */
};

