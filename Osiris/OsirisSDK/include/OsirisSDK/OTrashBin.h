#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OException.h"

/**
 @brief Maintains pointers to objects that must be removed at a given time.
 */
class OAPI OTrashBin
{
protected:
	/**
	 @cond HIDDEN
	 */
	class BaseItem {
	public:
		virtual ~BaseItem() = default;
	};

	template <class T>
	class Item : public BaseItem
	{
	public:
		Item(T* aObject) : _object(aObject) {}

		~Item() { delete _object; }
	private:
		T* _object;
	};
	/**
	 @endcond
	 */

public:
	/**
	 @brief Default class constructor.
	 */
	OTrashBin();

	/**
	 @brief Class destructor.
	 */
	virtual ~OTrashBin();

	/**
	 @brief Add item to be deleted at later time.
	 */
	template <class T>
	void trash(T* aObject);

	/**
	 @brief Clears the trash, removing all listed objects.
	 */
	void clear();

private:
	void addToTrash(BaseItem* aItem);

	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
};

template<class T>
inline void OTrashBin::trash(T * aObject)
{
	auto newItem = new Item<T>(aObject);
	OExceptionPointerCheck(newItem);
	addToTrash(newItem);
}
