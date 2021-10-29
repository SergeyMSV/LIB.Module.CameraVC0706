#include "modCameraVC0706.h"
/*
namespace mod
{

tCameraVC0706::tState::tState(tCameraVC0706 *obj)
	:p_obj(obj)
{
}

void tCameraVC0706::tState::Halt()
{
	ChangeState(tStateHalt::Instance(p_obj));
	return;
}

void tCameraVC0706::tState::Receive(std::vector<char>& data)
{
	if (m_ReceivedData.size() > 128)//[#]
	{
		m_ReceivedData.clear();
	}

	m_ReceivedData.insert(m_ReceivedData.end(), data.begin(), data.end());

	for (int i = 0; i < 10; ++i)//[#]
	{
		std::vector<char> Packet = CameraVC0706::Packet::FindPacketRet(m_ReceivedData);

		if (Packet.size() != 0)
		{
#ifdef LIB_MODULE_LOG_LIBTEST
			p_obj->p_log->WriteHex("Rcvd Packet", Packet);
#endif//LIB_MODULE_LOG_LIBTEST

			OnReceivedMsg(Packet);//ChangeState
			return;
		}
	}
}

void tCameraVC0706::tState::ChangeState(tState *state)
{
	p_obj->ChangeState(state);
}

}*/