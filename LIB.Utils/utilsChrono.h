///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsChrono.h
// 2021-12-29
// Standard ISO/IEC 114882, C++20
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstdint>
#include <chrono>

namespace utils
{

using tClock = std::chrono::steady_clock;
using tTimePoint = std::chrono::time_point<tClock>;

using ttime_ns = std::chrono::nanoseconds;
using ttime_us = std::chrono::microseconds;
using ttime_ms = std::chrono::milliseconds;

template<class _Period, class T>
std::uint32_t GetDuration(std::chrono::time_point<T> timeStart, std::chrono::time_point<T> timeNow)
{
	if (timeStart >= timeNow)//[!]time can be changed backwards by OS (Windows)
		return 0;

	auto Duration = std::chrono::duration_cast<_Period>(timeNow - timeStart).count();
	return static_cast<std::uint32_t>(Duration);
}

}
