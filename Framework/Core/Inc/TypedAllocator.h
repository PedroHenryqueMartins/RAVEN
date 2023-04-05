#pragma once

#include "BlockAllocator.h"

namespace RAVEN::Core
{
	//	A type safe block allocator. Essentially, it uses
	//	the block allocator to allocate and free memory, but
	//	add constructor/desctructor calls on top of that.
	template <class DataType>
	class TypedAllocator : private BlockAllocator
	{
	public:
		TypedAllocator(const char* name, size_t capacity)
			: BlockAllocator(name, sizeof(DataType), capacity)
		{

		}

		//	Variadic Template
		template <class... Args>
		DataType* New(Args&&... args)
		{
			auto* instance = static_cast<DataType*>(Allocate());
			new (instance) DataType(std::forward<Args>(args)...); // Perfect forwarding //	Placement new, calls constructor on designated location
			return instance;
		}

		void Delete(DataType* ptr)
		{
			if (ptr == nullptr)
				return;

			ptr->~DataType(); //	Explicit destructor call
			Free(ptr);
		}
	};
}