#include "modCameraVC0706.h"

namespace mod
{
namespace vc0706
{

tCamera::tStateError::tStateError(tCamera* obj, const std::string& value/*, const std::source_location loc*/)
	:tState(obj, "StateError")
{
	m_pObj->m_LastErrorMsg = "tStateError: ";// +value + " " + loc.file_name();
	//m_pObj->m_LastErrorMsg += "(" + std::to_string(loc.line());
	//m_pObj->m_LastErrorMsg += ":" + std::to_string(loc.column());
	//m_pObj->m_LastErrorMsg += ")'";
	//m_pObj->m_LastErrorMsg += loc.function_name();
	//m_pObj->m_LastErrorMsg += "'";
	m_pObj->m_pLog->WriteLine(true, m_pObj->m_LastErrorMsg);

	m_pObj->m_Control_Operation = false;
}

void tCamera::tStateError::operator()()
{
	ChangeState(new tStateHalt(m_pObj, "error", true));
}

}
}
