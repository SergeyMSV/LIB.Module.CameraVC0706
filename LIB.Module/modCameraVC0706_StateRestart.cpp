#include "modCameraVC0706.h"
/*
namespace mod
{

tCameraVC0706::tStateRestart::tStateRestart(tCameraVC0706 *obj)
	:tState(obj)
{
#ifdef LIB_MODULE_LOG
	p_obj->p_log->WriteLine("tStateRestart");
#endif//LIB_MODULE_LOG

	//m_ErrTimer.SetValue(utils::tTime(10));//[#]
}

void tCameraVC0706::tStateRestart::Control()
{
	//if (!m_ErrTimer.GetState())
	//{
	//	p_obj->OnFailed(tCameraVC0706Error_StateRestart_ErrTimer);

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

			p_obj->Board_PowerSupply(false);
			//p_obj->Board_Reset(true);

			p_obj->OnRestart();

			m_Timer.SetValue(utils::tTime(5));//[#] switched off for 5 s

			break;
		}
		default:
		{
			ChangeState(tStateStart::Instance(p_obj));
			return;
		}
		}
	}
}

void tCameraVC0706::tStateRestart::OnReceivedMsg(std::vector<char>& data)
{

}

}*/