///////////////////////////////////////////////////////////////////////////////////////////////////
// devSettings.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 04 29  |
// |            |               | 
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
	//<HW Version = "VC0703 1.00" / >
	//	<State>
	//	<Start Resolution = "640x480">
	//	<Operation CheckPresencePeriod_ms = "1000">
	//	<OperationImage ImagePeriod_ms = "10000" ChunkSize = "4096" ChunkDelayFromReq_ms = "50000" / >
	// 
	//StateOparation
	//mod::tCameraVC0706::tStateOperation::tSettings StateOperation;
	//mod::tCameraVC0706::tStateOperation::tSettings StateOperation;

private:
	std::string m_ConfigFileName;

public:
	tSettings() {}
	explicit tSettings(const std::string& fileName);

	std::string GetConfigFileName() const { return m_ConfigFileName; }

	//mod::tGnssTaskScript GetTaskScript(const std::string& id, bool userTaskScript) const;
	//mod::tGnssSettingsNMEA GetSettingsNMEA() const;
};

extern tSettings g_Settings;

}