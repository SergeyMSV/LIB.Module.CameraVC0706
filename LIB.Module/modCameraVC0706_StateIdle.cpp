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
		if (!HandleCmd(tPacketCmd::MakeGetVersion(m_pObj->m_SN), MsgStatus, 100) || MsgStatus != tMsgStatus::None)
			return false;

		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "CheckConnection");//[TBD]makes no sense
	}

	/////////////////////TEST
	{
		tMsgStatus MsgStatus;

		if (!HandleCmd(tPacketCmd::MakeFBufCtrlStopCurrentFrame(m_pObj->m_SN), MsgStatus, 100) || MsgStatus != tMsgStatus::None)
			return false;

		tFBufLen FBufLen;
		if (!HandleCmd(tPacketCmd::MakeGetFBufLenCurrent(m_pObj->m_SN), MsgStatus, FBufLen, 100) || MsgStatus != tMsgStatus::None)
			return false;

		//MsgStatus
		if (FBufLen.Value > 0)
		{
			const std::uint32_t ChunkAddr = 0;
			const std::uint32_t ChunkSize = 4096;//[TBD] from config ImageChunkSizeMax
			const std::uint32_t ChunkDelay = 5000;//in 0.01ms

			utils::tVectorUInt8 Data[4096];

			if (!HandleCmd(tPacketCmd::MakeReadFBufCurrent(m_pObj->m_SN, ChunkAddr, ChunkSize, ChunkDelay), MsgStatus, 100) || MsgStatus != tMsgStatus::None)
				return false;

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));//[TEST]
		}

		if (!HandleCmd(tPacketCmd::MakeFBufCtrlResumeFrame(m_pObj->m_SN), MsgStatus, 100) || MsgStatus != tMsgStatus::None)
			return false;

		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "MEDVED");//[TBD]makes no sense
	}
	/////////////////////

	return true;
}

}