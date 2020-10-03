#pragma once

#include <functional>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGPUObject.h"

/**
 @brief Class that represents a shader object that make up shader programs.
*/
class OAPI OShaderObject : public OGPUObject
{
public:
	/**
	 @brief Shader object type.
	 */
	enum class Type : uint8_t {
		Vertex,		/**< Vertex shader. */
		Fragment,	/**< Fragment/pixel shader. */
		Compute		/**< Compute/kernel shader. */
	};

	/**
	 @brief Class constructor.
	 @param aType Shader type.
	 @param aSource Shader source code.
	 */
	OShaderObject(Type aType, const char* aSource="");
	
	/**
	 @brief Class destructor.
	 */
	virtual ~OShaderObject();
	
	/**
	 @brief Set shader source code.
	 */
	void setSource(const char* source);

	/**
	 @brief Returns the shader source code.
	 */
	const char* source() const;

	/**
	 @brief Returns the shader type.
	 */
	Type type() const;

	/**
	 @brief Sets a preprocessor macro.
	 @param aName Macro name.
	 @param aValue Macro value.
	 */
	void addPreprocessorMacro(const char *aName, const char* aValue=nullptr);

	/**
	 @brief Pre-processor iterator callback.
	 The first item carries the macro name, and the second its value.
	 */
	using PreprocessorIteratorCb = std::function<void(const char*, const char*)>;

	/**
	 @brief Preprocessor iterator.
	 @param aCallback The function called for each preprocessor value.
	 */
	void ForEachPreprocessorMacro(PreprocessorIteratorCb aCallback);

protected:
	/**
	 @cond HIDDEN
	 */
	struct Implementation;
	Implementation* _impl = nullptr;
	/**
	 @endcond
	 */
};

