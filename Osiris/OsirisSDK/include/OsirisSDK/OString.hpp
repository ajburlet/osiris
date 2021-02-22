#pragma once

#include <string>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OStringDefs.h"

/**
 @brief Base string template wrapper class.
 */
template <typename CharT, typename Allocator>
class OBaseString : protected std::basic_string<char, std::char_traits<CharT>, OSTLAllocator<Allocator, CharT>>
{
private:
	using Super = std::basic_string<char, std::char_traits<CharT>, OSTLAllocator<Allocator, CharT>>;

public:
	/**
	 @brief String iterator.
	 */
	using Iterator = Super::iterator;

	/**
	 @brief String constant iterator.
	 */
	using ConstIterator = Super::const_iterator;

	/**
	 @brief Constant integer to denote no position.
	 */
	static constexpr auto NoPosition = basic_string::npos;

	/**
	 @brief Default class constructor.
	 */
	OBaseString() = default;

	/**
	 @brief Class constructor.
	 */
	OBaseString(const CharT* aStr) : Super(aStr) {}

	/**
	 @brief Copy constructor.
	 */
	OBaseString(const OBaseString& aOther) : Super(aOther) {}

	/**
	 @brief Move constructor.
	 */
	OBaseString(OBaseString&& aOther) : Super(std::move(aOther)) {}

	/**
	 @brief Class destructor.
	 */
	virtual ~OBaseString() = default;

	/**
	 @brief Assignment operator (c-string).
	 */
	OBaseString& operator=(const CharT* aOther);
	
	/**
	 @brief Assignment operator (c-string).
	 */
	OBaseString& operator=(const OBaseString& aOther);

	/**
	 @brief Equality comparison operator.
	 */
	bool operator==(const OBaseString& aOther) const;

	/**
	 @brief Equality comparison operator (c-string).
	 */
	bool operator==(const CharT* aOther) const;

	/**
	 @brief Inequality comparison operator.
	 */
	bool operator!=(const OBaseString& aOther) const;

	/**
	 @brief Inequality comparison operator (c-string).
	 */
	bool operator!=(const CharT* aOther) const;

	/**
	 @brief Less comparison operator.
	 */
	bool operator<(const OBaseString& aOther) const;

	/**
	 @brief Append operator.
	 */
	OBaseString& operator+=(const OBaseString& aOther);

	/**
	 @brief Append operator.
	 */
	OBaseString& operator+=(const CharT* aOther);

	/**
	 @brief Less comparison operator (c-string).
	 */
	bool operator<(const CharT* aOther) const;

	/**
	 @brief Subscript operator, returns a character in the string.
	 */
	CharT& operator[](uint32_t aIndex);
	
	/**
	 @brief Subscript operator, returns a character in the string (const).
	 */
	const CharT& operator[](uint32_t aIndex) const;

	/**
	 @brief Returns c-style null terminated string.
	 */
	const CharT* cString() const;

	/**
	 @brief Returns iterator to the first CharTacter of the string.
	 */
	Iterator begin();

	/**
	 @brief Returns iterator to the end of the string.
	 */
	Iterator end();

	/**
	 @brief Returns constant iterator to the first CharTacter of the string.
	 */
	ConstIterator begin() const;

	/**
	 @brief Returns constant iterator to the end of the string.
	 */
	ConstIterator end() const;

	/**
	 @brief Returns string length, or number of characters.
	 */
	size_t length() const;

	/**
	 @brief Append to string.
	 */
	void append(const CharT* aString);

	/**
	 @brief Append to string.
	 */
	void append(const OBaseString& aOther);

	/**
	 @brief Insert into string.
	 @param aString The string to be inserted.
	 @param aPosition The iterator pointing to the position of insertion.
	 */
	void insert(const CharT* aString, Iterator& aPosition = begin());

	/**
	 @brief Finds the first occurrence of a substring after a given position.
	 @param aString The substring to be found.
	 @param aPosition The position from which the search must start.
	 @return The iterator pointing to the position, or <code>end()</code> if not found.
	 */
	Iterator find(const CharT* aString, Iterator& aPosition=begin());
	
	/**
	 @brief Finds the first occurrence of a substring (const).
	 @param aString The substring to be found.
	 @param aPosition The position from which the search must start.
	 @return The iterator pointing to the position, or <code>end()</code> if not found.
	 */
	ConstIterator find(const CharT* aString, ConstIterator& aPosition=begin()) const;
	
	/**
	 @brief Finds the first occurrence of a substring after a given position.
	 @param aChar The character to be found.
	 @param aPosition The position from which the search must start.
	 @return The iterator pointing to the position, or <code>end()</code> if not found.
	 */
	Iterator find(CharT aChar, Iterator& aPosition=begin());
	
