#include "devSettings.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace dev
{

tSettings g_Settings;

tSettings::tSettings(const std::string& fileName)
	:m_ConfigFileName(fileName)
{
	boost::property_tree::ptree PTree;
	boost::property_tree::xml_parser::read_xml(m_ConfigFileName, PTree);

	if (auto Value = PTree.get_child_optional("App.Settings.OutputFile"))
	{
		auto ValueIter = (*Value).begin();

		if (ValueIter->first == "<xmlattr>")
		{
			Output.Path = ValueIter->second.get<std::string>("Path");
			Output.FileName = ValueIter->second.get<std::string>("FileName");
		}
	}

	if (auto Value = PTree.get_child_optional("App.Settings.Device"))
	{
		auto ValueIter = (*Value).begin();

		if (ValueIter->first == "<xmlattr>")
		{
			Main.Model = ValueIter->second.get<std::string>("Model");
			Main.ID = ValueIter->second.get<std::string>("ID");
		}
	}

	if (auto Value = PTree.get_child_optional("App.Settings.SerialPort"))
	{
		auto ValueIter = (*Value).begin();

		if (ValueIter->first == "<xmlattr>")
		{
			SerialPort.ID = ValueIter->second.get<std::string>("ID");
			SerialPort.BR = ValueIter->second.get<std::uint32_t>("BR");
			Camera.PortBR = SerialPort.BR;
		}
	}

	if (auto Value = PTree.get_child_optional("App.Settings.CameraVC0706"))
	{
		auto ValueIter = (*Value).begin();

		if (ValueIter->first == "<xmlattr>")
		{
			std::string Str = ValueIter->second.get<std::string>("Resolution");
			Camera.Resolution = utils::packet_CameraVC0706::ToResolution(Str);
			Camera.CheckPresencePeriod_ms = ValueIter->second.get<std::uint32_t>("CheckPresencePeriod_ms");
			Camera.ImagePeriod_ms = ValueIter->second.get<std::uint32_t>("ImagePeriod_ms");
			Camera.ImageChunkSize = ValueIter->second.get<std::uint32_t>("ImageChunkSize");
			Camera.ImageChunkDelayFromReq_us = ValueIter->second.get<std::uint32_t>("ImageChunkDelayFromReq_us");
		}
	}
}

}