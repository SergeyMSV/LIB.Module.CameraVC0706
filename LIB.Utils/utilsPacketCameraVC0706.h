///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketCameraVC0706.h
// 2017-02-01
// Standard ISO/IEC 114882, C++20
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "utilsPacket.h"

#include <iterator>
#include <string>
#include <vector>

namespace utils
{
	namespace packet_CameraVC0706
	{

constexpr char Version[][15] = {"VC0703 1.00", "VC0706 1.00" };

constexpr std::size_t ContainerCmdSize = 4;//STX, SerialNumber, Command(MsgId), PayloadSize
constexpr std::size_t ContainerRetSize = 5;//STX, SerialNumber, Command(MsgId), Status, PayloadSize
constexpr std::size_t ContainerCmdHeaderSize = ContainerCmdSize - 1;//SerialNumber, Command(MsgId), PayloadSize
constexpr std::size_t ContainerRetHeaderSize = ContainerRetSize - 1;//SerialNumber, Command(MsgId), Status, PayloadSize
constexpr std::size_t ContainerPayloadSizeMax = 16;
///////////////////////////////////////////////////////////////////////////////////////////////////
enum class tMsgId : std::uint8_t
{
	None               = 0x00,
	GetVersion         = 0x11,//Get Firmware version information
	SetSerialNumber    = 0x21,
	SetPort            = 0x24,
	SystemReset        = 0x26,
	ReadDataReg        = 0x30,
	WriteDataReg       = 0x31,
	ReadFBuf           = 0x32,
	WriteFBuf          = 0x33,
	GetFBufLength      = 0x34,//Get image size in frame buffer
	SetFBufLength      = 0x35,//Set image size in frame buffer
	FBufCtrl           = 0x36,
	CommMotionCtrl     = 0x37,
	CommMotionStatus   = 0x38,
	CommMotionDetected = 0x39,
	MirrorCtrl         = 0x3A,
	MirrorStatus       = 0x3B,
	ColourCtrl         = 0x3C,
	ColourStatus       = 0x3D,
	PowerSaveCtrl      = 0x3E,
	PowerSaveStatus    = 0x3F,
	AeCtrl             = 0x40,
	AeStatus           = 0x41,
	MotionCtrl         = 0x42,
	MotionStatus       = 0x43,
	TvOutCtrl          = 0x44,
	OsdAddChar         = 0x45,
	DownsizeCtrl       = 0x54,
	DownsizeStatus     = 0x55,
	GetFlashSize       = 0x60,
	EraseFlashSector   = 0x61,
	EraseFlashAll      = 0x62,
	ReadLogo           = 0x70,
	SetBitmap          = 0x71,
	BatchWrite         = 0x80,
};

constexpr tMsgId MsgIdSupported[] =
{
	tMsgId::GetVersion,
	tMsgId::SetSerialNumber,
	tMsgId::SetPort,
	tMsgId::SystemReset,
	tMsgId::ReadDataReg,
	tMsgId::WriteDataReg,
	tMsgId::ReadFBuf,
	tMsgId::WriteFBuf,
	tMsgId::GetFBufLength,
	tMsgId::SetFBufLength,
	tMsgId::FBufCtrl,
	tMsgId::CommMotionCtrl,
	tMsgId::CommMotionStatus,
	tMsgId::CommMotionDetected,
	tMsgId::MirrorCtrl,
	tMsgId::MirrorStatus,
	tMsgId::ColourCtrl,
	tMsgId::ColourStatus,
	tMsgId::PowerSaveCtrl,
	tMsgId::PowerSaveStatus,
	tMsgId::AeCtrl,
	tMsgId::AeStatus,
	tMsgId::MotionCtrl,
	tMsgId::MotionStatus,
	tMsgId::TvOutCtrl,
	tMsgId::OsdAddChar,
	tMsgId::DownsizeCtrl,
	tMsgId::DownsizeStatus,
	tMsgId::GetFlashSize,
	tMsgId::EraseFlashSector,
	tMsgId::EraseFlashAll,
	tMsgId::ReadLogo,
	tMsgId::SetBitmap,
	tMsgId::BatchWrite
};
///////////////////////////////////////////////////////////////////////////////////////////////////
enum class tMsgStatus : std::uint8_t
{
	None                = 0x00,
	CmdIsNotReceived    = 0x01,
	WrongPayloadSize    = 0x02,
	WrongPayloadFormat  = 0x03,
	CmdCannotBeExecuted = 0x04,
	CmdExecutionError   = 0x05,

