#include "modCameraVC0706.h"

using namespace utils::packet::vc0706;

namespace mod
{
namespace vc0706
{

tCameraVC0706::tStateStart::tStateStart(tCameraVC0706* obj)
	:tState(obj, "StateStart")
{
	if (m_pObj->IsControlRestart())
		m_pObj->m_Control_Restart = false;
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

	//HandleCmd(tPacketCmd::MakeSystemReset(m_pObj->m_SN), MsgStatus, 100, 1);
	//std::this_thread::sleep_for(std::chrono::milliseconds(100));

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

	m_pObj->m_pLog->WriteLine(true, Version, utils::log::tColor::Green); // [TBD] makes no sense

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

	m_pObj->m_pLog->WriteLine(true, "Resolution: " + ToString(Resolution), utils::log::tColor::Green);

	const tResolution SettingsResolution = m_pObj->GetSettings().Resolution;
	if (Resolution != SettingsResolution)
	{
		if (!HandleCmd(tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN, SettingsResolution), MsgStatus, 100, 2) || MsgStatus != tMsgStatus::None)
		{
			ChangeState(new tStateError(m_pObj, "HandleCmd"));
			return;
		}

		m_pObj->m_pLog->WriteLine(true, "Set Resolution: " + ToString(Resolution), utils::log::tColor::Green);
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

	m_pObj->m_pLog->WriteLine(true, "BRHS: " + ToString(UARTHSBaudrate), utils::log::tColor::Green);

	const tSettings Settings = m_pObj->GetSettings();

	tUARTHSBaudrate UARTHSBaudrateSet = ToUARTHSBaudrate(Settings.GetPortDataBR());

	if (UARTHSBaudrateSet != tUARTHSBaudrate::BR_ERR && UARTHSBaudrateSet != UARTHSBaudrate)
	{
		if (!HandleCmd(tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN, UARTHSBaudrateSet), MsgStatus, UARTHSBaudrate, 100) || MsgStatus != tMsgStatus::None)
			return;

		if (!HandleCmd(tPacketCmd::MakeReadDataReg_PortUARTHS(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN), MsgStatus, UARTHSBaudrate, 100) || MsgStatus != tMsgStatus::None)
			return;

		m_pObj->m_pLog->WriteLine(true, "BRHS: " + ToString(UARTHSBaudrate), utils::log::tColor::LightGreen);

		HandleCmd(tPacketCmd::MakeSystemReset(m_pObj->m_SN), MsgStatus, 100, 1);

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		ChangeState(new tStateStart(m_pObj));
		return;
	}

	tUARTBaudrate UARTBaudrate = tUARTBaudrate::BR9600;
	if (!HandleCmd(tPacketCmd::MakeReadDataReg_PortUART(tMemoryDataReg::I2C_EEPROM, m_pObj->m_SN), MsgStatus, UARTBaudrate, 100) || MsgStatus != tMsgStatus::None)
	{
		ChangeState(new tStateError(m_pObj, "HandleCmd"));
		return;
	}

	if (IsChangeState_ToStop())
		return;

	m_pObj->m_pLog->WriteLine(true, "BR: " + ToString(UARTBaudrate), utils::log::tColor::Green);

	m_pObj->OnReady();

	ChangeState(new tStateOperation(m_pObj));
}

}
}
