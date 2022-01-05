#include "modCameraVC0706.h"

namespace mod
{

tCameraVC0706::tStateError::tStateError(tCameraVC0706* obj, const std::string& value)
	:tState(obj, "StateError")
{
	m_pObj->m_LastErrorMsg = "tStateError: " + value;
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, m_pObj->m_LastErrorMsg);

	m_pObj->m_Control_Operation = false;
}

bool tCameraVC0706::tStateError::operator()()
{
	ChangeState(new tStateHalt(m_pObj, "error", true));
	return true;
}

}
