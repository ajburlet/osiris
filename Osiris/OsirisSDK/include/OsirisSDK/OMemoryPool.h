#pragma once

#include <stack>
#include <list>
#include <map>

#include "defs.h"

/**
 \brief Memory pool implementation for special memory management.

 In some cases (i.e. events) objects may have to be instanced in a regular basis. In order
 to avoid frequent malloc() calls and memory fragmentation, we resort to a memory pool.
 */
class OAPI OMemoryPool
{
public:
	/**
	 \brief Class constructor.
	 \param blockSize Size of each memory block in bytes.
	 \param segmentSize Number of blocks in a memory segment.
	 */
	OMemoryPool(size_t blockSize, size_t segmentSize);

	/**
	 \brief Class destructor.
	 */
	virtual ~OMemoryPool();

	/**
	 \brief Returns the size of the block in bytes.
	 */
	size_t blockSize() const;

	/**
	 \brief Returns the number of blocks in each segment.
	 */
	size_t segmentSize() const;

	/**
	 \brief Current available blocks.
	 */
	size_t availableBlocks() const;

	/**
	 * \brief Current segment count.
	 */
	size_t segmentCount() const;

	/**
	 \brief Memory allocation method that replaces to malloc().
	 */
	void* alloc(size_t sz);

	/**
	 \brief Memory release method that replaces free().
	 */
	void free(void *ptr);

private:
	size_t _blockSize;
	size_t _segmentSize;
	std::stack<void*> _segmentStack;
	std::list<void*> _availableBlocks;
	std::map<void*, int> _usedBlocks;

	/**
	 \brief Allocates new segment and adds to the segment stack.
	 */
	void createNewSegment();

	/**
	 \brief Find contiguous region of memory.
	 \param count Number of required contiguous blocks.
	 \return The memory address or NULL if there's none available.
	 */
	void* findContiguousSegment(size_t count);
};

