#pragma once

#include "DepthStencilState.h"

namespace RAVEN::Graphics
{
	class DepthStencilStateManager final
	{
	public:

		static void StaticInitialize();
		static void StaticTerminate();
		static DepthStencilStateManager* Get();

	public:

		void Initialize();
		void Terminate();

		void Set(DepthStencilState::DepthTest depthTest, DepthStencilState::DepthWrite depthWrite);

	private:

		std::array<DepthStencilState, 4> mDepthStencilStates;

	};
}