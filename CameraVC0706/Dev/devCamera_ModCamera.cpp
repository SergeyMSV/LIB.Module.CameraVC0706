#include "devCamera.h"
#include "devSettings.h"

#include <utilsFile.h>
#include <utilsPath.h>

namespace dev
{

tCam::tModCam::tModCam(tCam* obj)
	:mod::vc0706::tCamera(obj->m_pLog), m_pObj(obj), m_BoardCtrl(this, *obj->m_pIO)
{
	if (!g_Settings.SerialPort.DataID.empty())
		m_BoardData = new tBoardData(this, *obj->m_pIO);
}

tCam::tModCam::~tModCam()
{
	delete m_BoardData;
}

mod::vc0706::tSettings tCam::tModCam::GetSettings()
{
	return g_Settings.Camera;
}

void tCam::tModCam::OnStart()
{
	m_pObj->m_pLog->WriteLine(true, "OnStart");
}

void tCam::tModCam::OnReady()
{
	m_pObj->m_pLog->WriteLine(true, "OnReady");

	//OnModReady();
}

void tCam::tModCam::OnHalt()
{
	m_pObj->m_pLog->WriteLine(true, "OnHalt");

	//OnModHalt();
}

//void tCam::tModCam::OnRestart()
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
//void tCam::tModCam::OnFailed(mod::vc0706::tCameraVC0706Error cerr)
//{
//	p_obj->p_log->WriteLine("OnFailed: 0x" + utils::ToString((unsigned int)cerr, utils::tRadix_16));
//
//	OnModFailed();
//}

void tCam::tModCam::OnImageReady()
{
	std::string DTStr = utils::GetDateTime();
	std::string Path = g_Settings.Picture.Path + "/";
	std::string FileName = g_Settings.Picture.Prefix + DTStr + ".jpg";
	m_FileName = Path + FileName;
	m_FileNameTemp = Path + g_FileNameTempPrefix + FileName + ".tmp";
	m_File.open(m_FileNameTemp, std::ios::out | std::ios::binary);

	m_pObj->m_pLog->WriteLine(true, "Image: Ready", utils::log::tColor::LightBlue);
}

void tCam::tModCam::OnImageChunk(std::vector<std::uint8_t>& data)
{
	if (!m_File.is_open())
		return;

	m_File.write(reinterpret_cast<char*>(data.data()), data.size());

	m_pObj->m_pLog->WriteLine(true, "Image: Chunk " + std::to_string(data.size()), utils::log::tColor::LightBlue);
	//m_pObj->m_pLog->WriteHex(true, "Image: Chunk", utils::log::tColor::LightBlue, data);
}

void tCam::tModCam::OnImageComplete()
{
	m_File.close();

	std::rename(m_FileNameTemp.c_str(), m_FileName.c_str());

	utils::file::RemoveFilesOutdated(g_Settings.Picture.Path, g_Settings.Picture.Prefix, g_Settings.Picture.QtyMax);

	m_pObj->m_pLog->WriteLine(true, "Image: Complete", utils::log::tColor::LightBlue);
}

void tCam::tModCam::Board_PowerSupply(bool state)
{
	std::stringstream SStream;
	SStream << "Board_PowerSupply: " << state;
	m_pObj->m_pLog->WriteLine(true, SStream.str(), utils::log::tColor::LightMagenta);
}

void tCam::tModCam::Board_Reset(bool state)
{
	std::stringstream SStream;
	SStream << "Board_Reset:       " << state;
	m_pObj->m_pLog->WriteLine(true, SStream.str(), utils::log::tColor::LightMagenta);
}

bool tCam::tModCam::Board_SendCtrl(const std::vector<std::uint8_t>& data)
{
	return m_BoardCtrl.Send(data);
}

void tCam::tModCam::OnReceivedCtrl(std::vector<std::uint8_t>& data)
{
	Board_OnReceivedCtrl(data);
}

}
