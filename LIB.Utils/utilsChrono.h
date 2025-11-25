///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsChrono
// 2021-12-29
// C++20
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstdint>
#include <chrono>

namespace utils
{
namespace chrono
{

// system_clock - It is the current time according to the system (regular clock which we see on the toolbar of the computer). - That's better to show current time or something like that.
// steady_clock - It is a monotonic clock that will never be adjusted. It goes at a uniform rate. - That's better for measurements of durations of different operations.

using tClock = std::chrono::steady_clock; // this clock is for measurements.
using tTimePoint = std::chrono::time_point<tClock>;

using ttime_ns = std::chrono::nanoseconds;
using ttime_us = std::chrono::microseconds;
using ttime_ms = std::chrono::milliseconds;

template<class _Period, class T>
std::uint32_t GetDuration(std::chrono::time_point<T> timeStart, std::chrono::time_point<T> timeNow)
{
	if (timeStart >= timeNow) // [!] time can be changed backwards by OS (Windows)
		return 0;

	auto Duration = std::chrono::duration_cast<_Period>(timeNow - timeStart).count();
	return static_cast<std::uint32_t>(Duration);
}

class tTimeDuration
{
	tTimePoint m_TimeStart = tClock::now();

public:
	tTimeDuration() = default;
	virtual ~tTimeDuration() {}

	template<typename T>
	std::uint32_t Get() const { return GetDuration<T>(m_TimeStart, tClock::now()); }
};

class tTimePeriod
{
	std::uint32_t m_Period = 0;//in seconds

	tTimePoint m_StartTime = tClock::now();

protected:
	const bool m_Sync = false;

public:
	tTimePeriod() = delete;
	explicit tTimePeriod(bool sync);
	tTimePeriod(bool sync, std::uint32_t period, bool postpone);

	void Set(std::uint32_t period, bool postpone);
	bool IsReady();

	std::uint32_t GetPeriod() const { return m_Period; }

protected:
	bool IsReady(const tTimePoint& timePointNow);

	tTimePoint GetStartTime(const tTimePoint& timePointNow, const tTimePoint& startTime, std::uint32_t period) const;
	tTimePoint GetStartTime() const { return m_StartTime; }
};

class tTimePeriodCount : private tTimePeriod
{
	std::uint32_t m_RepPeriod = 0; // in seconds
	int m_RepQty = 0;
	int m_RepQtyCount = 0;

	tTimePoint m_RepStartTime = GetStartTime();

public:
	explicit tTimePeriodCount(bool sync);
	tTimePeriodCount(bool sync, std::uint32_t period, std::uint32_t repPeriod, int repQty, bool postpone);

	void Set(std::uint32_t period, std::uint32_t repPeriod, int repQty, bool postpone);
	bool IsReady();
	void Complete();

	std::uint32_t GetPeriod() const { return tTimePeriod::GetPeriod(); }
	std::uint32_t GetRepPeriod() const { return m_RepPeriod; }
	int GetRepQty() const { return m_RepQty; }

private:
	void SetRep(uint32_t repPeriod, int repQty);
};

}
}
