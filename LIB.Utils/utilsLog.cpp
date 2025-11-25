#include "utilsLog.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <time.h>

#ifdef LIB_UTILS_LOG

namespace utils
{
namespace log
{

void tLog::Write(bool timestamp, const std::string& msg, tColor color)
{
	WriteLog(timestamp, false, msg, color);
}

void tLog::Write(bool timestamp, const std::string& msg)
{
	Write(timestamp, msg, tColor::Default);
}

#ifdef LIB_UTILS_LOG_DEPRECATED
void tLog::Write(bool timestamp, tColor colorText, const std::string& msg)
{
	Write(timestamp, msg, colorText);
}
#endif // LIB_UTILS_LOG_DEPRECATED

void tLog::WriteLine()
{
	WriteLog(false, true, "", tColor::Default);
}

void tLog::WriteLine(bool timestamp, const std::string& msg, tColor color)
{
	WriteLog(timestamp, true, msg, color);
}

void tLog::WriteLine(bool timestamp, const std::string& msg)
{
	WriteLine(timestamp, msg, tColor::Default);
}

#ifdef LIB_UTILS_LOG_DEPRECATED
void tLog::WriteLine(bool timestamp, tColor colorText, const std::string& msg)
{
	WriteLine(timestamp, msg, colorText);
}
#endif // LIB_UTILS_LOG_DEPRECATED

static bool IsSymbol(char value)
{
	return value > 0x20 && value != 0x25 && value != 0x7F;
}

template <typename T>
static std::enable_if<std::is_same<T, char>::value || std::is_same<T, unsigned char>::value, std::string>::type MakeStringHex(const std::vector<T>& data, int dataLinesBegin, int dataLinesEnd)
{
	const int LinesQty = static_cast<int>((data.size() / 16) + (data.size() % 16 ? 1 : 0));
	const int LinesToSkip = dataLinesBegin && dataLinesEnd ? static_cast<int>(LinesQty - dataLinesBegin - dataLinesEnd) : 0;
	if (LinesToSkip > 0)
	{
		--dataLinesBegin; // transform to index
		dataLinesEnd = LinesQty - dataLinesEnd;
	}

	const auto IsLineSkipped = [&dataLinesBegin, &dataLinesEnd, &LinesToSkip](int linesCounter)->bool { return LinesToSkip > 0 && linesCounter > dataLinesBegin && linesCounter < dataLinesEnd; };

	std::stringstream Stream;
	bool SkipMsgInserted = false;
	for (int i = 0; i < LinesQty; ++i)
	{
		if (IsLineSkipped(i))
		{
			if (!SkipMsgInserted)
			{
				SkipMsgInserted = true;
				Stream << "< " + std::to_string(LinesToSkip) + " lines are skipped >\n";
			}
			continue;
		}

		Stream << std::setfill('0') << std::setw(4) << std::hex << (16 * i) << "   ";

		std::string Substr;
		std::size_t byteCount = 0;
		for (std::size_t k = (16 * i); byteCount < 16 && k < data.size(); ++byteCount, ++k)
		{
			if (byteCount == 8)
				Stream << "  ";

			Stream << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(data[k]) << ' ';

			Substr += IsSymbol(data[k]) ? data[k] : '.';
		}
		for (; byteCount < 16; ++byteCount)
		{
			if (byteCount == 8)
				Stream << "  ";
			Stream << "   ";
		}
		Stream << "  " + Substr;
		

		if (i < LinesQty - 1) // It's not needed for the last string
			Stream << '\n';
	}

	return Stream.str();
}

#ifdef LIB_UTILS_LOG_DEPRECATED
void tLog::WriteHex(bool timestamp, tColor colorText, const std::string& msg, const std::vector<std::uint8_t>& data)
{
	WriteHex(timestamp, msg, colorText, data, colorText);
}
#endif // LIB_UTILS_LOG_DEPRECATED

void tLog::WriteHex(const std::vector<std::uint8_t>& data, tColor dataColor, int dataLinesBegin, int dataLinesEnd)
{
	WriteLog(false, true, MakeStringHex(data, dataLinesBegin, dataLinesEnd), dataColor);
}

void tLog::WriteHex(const std::vector<std::uint8_t>& data, tColor dataColor)
{
	WriteHex(data, dataColor, 0, 0);
}

void tLog::WriteHex(const std::vector<std::uint8_t>& data)
{
	WriteHex(data, tColor::Default, 0, 0);
}

void tLog::WriteHex(bool timestamp, const std::string& msg, tColor msgColor, const std::vector<std::uint8_t>& data, tColor dataColor, int dataLinesBegin, int dataLinesEnd)
{
	WriteLog(timestamp, true, msg, msgColor);
	WriteHex(data, dataColor, dataLinesBegin, dataLinesEnd);
}

void tLog::WriteHex(bool timestamp, const std::string& msg, tColor msgColor, const std::vector<std::uint8_t>& data, tColor dataColor)
{
	WriteHex(timestamp, msg, msgColor, data, dataColor, 0, 0);
}

void tLog::WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data, tColor color, int dataLinesBegin, int dataLinesEnd)
{
	WriteHex(timestamp, msg, color, data, color, dataLinesBegin, dataLinesEnd);
}

void tLog::WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data, tColor color)
{
	WriteHex(timestamp, msg, color, data, color);
}

