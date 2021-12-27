#include "modCameraVC0706.h"

using namespace utils::packet_CameraVC0706;

namespace mod
{

tCameraVC0706::tStateOperationImage::tStateOperationImage(tCameraVC0706 *obj)
	:tState(obj, "tStateOperationImage")
{

}

tCameraVC0706::tStateOperationImage::~tStateOperationImage()
{
	if (m_ImageReady)
		m_pObj->OnImageComplete();
}

bool tCameraVC0706::tStateOperationImage::Go()
{
	tMsgStatus MsgStatus;

	if (!HandleCmd(tPacketCmd::MakeFBufCtrlStopCurrentFrame(m_pObj->m_SN), MsgStatus, 100) || MsgStatus != tMsgStatus::None)
		return false;

	tFBufLen FBufLen;
	if (!HandleCmd(tPacketCmd::MakeGetFBufLenCurrent(m_pObj->m_SN), MsgStatus, FBufLen, 100) || MsgStatus != tMsgStatus::None)
		return false;

	if (FBufLen.Value > 0)
	{
		m_ImageReady = true;
		m_pObj->OnImageReady(); // when picture is really exists

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

			if (!HandleRsp(tMsgId::ReadFBuf, MsgStatus, 100) || MsgStatus != tMsgStatus::None)
				return false;

			ChunkAddr += ChunkSize;

			if (!m_pObj->IsControlOperation())
			{
				ChangeState(new tStateStop(m_pObj));
				return true;
			}

			m_pObj->OnImageChunk(Chunk);

			if (!m_pObj->IsControlOperation())
			{
				ChangeState(new tStateStop(m_pObj));
				return true;
			}
		}

		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));//[TEST]
	}

	if (!HandleCmd(tPacketCmd::MakeFBufCtrlResumeFrame(m_pObj->m_SN), MsgStatus, 100) || MsgStatus != tMsgStatus::None)
		return false;
	
	ChangeState(new tStateOperation(m_pObj));
	return true;
}

}