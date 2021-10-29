#include "modCameraVC0706.h"
/*
namespace mod
{

tCameraVC0706::tStateIdle::tStateIdle(tCameraVC0706 *obj)
	:tState(obj)
{
#ifdef LIB_MODULE_LOG
	p_obj->p_log->WriteLine("tStateIdle");
#endif//LIB_MODULE_LOG

	m_ErrTimer.SetValue(utils::tTime(LIB_MODULE_CAMERA_VC0706_CHECK_CONNECT_FAIL_TIME));

	m_Timer.SetValue(utils::tTime(LIB_MODULE_CAMERA_VC0706_CHECK_CONNECT_TIME));

	m_Task = tTask_None;

	m_CheckConnect = false;
}

void tCameraVC0706::tStateIdle::Control()
{
	if (!m_ErrTimer.GetState())
	{
		p_obj->OnFailed(tCameraVC0706Error_StateIdle_ErrTimer);

		ChangeState(tStateReset::Instance(p_obj));
		return;
	}

	if (m_Step % 2 == 0 && !m_Timer.GetState())
	{
		switch (m_Step++)
		{
		case 0:
		{
			m_CheckConnect = true;

			CameraVC0706::Packet::tPacketCmd Packet;
			Packet.MsgId = CameraVC0706::Packet::MsgId_GetVersion;

			std::vector<char> LocalVector = Packet.ToVector();

			p_obj->Board_Send(LocalVector);

			break;
		}
		case 2:
		{
			m_CheckConnect = false;

			m_ErrTimer.SetValue(utils::tTime(LIB_MODULE_CAMERA_VC0706_CHECK_CONNECT_FAIL_TIME));

			m_Timer.SetValue(utils::tTime(LIB_MODULE_CAMERA_VC0706_CHECK_CONNECT_TIME));

			m_Step = 0;

			break;
		}
		}
	}

	if (!m_CheckConnect && m_Task != tTask_None)
	{
		switch (m_Task)
		{
		case tTask_GetImageReady:
		{
			ChangeState(tStateGetImageStart::Instance(p_obj));
			return;
		}
		case tTask_GetConfig:
		{
			ChangeState(tStateConfig::Instance(p_obj, m_ConfigMemory, m_ConfigAddress, m_ConfigSize));
			return;
		}
		case tTask_SetConfig:
		{
			ChangeState(tStateConfig::Instance(p_obj, m_ConfigMemory, m_ConfigAddress, m_ConfigData));
			return;
		}
		default:
		{
			m_Task = tTask_None;

			break;
		}
		}
	}
}

bool tCameraVC0706::tStateIdle::GetImageReady()
{
	if (m_Task == tTask_None)
	{
		m_Task = tTask_GetImageReady;

		return true;
	}

	return false;

	//ChangeState(tStateGetImageStart::Instance(p_obj));
	//return true;
}

#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG
bool tCameraVC0706::tStateIdle::GetConfig(CameraVC0706::Packet::tMemoryDevice memory, int address, int size)
{
	if (m_Task == tTask_None)
	{
		m_Task = tTask_GetConfig;

		m_ConfigMemory = memory;
		m_ConfigAddress = address;
		m_ConfigSize = size;

		return true;
	}

	return false;

	//ChangeState(tStateConfig::Instance(p_obj, memory, address, size));
	//return true;
}

bool tCameraVC0706::tStateIdle::SetConfig(CameraVC0706::Packet::tMemoryDevice memory, int address, std::vector<char>& data)
{
	if (m_Task == tTask_None)
	{
		m_Task = tTask_SetConfig;

		m_ConfigMemory = memory;
		m_ConfigAddress = address;
		m_ConfigData = data;

		return true;
	}

	return false;

	//ChangeState(tStateConfig::Instance(p_obj, memory, address, data));
	//return true;
}
#endif//LIB_MODULE_CAMERA_VC0706_CONFIG

void tCameraVC0706::tStateIdle::OnReceivedMsg(std::vector<char>& data)
{
	switch (m_Step)
	{
	case 1:
	{
		CameraVC0706::Packet::tPacketRet PacketRet;

		if (CameraVC0706::Packet::tPacketRet::TryParse(data, PacketRet))
		{
			m_Step++;
		}

		break;
	}
	}
}

}*/