#include "utilsChrono.h"

namespace utils
{
namespace chrono
{

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

tTimePeriod::tTimePeriod(bool sync)
	:m_Sync(sync)
{
}

tTimePeriod::tTimePeriod(bool sync, uint32_t period, bool postpone)
	:m_Sync(sync)
{
	Set(period, postpone);
}

void tTimePeriod::Set(uint32_t period, bool postpone)
{
	m_Period = period;
	m_StartTime = tClock::now();

	if (postpone)
		m_StartTime += std::chrono::seconds(m_Period);
}

bool tTimePeriod::IsReady()
{
	return IsReady(tClock::now());
}

bool tTimePeriod::IsReady(const tTimePoint& timePointNow)
{
	if (m_Period == 0 || m_StartTime > timePointNow)
		return false;

	m_StartTime = GetStartTime(timePointNow, m_StartTime, m_Period);

	return true;
}

tTimePoint tTimePeriod::GetStartTime(const tTimePoint& timePointNow, const tTimePoint& startTime, uint32_t period) const
{
	tTimePoint StartTime = startTime;

	if (!m_Sync)
	{
		StartTime = timePointNow + std::chrono::seconds(period);
		return StartTime;
	}

	while (StartTime <= timePointNow)
	{
		StartTime += std::chrono::seconds(period);
	}
	return StartTime;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

tTimePeriodCount::tTimePeriodCount(bool sync)
	:tTimePeriod(sync)
{
}

tTimePeriodCount::tTimePeriodCount(bool sync, uint32_t period, uint32_t repPeriod, int repQty, bool postpone)
	:tTimePeriod(sync, period, postpone), m_RepPeriod(repPeriod), m_RepQty(repQty)
{
	if (repPeriod >= period)
		m_RepPeriod = 0;
}

void tTimePeriodCount::Set(uint32_t period, uint32_t repPeriod, int repQty, bool postpone)
{
	tTimePeriod::Set(period, postpone);
	SetRep(repPeriod, repQty);
}

bool tTimePeriodCount::IsReady()
{
	auto TimeNow = tClock::now();
	auto StartTimePrev = GetStartTime();

	if (tTimePeriod::IsReady(TimeNow))
	{
		m_RepQtyCount = 1;
		m_RepStartTime = GetStartTime(TimeNow, StartTimePrev, m_RepPeriod);
		return true;
	}

	if (m_RepPeriod == 0 || m_RepQty == 0 || m_RepQtyCount == 0 || m_RepStartTime > TimeNow)
		return false;

	m_RepStartTime = GetStartTime(TimeNow, m_RepStartTime, m_RepPeriod);

	if (++m_RepQtyCount > m_RepQty)
		m_RepQtyCount = 0;

	return true;
}

void tTimePeriodCount::Complete()
{
	m_RepQtyCount = 0;
}

void tTimePeriodCount::SetRep(uint32_t repPeriod, int repQty)
{
	m_RepPeriod = repPeriod;
	m_RepStartTime = GetStartTime();
	m_RepQty = repQty;
	m_RepQtyCount = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
