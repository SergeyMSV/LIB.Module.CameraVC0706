#include "modCameraVC0706.h"

#include "utilsPacketCameraVC0706.h"

namespace mod
{

tCameraVC0706::tStateStart::tStateStart(tCameraVC0706* obj)
	:tState(obj, "StateStart")
{
	//m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, SStr.str());

	if (m_pObj->IsControlRestart())
	{
		m_pObj->m_Control_Restart = false;
	}
}

bool tCameraVC0706::tStateStart::Go()
{
	using namespace utils::packet_CameraVC0706;

	if (!m_pObj->IsControlOperation())//[TBD] it is to be placed after each separate operation in order to have possibility to leave this thread in case of ChangeState or something.
		return false;

	m_pObj->OnStart();

	m_pObj->Board_Reset(false);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));//[TBD] from settings

	m_pObj->Board_PowerSupply(true);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));//[TBD] from settings

	tPacketRet Rsp;

	m_ReceivedData.clear();

	m_pObj->Board_Send(tPacketCmd::MakeGetVersion(0x00).ToVector());//[#]SN
	if (!WaitForReceivedData(100))//[TBD] calculate according to BR and waiting for camera response
		return false;

	if (tPacketRet::Find(m_ReceivedData, Rsp) > 0 && Rsp.GetMsgId() == tMsgId::GetVersion)
	{
		std::string Version;
		tPacketRet::ParseGetVersion(Rsp, Version);

		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, Version);

		if (Version != "VC0703 1.00")
			return false;
	}

	m_ReceivedData.clear();

	m_pObj->Board_Send(tPacketCmd::MakeReadDataReg_VideoResolution(tMemoryDataReg::I2C_EEPROM, 0x00).ToVector());//[#]SN
	if (!WaitForReceivedData(100))//[TBD] calculate according to BR and waiting for camera response
		return false;

	if (tPacketRet::Find(m_ReceivedData, Rsp) > 0 && Rsp.GetMsgId() == tMsgId::ReadDataReg)
	{
		tVideoResolution Value = tVideoResolution::VR160x120;
		tPacketRet::ParseReadDataReg_VideoResolution(Rsp, Value);

		const tVideoResolution SettingsValue = tVideoResolution::VR320x240;//[TBD] from settings

		if (Value != SettingsValue)
		{
			m_pObj->Board_Send(tPacketCmd::MakeWriteDataReg_VideoResolution(tMemoryDataReg::I2C_EEPROM, 0x00, SettingsValue).ToVector());//[#]SN
			if (!WaitForReceivedData(100))
				return false;

			if (tPacketRet::Find(m_ReceivedData, Rsp) == 0 || Rsp.GetMsgId() != tMsgId::WriteDataReg || Rsp.GetMsgStatus() != tMsgStatus::None)
				return false;
		}

		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "Vi");

		//if (Version != "VC0703 1.00")
		//	return false;
	}

	m_ReceivedData.clear();

	m_pObj->Board_Send(tPacketCmd::MakeReadDataReg_PortUARTHS(tMemoryDataReg::I2C_EEPROM, 0x00).ToVector());//[#]SN
	if (!WaitForReceivedData(100))
		return false;

	if (tPacketRet::Find(m_ReceivedData, Rsp) > 0 && Rsp.GetMsgId() == tMsgId::ReadDataReg)
	{
		tUARTHSBaudrate Baudrate = tUARTHSBaudrate::BR921600;
		tPacketRet::ParseReadDataReg_PortUARTHS(Rsp, Baudrate);

		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "BRHS");

		//if (Version != "VC0703 1.00")
		//	return false;
	}

	m_ReceivedData.clear();

	m_pObj->Board_Send(tPacketCmd::MakeReadDataReg_PortUART(tMemoryDataReg::I2C_EEPROM, 0x00).ToVector());//[#]SN
	if (!WaitForReceivedData(100))
		return false;

	if (tPacketRet::Find(m_ReceivedData, Rsp) > 0 && Rsp.GetMsgId() == tMsgId::ReadDataReg)
	{
		tUARTBaudrate Baudrate = tUARTBaudrate::BR9600;
		tPacketRet::ParseReadDataReg_PortUART(Rsp, Baudrate);

		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Green, "BR");

		//if (Version != "VC0703 1.00")
		//	return false;
	}


	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	// p_obj->OnFailed(tCameraVC0706Error_StateStart_ErrTimer);

	m_pObj->OnReady();

	ChangeState(tStateIdle::Instance(m_pObj));
	return true;
}


}



/*
namespace mod
{

tCameraVC0706::tStateStart::tStateStart(tCameraVC0706 *obj)
	:tState(obj)
{
#ifdef LIB_MODULE_LOG
	p_obj->p_log->WriteLine("tStateStart");
#endif//LIB_MODULE_LOG

	//m_ErrTimer.SetValue(utils::tTime(10));//[#]
}

void tCameraVC0706::tStateStart::Control()
{
	//if (!m_ErrTimer.GetState())
	//{
	//	p_obj->OnFailed(tCameraVC0706Error_StateStart_ErrTimer);

	//	ChangeState(tStateRestart::Instance(p_obj));
	//	return;
	//}

	if (m_Step % 2 == 0 && !m_Timer.GetState())
	{
		switch (m_Step++)
		{
		case 0:
		{
			m_Step++;

			p_obj->OnStart();

			//p_obj->Board_Reset(false);
			p_obj->Board_PowerSupply(true);

			m_Timer.SetValue(utils::tTime(2));//[#]Wait for 2 sec upon Reset

			break;
		}
		default:
		{
			ChangeState(tStateInitialize::Instance(p_obj));
			return;
		}
		}
	}
}

void tCameraVC0706::tStateStart::OnReceivedMsg(std::vector<char>& data)
{

}

}*/