#include "modCameraVC0706.h"

using namespace utils::packet_CameraVC0706;

namespace mod
{

tCameraVC0706::tStateOperation::tStateOperation(tCameraVC0706 *obj)
	:tState(obj, "tStateOperation")
{

}

bool tCameraVC0706::tStateOperation::Go()
{
	if (!m_pObj->IsControlOperation())
	{
		ChangeState(new tStateStop(m_pObj));
		return true;
	}

	const auto ClockNow = tClock::now();

	const tCameraVC0706Settings Settings = m_pObj->GetSettings();

	const auto ImageTime = std::chrono::duration_cast<std::chrono::milliseconds>(ClockNow - m_pObj->m_ImageLastTime).count();
	const std::uint32_t ImagePeriod = Settings.ImagePeriod_ms;
	if (ImageTime > ImagePeriod)
	{
		m_pObj->m_ImageLastTime = ClockNow;

		ChangeState(new tStateOperationImage(m_pObj));
		return true;
	}

	const auto Duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(ClockNow - m_pObj->m_CheckLastTime).count();
	const std::uint32_t CheckPresencePeriod_ms = Settings.CheckPresencePeriod_ms;
	if (Duration_ms > CheckPresencePeriod_ms)
	{
		m_pObj->m_CheckLastTime = ClockNow;

		tMsgStatus MsgStatus;
		if (!HandleCmd(tPacketCmd::MakeGetVersion(m_pObj->m_SN), MsgStatus, 100) || MsgStatus != tMsgStatus::None)
			return false;

		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "CheckConnection");//[TBD]makes no sense
	}

	return true;
}

}