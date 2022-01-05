#include "devCamera.h"
#include "devSettings.h"

namespace dev
{

tCamera::tModCamera::tBoard::tBoard(tModCamera* obj, boost::asio::io_context& io)
	:m_pObj(obj), tSerialPort(io, g_Settings.SerialPortCtrl.ID, g_Settings.SerialPortCtrl.BR)
{

}

tCamera::tModCamera::tBoard::~tBoard()
{

}

void tCamera::tModCamera::tBoard::OnReceived(utils::tVectorUInt8& data)
{
	m_pObj->OnReceived(data);
}

}
