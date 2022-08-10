#include "devCamera.h"
#include "devSettings.h"

#include <utilsPath.h>

namespace dev
{

tCamera::tModCamera::tModCamera(tCamera* obj)
	:mod::tCameraVC0706(obj->m_pLog), m_pObj(obj), m_BoardCtrl(this, *obj->m_pIO)
{
	if (!g_Settings.SerialPort.DataID.empty())
		m_BoardData = new tBoardData(this, *obj->m_pIO);
}

tCamera::tModCamera::~tModCamera()
{
	delete m_BoardData;
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

//void tCamera::tModCamera::OnRestart()
//{
//	p_obj->p_log->WriteLine("OnRestart");
//
//	if (m_Board)
//	{
//		tBoard *Board = m_Board;
//
//		m_Board = 0;
//
//		delete Board;
//	}
//}
//
//void tCamera::tModCamera::OnFailed(mod::tCameraVC0706Error cerr)
//{
//	p_obj->p_log->WriteLine("OnFailed: 0x" + utils::ToString((unsigned int)cerr, utils::tRadix_16));
//
//	OnModFailed();
//}

void tCamera::tModCamera::OnImageReady()
{
	m_FileName = g_Settings.Output.Path + "/" + g_Settings.Output.FileName;
	const std::string FileNameTemp = m_FileName + ".tmp";
	m_File.open(FileNameTemp, std::ios::out | std::ios::binary);

	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightBlue, "Image: Ready");
}

void tCamera::tModCamera::OnImageChunk(utils::tVectorUInt8& data)
{
	if (!m_File.is_open())
		return;

	m_File.write(reinterpret_cast<char*>(data.data()), data.size());

	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightBlue, "Image: Chunk " + std::to_string(data.size()));
	//m_pObj->m_pLog->WriteHex(true, utils::tLogColour::LightBlue, "Image: Chunk", data);
}

void tCamera::tModCamera::OnImageComplete()
{
	m_File.close();

	const std::string FileNameTemp = m_FileName + ".tmp";

	std::remove(m_FileName.c_str());
	std::rename(FileNameTemp.c_str(), m_FileName.c_str());

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

bool tCamera::tModCamera::Board_SendCtrl(const utils::tVectorUInt8& data)
{
	return m_BoardCtrl.Send(data);
}

void tCamera::tModCamera::OnReceivedCtrl(utils::tVectorUInt8& data)
{
	Board_OnReceivedCtrl(data);
}

}
