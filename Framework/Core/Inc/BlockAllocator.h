#pragma once

namespace RAVEN::Core
{
	class BlockAllocator
	{
	public:
		BlockAllocator(const char* name, size_t blockSize, size_t capacity);
		~BlockAllocator();

		BlockAllocator(const BlockAllocator&) = delete; // Copy constructor
		BlockAllocator& operator=(const BlockAllocator&) = delete; // Copy assignment
		BlockAllocator(BlockAllocator&&) = delete; // Move Constructor
		BlockAllocator& operator=(BlockAllocator&&) = delete;	// Move assignment

		void* Allocate();
		void Free(void* ptr);

	private:
		std::string mName;
		std::vector<void*> mFreeBlocks;
		void* mData = nullptr;
		size_t mBlockSize = 0;
		size_t mCapacity = 0;
	};
}

#define MEMORY_POOL_DECLARE\
	static RAVEN::Core::BlockAllocator sAllocator;\
	static void* operator new(size_t size);\
	static void operator delete(void* ptr);
