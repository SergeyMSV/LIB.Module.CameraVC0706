#include "devSettings.h"

#include <utilsPath.h>

#include <boost/property_tree/json_parser.hpp>

namespace dev
{

namespace config
{

tDevice::tDevice(boost::property_tree::ptree a_PTree)
{
	Model = a_PTree.get<std::string>("device.model");
}

tSerialPort::tSerialPort(boost::property_tree::ptree a_PTree)
{
	CtrlID = a_PTree.get<std::string>("serial_port.ctrl_id");
	CtrlBR = a_PTree.get<uint32_t>("serial_port.ctrl_br");
	DataID = a_PTree.get<std::string>("serial_port.data_id");
	DataBR = a_PTree.get<uint32_t>("serial_port.data_br");

#if defined(_WIN32)
	CtrlID = a_PTree.get<std::string>("serial_port.ctrl_id_win");
	DataID = a_PTree.get<std::string>("serial_port.data_id_win");
#endif
}

tPicture::tPicture(boost::property_tree::ptree a_PTree)
{
	std::string PathRaw = a_PTree.get<std::string>("picture.path");
	Path = utils::path::GetPathNormal(PathRaw).string();
	Prefix = a_PTree.get<std::string>("picture.prefix");
	QtyMax = a_PTree.get<uint8_t>("picture.qtyMax");
}

}

tSettings g_Settings;

tSettings::tSettings(const std::string& fileName)
	:m_ConfigFileName(fileName)
{
	boost::property_tree::ptree PTree;
	boost::property_tree::json_parser::read_json(m_ConfigFileName, PTree);

	Device = config::tDevice(PTree);
	SerialPort = config::tSerialPort(PTree);
	Picture = config::tPicture(PTree);

	auto Str = PTree.get<std::string>("camera.resolution");
	Camera.Resolution = utils::packet::vc0706::ToResolution(Str);
	Camera.CheckPresencePeriod_ms = PTree.get<std::uint32_t>("camera.check_presence_period_ms");
	Camera.ImagePeriod_ms = PTree.get<std::uint32_t>("camera.image_period_ms");
	Camera.ImageChunkSize = PTree.get<std::uint32_t>("camera.image_chunk_size");
	Camera.ImageChunkDelayFromReq_us = PTree.get<std::uint32_t>("camera.image_chunk_delay_from_req_us");

	Camera.SetPortCtrlBR(SerialPort.CtrlBR);
	if (!SerialPort.DataID.empty())
		Camera.SetPortDataBR(SerialPort.DataBR);
}

}