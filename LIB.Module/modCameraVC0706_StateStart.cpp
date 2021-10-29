#include "modCameraVC0706.h"
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