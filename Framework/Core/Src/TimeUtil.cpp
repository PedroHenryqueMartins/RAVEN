#include "Precompiled.h"
#include "TimeUtil.h"

float RAVEN::Core::TimeUtil::GetTimeSeconds()
{
	using namespace std::chrono;
	static const auto startTime = high_resolution_clock::now();
	const auto currentTime = high_resolution_clock::now();
	const auto ms = duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
	return ms / 1000.0f;
}

float RAVEN::Core::TimeUtil::GetDeltaTimeSeconds()
{
	using namespace std::chrono;
	static auto lastTime = high_resolution_clock::now();
	const auto currentTime = high_resolution_clock::now();
	const auto ms = duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
	lastTime = currentTime;
	return ms / 1000.0f;
}
