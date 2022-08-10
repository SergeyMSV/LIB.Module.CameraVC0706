///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPath.h
// 2022-04-20
// Standard ISO/IEC 114882, C++20
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <vector>

namespace utils
{

	namespace linux
	{

std::string GetPathConfig(const std::string& fileName);
std::string GetPath(const std::string& path);

struct tCpuInfo
{
	std::string ModelName;
	double BogoMIPS = 0.0;
	std::string Hardware;

	bool operator == (const tCpuInfo&) const = default;
	bool operator != (const tCpuInfo&) const = default;
};

tCpuInfo GetCpuInfo();
std::string GetHostname();
double GetUptime();
std::string GetVersion();

	}

}
