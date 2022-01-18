#include "modCameraVC0706.h"

namespace mod
{

tCameraVC0706::tCameraVC0706(utils::tLog* log)
	:m_pLog(log)
{
	ChangeState(new tStateHalt(this, "the very start"));
	return;
}

void tCameraVC0706::operator()()
{
	while (!m_Control_OnExit)
	{
		(*m_pState)();
	}
}

void tCameraVC0706::Start()
{
	m_Control_Operation = true;
}

void tCameraVC0706::Start(bool exitOnError)
{
	m_Control_ExitOnError = exitOnError;
	Start();
}

void tCameraVC0706::Restart()
{
	m_Control_Restart = true;
}

void tCameraVC0706::Halt()
{
	m_Control_Operation = false;
}

void tCameraVC0706::Exit()
{
	m_Control_Exit = true;
	m_Control_Operation = false;
}

utils::tDevStatus tCameraVC0706::GetStatus() const
{
	//std::lock_guard<std::mutex> Lock(m_MtxState);

	return m_pState->GetStatus();
}

std::string tCameraVC0706::GetLastErrorMsg() const
{
	//std::lock_guard<std::mutex> lock(m_mtxstate);
	
	return m_LastErrorMsg;
}

void tCameraVC0706::Board_OnReceived(utils::tVectorUInt8& data)
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	m_ReceivedData.push(data);
}

bool tCameraVC0706::IsReceivedData() const
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	return m_ReceivedData.size() > 0;
}

utils::tVectorUInt8 tCameraVC0706::GetReceivedDataChunk()
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	if (m_ReceivedData.size() > 0)
	{
		utils::tVectorUInt8 Data(std::forward<utils::tVectorUInt8>(m_ReceivedData.front()));

		m_ReceivedData.pop();

		return Data;
	}

	return {};
}

void tCameraVC0706::ClearReceivedData()
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	while (!m_ReceivedData.empty())
	{
		m_ReceivedData.pop();
	}
}

void tCameraVC0706::ChangeState(tState *state)
{
	tState* Prev = m_pState;
	m_pState = state;
	delete Prev;
}

}
