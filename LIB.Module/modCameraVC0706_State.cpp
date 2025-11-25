#include "modCameraVC0706.h"

namespace mod
{
namespace vc0706
{

tCamera::tState::tState(tCamera* obj)
	:m_pObj(obj)
{
	m_pObj->ClearReceivedData();
}

tCamera::tState::tState(tCamera* obj, const std::string& strState)
	:tState(obj)
{
	m_pObj->m_pLog->WriteLine(true, strState);
}

tCamera::tState::~tState()
{

}

bool tCamera::tState::Halt()
{
	ChangeState(new tStateStop(m_pObj));
	return true;
}

bool tCamera::tState::WaitForReceivedData(std::uint32_t wait_ms)
{
	const utils::chrono::tTimePoint TimeStart = utils::chrono::tClock::now();

	while (true)
	{
		if (!m_pObj->IsControlOperation())
			return true;

		if (m_pObj->IsReceivedData())
		{
			const std::vector<std::uint8_t> Data = m_pObj->GetReceivedDataChunk();
			m_ReceivedData.insert(m_ReceivedData.end(), Data.cbegin(), Data.cend());
			return true;
		}

		if (!m_ReceivedData.empty())
			return true;

		if (wait_ms < utils::chrono::GetDuration<std::chrono::milliseconds>(TimeStart, utils::chrono::tClock::now()))
			return false;

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return false;
}

bool tCamera::tState::HandleCmd(const utils::packet::vc0706::tPacketCmd& packet, utils::packet::vc0706::tMsgStatus& responseStatus, std::uint32_t wait_ms, int repeatQty)
{
	utils::packet::vc0706::tEmpty Empty;

	for (int i = 0; i < repeatQty; ++i)
	{
		if (HandleCmd(packet, responseStatus, Empty, wait_ms))
			return true;
	}
	return false;
}

bool tCamera::tState::HandleRsp(const utils::packet::vc0706::tMsgId msgId, utils::packet::vc0706::tMsgStatus& responseStatus, std::uint32_t wait_ms)
{
	utils::packet::vc0706::tEmpty Empty;
	return HandleRsp(msgId, responseStatus, Empty, wait_ms);
}

bool tCamera::tState::IsChangeState_ToStop()
{
	if (!m_pObj->IsControlOperation())
	{
		ChangeState(new tStateStop(m_pObj));
		return true;
	}

	return false;
}

}
}
