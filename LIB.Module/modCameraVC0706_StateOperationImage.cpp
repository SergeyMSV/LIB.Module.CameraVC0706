#include "modCameraVC0706.h"

using namespace utils::packet::vc0706;

namespace mod
{
namespace vc0706
{

tCamera::tStateOperationImage::tStateOperationImage(tCamera *obj)
	:tState(obj, "tStateOperationImage"), m_Settings(m_pObj->GetSettings())
{

}

tCamera::tStateOperationImage::~tStateOperationImage()
{
	if (m_ImageReady)
		m_pObj->OnImageComplete();
}

void tCamera::tStateOperationImage::operator()()
{
	if (IsChangeState_ToStop())
		return;

	tMsgStatus MsgStatus;

	if (!HandleCmd(tPacketCmd::MakeFBufCtrlStopCurrentFrame(m_pObj->m_SN), MsgStatus, 100, 10) || MsgStatus != tMsgStatus::None)
	{
		ChangeState(new tStateError(m_pObj, "HandleCmd"));
		return;
	}	

	if (IsChangeState_ToStop())
		return;

	tFBufLen FBufLen;
	if (!HandleCmd(tPacketCmd::MakeGetFBufLenCurrent(m_pObj->m_SN), MsgStatus, FBufLen, 100, 10) || MsgStatus != tMsgStatus::None)
	{
		ChangeState(new tStateError(m_pObj, "HandleCmd"));
		return;
	}

	if (IsChangeState_ToStop())
		return;

	if (FBufLen.Value > 0)
	{
		m_ImageReady = true;
		m_pObj->OnImageReady(); // when picture is really exists

		std::uint32_t ChunkSizeMax = m_Settings.ImageChunkSize / 4;
		ChunkSizeMax *= 4;//it must be multiple of 4

		const std::uint32_t ChunkDelay = m_Settings.ImageChunkDelayFromReq_us / 10;//in 0.01ms => 5000 / 10 = 500 => 50ms
		const std::uint32_t ChunkDelay_ms = m_Settings.ImageChunkDelayFromReq_us / 1000;

		std::uint32_t ChunkQty = FBufLen.Value / ChunkSizeMax;
		if (FBufLen.Value % ChunkSizeMax)
			++ChunkQty;

		std::uint32_t ChunkAddr = 0;

		for (std::size_t i = 0; i < ChunkQty; ++i)
		{
			const std::uint32_t DataLeft = FBufLen.Value - ChunkAddr;
			const std::uint32_t ChunkSize = DataLeft > ChunkSizeMax ? ChunkSizeMax : DataLeft;

			if (!HandleCmd(tPacketCmd::MakeReadFBufCurrent(m_Settings.GetPortDataBR() == 0 ? tPort::UART : tPort::UARTHS, m_pObj->m_SN, ChunkAddr, ChunkSize, ChunkDelay), MsgStatus, 200, 10) || MsgStatus != tMsgStatus::None)
			{
				ChangeState(new tStateError(m_pObj, "HandleCmd"));
				return;
			}

			const std::uint32_t ChunkTransferTime = (((ChunkSize * 8 * 1000) / m_Settings.GetImageDataBR()) + ChunkDelay_ms) * 2;//ms, this interval is doubled

			std::vector<std::uint8_t> Chunk;
			Chunk.reserve(ChunkSize);
			while (Chunk.size() != ChunkSize)
			{
				if (!WaitForReceivedData(ChunkTransferTime))
				{
					ChangeState(new tStateError(m_pObj, "WaitForReceivedData"));
					return;
				}

				if (IsChangeState_ToStop())
					return;

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

			HandleRsp(tMsgId::ReadFBuf, MsgStatus, 1000);//[!] it's not needed to check this packet 
			//if (!HandleRsp(tMsgId::ReadFBuf, MsgStatus, 1000) || MsgStatus != tMsgStatus::None)
			//	return false;

			ChunkAddr += ChunkSize;

			if (IsChangeState_ToStop())
				return;

			m_pObj->OnImageChunk(Chunk);

			if (IsChangeState_ToStop())
				return;
		}
	}

	//[!] first response comes in 93ms from cmd sent at the br=115200
	//[!] it works well with wait_ms=150ms and doesn't work with wait_ms=100ms
	if (!HandleCmd(tPacketCmd::MakeFBufCtrlResumeFrame(m_pObj->m_SN), MsgStatus, 150, 10) || MsgStatus != tMsgStatus::None)
	{
		ChangeState(new tStateError(m_pObj, "HandleCmd"));
		return;
	}
	
	ChangeState(new tStateOperation(m_pObj));
}

}
}
