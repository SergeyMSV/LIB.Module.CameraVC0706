#include "modCameraVC0706.h"
/*
namespace mod
{

tCameraVC0706::tStateReset::tStateReset(tCameraVC0706 *obj)
	:tState(obj)
{
#ifdef LIB_MODULE_LOG
	p_obj->p_log->WriteLine("tStateReset");
#endif//LIB_MODULE_LOG

	m_ErrTimer.SetValue(utils::tTime(10));//[#]
}

void tCameraVC0706::tStateReset::Control()
{
	if (!m_ErrTimer.GetState())
	{
		p_obj->OnFailed(tCameraVC0706Error_StateReset_ErrTimer);

		ChangeState(tStateRestart::Instance(p_obj));
		return;
	}

	if (m_Step % 2 == 0 && !m_Timer.GetState())
	{
		switch (m_Step++)
		{
		case 0:
		{
			CameraVC0706::Packet::tPacketCmd Packet;
			Packet.MsgId = CameraVC0706::Packet::MsgId_SystemReset;

			std::vector<char> LocalVector = Packet.ToVector();

			p_obj->Board_Send(LocalVector);

			break;
		}
		default:
		{
			ChangeState(tStateInitialize::Instance(p_obj));
			return;
		}
		}
	}
}

void tCameraVC0706::tStateReset::OnReceivedMsg(std::vector<char>& data)
{
	switch (m_Step)
	{
	case 1:
	{
		CameraVC0706::Packet::tPacketRet PacketRet;

		if (CameraVC0706::Packet::tPacketRet::TryParse(data, PacketRet))
		{
			if (PacketRet.MsgId == CameraVC0706::Packet::MsgId_SystemReset && PacketRet.Status == CameraVC0706::Packet::CERR_None)
			{
				m_Step++;

				m_Timer.SetValue(utils::tTime(2));//[#]Wait for 2 sec upon Reset
			}
			else
			{
				p_obj->OnFailed(tCameraVC0706Error_StateReset_WrongAnswer);

				ChangeState(tStateRestart::Instance(p_obj));
				return;
			}
		}

		break;
	}
	}
}

}*/