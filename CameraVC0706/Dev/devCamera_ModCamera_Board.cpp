#include "devCamera.h"
#include "devSettings.h"

namespace dev
{

tCamera::tModCamera::tBoardCtrl::tBoardCtrl(tModCamera* obj, boost::asio::io_context& io)
	:m_pObj(obj), tPortSerialAsync(io, g_Settings.SerialPort.CtrlID, g_Settings.SerialPort.CtrlBR)
{

}

tCamera::tModCamera::tBoardCtrl::~tBoardCtrl()
{

}

void tCamera::tModCamera::tBoardCtrl::OnReceived(std::vector<std::uint8_t>& data)
{
	m_pObj->OnReceivedCtrl(data);
}

tCamera::tModCamera::tBoardData::tBoardData(tModCamera* obj, boost::asio::io_context& io)
	:m_pObj(obj), tPortSerialAsync(io, g_Settings.SerialPort.DataID, g_Settings.SerialPort.DataBR)
{

}

tCamera::tModCamera::tBoardData::~tBoardData()
{

}

void tCamera::tModCamera::tBoardData::OnReceived(std::vector<std::uint8_t>& data)
{
	m_pObj->OnReceivedCtrl(data);
}

}
