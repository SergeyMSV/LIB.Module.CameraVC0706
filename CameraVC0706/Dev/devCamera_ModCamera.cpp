#include "devCamera.h"
#include "devSettings.h"

namespace dev
{

tCamera::tModCamera::tModCamera(tCamera* obj)
	:mod::tCameraVC0706(obj->m_pLog), m_pObj(obj), m_Board(this, *obj->m_pIO)
{

}

tCamera::tModCamera::~tModCamera()
{

}

mod::tCameraVC0706Settings tCamera::tModCamera::GetSettings()
{
	return g_Settings.Camera;
}

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
*/

void tCamera::tModCamera::OnImageReady()
{
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightBlue, "Image: Ready");
}

void tCamera::tModCamera::OnImageChunk(utils::tVectorUInt8& data)
{
	m_pObj->m_pLog->WriteHex(true, utils::tLogColour::LightBlue, "Image: Chunk", data);

	//[TBD] write image in a file

//const std::string FileName = g_Settings.Output.Path + "/" + g_Settings.Output.FileName;
//const std::string FileNameTemp = FileName + ".tmp";
//std::fstream File = std::fstream(FileNameTemp, std::ios::out);
//if (File.is_open())
//{
//	File << value.ToJSON();
//	File.close();
//}
//std::remove(FileName.c_str());
//std::rename(FileNameTemp.c_str(), FileName.c_str());

//m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, value.ToJSON());
}

void tCamera::tModCamera::OnImageComplete()
{
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightBlue, "Image: Complete");
}

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
	m_pObj->m_pLog->WriteHex(true, utils::tLogColour::LightYellow, "Send", data);//[TBD]makes no sense
	return m_Board.Send(data);
}

void tCamera::tModCamera::OnReceived(utils::tVectorUInt8& data)
{
	m_pObj->m_pLog->WriteHex(true, utils::tLogColour::Yellow, "Received", data);//[TBD]makes no sense
	Board_OnReceived(data);
}

}
