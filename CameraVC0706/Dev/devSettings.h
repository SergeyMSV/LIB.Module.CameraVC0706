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

#include <boost/property_tree/ptree.hpp>

namespace dev
{

namespace config
{

struct tDevice
{
	std::string Model;

	tDevice() = default;
	explicit tDevice(boost::property_tree::ptree a_PTree);
};

struct tSerialPort
{
	std::string CtrlID;
	std::uint32_t CtrlBR = 0;
	std::string DataID;
	std::uint32_t DataBR = 0;

	tSerialPort() = default;
	explicit tSerialPort(boost::property_tree::ptree a_PTree);
};

struct tPicture
{
	std::string Path;
	std::string Prefix;
	uint8_t QtyMax = 0;

	tPicture() = default;
	explicit tPicture(boost::property_tree::ptree a_PTree);
};

}

struct tSettings
{
	config::tDevice Device;
	config::tSerialPort SerialPort;
	config::tPicture Picture;

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