	WrongDataSize       = 0xFE,//this code is for parser when it can't parse it for some reason
	WrongPacket         = 0xFF,//this code is for parser when it can't parse it for some reason
};
///////////////////////////////////////////////////////////////////////////////////////////////////

template <class TPayload, std::uint8_t stx, std::uint8_t containerSize>
struct tFormat 
{
	enum : std::uint8_t
	{
		STX = stx,
		containerMsgIdPosition = 2,
		containerSizePosition = containerSize - 1,
	};

protected:
	static tVectorUInt8 TestPacket(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
	{
		const std::size_t Size = std::distance(cbegin, cend);

		if (Size > containerSizePosition && *cbegin == STX)
		{
			const std::uint8_t DataSize = *(cbegin + containerSizePosition);

			if (DataSize <= ContainerPayloadSizeMax && Size >= GetSize(DataSize) && CheckMsgId(static_cast<tMsgId>(*(cbegin + containerMsgIdPosition))))
			{
				return tVectorUInt8(cbegin, cbegin + GetSize(DataSize));
			}
		}

		return {};
	}

	static bool TryParse(const tVectorUInt8& packetVector, TPayload& payload)
	{
		if (packetVector.size() > containerSizePosition && packetVector[0] == STX)
		{
			const std::uint8_t DataSize = *(packetVector.cbegin() + containerSizePosition);

			if (DataSize <= ContainerPayloadSizeMax && packetVector.size() == GetSize(DataSize))
			{
				payload = TPayload(packetVector.cbegin() + 1, packetVector.cend());//+1 STX

				return true;
			}
		}

		return false;
	}

	static std::size_t GetSize(std::size_t payloadSize) { return containerSize + payloadSize; }

	void Append(tVectorUInt8& dst, const TPayload& payload) const
	{
		dst.reserve(payload.size() + 1);//+1 STX

		dst.push_back(STX);

		for (const auto& i : payload)
		{
			dst.push_back(i);
		}
	}

private:
	static bool CheckMsgId(tMsgId msgId)
	{
		for (auto i : MsgIdSupported)
		{
			if (i == msgId)
				return true;
		}

		return false;
	}
};

template <class TPayload> struct tFormatCmd : public tFormat<TPayload, 'V', ContainerCmdSize> { };
template <class TPayload> struct tFormatRet : public tFormat<TPayload, 'v', ContainerRetSize> { };

struct tDataCmd
{
	std::uint8_t SerialNumber = 0;
	tMsgId MsgId = tMsgId::None;
	tVectorUInt8 Payload;

	tDataCmd() = default;
	tDataCmd(std::uint8_t serialNumber, tMsgId msgId, const tVectorUInt8& payload)
		:SerialNumber(serialNumber), MsgId(msgId), Payload(payload)
	{}

	tDataCmd(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
	{
		const std::size_t DataSize = std::distance(cbegin, cend);
		if (DataSize < ContainerCmdHeaderSize)
			return;

		SerialNumber = *cbegin++;
		MsgId = static_cast<tMsgId>(*cbegin++);
		++cbegin;//PayloadSize
		Payload = tVectorUInt8(cbegin, cend);
	}

	std::size_t size() const
	{
		return ContainerCmdHeaderSize + Payload.size();
	}

	std::uint8_t operator[] (const std::size_t index) const
	{
		if (index >= size())
			return 0;

		switch (index)
		{
		case 0: return SerialNumber;
		case 1: return static_cast<std::uint8_t>(MsgId);
		case 2: return static_cast<std::uint8_t>(Payload.size());
		}
		return Payload[index - 3];//ContainerCmdHeaderSize
	}

	bool operator == (const tDataCmd& val) const = default;
	bool operator != (const tDataCmd& val) const = default;
};

struct tDataRet
{
	std::uint8_t SerialNumber = 0;
	tMsgId MsgId = tMsgId::None;
	tMsgStatus MsgStatus = tMsgStatus::None;
	tVectorUInt8 Payload;