void tLog::WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data, int dataLinesBegin, int dataLinesEnd)
{
	WriteHex(timestamp, msg, tColor::Default, data, tColor::Default, dataLinesBegin, dataLinesEnd);
}

void tLog::WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data)
{
	WriteHex(timestamp, msg, tColor::Default, data, tColor::Default);
}

void tLog::WriteLog(bool timestamp, bool endl, const std::string& text, tColor textColor)
{
	std::lock_guard<std::mutex> Lock(m_Mtx);

	std::string Str;
	Str.reserve(80); // [#]

	if (timestamp)
	{
		const auto TimeNow = std::chrono::system_clock::now();
#ifdef LIB_UTILS_LOG_TIMESTAMP_MICROSECONDS
		const auto Time_us = std::chrono::time_point_cast<std::chrono::microseconds>(TimeNow);
		const auto TimeFract = static_cast<unsigned int>(Time_us.time_since_epoch().count() % 1000000);
		const int Digits = 6;
#else // LIB_UTILS_LOG_TIMESTAMP_MICROSECONDS
		const auto Time_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(TimeNow);
		const auto TimeFract = static_cast<unsigned int>(Time_ms.time_since_epoch().count() % 1000);
		const int Digits = 3;
#endif // LIB_UTILS_LOG_TIMESTAMP_MICROSECONDS
		time_t Time = std::chrono::system_clock::to_time_t(TimeNow);

		tm TmBuf;
#ifdef _WIN32
		localtime_s(&TmBuf, &Time);
#else // _WIN32
		localtime_r(&Time, &TmBuf);
#endif // _WIN32
		std::stringstream SStr;
		SStr << std::put_time(&TmBuf, "%T") << '.';
		SStr << std::setfill('0') << std::setw(Digits) << TimeFract << ' ';

		const std::string Label = GetLabel();
		if (!Label.empty())
			SStr << std::setfill(' ') << std::setw(4) << Label << ' ';
		Str = SStr.str();
	}

	std::string StrNoColor = Str + text + (endl ? "\n" : "");
	WriteLogFile(StrNoColor);

#ifndef LIB_UTILS_LOG_COLOR
	WriteLog(StrNoColor);
#else
	Str += "\x1b[";

	switch (textColor)
	{
	case tColor::Black:        Str += "30"; break;
	case tColor::Red:          Str += "31"; break;
	case tColor::Green:        Str += "32"; break;
	case tColor::Yellow:       Str += "33"; break;
	case tColor::Blue:         Str += "34"; break;
	case tColor::Magenta:      Str += "35"; break;
	case tColor::Cyan:         Str += "36"; break;
	case tColor::White:        Str += "37"; break;
	case tColor::Default:      Str += "39"; break;
	case tColor::LightGray:    Str += "90"; break;
	case tColor::LightRed:     Str += "91"; break;
	case tColor::LightGreen:   Str += "92"; break;
	case tColor::LightYellow:  Str += "93"; break;
	case tColor::LightBlue:    Str += "94"; break;
	case tColor::LightMagenta: Str += "95"; break;
	case tColor::LightCyan:    Str += "96"; break;
	case tColor::LightWhite:   Str += "97"; break;
	default:                   Str += "39"; break;
	}

	Str += "m" + text + "\x1b[0m" + (endl ? "\n" : "");
	WriteLog(Str);
#endif // LIB_UTILS_LOG_COLOR
}

}
}

#endif // LIB_UTILS_LOG
