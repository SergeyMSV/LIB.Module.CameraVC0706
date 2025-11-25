#include "devCamera.h"
#include "devSettings.h"

namespace dev
{

tCam::tModCam::tBoardCtrl::tBoardCtrl(tModCam* obj, boost::asio::io_context& io)
	:m_pObj(obj), tPortSerialAsync(io, g_Settings.SerialPort.CtrlID, g_Settings.SerialPort.CtrlBR)
{

}

tCam::tModCam::tBoardCtrl::~tBoardCtrl()
{

}

void tCam::tModCam::tBoardCtrl::OnReceived(std::vector<std::uint8_t>& data)
{
	m_pObj->OnReceivedCtrl(data);
}

tCam::tModCam::tBoardData::tBoardData(tModCam* obj, boost::asio::io_context& io)
	:m_pObj(obj), tPortSerialAsync(io, g_Settings.SerialPort.DataID, g_Settings.SerialPort.DataBR)
{

}

tCam::tModCam::tBoardData::~tBoardData()
{

}

void tCam::tModCam::tBoardData::OnReceived(std::vector<std::uint8_t>& data)
{
	m_pObj->OnReceivedCtrl(data);
}

}
