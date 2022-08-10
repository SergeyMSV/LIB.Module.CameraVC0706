#include "devCamera.h"
#include "devSettings.h"

namespace dev
{

tCamera::tModCamera::tBoardCtrl::tBoardCtrl(tModCamera* obj, boost::asio::io_context& io)
	:m_pObj(obj), tSerialPort(io, g_Settings.SerialPort.CtrlID, g_Settings.SerialPort.CtrlBR)
{

}

tCamera::tModCamera::tBoardCtrl::~tBoardCtrl()
{

}

void tCamera::tModCamera::tBoardCtrl::OnReceived(utils::tVectorUInt8& data)
{
	m_pObj->OnReceivedCtrl(data);
}

tCamera::tModCamera::tBoardData::tBoardData(tModCamera* obj, boost::asio::io_context& io)
	:m_pObj(obj), tSerialPort(io, g_Settings.SerialPort.DataID, g_Settings.SerialPort.DataBR)
{

}

tCamera::tModCamera::tBoardData::~tBoardData()
{

}

void tCamera::tModCamera::tBoardData::OnReceived(utils::tVectorUInt8& data)
{
	m_pObj->OnReceivedCtrl(data);
}

}
