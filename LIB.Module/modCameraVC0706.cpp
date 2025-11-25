#include "modCameraVC0706.h"

namespace mod
{
namespace vc0706
{

tCamera::tCamera(utils::log::tLog* log)
	:m_pLog(log)
{
	ChangeState(new tStateHalt(this, "the very start"));
	return;
}

void tCamera::operator()()
{
	while (!m_Control_OnExit)
	{
		(*m_pState)();
	}
}

void tCamera::Start()
{
	m_Control_Operation = true;
}

void tCamera::Start(bool exitOnError)
{
	m_Control_ExitOnError = exitOnError;
	Start();
}

void tCamera::Restart()
{
	m_Control_Restart = true;
}

void tCamera::Halt()
{
	m_Control_Operation = false;
}

void tCamera::Exit()
{
	m_Control_Exit = true;
	m_Control_Operation = false;
}

tStatus tCamera::GetStatus() const
{
	//std::lock_guard<std::mutex> Lock(m_MtxState);

	return m_pState->GetStatus();
}

std::string tCamera::GetLastErrorMsg() const
{
	//std::lock_guard<std::mutex> lock(m_mtxstate);
	
	return m_LastErrorMsg;
}

void tCamera::Board_OnReceivedCtrl(std::vector<std::uint8_t>& data)
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	m_ReceivedData.push(data);
}

bool tCamera::IsReceivedData() const
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	return m_ReceivedData.size() > 0;
}

std::vector<std::uint8_t> tCamera::GetReceivedDataChunk()
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	if (m_ReceivedData.size() > 0)
	{
		std::vector<std::uint8_t> Data(std::forward<std::vector<std::uint8_t>>(m_ReceivedData.front()));

		m_ReceivedData.pop();

		return Data;
	}

	return {};
}

void tCamera::ClearReceivedData()
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	while (!m_ReceivedData.empty())
	{
		m_ReceivedData.pop();
	}
}

void tCamera::ChangeState(tState *state)
{
	tState* Prev = m_pState;
	m_pState = state;
	delete Prev;
}

}
}
