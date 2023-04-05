#pragma once

#include "BlendState.h"

namespace RAVEN::Graphics
{
	class BlendStateManager final
	{
	public:

		static void StaticInitialize();
		static void StaticTerminate();
		static BlendStateManager* Get();

	public:

		void Initialize();
		void Terminate();

		void Set(BlendState::Mode blendMode);

	private:

		std::array<BlendState, 4> mBlendStates;

	};
}