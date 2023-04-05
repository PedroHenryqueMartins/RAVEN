#include "Precompiled.h"
#include "WindowMessageHandler.h"

#include "DebugUtil.h"

using namespace RAVEN::Core;

void WindowMessageHandler::Hook(HWND window, Callback cb)
{
	mWindow = window;
	mPreviousCallback = (Callback)GetWindowLongPtr(window, GWLP_WNDPROC);
	SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)cb);
}

void WindowMessageHandler::Unhook()
{
	SetWindowLongPtr(mWindow, GWLP_WNDPROC, (LONG_PTR)mPreviousCallback);
	mWindow = nullptr;
}

LRESULT WindowMessageHandler::ForwardMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	ASSERT(mPreviousCallback, "WindowMessageHandler -- No callback is hooked.");
	return CallWindowProc((WNDPROC)mPreviousCallback, window, message, wParam, lParam);
}
