#pragma once

/**
 @brief Attribute list class.

 This class was designed to be used by OEntity class objects, in order to store an entity's list of
 attributes.

 Be aware that this class uses an internal vector to store the attributes, and the indeces must be 
 sequential.
 */
class OAttributeList
{
public:
	/**
	 @brief Class constructor.
	 @param count Attribute count.
	 */
	OAttributeList(int count=0);

	/**
	 @brief Class destructor.
	 */
	virtual ~OAttributeList();

	/**
	 @brief Generic attribute sub-class.

	 This class will store an attribute inside the attribute list. The attribute necessarily
	 has a type and value. In case of strings the value is dinamically allocated. 
	 
	 If no type is assigned, objects will be initialized as Attribute:Unitialized. After a type
	 is assigned, the object will remain as the same time throughout it's entire lifetime. In case
	 a method related to a type other than the object's, an OException will be thrown.
	 */
	class Attribute {
	public:
		/**
		 @brief Class constructor.
		 */
		Attribute();

		/**
		 @brief Class copy constructor.
		 */
		Attribute(const Attribute& in);

		/**
		 @brief Class constructor, object initialized as boolean.
		 */
		Attribute(bool val);

		/**
		 @brief Class constructor, object initialized as integer.
		 */
		Attribute(int val);

		/**
		 @brief Class constructor, object initialized as float.
		 */
		Attribute(float val);

		/**
		 @brief Class constructor, object initialized as string.
		 */
		Attribute(const char* val);

		/**
		 @brief Class destructor. 
		 */
		virtual ~Attribute();

		/**
		 @brief Attribute types.
		 */
		enum Type {
			Uninitialized,	/**< Unitialized attribute, no type defined. */
			Boolean,	/**< Boolean attribute. */
			Integer,	/**< Integer attribute. */
			Float,		/**< Float attribute */
			String		/**< String attribute */
		};	

		/**
		 @brief Returns the attribute type.
		 */
		Type type() const;

		/**
		 @brief Value retrieval method for boolean attributes.
		 @throws OException In case of attribute type mismatch.
		 */
		bool boolVal() const;

		/**
		 @brief Value retrieval method for integer attributes.
		 @throws OException In case of attribute type mismatch.
		 */
		int intVal() const;

		/**
		 @brief Value retrieval method for float attributes.
		 @throws OException In case of attribute type mismatch.
		 */
		float floatVal() const;

		/**
		 @brief Value retrieval method for string attributes.
		 @throws OException In case of attribute type mismatch.
		 */
		const char* strVal() const;

		/**
		 @brief Value setting method for boolean attributes.
		 @throws OException In case of attribute type mismatch.
		 */
		void setVal(bool val);

		/**
		 @brief Value setting method for integer attributes.
		 @throws OException In case of attribute type mismatch.
		 */
		void setVal(int val);

		/**
		 @brief Value setting method for float attributes.
		 @throws OException In case of attribute type mismatch.
		 */
		void setVal(float val);

		/**
		 @brief Value setting method for string attributes.
		 @throws OException In case of attribute type mismatch.
		 */
		void setVal(const char* val);

		/**
		 @brief Assignment operator, copying another attribute. 
		 @throws OException In case of attribute type mismatch.
		 */
		Attribute& operator=(const Attribute& in);

		/**
		 @brief Assignment operator for boolean attributes.
		 @throws OException In case of attribute type mismatch.
		 */
		Attribute& operator=(bool in);

		/**
		 @brief Assignment operator for integer attributes.
		 @throws OException In case of attribute type mismatch.
		 */
		Attribute& operator=(int in);

		/**
		 @brief Assignment operator for float attributes.
		 @throws OException In case of attribute type mismatch.
		 */
		Attribute& operator=(float in);

		/**
		 @brief Assignment operator for string attributes.
		 @throws OException In case of attribute type mismatch.
		 */
		Attribute& operator=(const char* in);

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
	 @brief Returns the number of attributes in the list.
	 */
	int count() const;

	/**
	 @brief Attribute fetch method.
	 @param idx Attribute index.
	 */
	Attribute& get(int idx);

	/**
	 @brief Attribute fetch operator.
	 @param idx Attribute index.
	 */
	Attribute& operator[](int idx);

private:
	Attribute* _attrs;
	int _attrCount;
};
