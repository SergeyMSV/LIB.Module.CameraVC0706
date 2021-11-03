#include "modCameraVC0706.h"

namespace mod
{

tCameraVC0706::tStateStart::tStateStart(tCameraVC0706* obj, const std::string& value)
	:tState(obj, "StateStart")
{
	std::stringstream SStr;
	SStr << "tStateStart: " << value;
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, SStr.str());

	if (m_pObj->IsControlRestart())
	{
		m_pObj->m_Control_Restart = false;
	}
}

//void tCameraVC0706::tStateStart::OnTaskScriptDone()
//{
//	m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "OnTaskScriptDone");
//
//	if (m_NextState_Stop)
//	{
//		ChangeState(new tStateStop(m_pObj, "start single"));
//		return;
//	}
//
//	ChangeState(new tStateOperation(m_pObj));
//	return;
//}
//
//void tCameraVC0706::tStateStart::OnTaskScriptFailed(const std::string& msg)
//{
//	m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "OnTaskScriptFailed: " + msg);
//
//	ChangeState(new tStateError(m_pObj, "start"));
//	return;
//}

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