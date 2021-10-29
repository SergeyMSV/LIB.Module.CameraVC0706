#include "modCameraVC0706.h"
/*
namespace mod
{

tCameraVC0706::tStateGetImageStop::tStateGetImageStop(tCameraVC0706 *obj)
	:tState(obj)
{
#ifdef LIB_MODULE_LOG
	p_obj->p_log->WriteLine("tStateGetImageStop");
#endif//LIB_MODULE_LOG

	m_ErrTimer.SetValue(utils::tTime(1));//[#]

	m_ImageFrame = 0;//0 - current frame, 1 - next frame

	m_FBufSize = -1;
}

void tCameraVC0706::tStateGetImageStop::Control()
{
	if (!m_ErrTimer.GetState())
	{
		p_obj->OnFailed(tCameraVC0706Error_StateGetImageStop_ErrTimer);

		ChangeState(tStateReset::Instance(p_obj));
		return;
	}

	//p_obj->OnImageComplete();//TEST
	//ChangeState(tStateIdle::Instance(p_obj));//TEST
	//return;//TEST

	if (m_Step % 2 == 0 && !m_Timer.GetState())
	{
		switch (m_Step++)
		{
		case 0:
		{
			CameraVC0706::Packet::tPacketCmd Packet;
			Packet.MsgId = CameraVC0706::Packet::MsgId_FBufCtrl;

			Packet.Payload.push_back((char)CameraVC0706::Packet::tPayload_FBUF_CTRL_ControlFlag_ResumeFrame);

			std::vector<char> LocalVector = Packet.ToVector();

			p_obj->Board_Send(LocalVector);

			break;
		}
		default:
		{
			p_obj->OnImageComplete();

			ChangeState(tStateIdle::Instance(p_obj));
			return;
		}
		}
	}
}

void tCameraVC0706::tStateGetImageStop::OnReceivedMsg(std::vector<char>& data)
{
	switch (m_Step)
	{
	case 1:
	{
		CameraVC0706::Packet::tPacketRet PacketRet;

		if (CameraVC0706::Packet::tPacketRet::TryParse(data, PacketRet))
		{
			if (PacketRet.MsgId == CameraVC0706::Packet::MsgId_FBufCtrl && PacketRet.Status == CameraVC0706::Packet::CERR_None)
			{
				m_Step++;
			}
			else
			{
				p_obj->OnFailed(tCameraVC0706Error_StateGetImageStop_FBufCtrl_WrongAnswer);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}

		break;
	}
	}
}

}*/