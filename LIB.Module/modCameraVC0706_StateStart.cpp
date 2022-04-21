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

void tCameraVC0706::tStateStart::operator()()
{
	if (IsChangeState_ToStop())
		return;

	m_pObj->OnStart();

	if (IsChangeState_ToStop())
		return;

	m_pObj->Board_Reset(false);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));//[TBD] from settings

	if (IsChangeState_ToStop())
		return;

	m_pObj->Board_PowerSupply(true);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));//[TBD] from settings

	if (IsChangeState_ToStop())
		return;
	
	tMsgStatus MsgStatus;

	//[!]Setup: restores UARTHS BR if the chip doesn't respond
	//if (!HandleCmd(tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN, tUARTHSBaudrate::BR115200), MsgStatus, 200) || MsgStatus != tMsgStatus::None)
	//	return false;

	std::string Version;
	if (!HandleCmd(tPacketCmd::MakeGetVersion(m_pObj->m_SN), MsgStatus, Version, 100) || MsgStatus != tMsgStatus::None || !CheckVersion(Version))
	{
		ChangeState(new tStateError(m_pObj, "HandleCmd"));
		return;
	}

	if (IsChangeState_ToStop())
		return;

	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, Version);//[TBD] makes no sense


	//[!]Setup: sets UARTHS BR
	//if (!HandleCmd(tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN, tUARTHSBaudrate::BR460800), MsgStatus, 200) || MsgStatus != tMsgStatus::None)
	//	return false;

	//[!]Setup: sets port UARTHS
	//if (!HandleCmd(tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN, tPort::UARTHS), MsgStatus, 200) || MsgStatus != tMsgStatus::None)
	//	return false;
	//[!]Setup: sets port UART
	//if (!HandleCmd(tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN, tPort::UART), MsgStatus, 200) || MsgStatus != tMsgStatus::None)
	//	return false;

	tResolution Resolution = tResolution::VR160x120;
	if (!HandleCmd(tPacketCmd::MakeReadDataReg_Resolution(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN), MsgStatus, Resolution, 100) || MsgStatus != tMsgStatus::None)
	{
		ChangeState(new tStateError(m_pObj, "HandleCmd"));
		return;
	}

	if (IsChangeState_ToStop())
		return;

	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "Resolution: " + ToString(Resolution));

	const tResolution SettingsResolution = tResolution::VR640x480;//[TBD] from settings

	if (Resolution != SettingsResolution)
	{
		if (!HandleCmd(tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN, SettingsResolution), MsgStatus, 100, 2) || MsgStatus != tMsgStatus::None)
		{
			ChangeState(new tStateError(m_pObj, "HandleCmd"));
			return;
		}

		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "Set Resolution: " + ToString(Resolution));
	}

	if (IsChangeState_ToStop())
		return;

	tUARTHSBaudrate UARTHSBaudrate = tUARTHSBaudrate::BR921600;
	if (!HandleCmd(tPacketCmd::MakeReadDataReg_PortUARTHS(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN), MsgStatus, UARTHSBaudrate, 100) || MsgStatus != tMsgStatus::None)
	{
		ChangeState(new tStateError(m_pObj, "HandleCmd"));
		return;
	}

	if (IsChangeState_ToStop())
		return;

	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "BRHS: " + ToString(UARTHSBaudrate));

	tUARTBaudrate UARTBaudrate = tUARTBaudrate::BR9600;
	if (!HandleCmd(tPacketCmd::MakeReadDataReg_PortUART(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN), MsgStatus, UARTBaudrate, 100) || MsgStatus != tMsgStatus::None)
	{
		ChangeState(new tStateError(m_pObj, "HandleCmd"));
		return;
	}

	if (IsChangeState_ToStop())
		return;

	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "BR: " + ToString(UARTBaudrate));

	m_pObj->OnReady();

	ChangeState(new tStateOperation(m_pObj));
}

}
