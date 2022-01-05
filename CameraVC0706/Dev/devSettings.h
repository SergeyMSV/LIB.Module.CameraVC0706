///////////////////////////////////////////////////////////////////////////////////////////////////
// devSettings.h
// 2020-04-29
// Standard ISO/IEC 114882, C++20
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <devConfig.h>

#include <modCameraVC0706.h>

#include <utilsBase.h>

#include <string>

namespace dev
{

struct tSettings
{
	struct
	{
		std::string Model;
		std::string ID;
	}Main;

	struct
	{
		std::string ID;
		std::uint32_t BR = 0;
	}SerialPort;

	struct
	{
		std::string Path;
		std::string FileName;
	}Output;

	mod::tCameraVC0706Settings Camera;

private:
	std::string m_ConfigFileName;

public:
	tSettings() {}
	explicit tSettings(const std::string& fileName);

	std::string GetConfigFileName() const { return m_ConfigFileName; }
};

extern tSettings g_Settings;

}