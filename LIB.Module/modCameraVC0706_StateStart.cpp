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
	using tPacketCmd = utils::packet_CameraVC0706::tPacketCmd;
	using tPacketRet = utils::packet_CameraVC0706::tPacketRet;

	using tPayloadCmd = utils::packet_CameraVC0706::tPayloadCmd;
	using tPayloadRet = utils::packet_CameraVC0706::tPayloadRet;

	if (!m_pObj->IsControlOperation())
		return false;

	tPacketCmd Packet = tPacketCmd(tPayloadCmd::MakeGetVersion(0x00));//[#]SN

	utils::tVectorUInt8 Data = Packet.ToVector();

		//if (m_pObj->IsControlOperation())
		//{
		//	ChangeState(new tStateStart(m_pObj, "start...s"));
		//	return true;
		//}*/


	//p_obj->OnStart();

	//p_obj->Board_Reset(false);
	//p_obj->Board_PowerSupply(true);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	// p_obj->OnFailed(tCameraVC0706Error_StateStart_ErrTimer);
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