#include "utilsPacketCameraVC0706.h"

namespace utils
{
	namespace packet_CameraVC0706
	{


tPayloadCmd tPayloadCmd::MakeGetVersion(std::uint8_t sn)
{
	tPayloadCmd Cmd;
	Cmd.Value.SerialNumber = sn;
	Cmd.Value.MsgId = tMsgId::GetVersion;
	return Cmd;
}

tPayloadCmd tPayloadCmd::MakeSetSerialNumber(std::uint8_t sn, std::uint8_t value)
{
	tPayloadCmd Cmd;
	Cmd.Value.SerialNumber = sn;
	Cmd.Value.MsgId = tMsgId::SetSerialNumber;
	Cmd.Value.Payload.push_back(value);
	return Cmd;
}

#pragma pack(push, 1)
struct tSetPortUART_BR
{
	std::uint8_t S1RELH = 0;
	std::uint8_t S1RELL = 0;

	bool operator == (const tSetPortUART_BR& value) const
	{
		return S1RELH == value.S1RELH && S1RELL == value.S1RELL;
	}
};
#pragma pack(pop)

constexpr tSetPortUART_BR SetPortUART_BR[] = {
{ 0xAE, 0xC8 },//9600
{ 0x56, 0xE4 },//19200
{ 0x2A, 0xF2 },//38400
{ 0x1C, 0x1C },//57600
{ 0x0D, 0xA6 } //115200
};

#pragma pack(push, 1)
struct tSetPortUARTHS_BR
{
	std::uint8_t S1RELHH = 0;
	std::uint8_t S1RELHL = 0;
	std::uint8_t S1RELLH = 0;
	std::uint8_t S1RELLL = 0;

