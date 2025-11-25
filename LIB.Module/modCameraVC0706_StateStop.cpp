#include "modCameraVC0706.h"

namespace mod
{
namespace vc0706
{

tCamera::tStateStop::tStateStop(tCamera* obj)
	:tState(obj, "StateStop")
{

}

void tCamera::tStateStop::operator()()
{
	ChangeState(new tStateHalt(m_pObj, "stop"));
}

}
}
