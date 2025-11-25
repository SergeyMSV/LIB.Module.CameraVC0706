#include "modCameraVC0706.h"

namespace mod
{
namespace vc0706
{

tCameraVC0706::tStateStop::tStateStop(tCameraVC0706* obj)
	:tState(obj, "StateStop")
{

}

void tCameraVC0706::tStateStop::operator()()
{
	ChangeState(new tStateHalt(m_pObj, "stop"));
}

}
}
