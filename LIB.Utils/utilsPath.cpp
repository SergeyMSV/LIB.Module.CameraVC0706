#include "utilsPath.h"

#include <deque>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>

namespace utils
{

namespace linux
{

namespace PathConfig
{

#if defined(_WIN32)
constexpr char g_TestDirRootFS[] = "test_root_fs";
#endif

const std::vector<std::string> g_PathConfig =
{
	{""},
	{"../etc/"},
	{"/etc/"},
	{"~/"},
	{"/etc/default/"},
#if defined(_WIN32)
	{"../"}, // $(ProjectDir)
#endif
};

bool TestFile(const std::string& fileName)
{
	std::fstream File = std::fstream(fileName, std::ios::in);
	if (!File.is_open())
		return false;

	File.close();
	return true;
}

std::string TestPath(const std::string& path, std::string fileName, bool testDir)
{
	std::string FilePath;

	if (!path.empty())
	{
		FilePath = path + fileName;
		if (TestFile(FilePath))
			return FilePath;
	}

	FilePath = path + "." + fileName; // hidden file
	if (TestFile(FilePath))
		return FilePath;

	FilePath = path + fileName + "rc";
	if (TestFile(FilePath))
		return FilePath;

	FilePath = path + "." + fileName + "rc"; // hidden file
	if (TestFile(FilePath))
		return FilePath;

	FilePath = path + fileName + ".conf";
	if (TestFile(FilePath))
		return FilePath;

	FilePath = path + fileName + ".conf.json";
	if (TestFile(FilePath))
		return FilePath;

	if (!testDir)
		return {};

	FilePath = path + fileName + "/";
	FilePath = TestPath(FilePath, fileName, false);
	if (!FilePath.empty())
		return FilePath;

	return {};
}

}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string GetPathConfig(const std::string& fileName)
{
	if (fileName.empty())
		return {};

	for (auto& i : PathConfig::g_PathConfig)
	{
		std::string PathBase = i;

#if defined(_WIN32)
		if (PathBase.empty())
		{
			PathBase = "/";
		}
		//else if (PathBase.size() > 1) // "../etc" -> "/root/../etc"
		//{
		//	if (PathBase[0] == '.' && PathBase[1] == '.')
		//		PathBase = "/root/" + PathBase;
		//}
		else if (PathBase.size() > 0)
		{
			if (PathBase[0] == '~')
			{
				PathBase = "/root/";
			}
			else if (PathBase[0] != '/')
			{
				PathBase = "/" + PathBase;
			}
		}

		PathBase = PathConfig::g_TestDirRootFS + PathBase;
#endif

		std::string Path = PathConfig::TestPath(PathBase, fileName, true);
		if (!Path.empty())
			return Path;
	}

	return {};
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string GetPath(const std::string& path)
{
#if defined(_WIN32)
	if (path.empty())
		return {};

	std::string PathBase;

	if (path[0] == '/')
	{
		PathBase = PathConfig::g_TestDirRootFS + path;
	}
	else if (path[0] == '~')
	{
		std::string TempPath = path;
		TempPath.erase(TempPath.begin(), std::find_if(TempPath.begin(), TempPath.end(), [](char ch) { return ch != '~'; }));
		PathBase = std::string(PathConfig::g_TestDirRootFS) + "/root" + TempPath;
	}
	else
	{
		PathBase = path;
	}

	return PathBase;
#else
	return path;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////

static std::string GetFirstLine(const std::string& a_filePath)
{
	std::string FileName = GetPath(a_filePath);
	std::fstream File(FileName, std::ios::in);
	if (!File.good())
		return {};

	std::string Line;
	std::getline(File, Line);

	File.close();

	return Line;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

tCpuInfo GetCpuInfo()
{
	std::string FileName = GetPath("/proc/cpuinfo");
	std::fstream File(FileName, std::ios::in);
	if (!File.good())
		return {};

	tCpuInfo CpuInfo{};

	std::deque<std::string> Strings;

	while (!File.eof())
	{
		std::string Line;
		std::getline(File, Line);
		Line.erase(std::remove_if(Line.begin(), Line.end(), [](char ch) { return ch == '\t'; }), Line.end());

		size_t Pos = Line.find(":", 0);
		std::string PrmName = Line.substr(0, Pos);

		auto GetValueString = [&Pos](const std::string& a_line)
		{
			std::string Value = a_line.substr(Pos, a_line.size());
			Value.erase(Value.begin(), std::find_if(Value.begin(), Value.end(), [](char ch) { return ch != ' ' && ch != ':'; }));
			return Value;
		};

		if (PrmName == "model name")
		{
			CpuInfo.ModelName = GetValueString(Line);
		}
		else if (PrmName == "BogoMIPS")
		{
			std::string Value = GetValueString(Line);
			 double Num = strtod(Value.c_str(), nullptr);
			 if (Num > 0 && Num != HUGE_VAL)
				 CpuInfo.BogoMIPS = Num;
		}
		else if (PrmName == "Hardware")
		{
			CpuInfo.Hardware = GetValueString(Line);
		}
	}

	File.close();

	return CpuInfo;
}

std::string GetHostname()
{
	return GetFirstLine("/etc/hostname");
}

double GetUptime()
{
	std::string FileName = GetPath("/proc/uptime");
	std::fstream File(FileName, std::ios::in);
	if (!File.good())
		return {};

	double UptimeSeconds;
	File >> UptimeSeconds;

	File.close();

	return UptimeSeconds;
}

std::string GetVersion()
{
	return GetFirstLine("/proc/version");
}

}

}
