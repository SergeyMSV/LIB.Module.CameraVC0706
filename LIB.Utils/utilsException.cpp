#include "utilsException.h"

#include <filesystem>

namespace utils
{
namespace log_ex
{

std::string GetLogMessage(const std::string& msg, const std::string& filename, int line)
{
	const std::filesystem::path Path(filename); // C++17
	return msg + " " + Path.filename().string() + ":" + std::to_string(line);
}

}
}
