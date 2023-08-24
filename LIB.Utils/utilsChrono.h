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

class tTimePeriod
{
	uint32_t m_Period = 0;//in seconds

	utils::tTimePoint m_StartTime = utils::tClock::now();

protected:
	const bool m_Sync = false;

public:
	tTimePeriod() = delete;
	explicit tTimePeriod(bool sync);
	tTimePeriod(bool sync, uint32_t period, bool postpone);

	void Set(uint32_t period, bool postpone);
	bool IsReady();

	uint32_t GetPeriod() const { return m_Period; }

protected:
	bool IsReady(const tTimePoint& timePointNow);

	utils::tTimePoint GetStartTime(const tTimePoint& timePointNow, const utils::tTimePoint& startTime, uint32_t period) const;
	utils::tTimePoint GetStartTime() const { return m_StartTime; }
};

class tTimePeriodCount : private tTimePeriod
{
	uint32_t m_RepPeriod = 0;//in seconds
	int m_RepQty = 0;
	int m_RepQtyCount = 0;

	utils::tTimePoint m_RepStartTime = GetStartTime();

public:
	explicit tTimePeriodCount(bool sync);
	tTimePeriodCount(bool sync, uint32_t period, uint32_t repPeriod, int repQty, bool postpone);

	void Set(uint32_t period, uint32_t repPeriod, int repQty, bool postpone);
	bool IsReady();
	void Complete();

	uint32_t GetPeriod() const { return tTimePeriod::GetPeriod(); }
	uint32_t GetRepPeriod() const { return m_RepPeriod; }
	int GetRepQty() const { return m_RepQty; }

private:
	void SetRep(uint32_t repPeriod, int repQty);
};

}
