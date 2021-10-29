#include "devCamera.h"
/*
namespace dev
{

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

}
*/