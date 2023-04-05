#pragma once

namespace RAVEN::Graphics::DebugUI
{
	void StaticInitialize(HWND window, bool docking = false, bool multiViewport = false);
	void StaticTerminate();

	void BeginRender();
	void EndRender();
}