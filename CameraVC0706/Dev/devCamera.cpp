#include "devCamera.h"

namespace dev
{

tCam::tCam(utils::log::tLog* log, boost::asio::io_context& io)
	:m_pLog(log), m_pIO(&io)
{
	m_pMod = new tModCam(this);
}

tCam::~tCam()
{
	delete m_pMod;
}

void tCam::operator()()
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

void tCam::Start()
{
	if (m_pMod)
	{
		m_pMod->Start();
	}
}

void tCam::Restart()
{
	if (m_pMod)
	{
		m_pMod->Restart();
	}
}

void tCam::Halt()
{
	if (m_pMod)
	{
		m_pMod->Halt();
	}
}

void tCam::Exit()
{
	if (m_pMod)
	{
		m_pMod->Exit();
	}
}

mod::vc0706::tStatus tCam::GetStatus() const
{
	if (m_pMod)
	{
		return m_pMod->GetStatus();
	}

	return mod::vc0706::tStatus::Unknown;
}

std::string tCam::GetLastErrorMsg() const
{
	if (m_pMod)
	{
		return m_pMod->GetLastErrorMsg();
	}

	return {};
}

}
