#include "OsirisSDK/OException.h"
#include "OsirisSDK/OTrashBin.h"
#include "OsirisSDK/OTrashBinOwner.h"

OTrashBinOwner::OTrashBinOwner()
{
	OExceptionPointerCheck(_trashBin = new OTrashBin);
}

OTrashBinOwner::~OTrashBinOwner()
{
	if (_trashBin) delete _trashBin;
}

OTrashBinOwner & OTrashBinOwner::operator=(OTrashBinOwner && aOther)
{
	if (_trashBin == nullptr) delete _trashBin;
	_trashBin = aOther._trashBin;
	aOther._trashBin = nullptr;
	return *this;
}

OTrashBin & OTrashBinOwner::trashBin()
{
	return *_trashBin;
}
