#pragma once

#include "Common.h"

namespace RAVEN
{
	class GameObjectHandle
	{
	public:
		GameObjectHandle() = default;

		int64_t GetInt64() { return static_cast<uint64_t>(mIndex) << 32 | mGeneration; }

	private:
		friend class GameWorld;

		int mIndex = -1;
		int mGeneration = -1;
	};
}