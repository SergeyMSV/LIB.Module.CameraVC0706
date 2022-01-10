#include "devCamera.h"
#include "devSettings.h"

namespace dev
{

tCamera::tModCamera::tBoardCtrl::tBoardCtrl(tModCamera* obj, boost::asio::io_context& io)
	:m_pObj(obj), tSerialPort(io, g_Settings.SerialPortCtrl.ID, g_Settings.SerialPortCtrl.BR)
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
	:m_pObj(obj), tSerialPort(io, g_Settings.SerialPortData.ID, g_Settings.SerialPortData.BR)
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
