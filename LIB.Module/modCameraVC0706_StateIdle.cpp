#include "modCameraVC0706.h"

using namespace utils::packet_CameraVC0706;

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

	const auto Duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(tClock::now() - m_CheckLastTime).count();
	const std::uint32_t IdleCheckPeriod_ms = 1000;//[TBD] from settings
	if (Duration_ms > IdleCheckPeriod_ms)
	{
		tMsgStatus MsgStatus;
		if (!HandleCmd(tPacketCmd::MakeGetVersion(0x00), MsgStatus, 100) || MsgStatus != tMsgStatus::None)
			return false;

		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "CheckConnection");//[TBD]makes no sense
	}

	/////////////////////TEST
	{
		tMsgStatus MsgStatus;

		if (!HandleCmd(tPacketCmd::MakeFBufCtrlStopCurrentFrame(0x00), MsgStatus, 100) || MsgStatus != tMsgStatus::None)
			return false;

		tFBufLen FBufLen;
		if (!HandleCmd(tPacketCmd::MakeGetFBufLenCurrent(0x00), MsgStatus, FBufLen, 100) || MsgStatus != tMsgStatus::None)
			return false;

		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "MEDVED");//[TBD]makes no sense
	}
	/////////////////////

	return true;
}

}