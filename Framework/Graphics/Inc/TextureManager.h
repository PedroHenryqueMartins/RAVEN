#pragma once
#include "Texture.h"

namespace RAVEN::Graphics
{
	using TextureID = std::size_t;

	class TextureManager final
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static TextureManager* Get();
	public:
		~TextureManager();

		TextureID LoadTexture(std::filesystem::path fileName);

		const Texture* GetTexture(TextureID id) const;

		void BindVS(TextureID id, uint32_t slotIndex) const ;
		void BindPS(TextureID id, uint32_t slotIndex) const ;

	private:
		std::unordered_map<TextureID, std::unique_ptr<Texture>> mInventory;
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