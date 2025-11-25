#include "devCamera.h"

namespace dev
{

tCamera::tCamera(utils::log::tLog* log, boost::asio::io_context& io)
	:m_pLog(log), m_pIO(&io)
{
	m_pMod = new tModCamera(this);
}

tCamera::~tCamera()
{
	delete m_pMod;
}

void tCamera::operator()()
{
	if (m_pMod)
	{
		if (m_StartAuto)
		{
			m_StartAuto = false;

			m_pMod->Start(true);
		}

		(*m_pMod)();
	}
}

void tCamera::Start()
{
	if (m_pMod)
	{
		m_pMod->Start();
	}
}

void tCamera::Restart()
{
	if (m_pMod)
	{
		m_pMod->Restart();
	}
}

void tCamera::Halt()
{
	if (m_pMod)
	{
		m_pMod->Halt();
	}
}

void tCamera::Exit()
{
	if (m_pMod)
	{
		m_pMod->Exit();
	}
}

mod::vc0706::tStatus tCamera::GetStatus() const
{
	if (m_pMod)
	{
		return m_pMod->GetStatus();
	}

	return mod::vc0706::tStatus::Unknown;
}

std::string tCamera::GetLastErrorMsg() const
{
	if (m_pMod)
	{
		return m_pMod->GetLastErrorMsg();
	}

	return {};
}

}
