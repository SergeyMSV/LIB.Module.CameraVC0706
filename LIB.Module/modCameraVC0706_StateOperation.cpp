#include "modCameraVC0706.h"

using namespace utils::packet::vc0706;

namespace mod
{
namespace vc0706
{

tCamera::tStateOperation::tStateOperation(tCamera *obj)
	:tState(obj, "tStateOperation")
{

}

void tCamera::tStateOperation::operator()()
{
	if (IsChangeState_ToStop())
		return;

	const auto TimeNow = utils::chrono::tClock::now();

	const tSettings Settings = m_pObj->GetSettings();

	const auto ImageTime = utils::chrono::GetDuration<std::chrono::milliseconds>(m_pObj->m_ImageLastTime, TimeNow);
	if (ImageTime > Settings.ImagePeriod_ms)
	{
		m_pObj->m_ImageLastTime = TimeNow;

		ChangeState(new tStateOperationImage(m_pObj));
		return;
	}

	const auto Duration_ms = utils::chrono::GetDuration<std::chrono::milliseconds>(m_pObj->m_CheckLastTime, TimeNow);
	if (Duration_ms > Settings.CheckPresencePeriod_ms)
	{
		m_pObj->m_CheckLastTime = TimeNow;

		tMsgStatus MsgStatus;
		if (!HandleCmd(tPacketCmd::MakeGetVersion(m_pObj->m_SN), MsgStatus, 100, 2) || MsgStatus != tMsgStatus::None)
		{
			ChangeState(new tStateError(m_pObj, "HandleCmd"));
			return;
		}

		m_pObj->m_pLog->WriteLine(true, "CheckConnection", utils::log::tColor::Green); // [TBD]makes no sense
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

}
}
