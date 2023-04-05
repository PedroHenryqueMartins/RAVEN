#include "Precompiled.h"
#include "RAVEN.h"

RAVEN::App& RAVEN::MainApp()
{
	static App sApp;
	return sApp;
}