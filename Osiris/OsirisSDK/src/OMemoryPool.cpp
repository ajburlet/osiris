#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sstream>

#include "OsirisSDK/OMemoryPool.h"
#include "OsirisSDK/OException.h"

using namespace std;

OMemoryPool::OMemoryPool(size_t blockSize, size_t segmentCount) :
	_blockSize(blockSize),
	_segmentSize(segmentCount),
	_availableBlocksOrdered(true)
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
		throw OEx(ss.str().c_str());
	}

	int neededBlocks = (int)ceil((float)sz / _blockSize);
	
	void *seg;
	if (neededBlocks > 1) {
		/* more than one block needed, find a contiguous segment. */
		seg = findContiguousSegment(neededBlocks);
		if (!seg) seg = createNewSegment();
	} else {
		/* only one block needed, so just get the first available block (faster) */
		if (_availableBlocks.size() == 0) seg = createNewSegment();
		else seg = _availableBlocks.front();
	}
	
	removeFromAvailablePool(seg, neededBlocks);

	return seg;
}

void OMemoryPool::free(void * ptr)
{
	map<void*, int>::iterator it = _usedBlocks.find(ptr);
	if (it == _usedBlocks.end()) {
		stringstream ss;
		ss << "Unable to free pointer " << ptr << " since it does not belong to this memory pool.";
		throw OEx(ss.str().c_str());
	}

	for (int i = 0; i < it->second; i++) _availableBlocks.push_back((char*)ptr + i * _blockSize);
	_availableBlocksOrdered = false;
	_usedBlocks.erase(ptr);
}

void OMemoryPool::printDebugInfo()
{
	fprintf(stderr, "Available memory: %d bytes (%d blocks):\n", 
		_availableBlocks.size()*_blockSize, 
		_availableBlocks.size());
	for (list<void*>::iterator it = _availableBlocks.begin(); it != _availableBlocks.end(); it++)
		fprintf(stderr, "\t0x%x\n", (unsigned int)*it);

	fprintf(stderr, "Allocated chunks:\n");
	for (map<void*, int>::iterator it = _usedBlocks.begin(); it != _usedBlocks.end(); it++)
		fprintf(stderr, "\t0x%x - %d bytes (%d blocks)\n", (unsigned int)it->first, it->second*_blockSize, it->second);
}

void* OMemoryPool::createNewSegment()
{
	void *newArea = malloc(_blockSize*_segmentSize);
	_segmentStack.push(newArea);
	for (size_t i = 0; i < _segmentSize; i++) _availableBlocks.push_back((char*)newArea + i*_blockSize);
	return newArea;
}

void * OMemoryPool::findContiguousSegment(size_t count)
{
	/* check if there is enough room */
	if (count > _availableBlocks.size()) return NULL;

	/* to find contiguous segments, the available blocks must be ordered */
	if (!_availableBlocksOrdered) {
		_availableBlocks.sort();
		_availableBlocksOrdered = true;
	}

	/* since there is, we now need to determine if we can find a contiguous segment of the needed length */
	void *seg = NULL;
	char *currSeg = (char*)_availableBlocks.front();
	int currSegSize = 0;
	for (list<void*>::iterator it = _availableBlocks.begin(); it != _availableBlocks.end(); it++) {
		currSegSize++;

		/* checking if this block is adjacent to the previous one */
		if (currSegSize > 1 && (char*)(*it) - currSeg != (currSegSize-1)*_blockSize) {
			currSegSize = 1;
			currSeg = (char*)(*it);
			continue;
		}

		/* 
		 * if we reached the requested segment,  we remove the blocks from the 
		 * available list and create an used block entry 
		 */
		if (currSegSize == count) {
			seg = currSeg;
			break;
		}
	}

	return seg;
}

void OMemoryPool::removeFromAvailablePool(void * ptr, size_t count)
{
	for (size_t i = 0; i < count; i++) _availableBlocks.remove((char*)ptr + i*_blockSize);
	_usedBlocks[ptr] = count;
}
