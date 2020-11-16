#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OSystemMemoryAllocator.h"
#include "OsirisSDK/OException.h"

/**
 @brief Maintains pointers to objects that must be removed at a given time.
 */
class OAPI OTrashBin : public OMemoryManagedObject<OSystemMemoryAllocator<OMemoryManager::Scope::Default>>
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
	 @brief Deleted copy constructor.
	 */
	OTrashBin(const OTrashBin& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OTrashBin(OTrashBin&& aOther);

	/**
	 @brief Class destructor.
	 */
	virtual ~OTrashBin();

	/**
	 @brief Deleted copy assignment operator.
	 */
	OTrashBin& operator=(const OTrashBin& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OTrashBin& operator=(OTrashBin&& aOther);

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

inline OTrashBin::OTrashBin(OTrashBin&& aOther)
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}

template<class T>
inline void OTrashBin::trash(T * aObject)
{
	auto newItem = new Item<T>(aObject);
	OExceptionPointerCheck(newItem);
	addToTrash(newItem);
}
