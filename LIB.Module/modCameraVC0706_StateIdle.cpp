#include "modCameraVC0706.h"

namespace mod
{

tCameraVC0706::tStateIdle::tStateIdle(tCameraVC0706 *obj)
	:tState(obj, "tStateIdle")
{

}

bool tCameraVC0706::tStateIdle::Go()
{
	if (!m_pObj->IsControlOperation())
	{
		ChangeState(new tStateStop(m_pObj));
		return true;
	}

	//const auto Time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(tClock::now() - m_StartTime).count();//C++11
	//const double Time_us = static_cast<double>(Time_ns) / 1000;//C++11
	//if (Time_us > m_SettingsNMEA.PeriodMax)
	//{
	//	ChangeState(new tStateError(m_pObj, "operation: no data within PeriodMAX"));
	//	return true;
	//}

	return true;
}

}