	bool operator == (const tSetPortUARTHS_BR& value) const
	{
		return S1RELHH == value.S1RELHH && S1RELHL == value.S1RELHL && S1RELLH == value.S1RELLH && S1RELLL == value.S1RELLL;
	}
};
#pragma pack(pop)

constexpr tSetPortUARTHS_BR SetPortUARTHS_BR[] = {
{ 0x00, 0x2B, 0x03, 0xC8 },//38400
{ 0x00, 0x1D, 0x01, 0x30 },//57600
{ 0x00, 0x0E, 0x02, 0x98 },//115200
{ 0x00, 0x03, 0x02, 0xA6 },//460800
{ 0x00, 0x01, 0x03, 0x53 } //921600
};

tPayloadCmd tPayloadCmd::MakeSetPortUART(std::uint8_t sn, tUARTBaudrate baudrate)
{
	assert(static_cast<std::size_t>(baudrate) < (sizeof(SetPortUART_BR) / sizeof(tSetPortUART_BR)));

	tPayloadCmd Cmd;
	Cmd.Value.SerialNumber = sn;
	Cmd.Value.MsgId = tMsgId::SetPort;
	Cmd.Value.Payload.push_back(static_cast<std::uint8_t>(tPort::UART));
	const tSetPortUART_BR Port = SetPortUART_BR[static_cast<std::size_t>(baudrate)];
	Cmd.Value.Payload.push_back(Port.S1RELH);
	Cmd.Value.Payload.push_back(Port.S1RELL);
	return Cmd;
}

tPayloadCmd tPayloadCmd::MakeSetPortUARTHS(std::uint8_t sn, tUARTHSBaudrate baudrate)
{
	assert(static_cast<std::size_t>(baudrate) < (sizeof(SetPortUARTHS_BR) / sizeof(tSetPortUARTHS_BR)));

	tPayloadCmd Cmd;
	Cmd.Value.SerialNumber = sn;
	Cmd.Value.MsgId = tMsgId::SetPort;
	Cmd.Value.Payload.push_back(static_cast<std::uint8_t>(tPort::UARTHS));
	const tSetPortUARTHS_BR Port = SetPortUARTHS_BR[static_cast<std::size_t>(baudrate)];
	Cmd.Value.Payload.push_back(Port.S1RELHH);
	Cmd.Value.Payload.push_back(Port.S1RELHL);
	Cmd.Value.Payload.push_back(Port.S1RELLH);
	Cmd.Value.Payload.push_back(Port.S1RELLL);
	return Cmd;
}

tPayloadCmd tPayloadCmd::MakeSystemReset(std::uint8_t sn)
{
	tPayloadCmd Cmd;
	Cmd.Value.SerialNumber = sn;
	Cmd.Value.MsgId = tMsgId::SystemReset;
	return Cmd;
}

constexpr tDataReg DataReg_Port{ 0x0007, 1 };
constexpr tDataReg DataReg_PortUART_BR{ 0x0008, 2 };
constexpr tDataReg DataReg_PortUARTHS_BR{ 0x000A, 4 };
constexpr tDataReg DataReg_VideoResolution{ 0x0019, 1 };
constexpr tDataReg DataReg_VideoCompression{ 0x001A, 1 };

tPayloadCmd tPayloadCmd::MakeReadDataReg(tMemoryDataReg memory, std::uint8_t sn, tDataReg reg)
{
	tPayloadCmd Cmd;
	Cmd.Value.SerialNumber = sn;
	Cmd.Value.MsgId = tMsgId::ReadDataReg;
	Cmd.Value.Payload.push_back(static_cast<std::uint8_t>(memory));
	Cmd.Value.Payload.push_back(reg.Size);
	Cmd.Value.Payload.push_back(static_cast<std::uint8_t>(reg.Address >> 8));
	Cmd.Value.Payload.push_back(static_cast<std::uint8_t>(reg.Address));
	return Cmd;
}

tPayloadCmd tPayloadCmd::MakeReadDataReg_Port(tMemoryDataReg memory, std::uint8_t sn)
{
	return MakeReadDataReg(memory, sn, DataReg_Port);
}

tPayloadCmd tPayloadCmd::MakeReadDataReg_PortUART(tMemoryDataReg memory, std::uint8_t sn)
{
	return MakeReadDataReg(memory, sn, DataReg_PortUART_BR);
}

tPayloadCmd tPayloadCmd::MakeReadDataReg_PortUARTHS(tMemoryDataReg memory, std::uint8_t sn)
{
	return MakeReadDataReg(memory, sn, DataReg_PortUARTHS_BR);
}

tPayloadCmd tPayloadCmd::MakeReadDataReg_VideoResolution(tMemoryDataReg memory, std::uint8_t sn)
{
	return MakeReadDataReg(memory, sn, DataReg_VideoResolution);
}

tPayloadCmd tPayloadCmd::MakeReadDataReg_VideoCompression(tMemoryDataReg memory, std::uint8_t sn)
{
	return MakeReadDataReg(memory, sn, DataReg_VideoCompression);
}

tPayloadCmd tPayloadCmd::MakeWriteDataReg(tMemoryDataReg memory, std::uint8_t sn, tDataReg reg, const tVectorUInt8& data)
{
	assert(reg.Size == data.size());

	tPayloadCmd Cmd;
	Cmd.Value.SerialNumber = sn;
	Cmd.Value.MsgId = tMsgId::WriteDataReg;
	Cmd.Value.Payload.push_back(static_cast<std::uint8_t>(memory));
	Cmd.Value.Payload.push_back(reg.Size);
	Cmd.Value.Payload.push_back(static_cast<std::uint8_t>(reg.Address >> 8));
	Cmd.Value.Payload.push_back(static_cast<std::uint8_t>(reg.Address));
	Cmd.Value.Payload.insert(Cmd.Value.Payload.end(), data.cbegin(), data.cend());
	return Cmd;
}

tPayloadCmd tPayloadCmd::MakeWriteDataReg_Port(tMemoryDataReg memory, std::uint8_t sn, tPort port)
{
	return MakeWriteDataReg(memory, sn, DataReg_Port, { static_cast<std::uint8_t>(port) });
}

tPayloadCmd tPayloadCmd::MakeWriteDataReg_PortUART(tMemoryDataReg memory, std::uint8_t sn, tUARTBaudrate baudrate)
{
	assert(static_cast<std::size_t>(baudrate) < (sizeof(SetPortUART_BR) / sizeof(tSetPortUART_BR)));

	return MakeWriteDataReg(memory, sn, DataReg_PortUART_BR, ToVector(SetPortUART_BR[static_cast<std::size_t>(baudrate)]));
}

tPayloadCmd tPayloadCmd::MakeWriteDataReg_PortUARTHS(tMemoryDataReg memory, std::uint8_t sn, tUARTHSBaudrate baudrate)
{
	return MakeWriteDataReg(memory, sn, DataReg_PortUARTHS_BR, ToVector(SetPortUARTHS_BR[static_cast<std::size_t>(baudrate)]));
}

tPayloadCmd tPayloadCmd::MakeWriteDataReg_VideoResolution(tMemoryDataReg memory, std::uint8_t sn, tVideoResolution resolution)
{
	return MakeWriteDataReg(memory, sn, DataReg_VideoResolution, {static_cast<std::uint8_t>(resolution)});
}

enum class tFBufType : std::uint8_t
{
	Current,
	Next,
};

enum class tFBufTransferMode : std::uint8_t
{
	MCU,
	DMA,
};

#pragma pack(push, 1)
union tFBufControlModeRead
{
	struct
	{
		std::uint8_t TRANSFER_MODE : 1;//tFBufTransferMode
		std::uint8_t NONAME_1 : 2;
		std::uint8_t NONAME_2 : 1;
		std::uint8_t : 4;
	}Field;

