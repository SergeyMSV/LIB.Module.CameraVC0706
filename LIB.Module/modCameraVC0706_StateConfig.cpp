#include "modCameraVC0706.h"
/*
#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG

#include <stdio.h>

namespace mod
{

tCameraVC0706::tStateConfig::tStateConfig(tCameraVC0706 *obj, CameraVC0706::Packet::tMemoryDevice memory, int address, int size)
	:tState(obj)
{
#ifdef LIB_MODULE_LOG
	p_obj->p_log->WriteLine("tStateConfig");
#endif//LIB_MODULE_LOG

	m_ErrTimer.SetValue(utils::tTime(3));//[#]

	m_Memory = memory;

	m_Address = address;
	m_SizeToRead = size;

	m_Step = 0;
}

tCameraVC0706::tStateConfig::tStateConfig(tCameraVC0706 *obj, CameraVC0706::Packet::tMemoryDevice memory, int address, std::vector<char>& data)
	:tState(obj)
{
#ifdef LIB_MODULE_LOG
	p_obj->p_log->WriteLine("tStateConfig");
#endif//LIB_MODULE_LOG

	m_ErrTimer.SetValue(utils::tTime(3));//[#]

	m_Memory = memory;

	m_Address = address;
	m_SizeToRead = -1;
	m_Data = data;

	m_Step = 10;
}

void tCameraVC0706::tStateConfig::Control()
{
	if (!m_ErrTimer.GetState())
	{
		p_obj->OnFailed(tCameraVC0706Error_StateConfig_ErrTimer);

		ChangeState(tStateReset::Instance(p_obj));
		return;
	}

	if (m_Step % 2 == 0 && !m_Timer.GetState())
	{
		switch (m_Step++)
		{
		case 0://READ
		{
			CameraVC0706::Packet::tPacketCmd Packet;
			Packet.MsgId = CameraVC0706::Packet::MsgId_ReadDataReg;

			Packet.Payload.push_back((char)m_Memory);
			Packet.Payload.push_back((char)m_SizeToRead);

			std::vector<char> LocalVector = utils::ToVector((unsigned short)m_Address, utils::tEndian_Big);

			Packet.Payload.insert(Packet.Payload.end(), LocalVector.begin(), LocalVector.end());

			LocalVector = Packet.ToVector();

			p_obj->Board_Send(LocalVector);

			break;
		}
		case 2:
		{
			p_obj->OnGetConfig(m_Data);

			ChangeState(tStateIdle::Instance(p_obj));
			return;
		}
		case 10://WRITE
		{
			CameraVC0706::Packet::tPacketCmd Packet;
			Packet.MsgId = CameraVC0706::Packet::MsgId_WriteDataReg;

			Packet.Payload.push_back((char)m_Memory);
			Packet.Payload.push_back((char)m_Data.size());

			std::vector<char> LocalVector = utils::ToVector((unsigned short)m_Address, utils::tEndian_Big);

			Packet.Payload.insert(Packet.Payload.end(), LocalVector.begin(), LocalVector.end());

			Packet.Payload.insert(Packet.Payload.end(), m_Data.begin(), m_Data.end());

			LocalVector = Packet.ToVector();

			p_obj->Board_Send(LocalVector);

			break;
		}
		case 12:
		{
			p_obj->OnSetConfig();
		}
		default:
		{
			ChangeState(tStateReset::Instance(p_obj));
			return;
		}
		}
	}
}

void tCameraVC0706::tStateConfig::OnReceivedMsg(std::vector<char>& data)
{
	switch (m_Step)
	{
	case 1:
	{
		CameraVC0706::Packet::tPacketRet PacketRet;

		if (CameraVC0706::Packet::tPacketRet::TryParse(data, PacketRet))
		{
			if (PacketRet.MsgId == CameraVC0706::Packet::MsgId_ReadDataReg && PacketRet.Status == CameraVC0706::Packet::CERR_None)
			{
				m_Step++;//END

				m_Data = PacketRet.Payload;
			}
			else
			{
				p_obj->OnFailed(tCameraVC0706Error_StateConfig_Read_WrongAnswer);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}

		break;
	}
	case 11://WRITE
	{
		CameraVC0706::Packet::tPacketRet PacketRet;

		if (CameraVC0706::Packet::tPacketRet::TryParse(data, PacketRet))
		{
			if (PacketRet.MsgId == CameraVC0706::Packet::MsgId_WriteDataReg && PacketRet.Status == CameraVC0706::Packet::CERR_None)
			{
				m_Step++;
			}
			else
			{
				p_obj->OnFailed(tCameraVC0706Error_StateConfig_Write_WrongAnswer);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}

		break;
	}
	}
}

}

#endif//LIB_MODULE_CAMERA_VC0706_CONFIG
*/