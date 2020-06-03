#include "OsirisSDK/OException.h"
#include "OsirisSDK/OList.hpp"
#include "OsirisSDK/OTrashBin.h"

struct OTrashBin::Impl {
	using ItemList = OList<BaseItem*>;

	ItemList itemList;
};

OTrashBin::OTrashBin()
{
	OExceptionPointerCheck(_impl = new Impl);
}

OTrashBin::~OTrashBin()
{
	if (_impl != nullptr) delete _impl;
}

void OTrashBin::addToTrash(BaseItem* aItem)
{
	_impl->itemList.pushBack(aItem);
}

void OTrashBin::clear()
{
	while (!_impl->itemList.empty()) {
		auto& item = _impl->itemList.front();
		delete item;
		_impl->itemList.popFront();
	}
}
