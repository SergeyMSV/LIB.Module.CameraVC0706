#include "modCameraVC0706.h"

namespace mod
{

tCameraVC0706::tState::tState(tCameraVC0706* obj)
	:m_pObj(obj)
{
	m_pObj->ClearReceivedData();
}

tCameraVC0706::tState::tState(tCameraVC0706* obj, const std::string& strState)
	:tState(obj)
{
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, strState);
}

tCameraVC0706::tState::~tState()
{

}

bool tCameraVC0706::tState::Halt()
{
	ChangeState(new tStateStop(m_pObj));
	return true;
}

bool tCameraVC0706::tState::WaitForReceivedData(std::uint32_t wait_ms)
{
	const utils::tTimePoint TimeStart = utils::tClock::now();

	while (true)
	{
		if (!m_pObj->IsControlOperation())
			return true;

		if (m_pObj->IsReceivedData())
		{
			const utils::tVectorUInt8 Data = m_pObj->GetReceivedDataChunk();
			m_ReceivedData.insert(m_ReceivedData.end(), Data.cbegin(), Data.cend());
			return true;
		}

		if (!m_ReceivedData.empty())
			return true;

		const utils::tTimePoint TimeNow = utils::tClock::now();
		auto Duration = std::chrono::duration_cast<utils::ttime_ms>(TimeNow - TimeStart).count();

		if (wait_ms < utils::GetDuration<utils::ttime_ms>(TimeStart, utils::tClock::now()))
			return false;

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return false;
}

bool tCameraVC0706::tState::HandleCmd(const utils::packet_CameraVC0706::tPacketCmd& packet, utils::packet_CameraVC0706::tMsgStatus& responseStatus, std::uint32_t wait_ms, int repeatQty)
{
	utils::packet_CameraVC0706::tEmpty Empty;

	for (int i = 0; i < repeatQty; ++i)
	{
		if (HandleCmd(packet, responseStatus, Empty, wait_ms))
			return true;
	}
	return false;
}

bool tCameraVC0706::tState::HandleRsp(const utils::packet_CameraVC0706::tMsgId msgId, utils::packet_CameraVC0706::tMsgStatus& responseStatus, std::uint32_t wait_ms)
{
	utils::packet_CameraVC0706::tEmpty Empty;
	return HandleRsp(msgId, responseStatus, Empty, wait_ms);
}

bool tCameraVC0706::tState::IsChangeState_ToStop()
{
	if (!m_pObj->IsControlOperation())
	{
		ChangeState(new tStateStop(m_pObj));
		return true;
	}

	return false;
}

}