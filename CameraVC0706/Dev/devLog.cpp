#include "devLog.h"

#include <iostream>

namespace dev
{

tLog::tSettings tLog::LogSettings;

tLog::tLog(tLog::tID id, const char* sign)
	:m_ID(id), m_Label(sign)
{

}

std::string tLog::GetLabel() const
{
	return m_Label;
}

void tLog::WriteLog(const std::string& msg)
{
	if (LogSettings.Field.Enabled && LogSettings.Value & static_cast<std::uint32_t>(m_ID))
	{
		std::cout << msg;
	}
}

}