	tDataRet() = default;
	tDataRet(std::uint8_t serialNumber, tMsgId msgId, tMsgStatus msgStatus, const tVectorUInt8& payload)
		:SerialNumber(serialNumber), MsgId(msgId), MsgStatus(msgStatus), Payload(payload)
	{}

	tDataRet(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
	{
		const std::size_t DataSize = std::distance(cbegin, cend);
		if (DataSize < ContainerRetHeaderSize)
			return;

		SerialNumber = *cbegin++;
		MsgId = static_cast<tMsgId>(*cbegin++);
		MsgStatus = static_cast<tMsgStatus>(*cbegin++);
		++cbegin;//PayloadSize

		Payload = tVectorUInt8(cbegin, cend);
	}

	std::size_t size() const
	{
		return ContainerRetHeaderSize + Payload.size();
	}

	std::uint8_t operator[] (const std::size_t index) const
	{
		if (index >= size())
			return 0;

		switch (index)
		{
		case 0: return SerialNumber;
		case 1: return static_cast<std::uint8_t>(MsgId);
		case 2: return static_cast<std::uint8_t>(MsgStatus);
		case 3: return static_cast<std::uint8_t>(Payload.size());
		}
		return Payload[index - 4];//ContainerRetHeaderSize
	}

	bool operator == (const tDataRet& val) const = default;
	bool operator != (const tDataRet& val) const = default;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

enum class tPort : std::uint8_t
{
	UART   = 0x01,
	UARTHS = 0x02,
	//SPI    = 0x03,
};

enum class tUARTBaudrate : std::uint8_t
{
	BR9600,
	BR19200,
	BR38400,
	BR57600,
	BR115200,
};

enum class tUARTHSBaudrate : std::uint8_t
{
	BR38400,
	BR57600,
	BR115200,
	BR460800,
	BR921600,
};

enum class tResolution : std::uint8_t
{
	VR640x480 = 0x00,
	VR320x240 = 0x11,
	VR160x120 = 0x22,
};

enum class tMemoryDataReg : std::uint8_t
{
	//REG_Chip		= 1,
	//REG_Sensor	= 2,
	//REG_CCIR656	= 3,
	I2C_EEPROM		= 4,
	//SPI_EEPROM	= 5,
	//SPI_Flash		= 6,
};

#pragma pack(push, 1)
struct tDataReg
{
	std::uint16_t Address;
	std::uint8_t Size;
};
#pragma pack(pop)

struct tPayloadCmd : public packet::tPayload<tDataCmd>
{
	tPayloadCmd() = default;
	tPayloadCmd(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
		:tPayload(cbegin, cend)
	{}
};

struct tPayloadRet : public packet::tPayload<tDataRet>
{
	tPayloadRet() = default;
	tPayloadRet(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
		:tPayload(cbegin, cend)
	{}
};

class tPacketCmd : public packet::tPacket<tFormatCmd, tPayloadCmd>
{
	explicit tPacketCmd(const payload_value_type & payloadValue)
		: tPacket(payloadValue)
	{}

public:
	tPacketCmd() = default;

	tMsgId GetMsgId() const;

	static tPacketCmd MakeGetVersion(std::uint8_t sn);
	static tPacketCmd MakeSetSerialNumber(std::uint8_t sn, std::uint8_t value);
	static tPacketCmd MakeSetPort(std::uint8_t sn, tUARTBaudrate baudrate);
	static tPacketCmd MakeSetPort(std::uint8_t sn, tUARTHSBaudrate baudrate);
	static tPacketCmd MakeSystemReset(std::uint8_t sn);

	static tPacketCmd MakeReadDataReg(tMemoryDataReg memory, std::uint8_t sn, tDataReg reg);
	static tPacketCmd MakeReadDataReg_Port(tMemoryDataReg memory, std::uint8_t sn);
	static tPacketCmd MakeReadDataReg_PortUART(tMemoryDataReg memory, std::uint8_t sn);
	static tPacketCmd MakeReadDataReg_PortUARTHS(tMemoryDataReg memory, std::uint8_t sn);
	static tPacketCmd MakeReadDataReg_Resolution(tMemoryDataReg memory, std::uint8_t sn);
	static tPacketCmd MakeReadDataReg_Compression(tMemoryDataReg memory, std::uint8_t sn);

	static tPacketCmd MakeWriteDataReg(tMemoryDataReg memory, std::uint8_t sn, tDataReg reg, const tVectorUInt8& data);
	static tPacketCmd MakeWriteDataReg(tMemoryDataReg memory, std::uint8_t sn, tPort port);
	static tPacketCmd MakeWriteDataReg(tMemoryDataReg memory, std::uint8_t sn, tUARTBaudrate baudrate);
	static tPacketCmd MakeWriteDataReg(tMemoryDataReg memory, std::uint8_t sn, tUARTHSBaudrate baudrate);
	static tPacketCmd MakeWriteDataReg(tMemoryDataReg memory, std::uint8_t sn, tResolution resolution);

	static tPacketCmd MakeReadFBufCurrent(std::uint8_t sn, std::uint32_t address, std::uint32_t size, std::uint16_t delay);//size must be multiple of 4; delay is in 0.01ms
	//static tPacketCmd MakeReadFBufNext(std::uint8_t sn);
	static tPacketCmd MakeGetFBufLenCurrent(std::uint8_t sn);
	static tPacketCmd MakeGetFBufLenNext(std::uint8_t sn);
	//static tPacketCmd MakeSetFBufLen(std::uint8_t sn);
	static tPacketCmd MakeFBufCtrlStopCurrentFrame(std::uint8_t sn);
	//static tPacketCmd MakeFBufCtrlStopNextFrame(std::uint8_t sn);
	static tPacketCmd MakeFBufCtrlResumeFrame(std::uint8_t sn);
	//static tPacketCmd MakeFBufCtrlStepFrame(std::uint8_t sn);
};

#pragma pack(push, 1)
union tFBufLen
{
	struct qq
	{
		std::uint32_t HH : 8;
		std::uint32_t HL : 8;
		std::uint32_t LH : 8;
		std::uint32_t LL : 8;
	}Field;

	std::uint32_t Value = 0;
};
#pragma pack(pop)

//using tFBufLen1 = std::uint32_t;

class tPacketRet : public packet::tPacket<tFormatRet, tPayloadRet>
{
public:
	tMsgId GetMsgId() const;
	tMsgStatus GetMsgStatus() const;

	static tMsgStatus Parse(const tPacketRet& packet, std::string& version);
	static tMsgStatus Parse(const tPacketRet& packet, tPort& port);
	static tMsgStatus Parse(const tPacketRet& packet, tUARTBaudrate& baudrate);
	static tMsgStatus Parse(const tPacketRet& packet, tUARTHSBaudrate& baudrate);
	static tMsgStatus Parse(const tPacketRet& packet, tResolution& resolution);
	static tMsgStatus Parse(const tPacketRet& packet, tFBufLen& value);
	//static tMsgStatus Parse(const tPacketRet& packet, tFBufLen1& value);

private:
	static tMsgStatus Check(const tPacketRet::payload_value_type& payloadValue, tMsgId msgId);
	static tMsgStatus Check(const tPacketRet::payload_value_type& payloadValue, tMsgId msgId, std::size_t dataSize);
};

bool CheckVersion(const std::string& value);

std::string ToString(tPort value);
std::string ToString(tUARTBaudrate value);
std::string ToString(tUARTHSBaudrate value);
std::string ToString(tResolution value);

tResolution ToResolution(const std::string& value);

	}
}
