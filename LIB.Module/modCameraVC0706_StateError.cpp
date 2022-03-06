#include "modCameraVC0706.h"

namespace mod
{

tCameraVC0706::tStateError::tStateError(tCameraVC0706* obj, const std::string& value/*, const std::source_location loc*/)
	:tState(obj, "StateError")
{
	m_pObj->m_LastErrorMsg = "tStateError: ";// +value + " " + loc.file_name();
	//m_pObj->m_LastErrorMsg += "(" + std::to_string(loc.line());
	//m_pObj->m_LastErrorMsg += ":" + std::to_string(loc.column());
	//m_pObj->m_LastErrorMsg += ")'";
	//m_pObj->m_LastErrorMsg += loc.function_name();
	//m_pObj->m_LastErrorMsg += "'";
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, m_pObj->m_LastErrorMsg);

	m_pObj->m_Control_Operation = false;
}

void tCameraVC0706::tStateError::operator()()
{
	ChangeState(new tStateHalt(m_pObj, "error", true));
}

}