	/**
	 @brief Finds the first occurrence of a substring (const).
	 @param aChar The character to be found.
	 @param aPosition The position from which the search must start.
	 @return The iterator pointing to the position, or <code>end()</code> if not found.
	 */
	ConstIterator find(CharT aChar, ConstIterator& aPosition=begin()) const;
	
	/**
	 @brief Finds the last occurrence of a substring after a given position.
	 @param aString The substring to be found.
	 @param aPosition The position from which the search must start.
	 @return The iterator pointing to the position, or <code>end()</code> if not found.
	 */
	Iterator findReverse(const CharT* aString, Iterator& aPosition=begin());
	
	/**
	 @brief Finds the last occurrence of a substring (const).
	 @param aString The substring to be found.
	 @param aPosition The position from which the search must start.
	 @return The iterator pointing to the position, or <code>end()</code> if not found.
	 */
	ConstIterator findReverse(const CharT* aString, ConstIterator& aPosition=begin()) const;
	
	/**
	 @brief Finds the last occurrence of a substring after a given position.
	 @param aChar The character to be found.
	 @param aPosition The position from which the search must start.
	 @return The iterator pointing to the position, or <code>end()</code> if not found.
	 */
	Iterator findReverse(CharT aChar, Iterator& aPosition=begin());
	
	/**
	 @brief Finds the last occurrence of a substring (const).
	 @param aChar The character to be found.
	 @param aPosition The position from which the search must start.
	 @return The iterator pointing to the position, or <code>end()</code> if not found.
	 */
	ConstIterator findReverse(CharT aChar, ConstIterator& aPosition=begin()) const;
	
	/**
	 @copydoc find(const CharT*, Iterator&)
	 */
	Iterator find(const OBaseString& aString, Iterator& aPosition=begin()) { return find(aString.cString(), aPosition); }
	
	/**
	 @copydoc find(const CharT*, ConstIterator&)
	 */
	ConstIterator find(const OBaseString& aString, ConstIterator& aPosition=begin()) const { return find(aString.cString, aPosition); }
	
	/**
	 @copydoc findReverse(const CharT*, Iterator&)
	 */
	Iterator findReverse(const OBaseString& aString, Iterator& aPosition=begin()) { return findReverse(aString.cString(), aPosition); }
	
	/**
	 @copydoc findReverse(const CharT*, ConstIterator&)
	 */
	ConstIterator findReverse(const OBaseString& aString, ConstIterator& aPosition=begin()) const { return findReverse(aString.cString, aPosition); }

	/**
	 @brief Provides a substring given a position inside the string and length.
	 @param aPosition The position where the substring starts.
	 @param aLength The length of the substring.
	 @return Another string object with the substring.
	 */
	OBaseString substring(size_t aPosition, size_t aLength=NoPosition) const;

	/**
	 @brief Provides a substring given two iterators.
	 @param aStart Start iterator.
	 @param aEnd End iterator.
	 @return Another string object with the substring.
	 */
	OBaseString substring(Iterator aStart, Iterator& aEnd=end()) const;
	
	/**
	 @brief Provides a substring given two iterators.
	 @param aStart Start iterator.
	 @param aEnd End iterator.
	 @return Another string object with the substring.
	 */
	OBaseString substring(ConstIterator aStart, ConstIterator& aEnd=end()) const;

	/**
	 @brief Finds first occurence of one of the characters.
	 @param aCharacters String with characters to look for.
	 @param aStart Iterator marking the position from where the search must start.
	 @return An iterator pointing to the position where one of the characters was found, or <code>end</code>,
		 if nothing is found.
	 */
	Iterator findFirstOf(const CharT* aCharacters, Iterator& aStart = begin());
	
	/**
	 @copydoc findFirstOf(const CharT*, Iterator&)
	 */
	Iterator findFirstOf(const OBaseString& aCharacters, Iterator& aStart = begin());

	/**
	 @copydoc findFirstOf(const CharT*, Iterator&)
	 */
	ConstIterator findFirstOf(const CharT* aCharacters, ConstIterator& aStart = begin()) const;

	/**
	 @copydoc findFirstOf(const CharT*, Iterator)
	 */
	ConstIterator findFirstOf(const OBaseString& aCharacters, ConstIterator& aStart = begin()) const;
	
	/**
	 @brief Finds last occurence of one of the characters.
	 @param aCharacters String with characters to look for.
	 @param aStart Iterator marking the position from where the search must start.
	 @return An iterator pointing to the position where one of the characters was found, or <code>end</code>,
		 if nothing is found.
	 */
	Iterator findLastOf(const CharT* aCharacters, Iterator& aStart = end());
	
	/**
	 @copydoc findLastOf(const CharT*, Iterator, Iterator)
	 */
	Iterator findLastOf(const OBaseString& aCharacters, Iterator& aStart = end());

