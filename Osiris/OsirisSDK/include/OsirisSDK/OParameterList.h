#pragma once

/**
 @brief Parameter list class.

 This class was designed to be used by OEntity class objects, in order to store an entity's list of
 parameters.

 Be aware that this class uses an internal vector to store the parameters, and the indeces must be 
 sequential.
 */
class OAPI OParameterList
{
public:
	/**
	 @brief Class constructor.
	 @param count Parameter count.
	 */
	OParameterList(int count=0);

	/**
	 @brief Class destructor.
	 */
	virtual ~OParameterList();

	/**
	 @brief Generic parameter sub-class.

	 This class will store an parameter inside the parameter list. The parameter necessarily
	 has a type and value. In case of strings the value is dinamically allocated. 
	 
	 If no type is assigned, objects will be initialized as Parameter:Unitialized. After a type
	 is assigned, the object will remain as the same time throughout it's entire lifetime. In case
	 a method related to a type other than the object's, an OException will be thrown.
	 */
	class OAPI Parameter {
	public:
		/**
		 @brief Class constructor.
		 */
		Parameter();

		/**
		 @brief Class copy constructor.
		 */
		Parameter(const Parameter& in);

		/**
		 @brief Class constructor, object initialized as boolean.
		 */
		Parameter(bool val);

		/**
		 @brief Class constructor, object initialized as integer.
		 */
		Parameter(int val);

		/**
		 @brief Class constructor, object initialized as float.
		 */
		Parameter(float val);

		/**
		 @brief Class constructor, object initialized as string.
		 */
		Parameter(const char* val);

		/**
		 @brief Class destructor. 
		 */
		virtual ~Parameter();

		/**
		 @brief Parameter types.
		 */
		enum Type {
			Uninitialized,	/**< Unitialized parameter, no type defined. */
			Boolean,	/**< Boolean parameter. */
			Integer,	/**< Integer parameter. */
			Float,		/**< Float parameter */
			String		/**< String parameter */
		};	

		/**
		 @brief Returns the parameter type.
		 */
		Type type() const;

		/**
		 @brief Value retrieval method for boolean parameters.
		 @throws OException In case of parameter type mismatch.
		 */
		bool boolVal() const;

		/**
		 @brief Value retrieval method for integer parameters.
		 @throws OException In case of parameter type mismatch.
		 */
		int intVal() const;

		/**
		 @brief Value retrieval method for float parameters.
		 @throws OException In case of parameter type mismatch.
		 */
		float floatVal() const;

		/**
		 @brief Value retrieval method for string parameters.
		 @throws OException In case of parameter type mismatch.
		 */
		const char* strVal() const;

		/**
		 @brief Value setting method for boolean parameters.
		 @throws OException In case of parameter type mismatch.
		 */
		void setVal(bool val);

		/**
		 @brief Value setting method for integer parameters.
		 @throws OException In case of parameter type mismatch.
		 */
		void setVal(int val);

		/**
		 @brief Value setting method for float parameters.
		 @throws OException In case of parameter type mismatch.
		 */
		void setVal(float val);

		/**
		 @brief Value setting method for string parameters.
		 @throws OException In case of parameter type mismatch.
		 */
		void setVal(const char* val);

		/**
		 @brief Assignment operator, copying another parameter. 
		 @throws OException In case of parameter type mismatch.
		 */
		Parameter& operator=(const Parameter& in);

		/**
		 @brief Assignment operator for boolean parameters.
		 @throws OException In case of parameter type mismatch.
		 */
		Parameter& operator=(bool in);

		/**
		 @brief Assignment operator for integer parameters.
		 @throws OException In case of parameter type mismatch.
		 */
		Parameter& operator=(int in);

		/**
		 @brief Assignment operator for float parameters.
		 @throws OException In case of parameter type mismatch.
		 */
		Parameter& operator=(float in);

		/**
		 @brief Assignment operator for string parameters.
		 @throws OException In case of parameter type mismatch.
		 */
		Parameter& operator=(const char* in);

	private:
		union {
			bool _bool;
			int _int;
			float _float;
			char* _str;
		} _value;

		Type _type;
	};

	/**
	 @brief Returns the number of parameters in the list.
	 */
	int count() const;

	/**
	 @brief Parameter fetch method.
	 @param idx Parameter index.
	 */
	Parameter& get(int idx);

	/**
	 @brief Parameter fetch operator.
	 @param idx Parameter index.
	 */
	Parameter& operator[](int idx);

private:
	Parameter* _attrs;
	int _attrCount;
};
