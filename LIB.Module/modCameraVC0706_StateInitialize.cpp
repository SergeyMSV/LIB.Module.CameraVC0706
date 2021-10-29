#include "modCameraVC0706.h"
/*
#include <stdio.h>

//#define LIB_MODULE_CAMERA_VC0706_STATE_INITIALIZE_TEST_1

//VC0703 1.00
//Ctrl infr exist
//User - defined sensor
//525
//Init end

namespace mod
{

tCameraVC0706::tStateInitialize::tStateInitialize(tCameraVC0706 *obj)
	:tState(obj)
{
#ifdef LIB_MODULE_LOG
	p_obj->p_log->WriteLine("tStateInitialize");
#endif//LIB_MODULE_LOG

	m_ErrTimer.SetValue(utils::tTime(30));//[#]

#ifdef LIB_MODULE_CAMERA_VC0706_INITIALIZE_CONFIG
	m_Memory = CameraVC0706::Packet::tMemoryDevice_I2C_EEPROM;
#endif//LIB_MODULE_CAMERA_VC0706_INITIALIZE_CONFIG

	m_BaudrateStep = 0;

	m_BaudrateRequired = false;

	m_ResetNeeded = false;
}

void tCameraVC0706::tStateInitialize::Tick10ms()
{
	m_BaudrateTimer.Tick10ms();

	tState::Tick10ms();
}

void tCameraVC0706::tStateInitialize::Tick100ms()
{
	m_BaudrateTimer.Tick100ms();

	tState::Tick100ms();
}

void tCameraVC0706::tStateInitialize::Control()
{
	if (!m_ErrTimer.GetState())
	{
		p_obj->OnFailed(tCameraVC0706Error_StateInitialize_ErrTimer);

		ChangeState(tStateReset::Instance(p_obj));
		return;
	}

	if (m_Step == 1 && !m_BaudrateTimer.GetState())//Find out baudrate of the connected camera
	{
		m_Step = 0;

		m_BaudrateRequired = false;

		switch (m_BaudrateStep++)
		{
#ifdef LIB_MODULE_CAMERA_VC0706_AUTOBAUDING_SHORT
		case 0: p_obj->Board_SetSerialPort(115200); m_BaudrateRequired = true; break;
		case 1: p_obj->Board_SetSerialPort(38400); break;
#else//LIB_MODULE_CAMERA_VC0706_AUTOBAUDING_SHORT
		case 0: p_obj->Board_SetSerialPort(115200); m_BaudrateRequired = true; break;
		case 1: p_obj->Board_SetSerialPort(38400); break;
		case 2: p_obj->Board_SetSerialPort(9600); break;
		case 3: p_obj->Board_SetSerialPort(19200); break;
		case 4: p_obj->Board_SetSerialPort(57600); break;

		//case 0: p_obj->Board_SetSerialPort(9600); break;
		//case 1: p_obj->Board_SetSerialPort(19200); break;
		//case 2: p_obj->Board_SetSerialPort(38400); break;
		//case 3: p_obj->Board_SetSerialPort(57600); break;
		//case 4: p_obj->Board_SetSerialPort(115200); m_BaudrateRequired = true; break;
#endif//LIB_MODULE_CAMERA_VC0706_AUTOBAUDING_SHORT
		default:
		{
			p_obj->OnFailed(tCameraVC0706Error_StateInitialize_BaudrateStep);

			ChangeState(tStateReset::Instance(p_obj));
			return;
		}
		}

		m_BaudrateTimer.SetValue(50);//500 ms
		//m_BaudrateTimer.SetValue(utils::tTime(1));
	}

	if (m_Step % 2 != 0 && !m_Timer.GetState() && m_Step == 5)//Repeate next Cmd after BR change. Connected with [*]Step=4
	{
		m_Step--;
	}

	if (m_Step % 2 == 0 && !m_Timer.GetState())
	{
		switch (m_Step++)
		{
		case 0:
		{
			CameraVC0706::Packet::tPacketCmd Packet;
			Packet.MsgId = CameraVC0706::Packet::MsgId_GetVersion;

			std::vector<char> LocalVector = Packet.ToVector();

			p_obj->Board_Send(LocalVector);

			break;
		}
#ifdef LIB_MODULE_CAMERA_VC0706_SERIAL_PORT_SET_BAUDRATE
		case 2://SerialPort
		{
			if (!m_BaudrateRequired)
			{
				CameraVC0706::Packet::tPacketCmd Packet;
				Packet.MsgId = CameraVC0706::Packet::MsgId_SetPort;

				Packet.Payload.push_back(CameraVC0706::Packet::tSerialCommInterface_UART);
				//Packet.Payload.push_back(0x2A);//S1RELH 38400
				//Packet.Payload.push_back(0xF2);//S1RELL 38400
				Packet.Payload.push_back(0x0D);//S1RELH 115200
				Packet.Payload.push_back(0xA6);//S1RELL 115200

				std::vector<char> LocalVector = Packet.ToVector();

				p_obj->Send(LocalVector);
			}
			else
			{
				m_Step += 3;
			}

			break;
		}
		case 4:
		{
			CameraVC0706::Packet::tPacketCmd Packet;
			Packet.MsgId = CameraVC0706::Packet::MsgId_GetVersion;

			std::vector<char> LocalVector = Packet.ToVector();

			p_obj->Send(LocalVector);

			m_Timer.SetValue(20);//200 ms

			break;
		}
#else//LIB_MODULE_CAMERA_VC0706_SERIAL_PORT_SET_BAUDRATE
		case 2:
		{
			m_Step = 6;

			break;
		}
#endif//LIB_MODULE_CAMERA_VC0706_SERIAL_PORT_SET_BAUDRATE
#ifdef LIB_MODULE_CAMERA_VC0706_INITIALIZE_CONFIG
		case 6://Get VideoResolution
		{
			if (p_obj->m_Settings.Field.VideoResolution != 0)
			{
				CameraVC0706::Packet::tPacketCmd Packet;
				Packet.MsgId = CameraVC0706::Packet::MsgId_ReadDataReg;

				Packet.Payload.push_back((char)m_Memory);
				Packet.Payload.push_back(1);//DataSize, bytes

				std::vector<char> LocalVector = utils::ToVector(CameraVC0706::Packet::REG_Address_Settings_VideoResolution, utils::tEndian_Big);

				Packet.Payload.insert(Packet.Payload.end(), LocalVector.begin(), LocalVector.end());

				LocalVector = Packet.ToVector();

				p_obj->Board_Send(LocalVector);
			}
			else
			{
				m_Step += 3;
			}

			break;
		}
		case 8://Set VideoResolution
		{
			if (p_obj->m_Settings.Field.VideoResolution != 0)
			{
				m_ResetNeeded = true;

				CameraVC0706::Packet::tPacketCmd Packet;
				Packet.MsgId = CameraVC0706::Packet::MsgId_WriteDataReg;

				Packet.Payload.push_back((char)m_Memory);
				Packet.Payload.push_back(1);//DataSize, bytes

				std::vector<char> LocalVector = utils::ToVector(CameraVC0706::Packet::REG_Address_Settings_VideoResolution, utils::tEndian_Big);

				Packet.Payload.insert(Packet.Payload.end(), LocalVector.begin(), LocalVector.end());

				std::vector<char> Data;

				switch (p_obj->m_Settings.Field.VideoResolution)
				{
				case 1: Data.push_back((char)mod::CameraVC0706::Packet::tVideoResolution_160x120); break;
				case 2: Data.push_back((char)mod::CameraVC0706::Packet::tVideoResolution_320x240); break;
				case 3: Data.push_back((char)mod::CameraVC0706::Packet::tVideoResolution_640x480); break;
				}

				Packet.Payload.insert(Packet.Payload.end(), Data.begin(), Data.end());

				LocalVector = Packet.ToVector();

				p_obj->Board_Send(LocalVector);
			}

			break;
		}
		case 10://Get SerialPort
		{
			CameraVC0706::Packet::tPacketCmd Packet;
			Packet.MsgId = CameraVC0706::Packet::MsgId_ReadDataReg;

			Packet.Payload.push_back((char)m_Memory);
			Packet.Payload.push_back(7);//DataSize, bytes

			std::vector<char> LocalVector = utils::ToVector(CameraVC0706::Packet::REG_Address_Settings_SerialPort, utils::tEndian_Big);

			Packet.Payload.insert(Packet.Payload.end(), LocalVector.begin(), LocalVector.end());

			LocalVector = Packet.ToVector();

			p_obj->Board_Send(LocalVector);

			break;
		}
		case 12://Set SerialPort
		{
			m_ResetNeeded = true;

			CameraVC0706::Packet::tPacketCmd Packet;
			Packet.MsgId = CameraVC0706::Packet::MsgId_WriteDataReg;

			Packet.Payload.push_back((char)m_Memory);
			Packet.Payload.push_back(2);//DataSize, bytes

			std::vector<char> LocalVector = utils::ToVector(CameraVC0706::Packet::REG_Address_Settings_SerialPort_UART_BR, utils::tEndian_Big);

			Packet.Payload.insert(Packet.Payload.end(), LocalVector.begin(), LocalVector.end());

			std::vector<char> Data;

			Data.push_back(CameraVC0706::Packet::SerialCommUartBaudrate_115200.S1RELH);//S1RELH 115200
			Data.push_back(CameraVC0706::Packet::SerialCommUartBaudrate_115200.S1RELL);//S1RELL 115200

			Packet.Payload.insert(Packet.Payload.end(), Data.begin(), Data.end());

			LocalVector = Packet.ToVector();

			p_obj->Board_Send(LocalVector);

			break;
		}
		//case 10:
		//{
		//	CameraVC0706::Packet::tPacketCmd Packet;
		//	Packet.MsgId = CameraVC0706::Packet::MsgId_ReadDataReg;

		//	Packet.Payload.push_back((char)CameraVC0706::Packet::tMemoryDevice_I2C_EEPROM);//[#]
		//	Packet.Payload.push_back((char)7);//bytes

		//	std::vector<char> LocalVector = utils::ToVector((unsigned short)0x0007, utils::tEndian_Big);

		//	Packet.Payload.insert(Packet.Payload.end(), LocalVector.begin(), LocalVector.end());

		//	LocalVector = Packet.ToVector();

		//	p_obj->Send(LocalVector);
		//	
		//	break;
		//}
#endif//LIB_MODULE_CAMERA_VC0706_INITIALIZE_CONFIG
#ifdef LIB_MODULE_CAMERA_VC0706_STATE_INITIALIZE_TEST_1
		case 6:
		{
			CameraVC0706::Packet::tPacketCmd Packet;
			Packet.MsgId = CameraVC0706::Packet::MsgId_PowerSaveStatus;
			//Packet.MsgId = CameraVC0706::Packet::MsgId_DownsizeStatus;
			//Packet.MsgId = CameraVC0706::Packet::MsgId_OsdAddChar;

			Packet.Payload.push_back(0x01);//power-save control mode

			//Packet.Payload.push_back(0x07);//Characters number
			//Packet.Payload.push_back(0x24);//Start address (Write 7 characters, Vimicro, to channel on col 2 row 4 in turn)

			//Packet.Payload.push_back(0x1F);//V
			//Packet.Payload.push_back(0x2C);//i
			//Packet.Payload.push_back(0x30);//m
			//Packet.Payload.push_back(0x2C);//i
			//Packet.Payload.push_back(0x26);//c
			//Packet.Payload.push_back(0x35);//r
			//Packet.Payload.push_back(0x32);//o

			std::vector<char> LocalVector = Packet.ToVector();

			p_obj->Send(LocalVector);

			break;
		}
#endif//LIB_MODULE_CAMERA_VC0706_STATE_INITIALIZE_TEST_1
		default:
		{
			if (m_ResetNeeded)
			{
				ChangeState(tStateReset::Instance(p_obj));//Needed when config is modified
				return;
			}
			else
			{
				p_obj->OnReady();

				ChangeState(tStateIdle::Instance(p_obj));
				return;
			}
		}
		}
	}
}

void tCameraVC0706::tStateInitialize::OnReceivedMsg(std::vector<char>& data)
{
	switch (m_Step)
	{
	case 1:
	{
		CameraVC0706::Packet::tPacketRet PacketRet;

		if (CameraVC0706::Packet::tPacketRet::TryParse(data, PacketRet))
		{
			char Model1[] = "VC0703 1.00";
			std::vector<char> Model1Vector = utils::ToVector(Model1, sizeof(Model1) - 1);

			char Model2[] = "VC0706 1.00";
			std::vector<char> Model2Vector = utils::ToVector(Model2, sizeof(Model2) - 1);

			if (PacketRet.MsgId == CameraVC0706::Packet::MsgId_GetVersion && PacketRet.Status == CameraVC0706::Packet::CERR_None && (PacketRet.Payload == Model1Vector || PacketRet.Payload == Model2Vector))
			{
				m_Step++;
			}
			else
			{
				p_obj->OnFailed(tCameraVC0706Error_StateInitialize_GetVersion_WrongAnswer);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}

		break;
	}
#ifdef LIB_MODULE_CAMERA_VC0706_SERIAL_PORT_SET_BAUDRATE
	case 3:
	{
		CameraVC0706::Packet::tPacketRet PacketRet;

		if (CameraVC0706::Packet::tPacketRet::TryParse(data, PacketRet))
		{
			if (PacketRet.MsgId == CameraVC0706::Packet::MsgId_SetPort && PacketRet.Status == CameraVC0706::Packet::CERR_None)
			{
				m_Step++;

				p_obj->Board_SetSerialPort(115200);//Required baudrate
			}
			else
			{
				p_obj->OnFailed(tCameraVC0706Error_StateInitialize_SetPort_WrongAnswer);

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
			if (PacketRet.MsgId == CameraVC0706::Packet::MsgId_GetVersion && PacketRet.Status == CameraVC0706::Packet::CERR_None)
			{
				m_Step++;
			}
			else
			{
				p_obj->OnFailed(tCameraVC0706Error_StateInitialize_GetVersion_WrongAnswer2);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}

		break;
	}
#endif//LIB_MODULE_CAMERA_VC0706_SERIAL_PORT_SET_BAUDRATE
#ifdef LIB_MODULE_CAMERA_VC0706_INITIALIZE_CONFIG
	case 7://Get VideoResolutiuon
	{
		CameraVC0706::Packet::tPacketRet PacketRet;

		if (CameraVC0706::Packet::tPacketRet::TryParse(data, PacketRet))
		{
			if (PacketRet.MsgId == CameraVC0706::Packet::MsgId_ReadDataReg && PacketRet.Status == CameraVC0706::Packet::CERR_None)
			{
				if ((PacketRet.Payload[0] == (char)CameraVC0706::Packet::tVideoResolution_160x120 && p_obj->m_Settings.Field.VideoResolution == 1) ||
					(PacketRet.Payload[0] == (char)CameraVC0706::Packet::tVideoResolution_320x240 && p_obj->m_Settings.Field.VideoResolution == 2) ||
					(PacketRet.Payload[0] == (char)CameraVC0706::Packet::tVideoResolution_640x480 && p_obj->m_Settings.Field.VideoResolution == 3))
				{
					m_Step += 3;
				}
				else
				{
					m_Step++;
				}
			}
			else
			{
				p_obj->OnFailed(tCameraVC0706Error_StateInitialize_Read_Config_VideoResolution_WrongAnswer);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}

		break;
	}
	case 9://Set VideoResolution
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
				p_obj->OnFailed(tCameraVC0706Error_StateInitialize_Write_Config_VideoResolution_WrongAnswer);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}

		break;
	}
	case 11://Get SerialPort
	{
		CameraVC0706::Packet::tPacketRet PacketRet;

		if (CameraVC0706::Packet::tPacketRet::TryParse(data, PacketRet))
		{
			if (PacketRet.MsgId == CameraVC0706::Packet::MsgId_ReadDataReg && PacketRet.Status == CameraVC0706::Packet::CERR_None)
			{
				if (PacketRet.Payload[0] == (char)mod::CameraVC0706::Packet::tSerialCommInterface_UART)
				{
#ifdef LIB_MODULE_LOG
					p_obj->p_log->WriteLine("SerialPort=UART");
#endif//LIB_MODULE_LOG

					if (PacketRet.Payload[1] == CameraVC0706::Packet::SerialCommUartBaudrate_115200.S1RELH &&
						PacketRet.Payload[2] == CameraVC0706::Packet::SerialCommUartBaudrate_115200.S1RELL)
					{
						m_Step += 3;
					}
					else
					{
						m_Step++;
					}
				}
				else////mod::CameraVC0706::Packet::tSerialCommInterface_HSUART - if the response received - baudrate is appropriate and HS UART is not a subject of setup.
				{
					m_Step += 3;
				}
			}
			else
			{
				p_obj->OnFailed(tCameraVC0706Error_StateInitialize_Read_Config_SerialPort_WrongAnswer);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}

		break;
	}
	case 13://Set SerialPort
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
				p_obj->OnFailed(tCameraVC0706Error_StateInitialize_Write_Config_SerialPort_WrongAnswer);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}

		break;
	}
#endif//LIB_MODULE_CAMERA_VC0706_INITIALIZE_CONFIG
#ifdef LIB_MODULE_CAMERA_VC0706_STATE_INITIALIZE_TEST_1
	case 7:
	{
		CameraVC0706::Packet::tPacketRet PacketRet;

		if (CameraVC0706::Packet::tPacketRet::TryParse(data, PacketRet))
		{
			if (PacketRet.MsgId == CameraVC0706::Packet::MsgId_DownsizeStatus && PacketRet.Status == CameraVC0706::Packet::CERR_None)
				//if (PacketRet.MsgId == CameraVC0706::Packet::MsgId_OsdAddChar && PacketRet.Status == CameraVC0706::Packet::CERR_None)
			{
				m_Step++;
			}
			else
			{
				p_obj->OnFailed(tCameraVC0706Error_StateInitialize_OsdAddChar_WrongAnswer);

				ChangeState(tStateReset::Instance(p_obj));
				return;
			}
		}

		break;
	}
#endif//LIB_MODULE_CAMERA_VC0706_STATE_INITIALIZE_TEST_1
	}
}

}*/