	/**
	 @copydoc findLastOf(const CharT*, Iterator, Iterator)
	 */
	ConstIterator findLastOf(const CharT* aCharacters, ConstIterator& aStart = end()) const;
	
	/**
	 @copydoc findLastOf(const CharT*, Iterator, Iterator)
	 */
	ConstIterator findLastOf(const OBaseString& aCharacters, ConstIterator& aStart = end()) const;
};

template<typename CharT, typename Allocator>
inline OBaseString<CharT, Allocator>& OBaseString<CharT, Allocator>::operator=(const CharT * aOther)
{
	return OBaseString(Super::operator=(aOther));
}

template<typename CharT, typename Allocator>
inline OBaseString<CharT,Allocator>& OBaseString<CharT, Allocator>::operator=(const OBaseString & aOther)
{
	Super::operator=(aOther);
	return *this;
}

template<typename CharT, typename Allocator>
inline bool OBaseString<CharT,Allocator>::operator==(const OBaseString & aOther) const
{
	return (compare(aOther) == 0);
}

template<typename CharT, typename Allocator>
inline bool OBaseString<CharT,Allocator>::operator==(const CharT * aOther) const
{
	return (compare(aOther) == 0);
}

template<typename CharT, typename Allocator>
inline bool OBaseString<CharT,Allocator>::operator!=(const OBaseString & aOther) const
{
	return !operator==(aOther);
}

template<typename CharT, typename Allocator>
inline bool OBaseString<CharT,Allocator>::operator!=(const CharT * aOther) const
{
	return !operator==(aOther);
}

template<typename CharT, typename Allocator>
inline bool OBaseString<CharT,Allocator>::operator<(const OBaseString & aOther) const
{
	return (compare(aOther) < 0);
}

template<typename CharT, typename Allocator>
inline OBaseString<CharT,Allocator>& OBaseString<CharT, Allocator>::operator+=(const OBaseString & aOther)
{
	Super::operator+=(aOther);
	return *this;
}

template<typename CharT, typename Allocator>
inline OBaseString<CharT,Allocator>& OBaseString<CharT, Allocator>::operator+=(const CharT * aOther)
{
	Super::operator+=(aOther);
	return *this;
}

template<typename CharT, typename Allocator>
inline bool OBaseString<CharT,Allocator>::operator<(const CharT * aOther) const
{
	return (compare(aOther) < 0);
}

template<typename CharT, typename Allocator>
inline CharT & OBaseString<CharT, Allocator>::operator[](uint32_t aIndex)
{
	return Super::operator[](aIndex);
}

template<typename CharT, typename Allocator>
inline const CharT & OBaseString<CharT, Allocator>::operator[](uint32_t aIndex) const
{
	return Super::operator[](aIndex);
}

