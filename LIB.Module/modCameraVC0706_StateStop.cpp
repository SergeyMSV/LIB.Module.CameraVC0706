#include "modCameraVC0706.h"

namespace mod
{

tCameraVC0706::tStateStop::tStateStop(tCameraVC0706* obj)
	:tState(obj, "StateStop")
{

}

bool tCameraVC0706::tStateStop::Go()
{
	ChangeState(new tStateHalt(m_pObj, "stop"));
	return true;
}

}
