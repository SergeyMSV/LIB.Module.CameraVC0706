#include "modCameraVC0706.h"
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