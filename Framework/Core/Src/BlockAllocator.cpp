#include "Precompiled.h"
#include "BlockAllocator.h"

#include "DebugUtil.h"

using namespace RAVEN::Core;

BlockAllocator::BlockAllocator(const char* name, size_t blockSize, size_t capacity)
	: mName(name)
	, mData(nullptr)
	, mBlockSize(blockSize)
	, mCapacity(capacity)
	, mFreeBlocks(capacity)
{
	ASSERT(blockSize > 0, "BlockAllocator -- Invalid block size initialized.");
	ASSERT(capacity > 0, "BlockAllocator -- Invalid capacity initialized.");

	//	Do one malloc to allocate all the memory needed
	mData = std::malloc(blockSize * capacity);

	//	Create free block pointers
	for (size_t i = 0; i < capacity; ++i)
	{
		mFreeBlocks[i] = static_cast<uint8_t*>(mData) + (i * mBlockSize);
	}
}

BlockAllocator::~BlockAllocator()
{
	ASSERT(mFreeBlocks.size() == mCapacity, "Not all blocks are freed. Potential memory leak.");
	//	Do one free to release all allocated memory
	std::free(mData);
}

void* BlockAllocator::Allocate()
{
	//	Return a free block, or nullptr if all blocks are occupied
	if (mFreeBlocks.empty())
	{
		LOG("No free blocks left.");
		return nullptr;
	}

	//	Get next free block.
	void* freeBlock = mFreeBlocks.back();
	mFreeBlocks.pop_back();

	return freeBlock;
}

void BlockAllocator::Free(void* ptr)
{
	//	Valid ptr, collect into free blocks
	if (ptr == nullptr)
		return;

	const auto start = static_cast<uint8_t*>(mData);
	const auto end = static_cast<uint8_t*>(mData) + (mBlockSize * mCapacity);
	const auto current = static_cast<uint8_t*>(mData);
	const auto diff = current - start;

	mFreeBlocks.emplace_back(ptr);
}

#define MEMORY_POOL_DEFINE(Class, Capacity)\
	RAVEN::Core::BlockAllocator Class::sAllocator(#Class##"Allocator",sizeof(Class),Capacity);\
	void* Class::operator new(size_t size) { return sAllocator.Allocate(); }\
	void Class::operator delete(void* ptr) { sAllocator.Free(ptr); }
