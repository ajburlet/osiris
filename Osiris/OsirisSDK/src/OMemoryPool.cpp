#include <stdlib.h>
#include <math.h>
#include <sstream>

#include "OsirisSDK/OMemoryPool.h"
#include "OsirisSDK/OException.h"

using namespace std;

OMemoryPool::OMemoryPool(size_t blockSize, size_t segmentCount) :
	_blockSize(blockSize),
	_segmentSize(segmentCount)
{
}

OMemoryPool::~OMemoryPool()
{
	while (!_segmentStack.empty()) {
		free(_segmentStack.top());
		_segmentStack.pop();
	}
}

size_t OMemoryPool::blockSize() const
{
	return _blockSize;
}

size_t OMemoryPool::segmentSize() const
{
	return _segmentSize;
}

size_t OMemoryPool::availableBlocks() const
{
	return _availableBlocks.size();
}

size_t OMemoryPool::segmentCount() const
{
	return _segmentStack.size();
}

void * OMemoryPool::alloc(size_t sz)
{
	if (sz > _segmentSize*_blockSize) {
		stringstream ss;
		ss << "Requested alloc size (" << sz << " bytes) is larger than the segment size (" 
		   << _segmentSize*_blockSize << " bytes).";
		throw OException(ss.str().c_str());
	}

	int neededBlocks = (int)ceil((float)sz / _blockSize);
	void *seg = findContiguousSegment(neededBlocks);

	if (!seg) {
		createNewSegment();
		seg = findContiguousSegment(neededBlocks);
	}

	return seg;
}

void OMemoryPool::free(void * ptr)
{
	map<void*, int>::iterator it = _usedBlocks.find(ptr);
	if (it == _usedBlocks.end()) {
		stringstream ss;
		ss << "Unable to free pointer " << ptr << " since it does not belong to this memory pool.";
		throw OException(ss.str().c_str());
	}

	for (int i = 0; i < it->second; i++) _availableBlocks.push_back((char*)ptr + i * _blockSize);
	_availableBlocks.sort();
	_usedBlocks.erase(ptr);
}

void OMemoryPool::createNewSegment()
{
	void *newArea = malloc(_blockSize*_segmentSize);
	_segmentStack.push(newArea);
	for (size_t i = 0; i < _segmentSize; i++) _availableBlocks.push_back((char*)newArea + i);
}

void * OMemoryPool::findContiguousSegment(size_t count)
{
	list<void*>::iterator it;
	list<void*>::iterator currSegIt = _availableBlocks.begin();
	char *currSeg = (char*)_availableBlocks.front();
	int currSegSize = 0;
	for (it = _availableBlocks.begin(); it != _availableBlocks.end(); it++) {
		/* checking if this block is adjacent to the previous one */
		if ((char*)(*it) - currSeg != currSegSize*_blockSize) {
			currSegSize = 0;
			currSeg = (char*)(*it);
			currSegIt = it;
			continue;
		}
		currSegSize++;

		/* 
		 * if we reached the requested segment,  we remove the blocks from the 
		 * available list and create an used block entry 
		 */
		if (currSegSize == count) {
			list<void*>::iterator it2 = currSegIt;
			for (size_t i = 0; i < count; i++) {
				_availableBlocks.remove(*it2);
				it2++;
			}
			_usedBlocks[*currSegIt] = count;
			break;
		}
	}

	if (currSegSize != count) currSeg = NULL;

	return currSeg;
}
