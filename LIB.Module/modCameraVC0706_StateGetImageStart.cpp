#include "modCameraVC0706.h"
/*
namespace mod
{

tCameraVC0706::tStateGetImageStart::tStateGetImageStart(tCameraVC0706 *obj)
	:tState(obj)
{
#ifdef LIB_MODULE_LOG
	p_obj->p_log->WriteLine("tStateGetImageStart");
#endif//LIB_MODULE_LOG

	m_ErrTimer.SetValue(utils::tTime(4));//[#]

	m_ImageFrame = 0;//0 - current frame, 1 - next frame

	m_FBufSize = -1;
}

void tCameraVC0706::tStateGetImageStart::Control()
{
	if (!m_ErrTimer.GetState())
	{
		p_obj->OnFailed(tCameraVC0706Error_StateGetImageStart_ErrTimer);

		ChangeState(tStateReset::Instance(p_obj));
		return;
	}

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
		case 2:
		{
			CameraVC0706::Packet::tPacketCmd Packet;
			Packet.MsgId = CameraVC0706::Packet::MsgId_FBufCtrl;

			Packet.Payload.push_back((char)CameraVC0706::Packet::tPayload_FBUF_CTRL_ControlFlag_StopCurrentFrame);

			std::vector<char> LocalVector = Packet.ToVector();

			p_obj->Board_Send(LocalVector);

			break;
		}
		case 4:
		{
			CameraVC0706::Packet::tPacketCmd Packet;
			Packet.MsgId = CameraVC0706::Packet::MsgId_GetFBufLength;

			Packet.Payload.push_back(m_ImageFrame);

			std::vector<char> LocalVector = Packet.ToVector();

			p_obj->Board_Send(LocalVector);

			break;
		}
		default:
		{
			if (m_FBufSize > 0)
			{
				p_obj->OnImageReady();

				ChangeState(tStateGetImageIdle::Instance(p_obj, m_ImageFrame, m_FBufSize));
				return;
			}
			else
			{
				p_obj->OnFailed(tCameraVC0706Error_StateGetImageStart_WrongSize);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}
		}
	}
}

void tCameraVC0706::tStateGetImageStart::OnReceivedMsg(std::vector<char>& data)
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
				p_obj->OnFailed(tCameraVC0706Error_StateGetImageStart_FBufCtrl_WrongAnswer);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}

		break;
	}
	case 3:
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
				p_obj->OnFailed(tCameraVC0706Error_StateGetImageStart_FBufCtrl_WrongAnswer);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}

		break;
	}
	case 5:
	{
		CameraVC0706::Packet::tPacketRet PacketRet;

		if (CameraVC0706::Packet::tPacketRet::TryParse(data, PacketRet))
		{
			if (PacketRet.MsgId == CameraVC0706::Packet::MsgId_GetFBufLength && PacketRet.Status == CameraVC0706::Packet::CERR_None)
			{
				m_FBufSize = utils::ReadInt(PacketRet.Payload.begin(), PacketRet.Payload.end(), utils::tEndian_Big);

				m_Step++;
			}
			else
			{
				p_obj->OnFailed(tCameraVC0706Error_StateGetImageStart_GetFBufLength_WrongAnswer);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}

		break;
	}
	}
}

}*/