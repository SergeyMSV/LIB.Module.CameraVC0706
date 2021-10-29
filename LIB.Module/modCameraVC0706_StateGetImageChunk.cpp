#include "modCameraVC0706.h"
/*
namespace mod
{

tCameraVC0706::tStateGetImageChunk::tStateGetImageChunk(tCameraVC0706 *obj, char imageFrame, int fBufSize, int chunkStartAddress, int chunkSize)
	:tState(obj)
{
#ifdef LIB_MODULE_LOG
	p_obj->p_log->WriteLine("tStateGetImageChunk");
#endif//LIB_MODULE_LOG

	m_ErrTimer.SetValue(utils::tTime(30));//[#] it depends on chunkSize and baudrate

	m_ImageFrame = imageFrame;

	m_FBufSize = fBufSize;

	m_ChunkStartAddress = chunkStartAddress;

	m_ChunkSize = chunkSize;

	m_ChunkTransferStep = 0;//No transfer

	int Remains = m_FBufSize - m_ChunkStartAddress;

	if (Remains < m_ChunkSize)
	{
		m_ChunkSize = Remains;
	}

	m_Chunk = std::vector<char>(m_ChunkSize);

	m_ChunkIndex = 0;
}

void tCameraVC0706::tStateGetImageChunk::Control()
{
	if (!m_ErrTimer.GetState())
	{
		p_obj->OnFailed(tCameraVC0706Error_StateGetImageChunk_ErrTimer);

		ChangeState(tStateReset::Instance(p_obj));
		return;
	}

	if (m_Step % 2 == 0 && !m_Timer.GetState())
	{
		switch (m_Step++)
		{
		case 0:
		{
			if (m_FBufSize > 0 && m_FBufSize > m_ChunkStartAddress && m_ChunkSize > 0)
			{
				CameraVC0706::Packet::tPacketCmd Packet;
				Packet.MsgId = CameraVC0706::Packet::MsgId_ReadFBuf;

				Packet.Payload.push_back(m_ImageFrame);

				CameraVC0706::Packet::tPayload_READ_FBUF_ControlMode ControlMode;
				ControlMode.Value = 0;
				//ControlMode.Field.TRANSFER_MODE = 1;//DMA
				ControlMode.Field.TRANSFER_MODE = 0;//MCU
				ControlMode.Field.NONAME_1 = 1;
				ControlMode.Field.NONAME_2 = 1;

				Packet.Payload.push_back(ControlMode.Value);

				std::vector<char> LocalVector = utils::ToVector(m_ChunkStartAddress, utils::tEndian_Big);//4 bytes

				Packet.Payload.insert(Packet.Payload.end(), LocalVector.begin(), LocalVector.end());

				LocalVector = utils::ToVector(m_ChunkSize, utils::tEndian_Big);//4 bytes

				Packet.Payload.insert(Packet.Payload.end(), LocalVector.begin(), LocalVector.end());

				LocalVector = utils::ToVector((unsigned short)3000, utils::tEndian_Big);//DELAY 2 bytes

				Packet.Payload.insert(Packet.Payload.end(), LocalVector.begin(), LocalVector.end());

				LocalVector = Packet.ToVector();

				p_obj->Board_Send(LocalVector);
			}
			else
			{
				p_obj->OnFailed(tCameraVC0706Error_StateGetImageChunk_WrongSize);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}

			break;
		}
		default:
		{
			p_obj->OnImageChunk(m_Chunk);

			m_ChunkStartAddress += m_ChunkSize;

			if (m_ChunkStartAddress < m_FBufSize)
			{
				ChangeState(tStateGetImageIdle::Instance(p_obj, m_ImageFrame, m_FBufSize, m_ChunkStartAddress));
				return;
			}
			else
			{
				ChangeState(tStateGetImageStop::Instance(p_obj));
				return;
			}
		}
		}
	}
}

void tCameraVC0706::tStateGetImageChunk::Receive(std::vector<char>& data)
{
	if (m_ChunkTransferStep == 0)
	{
		tState::Receive(data);

		if (m_ChunkTransferStep == 1)//is being transferred
		{
			HandleReceived(m_ReceivedData);
		}
	}
	else if (m_ChunkTransferStep == 1)//is being transferred
	{
		HandleReceived(data);
	}

	if (m_ChunkTransferStep == 2)//has been transferred
	{
		tState::Receive(data);
	}
}

void tCameraVC0706::tStateGetImageChunk::OnReceivedMsg(std::vector<char>& data)
{
	switch (m_Step)
	{
	case 1:
	{
		CameraVC0706::Packet::tPacketRet PacketRet;

		if (CameraVC0706::Packet::tPacketRet::TryParse(data, PacketRet))
		{
			if (PacketRet.MsgId == CameraVC0706::Packet::MsgId_ReadFBuf && PacketRet.Status == CameraVC0706::Packet::CERR_None)
			{
				if (m_ChunkTransferStep == 0)
				{
					m_ChunkTransferStep = 1;//is being transferred
				}
				else
				{
					m_Step++;
				}
			}
			else
			{
				p_obj->OnFailed(tCameraVC0706Error_StateGetImageChunk_ReadFBuf_WrongAnswer);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}

		break;
	}
	}
}

void tCameraVC0706::tStateGetImageChunk::HandleReceived(std::vector<char>& data)
{
	unsigned int i = 0;

	for (i = 0; i < data.size() && m_ChunkIndex < m_Chunk.size(); ++i)
	{
		m_Chunk[m_ChunkIndex++] = data[i];
	}

	data.erase(data.begin(), data.begin() + i);

	if (m_ChunkIndex == m_Chunk.size())
	{
		m_ChunkTransferStep = 2;//has been transferred
	}
}

}*/