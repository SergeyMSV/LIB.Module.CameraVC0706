#include "devLog.h"

#include <Windows.h>
/*
namespace dev
{

tLog::tLog(unsigned int id)
{
	m_Id = id;

	dev::tLog::tSettings LogSettings;

	LogSettings.Value = 0;

#ifdef DEBUG_DEVICE
	LogSettings.Field.Board_ModCamera = 1;
	LogSettings.Field.Dev_ModCamera = 1;
#endif//DEBUG_DEVICE

	//gp_Properties->SetDataSetLogSettings(LogSettings);
}

void tLog::WriteLog(const std::string& msg, bool timestamp, bool showAlways)
{
	//if (gp_Properties->GetDataSetLogSettings().Value & m_Id || showAlways)
	{
		std::string Message;

		if (timestamp)
		{
			unsigned int UptimeInMs = GetTickCount();

			int Hour = UptimeInMs / 3600000;
			int Min = (UptimeInMs - (Hour * 3600000)) / 60000;
			int Sec = (UptimeInMs - (Hour * 3600000) - (Min * 60000)) / 1000;
			int MSec = UptimeInMs - (Hour * 3600000) - (Min * 60000) - (Sec * 1000);

			char Str[20];

			//[333.333:55555] = 15
			sprintf(Str, "[%.2d.%.2d:%.3d]", (unsigned char)Min, (unsigned char)Sec, (unsigned short)MSec);

			//////////////////////////////////////////////////////

			//TIME_STRUCT Time;
			//DATE_STRUCT Date;

			//_time_get(&Time);
			//_time_to_date(&Time, &Date);

			//char Str[20];

			//[333.333:55555] = 15
			//sprintf(Str, "[%.2d.%.2d:%.3d]", (unsigned char)Date.MINUTE, (unsigned char)Date.SECOND, (unsigned short)Date.MILLISEC);

			Message += Str;
		}

		Message += msg;

		printf(Message.c_str());
	}
}

}
*/