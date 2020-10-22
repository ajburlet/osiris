#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OGPUObject.h"

class OShaderObject;

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
	 @brief Deleted copy constructor.
	 */
	OShaderProgram(const OShaderProgram& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OShaderProgram(OShaderProgram&& aOther);

	/**
	 @brief Class destructor.
	*/
	virtual ~OShaderProgram();

	/**
	 @brief Deleted copy assignment operator.
	 */
	OShaderProgram& operator=(const OShaderProgram& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OShaderProgram& operator=(OShaderProgram&& aOther);

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
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
};

inline OShaderProgram::OShaderProgram(OShaderProgram&& aOther)
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}
