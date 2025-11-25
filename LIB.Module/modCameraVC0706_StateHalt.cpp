#include "modCameraVC0706.h"

namespace mod
{
namespace vc0706
{

tCameraVC0706::tStateHalt::tStateHalt(tCameraVC0706* obj, const std::string& value)
	:tStateHalt(obj, value, false)
{

}

tCameraVC0706::tStateHalt::tStateHalt(tCameraVC0706* obj, const std::string& value, bool error)
	:tState(obj, "StateHalt"), m_Error(error)
{
	if (m_pObj->IsControlRestart())
	{
		m_pObj->m_Control_Restart = false;
	}
}

void tCameraVC0706::tStateHalt::operator()()
{
	if (!m_Off)
	{
		m_Off = true;
		m_pObj->Board_PowerSupply(false);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));//[TBD] from settings
		m_pObj->Board_Reset(false);
	}

	if (m_pObj->m_Control_Exit)
	{
		m_pObj->m_Control_OnExit = true;
		return;
	}

	if (m_Error && m_pObj->m_Control_ExitOnError)
	{
		m_pObj->m_Control_OnExit = true;
		return;
	}

	if (m_pObj->IsControlOperation())
	{
		ChangeState(new tStateStart(m_pObj/*, "start...s"*/));
		return;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

}
}

/*
namespace mod
{

tCameraVC0706::tStateHalt::tStateHalt(tCameraVC0706 *obj, bool halted)
	:tState(obj)
{
#ifdef LIB_MODULE_LOG
	p_obj->p_log->WriteLine("tStateHalt");
#endif//LIB_MODULE_LOG

	m_Halted = halted;
}

void tCameraVC0706::tStateHalt::Control()
{
	if (!m_Halted)
	{
		m_Halted = true;

		//p_obj->Board_Reset(true);

		p_obj->Board_PowerSupply(false);

		p_obj->OnHalt();
	}
}

void tCameraVC0706::tStateHalt::Start()
{
	ChangeState(tStateStart::Instance(p_obj));
	return;
}

}*/
