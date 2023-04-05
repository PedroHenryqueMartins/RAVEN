#pragma once

namespace RAVEN::Graphics
{
	// Description:
	// This class allows us to control whether we want to check/update the z buffer:
	//
	//	bool CheckDepthBuffer(int x, int y, float depth, bool depthTest, bool depthWrite)
	//	{
	//		if (!depthTest)
	//			return true;
	//
	//		if (depth <= mDepthBuffer[x][y])
	//		{
	//			if (depthWrite)
	//				mDepthBuffer[x][y] = depth;
	//			return true;
	//		}
	//		return false;
	//	}

	class DepthStencilState final
	{
	public:
		static void ClearState();

	public:
		enum class DepthTest
		{
			Enable,
			Disable
		};
		enum class DepthWrite
		{
			Enable,
			Disable
		};

		DepthStencilState() = default;
		~DepthStencilState();

		DepthStencilState(const DepthStencilState&) = delete;
		DepthStencilState& operator=(const DepthStencilState&) = delete;

		void Initialize(DepthTest depthTest, DepthWrite depthWrite);
		void Terminate();

		void Set();

	private:
		ID3D11DepthStencilState* mDepthStencilState = nullptr;
	};
}
