#include "devCamera.h"

namespace dev
{

tCamera::tCamera(utils::tLog* log, boost::asio::io_context& io)
	:m_pLog(log), m_pIO(&io)
{
	m_pModFSMachine = new tModCamera(this);
}

tCamera::~tCamera()
{
	delete m_pModFSMachine;
}

void tCamera::operator()()
{
	if (m_pModFSMachine)
	{
		if (m_StartAuto)
		{
			m_StartAuto = false;

			m_pModFSMachine->Start(true);
		}

		(*m_pModFSMachine)();
	}
}

void tCamera::Start()
{
	if (m_pModFSMachine)
	{
		m_pModFSMachine->Start();
	}
}

void tCamera::Restart()
{
	if (m_pModFSMachine)
	{
		m_pModFSMachine->Restart();
	}
}

void tCamera::Halt()
{
	if (m_pModFSMachine)
	{
		m_pModFSMachine->Halt();
	}
}

void tCamera::Exit()
{
	if (m_pModFSMachine)
	{
		m_pModFSMachine->Exit();
	}
}

utils::tDevStatus tCamera::GetStatus() const
{
	if (m_pModFSMachine)
	{
		return m_pModFSMachine->GetStatus();
	}

	return utils::tDevStatus::Unknown;
}

std::string tCamera::GetLastErrorMsg() const
{
	if (m_pModFSMachine)
	{
		return m_pModFSMachine->GetLastErrorMsg();
	}

	return {};
}

}
