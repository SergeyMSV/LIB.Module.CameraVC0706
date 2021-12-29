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

	const auto TimeNow = utils::tClock::now();

	const tCameraVC0706Settings Settings = m_pObj->GetSettings();

	const auto ImageTime = utils::GetDuration<utils::ttime_ms>(m_pObj->m_ImageLastTime, TimeNow);
	if (ImageTime > Settings.ImagePeriod_ms)
	{
		m_pObj->m_ImageLastTime = TimeNow;

		ChangeState(new tStateOperationImage(m_pObj));
		return true;
	}

	const auto Duration_ms = utils::GetDuration<utils::ttime_ms>(m_pObj->m_CheckLastTime, TimeNow);
	if (Duration_ms > Settings.CheckPresencePeriod_ms)
	{
		m_pObj->m_CheckLastTime = TimeNow;

		tMsgStatus MsgStatus;
		if (!HandleCmd(tPacketCmd::MakeGetVersion(m_pObj->m_SN), MsgStatus, 100) || MsgStatus != tMsgStatus::None)
			return false;

		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "CheckConnection");//[TBD]makes no sense
	}

	return true;
}

}