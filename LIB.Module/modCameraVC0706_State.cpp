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

bool tCameraVC0706::tState::operator()()
{
	if (!Go())
	{
		ChangeState(new tStateError(m_pObj, "Go failed"));
		return true;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	return true;
}

bool tCameraVC0706::tState::Halt()
{
	ChangeState(new tStateStop(m_pObj));
	return true;
}

bool tCameraVC0706::tState::WaitForReceivedData(std::uint32_t wait_ms)
{
	std::chrono::time_point<tClock> StartTime = tClock::now();

	while (true)
	{
		if (!m_pObj->IsControlOperation())
			return false;

		if (m_pObj->IsReceivedData())
		{
			const utils::tVectorUInt8 Data = m_pObj->GetReceivedDataChunk();
			m_ReceivedData.insert(m_ReceivedData.end(), Data.cbegin(), Data.cend());
			return true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		const auto Duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(tClock::now() - StartTime).count();
		if (wait_ms < Duration_ms)
			return false;
	}

	return false;
}

}