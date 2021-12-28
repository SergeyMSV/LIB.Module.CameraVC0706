#include "modCameraVC0706.h"

using namespace utils::packet_CameraVC0706;

namespace mod
{

tCameraVC0706::tStateStart::tStateStart(tCameraVC0706* obj)
	:tState(obj, "StateStart")
{
	if (m_pObj->IsControlRestart())
	{
		m_pObj->m_Control_Restart = false;
	}
}

bool tCameraVC0706::tStateStart::Go()
{
	if (!m_pObj->IsControlOperation())
		return false;

	m_pObj->OnStart();

	if (!m_pObj->IsControlOperation())
		return false;

	m_pObj->Board_Reset(false);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));//[TBD] from settings

	if (!m_pObj->IsControlOperation())
		return false;

	m_pObj->Board_PowerSupply(true);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));//[TBD] from settings

	if (!m_pObj->IsControlOperation())
		return false;
	
	tMsgStatus MsgStatus;

	std::string Version;
	if (!HandleCmd(tPacketCmd::MakeGetVersion(m_pObj->m_SN), MsgStatus, Version, 100) || MsgStatus != tMsgStatus::None || !CheckVersion(Version))
		return false;

	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, Version);//[TBD] makes no sense

	tResolution Resolution = tResolution::VR160x120;
	if (!HandleCmd(tPacketCmd::MakeReadDataReg_Resolution(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN), MsgStatus, Resolution, 100) || MsgStatus != tMsgStatus::None)
		return false;

	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "Resolution: " + ToString(Resolution));

	const tResolution SettingsResolution = tResolution::VR640x480;//[TBD] from settings

	if (Resolution != SettingsResolution)
	{
		if (!HandleCmd(tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN, SettingsResolution), MsgStatus, 100) || MsgStatus != tMsgStatus::None)
			return false;

		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "Set Resolution: " + ToString(Resolution));
	}

	tUARTHSBaudrate UARTHSBaudrate = tUARTHSBaudrate::BR921600;
	if (!HandleCmd(tPacketCmd::MakeReadDataReg_PortUARTHS(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN), MsgStatus, UARTHSBaudrate, 100) || MsgStatus != tMsgStatus::None)
		return false;

	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "BRHS: " + ToString(UARTHSBaudrate));

	tUARTBaudrate UARTBaudrate = tUARTBaudrate::BR9600;
	if (!HandleCmd(tPacketCmd::MakeReadDataReg_PortUART(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN), MsgStatus, UARTBaudrate, 100) || MsgStatus != tMsgStatus::None)
		return false;

	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "BR: " + ToString(UARTBaudrate));

	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	// p_obj->OnFailed(tCameraVC0706Error_StateStart_ErrTimer);

	m_pObj->OnReady();

	ChangeState(tStateOperation::Instance(m_pObj));
	return true;
}

}