template<typename CharT, typename Allocator>
inline const CharT * OBaseString<CharT,Allocator>::cString() const
{
	return c_str();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::Iterator OBaseString<CharT,Allocator>::begin()
{
	return Super::begin();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::Iterator OBaseString<CharT,Allocator>::end()
{
	return Super::end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::ConstIterator OBaseString<CharT,Allocator>::begin() const
{
	return Super::begin();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::ConstIterator OBaseString<CharT,Allocator>::end() const
{
	return Super::end();
}

template<typename CharT, typename Allocator>
inline size_t OBaseString<CharT, Allocator>::length() const
{
	return Super::length();
}

template<typename CharT, typename Allocator>
inline void OBaseString<CharT, Allocator>::append(const CharT * aString)
{
	Super::append(aString);
}

template<typename CharT, typename Allocator>
inline void OBaseString<CharT, Allocator>::append(const OBaseString & aOther)
{
	Super::append(aOther);
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::Iterator OBaseString<CharT,Allocator>::find(const CharT * aString, Iterator & aPosition)
{
	auto pos = Super::find(aString, aPosition - begin());
	return (pos != Super::npos) ? begin() + pos : end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::ConstIterator OBaseString<CharT,Allocator>::find(const CharT * aString, ConstIterator& aPosition) const
{
	auto pos = Super::find(aString, aPosition - begin());
	return (pos != Super::npos) ? begin() + pos : end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::Iterator OBaseString<CharT, Allocator>::find(CharT aChar, Iterator & aPosition)
{
	auto pos = Super::find(aChar, aPosition - begin());
	return (pos != Super::npos) ? begin() + pos : end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::ConstIterator OBaseString<CharT, Allocator>::find(CharT aChar, ConstIterator& aPosition) const
{
	auto pos = Super::find(aChar, aPosition - begin());
	return (pos != Super::npos) ? begin() + pos : end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::Iterator OBaseString<CharT,Allocator>::findReverse(const CharT * aString, Iterator & aPosition)
{
	auto pos = Super::rfind(aString, aPosition - begin());
	return (pos != Super::npos) ? begin() + pos : end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::ConstIterator OBaseString<CharT,Allocator>::findReverse(const CharT * aString, ConstIterator& aPosition) const
{
	auto pos = Super::rfind(aString, aPosition - begin());
	return (pos != Super::npos) ? begin() + pos : end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::Iterator OBaseString<CharT, Allocator>::findReverse(CharT aChar, Iterator & aPosition)
{
	auto pos = Super::rfind(aChar, aPosition - begin());
	return (pos != Super::npos) ? begin() + pos : end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::ConstIterator OBaseString<CharT, Allocator>::findReverse(CharT aChar, ConstIterator& aPosition) const
{
	auto pos = Super::rfind(aChar, aPosition - begin());
	return (pos != Super::npos) ? begin() + pos : end();
}

template<typename CharT, typename Allocator>
inline OBaseString<CharT,Allocator> OBaseString<CharT, Allocator>::substring(size_t aPosition, size_t aLength) const
{
	if (aPosition > length()) throw OException("String position out of bounds.");
	return OBaseString(Super::substr(aPosition, aLength));
}

template<typename CharT, typename Allocator>
inline OBaseString<CharT,Allocator> OBaseString<CharT, Allocator>::substring(Iterator aStart, Iterator& aEnd) const
{
	return substring(aStart-begin(), aEnd-aStart);
}

template<typename CharT, typename Allocator>
inline OBaseString<CharT,Allocator> OBaseString<CharT, Allocator>::substring(ConstIterator aStart, ConstIterator& aEnd) const
{
	return substring(aStart-begin(), aEnd-aStart);
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::Iterator OBaseString<CharT, Allocator>::findFirstOf(const CharT * aCharacters, Iterator& aStart)
{
	auto pos = Super::find_first_of(aCharacters, aStart - begin());
	return (pos != NoPosition) ? (begin() + pos) : end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::Iterator OBaseString<CharT, Allocator>::findFirstOf(const OBaseString & aCharacters, Iterator& aStart)
{
	auto pos = Super::find_first_of(aCharacters, aStart - begin());
	return (pos != NoPosition) ? (begin() + pos) : end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::ConstIterator OBaseString<CharT, Allocator>::findFirstOf(const CharT * aCharacters, ConstIterator& aStart) const
{
	auto pos = Super::find_first_of(aCharacters, aStart - begin());
	return (pos != NoPosition) ? (begin() + pos) : end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::ConstIterator OBaseString<CharT, Allocator>::findFirstOf(const OBaseString& aCharacters, ConstIterator& aStart) const
{
	auto pos = Super::find_first_of(aCharacters, aStart - begin());
	return (pos != NoPosition) ? (begin() + pos) : end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::Iterator OBaseString<CharT, Allocator>::findLastOf(const CharT * aCharacters, Iterator& aStart)
{
	auto pos = Super::find_last_of(aCharacters, aStart - begin());
	return (pos != NoPosition) ? (begin() + pos) : end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::Iterator OBaseString<CharT, Allocator>::findLastOf(const OBaseString & aCharacters, Iterator& aStart)
{
	auto pos = Super::find_last_of(aCharacters, aStart - begin());
	return (pos != NoPosition) ? (begin() + pos) : end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::ConstIterator OBaseString<CharT, Allocator>::findLastOf(const CharT * aCharacters, ConstIterator& aStart) const
{
	auto pos = Super::find_last_of(aCharacters, aStart - begin());
	return (pos != NoPosition) ? (begin() + pos) : end();
}

template<typename CharT, typename Allocator>
inline typename OBaseString<CharT,Allocator>::ConstIterator OBaseString<CharT, Allocator>::findLastOf(const OBaseString& aCharacters, ConstIterator& aStart) const
{
	auto pos = Super::find_last_of(aCharacters, aStart - begin());
	return (pos != NoPosition) ? (begin() + pos) : end();
}


/**
 @brief Add operator.
 */
template <typename CharT, typename Allocator>
OBaseString<CharT, Allocator> operator+(const OBaseString<CharT,Allocator>& aLeft, const OBaseString<CharT,Allocator>& aRight)
{
	return OBaseString<CharT, Allocator>(aLeft).append(aRight);
}

/**
 @brief Add operator.
 */
template <typename CharT, typename Allocator>
OBaseString<CharT, Allocator> operator+(const OBaseString<CharT,Allocator>& aLeft, const CharT* aRight)
{
	return OBaseString<CharT, Allocator>(aLeft).append(aRight);
}

/**
 @brief Add operator.
 */
template <typename CharT, typename Allocator>
OBaseString<CharT, Allocator> operator+(const CharT* aLeft, const OBaseString<CharT,Allocator>& aRight)
{
	return OBaseString<CharT, Allocator>(aLeft).append(aRight);
}
