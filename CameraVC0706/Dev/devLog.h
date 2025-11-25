///////////////////////////////////////////////////////////////////////////////////////////////////
// devLog.h
// 2020-01-15
// Standard ISO/IEC 114882, C++14
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <utilsLog.h>

#include <cstdint>

namespace dev
{

class tLog : public utils::log::tLog
{
public:
	enum class tID : std::uint32_t
	{
		NoID = 0,
		Enabled = (1 << 0),
		CAM = (1 << 1),
	};

	union tSettings
	{
		struct
		{
			std::uint32_t Enabled : 1;//if it's equal to 0 no logs are shown
			std::uint32_t CAM     : 1;
			std::uint32_t         : 29;
		}Field;

		std::uint32_t Value = 0;
	};

	static tSettings LogSettings;

private:
	tID m_ID = tID::NoID;
	const std::string m_Label;

	tLog() = delete;

public:
	tLog(tLog::tID id, const char* sign);

protected:
	std::string GetLabel() const override;

	void WriteLog(const std::string& msg) override;
};

}
