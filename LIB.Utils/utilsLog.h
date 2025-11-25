///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsLog
// 2016-05-16 (before)
// Standard ISO/IEC 114882, C++11
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <libConfig.h>

#include <cstdint>

#include <mutex>
#include <string>
#include <vector>

namespace utils
{
namespace log
{

enum class tColor
{
	Black,
	Red,
	Green,
	Yellow,
	Blue,
	Magenta,
	Cyan,
	White,
	Default,
	LightGray,
	LightRed,
	LightGreen,
	LightYellow,
	LightBlue,
	LightMagenta,
	LightCyan,
	LightWhite,
};

#ifdef LIB_UTILS_LOG

class tLog
{
	mutable std::mutex m_Mtx;

public:
	tLog() = default;
	virtual ~tLog() {}

	void Write(bool timestamp, const std::string& msg, tColor color);
	void Write(bool timestamp, const std::string& msg);
#ifdef LIB_UTILS_LOG_DEPRECATED
	void Write(bool timestamp, tColor colorText, const std::string& msg); // DEPRECATED
#endif // LIB_UTILS_LOG_DEPRECATED

	void WriteLine();
	void WriteLine(bool timestamp, const std::string& msg, tColor color);
	void WriteLine(bool timestamp, const std::string& msg);
#ifdef LIB_UTILS_LOG_DEPRECATED
	void WriteLine(bool timestamp, tColor colorText, const std::string& msg); // DEPRECATED

	void WriteHex(bool timestamp, tColor colorText, const std::string& msg, const std::vector<std::uint8_t>& data); // DEPRECATED
#endif // LIB_UTILS_LOG_DEPRECATED

	void WriteHex(const std::vector<std::uint8_t>& data, tColor dataColor, int dataLinesBegin, int dataLinesEnd);
	void WriteHex(const std::vector<std::uint8_t>& data, tColor dataColor);
	void WriteHex(const std::vector<std::uint8_t>& data);
	void WriteHex(bool timestamp, const std::string& msg, tColor msgColor, const std::vector<std::uint8_t>& data, tColor dataColor, int dataLinesBegin, int dataLinesEnd);
	void WriteHex(bool timestamp, const std::string& msg, tColor msgColor, const std::vector<std::uint8_t>& data, tColor dataColor);
	void WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data, tColor color, int dataLinesBegin, int dataLinesEnd);
	void WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data, tColor color);
	void WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data, int dataLinesBegin, int dataLinesEnd);
	void WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data);

protected:
	virtual std::string GetLabel() const { return {}; }

	virtual void WriteLog(const std::string& text) = 0;
	virtual void WriteLogFile(const std::string& text) {}

private:
	virtual void WriteLog(bool timestamp, bool endl, const std::string& text, tColor textColor);
};

#else // LIB_UTILS_LOG

class tLog
{
public:
	tLog() = default;
	virtual ~tLog() {}

	void Write(bool timestamp, const std::string& msg, tColor color) {}
	void Write(bool timestamp, const std::string& msg) {}
#ifdef LIB_UTILS_LOG_DEPRECATED
	void Write(bool timestamp, tColor colorText, const std::string& msg) {} // DEPRECATED
#endif // LIB_UTILS_LOG_DEPRECATED

	void WriteLine() {}
	void WriteLine(bool timestamp, const std::string& msg, tColor color) {}
	void WriteLine(bool timestamp, const std::string& msg) {}
#ifdef LIB_UTILS_LOG_DEPRECATED
	void WriteLine(bool timestamp, tColor colorText, const std::string& msg) {} // DEPRECATED

	void WriteHex(bool timestamp, tColor colorText, const std::string& msg, const std::vector<std::uint8_t>& data) {} // DEPRECATED
#endif // LIB_UTILS_LOG_DEPRECATED

	void WriteHex(const std::vector<std::uint8_t>& data, tColor dataColor, int dataLinesBegin, int dataLinesEnd) {}
	void WriteHex(const std::vector<std::uint8_t>& data, tColor dataColor) {}
	void WriteHex(const std::vector<std::uint8_t>& data) {}
	void WriteHex(bool timestamp, const std::string& msg, tColor msgColor, const std::vector<std::uint8_t>& data, tColor dataColor, int dataLinesBegin, int dataLinesEnd) {}
	void WriteHex(bool timestamp, const std::string& msg, tColor msgColor, const std::vector<std::uint8_t>& data, tColor dataColor) {}
	void WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data, tColor color, int dataLinesBegin, int dataLinesEnd) {}
	void WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data, tColor color) {}
	void WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data, int dataLinesBegin, int dataLinesEnd) {}
	void WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data) {}

protected:
	virtual std::string GetLabel() const { return {}; }

	virtual void WriteLog(const std::string& text) = 0;
	virtual void WriteLogFile(const std::string& text) {}
};

#endif // LIB_UTILS_LOG

}
}
