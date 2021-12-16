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

//bool tCamera::StartUserTaskScript(const std::string& taskScriptID)
//{
//	if (m_pModFSMachine)
//	{
//		return m_pModFSMachine->StartUserTaskScript(taskScriptID);
//	}
//
//	return false;
//}

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

/*
tCamera::tCamera()
{
	p_log = new tLog(tLog::LogId_Dev_ModCamera);

	mod::tCameraVC0706Settings Settings;

	Settings.Value = 0;
	//Settings.Field.VideoResolution = 0;//don't change
	//Settings.Field.VideoResolution = 1;//160x120
	//Settings.Field.VideoResolution = 2;//320x240
	Settings.Field.VideoResolution = 3;//640x480
	///////////////////////////////////
	///////////////////////////////////
	//TODO: Appropriate settings should be set
	///////////////////////////////////
	///////////////////////////////////

	m_ModCamera = new tModCamera(this, Settings);
}

tCamera::~tCamera()
{
	delete m_ModCamera;

	delete p_log;
}

void tCamera::Tick10ms()
{
	m_ModCamera->Tick10ms();
}

void tCamera::Control()
{
	m_ModCamera->Control();
}

void tCamera::Start()
{
	m_ModCamera->Start();
}

void tCamera::Halt()
{
	m_ModCamera->Halt();
}
*/
}
