///////////////////////////////////////////////////////////////////////////////////////////////////
// LOG
//
// Created by Sergey Maslennikov
// Tel.:   +7-916-540-09-19
// E-mail: maslennikovserge@yandex.ru
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   long ago    |
// |      2     |   2016 05 20  | Refactored
// |      3     |   2016 06 02  | Changed owner's identification (added m_Id)
// |            |               | Added WriteLog(std::string msg, bool timestamp, bool error);
// |      3c    |   2016 06 27  |
// |            |               |
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DEV_LOG_H
#define DEV_LOG_H
/*
#include <devConfig.h>

#include <utilsLog.h>

#include <string>

namespace dev
{

class tLog : public utils::log::tLog
{
public:
	static const unsigned int LogId_Board_ModCamera      = (1 << 0);
	static const unsigned int LogId_Dev_ModCamera        = (1 << 1);
	//...

	union tSettings
	{
		struct
		{
			unsigned int Board_ModCamera      : 1;
			unsigned int Dev_ModCamera        : 1;
			unsigned int : 30;
		}Field;

		unsigned int Value;
	};

private:
	unsigned int m_Id;

	tLog() { }

public:
	tLog(unsigned int id);

	virtual ~tLog() { }

protected:
	virtual void WriteLog(const std::string& msg, bool timestamp, bool showAlways);
};

}
*/
#endif//DEV_LOG_H
