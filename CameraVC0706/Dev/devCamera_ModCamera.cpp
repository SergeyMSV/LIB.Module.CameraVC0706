#include "devCamera.h"
/*
void WriteToFileImage(std::vector<char>& data);//TEST
void WriteToFileImageComplete();//TEST
void OnModReady();
void OnModHalt();
void OnModFailed();

extern std::pair<std::string, int> g_DefaultSettings_SerialPort;

namespace dev
{

tCamera::tModCamera::tModCamera(tCamera* obj, mod::tCameraVC0706Settings settings)
	:mod::tCameraVC0706(obj->p_log, settings), p_obj(obj)
{
	m_Board = 0;
}

tCamera::tModCamera::~tModCamera()
{
	if (m_Board)
		delete m_Board;
}

void tCamera::tModCamera::Control()
{
	if (m_Board)
		m_Board->Control();

	mod::tCameraVC0706::Control();
}

void tCamera::tModCamera::Board_PowerSupply(bool state)
{
	if (m_Board)
		m_Board->PowerSupply(state);
}

void tCamera::tModCamera::Board_Reset(bool state)
{
	if (m_Board)
		m_Board->Reset(state);
}

void tCamera::tModCamera::Board_SetSerialPort(int value)
{
	p_obj->p_log->WriteLine("Board_SetSerialPort: " + utils::ToString(value, utils::tRadix_10));

	g_DefaultSettings_SerialPort.second = value;

	if (m_Board)
	{
		tBoard *Board = m_Board;

		m_Board = 0;

		delete Board;

		Board = new tBoard(this);

		m_Board = Board;
	}
}

bool tCamera::tModCamera::Board_Send(std::vector<char>& data)
{
	if (m_Board)
		return m_Board->Send(data);

	return false;
}

void tCamera::tModCamera::OnReceived(std::vector<char>& data)
{
	Board_OnReceived(data);
}

void tCamera::tModCamera::OnStart()
{
	p_obj->p_log->WriteLine("OnStart");

	if (m_Board == 0)
	{
		m_Board = new tBoard(this);
	}
}

void tCamera::tModCamera::OnReady()
{
	p_obj->p_log->WriteLine("OnReady");

	OnModReady();
}

void tCamera::tModCamera::OnHalt()
{
	p_obj->p_log->WriteLine("OnHalt");

	if (m_Board)
	{
		tBoard *Board = m_Board;

		m_Board = 0;

		delete Board;
	}

	OnModHalt();
}

void tCamera::tModCamera::OnRestart()
{
	p_obj->p_log->WriteLine("OnRestart");

	if (m_Board)
	{
		tBoard *Board = m_Board;

		m_Board = 0;

		delete Board;
	}
}

void tCamera::tModCamera::OnFailed(mod::tCameraVC0706Error cerr)
{
	p_obj->p_log->WriteLine("OnFailed: 0x" + utils::ToString((unsigned int)cerr, utils::tRadix_16));

	OnModFailed();
}

void tCamera::tModCamera::OnImageReady()
{
	p_obj->p_log->WriteLine("OnImageReady");
}

void tCamera::tModCamera::OnImageChunk(std::vector<char>& data)
{
	p_obj->p_log->WriteHex("OnImageChunk", data);

	WriteToFileImage(data);
}

void tCamera::tModCamera::OnImageComplete()
{
	p_obj->p_log->WriteLine("OnImageComplete");

	WriteToFileImageComplete();
}

#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG
void tCamera::tModCamera::OnGetConfig(std::vector<char>& data)
{
	p_obj->p_log->WriteHex("OnGetConfig", data);
}

void tCamera::tModCamera::OnSetConfig()
{
	p_obj->p_log->WriteLine("OnSetConfig");
}
#endif//LIB_MODULE_CAMERA_VC0706_CONFIG

}
*/