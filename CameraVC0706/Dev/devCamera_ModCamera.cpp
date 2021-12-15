#include "devCamera.h"
#include "devSettings.h"
/*
void WriteToFileImage(std::vector<char>& data);//TEST
void WriteToFileImageComplete();//TEST
void OnModReady();
void OnModHalt();
void OnModFailed();

extern std::pair<std::string, int> g_DefaultSettings_SerialPort;
*/
namespace dev
{

tCamera::tModCamera::tModCamera(tCamera* obj)
	:mod::tCameraVC0706(obj->m_pLog), m_pObj(obj), m_Board(this, *obj->m_pIO)
{

}

tCamera::tModCamera::~tModCamera()
{

}

//mod::tGnssTaskScript tGNSS::tModGnssReceiver::GetTaskScript(const std::string& id, bool userTaskScript)
//{
//	return g_Settings.GetTaskScript(id, userTaskScript);
//}
//
//mod::tGnssSettingsNMEA tGNSS::tModGnssReceiver::GetSettingsNMEA()
//{
//	return g_Settings.GetSettingsNMEA();
//}
//
//void tCamera::tModCamera::OnChanged(const mod::tGnssDataSet& value)
//{
//	const std::string FileName = g_Settings.Output.Path + "/" + g_Settings.Output.FileName;
//	const std::string FileNameTemp = FileName + ".tmp";
//	std::fstream File = std::fstream(FileNameTemp, std::ios::out);
//	if (File.is_open())
//	{
//		File << value.ToJSON();
//		File.close();
//	}
//	std::remove(FileName.c_str());
//	std::rename(FileNameTemp.c_str(), FileName.c_str());
//
//	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, value.ToJSON());
//}

void tCamera::tModCamera::Board_PowerSupply(bool state)
{
	std::stringstream SStream;
	SStream << "Board_PowerSupply: " << state;
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightMagenta, SStream.str());
}

void tCamera::tModCamera::Board_Reset(bool state)
{
	std::stringstream SStream;
	SStream << "Board_Reset:       " << state;
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightMagenta, SStream.str());
}

bool tCamera::tModCamera::Board_Send(const utils::tVectorUInt8& data)
{
	return m_Board.Send(data);
}

void tCamera::tModCamera::OnReceived(utils::tVectorUInt8& data)
{
	Board_OnReceived(data);
}



/*
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
*/

void tCamera::tModCamera::OnStart()
{
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, "OnStart");
}

void tCamera::tModCamera::OnReady()
{
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, "OnReady");

	//OnModReady();
}

void tCamera::tModCamera::OnHalt()
{
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, "OnHalt");

	//OnModHalt();
}
/*
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
*/
}
