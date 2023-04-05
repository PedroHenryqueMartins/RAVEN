#pragma once
#include "Model.h"

namespace RAVEN::Graphics
{
	using ModelID = std::size_t;

	class ModelManager final
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static ModelManager* Get();
	public:

		ModelID LoadModel(std::filesystem::path fileName);

		void AddAnimation(ModelID modelID, std::filesystem::path fileName);

		const Model* GetModel(ModelID id) const;
		ModelID GetModelID(const std::filesystem::path& fileName) const;

	private:
		std::unordered_map<ModelID, std::unique_ptr<Model>> mInventory;
	};
}


//	Array
// - contiguous block of memory locations
// - random access
// - access by using a index/subscript (used for pointer arithmetic)

//	Linked Lists (another example)
// - allocation on demand
// - sequential access
// - insertion/removal is cheap

//	Hashing = convert some variable lengthed data to a fixed lengthed data

//	Associative Array
//	Instead of using indices, use key-value pairs
//	std::map -> binary tree underneath - red/black tree sorting
//	std::unsorted_map -> search is O(c) = fastest it can be / keep the key-value idea but not with binary tree, with hashtable hence it can be constant time access.