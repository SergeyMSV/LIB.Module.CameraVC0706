#include "modCameraVC0706.h"

namespace mod
{

tCameraVC0706::tStateHalt::tStateHalt(tCameraVC0706* obj, const std::string& value)
	:tStateHalt(obj, value, false)
{

}

tCameraVC0706::tStateHalt::tStateHalt(tCameraVC0706* obj, const std::string& value, bool error)
	:tState(obj), m_Error(error)
{
	std::stringstream SStr;
	SStr << "tStateHalt: " << value;
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, SStr.str());

/*	if (m_pObj->IsControlRestart())
	{
		m_pObj->m_Control_Restart = false;
	}*/
}

bool tCameraVC0706::tStateHalt::Go()
{
	if (m_pObj->m_Control_Exit)
		return false;

	if (m_Error && m_pObj->m_Control_ExitOnError)
		return false;

	if (m_pObj->IsControlOperation())
	{
		ChangeState(new tStateStart(m_pObj/*, "start...s"*/));
		return true;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	return true;
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