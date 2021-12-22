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

		if (FBufLen.Value > 0)
		{
			const std::size_t ChunkSizeMax = 768;// 4096;//[TBD] from config ImageChunkSizeMax
			const std::uint32_t ChunkDelay = 5000;//in 0.01ms //[TBD] from config ImageChunkSizeMax

			std::size_t ChunkQty = FBufLen.Value / ChunkSizeMax;
			if (FBufLen.Value % ChunkSizeMax)
				++ChunkQty;

			std::uint32_t ChunkAddr = 0;

			for (std::size_t i = 0; i < ChunkQty; ++i)
			{
				const std::size_t DataLeft = FBufLen.Value - ChunkAddr;
				const std::uint32_t ChunkSize = DataLeft > ChunkSizeMax ? ChunkSizeMax : DataLeft;

				if (!HandleCmd(tPacketCmd::MakeReadFBufCurrent(m_pObj->m_SN, ChunkAddr, ChunkSize, ChunkDelay), MsgStatus, 100) || MsgStatus != tMsgStatus::None)
					return false;

				utils::tVectorUInt8 Chunk;
				Chunk.reserve(ChunkSize);
				while (Chunk.size() != ChunkSize)
				{
					if (!WaitForReceivedData(1000))//[TBD] period of time for transferring data chunk
						return false;

					const std::size_t DataWaiting = ChunkSize - Chunk.size();

					if (DataWaiting < m_ReceivedData.size())
					{
						Chunk.insert(Chunk.end(), m_ReceivedData.cbegin(), m_ReceivedData.cbegin() + DataWaiting);
						m_ReceivedData.erase(m_ReceivedData.begin(), m_ReceivedData.begin() + DataWaiting);
					}
					else
					{
						Chunk.insert(Chunk.end(), m_ReceivedData.cbegin(), m_ReceivedData.cend());
						m_ReceivedData.clear();
					}
				}

				while (true)
				{
					if (!WaitForReceivedData(100))//[TBD] calculate according to BR and waiting for camera response
						return false;

					tPacketRet Rsp;
					if (tPacketRet::Find(m_ReceivedData, Rsp) > 0 && Rsp.GetMsgId() == tMsgId::ReadFBuf)//packet.GetMsgId())
					{
						break;
						//responseStatus = Rsp.GetMsgStatus();
						//return true;
					}

					if (m_ReceivedData.size() > ContainerCmdSize + ContainerPayloadSizeMax)
						return false;
				}

				//[TBD] store the data into a file

				ChunkAddr += ChunkSize;
			}

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