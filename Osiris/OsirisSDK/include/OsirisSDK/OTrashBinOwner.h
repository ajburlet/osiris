#pragma once

#include "OsirisSDK/defs.h"

class OTrashBin;

/**
 @brief Base class for all classes that have a trash bin.
 */
class OAPI OTrashBinOwner
{
public:
	/**
	 @brief Default class constructor.
	 */
	OTrashBinOwner();

	/**
	 @brief Deleted copy constructor.
	 */
	OTrashBinOwner(const OTrashBinOwner& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OTrashBinOwner(OTrashBinOwner&& aOther);

	/**
	 @brief Class destructor.
	 */
	~OTrashBinOwner();

	/**
	 @brief Deleted copy assignment operator.
	 */
	OTrashBinOwner& operator=(const OTrashBinOwner& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OTrashBinOwner& operator=(OTrashBinOwner&& aOther);

	/**
	 @brief The class trash bin.
	 */
	OTrashBin& trashBin();

private:
	OTrashBin* _trashBin = nullptr;
};

inline OTrashBinOwner::OTrashBinOwner(OTrashBinOwner&& aOther)
{
	_trashBin = aOther._trashBin;
	aOther._trashBin = nullptr;
}