	std::uint8_t Value;
};
#pragma pack(pop)

tPayloadCmd tPayloadCmd::MakeReadFBufCurrent(std::uint8_t sn, std::uint32_t address, std::uint32_t size, std::uint16_t delay)
{
	tPayloadCmd Cmd;
	Cmd.Value.SerialNumber = sn;
	Cmd.Value.MsgId = tMsgId::ReadFBuf;
	Cmd.Value.Payload.push_back(static_cast<std::uint8_t>(tFBufType::Current));

	tFBufControlModeRead ControlModeRead;
	ControlModeRead.Value = 0;
	ControlModeRead.Field.TRANSFER_MODE = static_cast<std::uint8_t>(tFBufTransferMode::DMA);
	ControlModeRead.Field.NONAME_1 = 3;
	ControlModeRead.Field.NONAME_2 = 1;
	Cmd.Value.Payload.push_back(ControlModeRead.Value);

	tVectorUInt8 LocalVec = ToVector(address);
	std::reverse(LocalVec.begin(), LocalVec.end());
	Cmd.Value.Payload.insert(Cmd.Value.Payload.end(), LocalVec.cbegin(), LocalVec.cend());

	LocalVec = ToVector(size);
	std::reverse(LocalVec.begin(), LocalVec.end());
	Cmd.Value.Payload.insert(Cmd.Value.Payload.end(), LocalVec.cbegin(), LocalVec.cend());

	LocalVec = ToVector(delay);
	std::reverse(LocalVec.begin(), LocalVec.end());
	Cmd.Value.Payload.insert(Cmd.Value.Payload.end(), LocalVec.cbegin(), LocalVec.cend());

	return Cmd;
}


/*
CameraVC0706::Packet::tPayload_READ_FBUF_ControlMode ControlMode;
ControlMode.Value = 0;
//ControlMode.Field.TRANSFER_MODE = 1;//DMA
ControlMode.Field.TRANSFER_MODE = 0;//MCU
ControlMode.Field.NONAME_1 = 1;
ControlMode.Field.NONAME_2 = 1;
*/

/*union tFBufControlModeWrite
{
	struct
	{
		std::uint8_t TRANSFER_MODE : 1;//tFBufTransferMode
		std::uint8_t NONAME_1 : 2;
		std::uint8_t NONAME_2 : 1;
		std::uint8_t FIRST_WRITE : 1;//0 - no, 1 - yes
		std::uint8_t : 3;
	}Field;

	std::uint8_t Value;
};*/

tPayloadCmd tPayloadCmd::MakeGetFBufLenCurrent(std::uint8_t sn)
{
	tPayloadCmd Cmd;
	Cmd.Value.SerialNumber = sn;
	Cmd.Value.MsgId = tMsgId::GetFBufLength;
	Cmd.Value.Payload.push_back(static_cast<std::uint8_t>(tFBufType::Current));
	return Cmd;
}

tPayloadCmd tPayloadCmd::MakeGetFBufLenNext(std::uint8_t sn)
{
	tPayloadCmd Cmd;
	Cmd.Value.SerialNumber = sn;
	Cmd.Value.MsgId = tMsgId::GetFBufLength;
	Cmd.Value.Payload.push_back(static_cast<std::uint8_t>(tFBufType::Next));
	return Cmd;
}

enum class tFBufCtrlFrame : std::uint8_t
{
	StopCurrent,
	StopNext,
	Resume,
	Step,
};

tPayloadCmd tPayloadCmd::MakeFBufCtrlStopCurrentFrame(std::uint8_t sn)
{
	tPayloadCmd Cmd;
	Cmd.Value.SerialNumber = sn;
	Cmd.Value.MsgId = tMsgId::FBufCtrl;
	Cmd.Value.Payload.push_back(static_cast<std::uint8_t>(tFBufCtrlFrame::StopCurrent));
	return Cmd;
}

tPayloadCmd tPayloadCmd::MakeFBufCtrlResumeFrame(std::uint8_t sn)
{
	tPayloadCmd Cmd;
	Cmd.Value.SerialNumber = sn;
	Cmd.Value.MsgId = tMsgId::FBufCtrl;
	Cmd.Value.Payload.push_back(static_cast<std::uint8_t>(tFBufCtrlFrame::Resume));
	return Cmd;
}


tMsgStatus tPayloadRet::ParseReadDataReg_Port(const tPayloadRet& payload, tPort& port)
{
	tMsgStatus Status = Check(payload, tMsgId::ReadDataReg, 1);
	if (Status != tMsgStatus::None)
		return Status;

	port = static_cast<tPort>(payload.Value.Payload[0]);

	return tMsgStatus::None;
}

tMsgStatus tPayloadRet::ParseReadDataReg_PortUART(const tPayloadRet& payload, tUARTBaudrate& baudrate)
{
	tMsgStatus Status = Check(payload, tMsgId::ReadDataReg, 2);
	if (Status != tMsgStatus::None)
		return Status;

	tSetPortUART_BR Data;
	Data.S1RELH = payload.Value.Payload[0];
	Data.S1RELL = payload.Value.Payload[1];

	int BrIndex = 0;
	for (auto& i : SetPortUART_BR)
	{
		if (i == Data)
			break;

		++BrIndex;
	}

	baudrate = static_cast<tUARTBaudrate>(BrIndex);

	return tMsgStatus::None;
}

tMsgStatus tPayloadRet::ParseReadDataReg_PortUARTHS(const tPayloadRet& payload, tUARTHSBaudrate& baudrate)
{
	tMsgStatus Status = Check(payload, tMsgId::ReadDataReg, 4);
	if (Status != tMsgStatus::None)
		return Status;

	tSetPortUARTHS_BR Data;
	Data.S1RELHH = payload.Value.Payload[0];
	Data.S1RELHL = payload.Value.Payload[1];
	Data.S1RELLH = payload.Value.Payload[2];
	Data.S1RELLL = payload.Value.Payload[3];

	int BrIndex = 0;
	for (auto& i : SetPortUARTHS_BR)
	{
		if (i == Data)
			break;

		++BrIndex;
	}

	baudrate = static_cast<tUARTHSBaudrate>(BrIndex);

	return tMsgStatus::None;
}

tMsgStatus tPayloadRet::ParseReadDataReg_VideoResolution(const tPayloadRet& payload, tVideoResolution& resolution)
{
	tMsgStatus Status = Check(payload, tMsgId::ReadDataReg, 1);
	if (Status != tMsgStatus::None)
		return Status;

	resolution = static_cast<tVideoResolution>(payload.Value.Payload[0]);

	return tMsgStatus::None;
}

tMsgStatus tPayloadRet::Check(const tPayloadRet& payload, tMsgId msgId, std::size_t dataSize)
{
	if (payload.Value.MsgId != msgId)
		return tMsgStatus::WrongPacket;

	if (payload.Value.MsgStatus != tMsgStatus::None)
		return payload.Value.MsgStatus;

	if (payload.Value.Payload.size() != dataSize)
		return tMsgStatus::WrongDataSize;

	return tMsgStatus::None;
}

	}
}