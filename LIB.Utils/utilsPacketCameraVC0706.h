///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketCameraVC0706.h
//
// Standard ISO/IEC 114882, C++14
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2017 02 01  |
// |            |               |
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

//#include <libConfig.h>

//#include"utilsBase.h"
#include "utilsPacket.h"

//#include <deque>
#include <string>
#include <vector>

namespace utils
{
	namespace packet_CameraVC0706
	{

const size_t ContainerSize = 2;//STX, PayloadSize
const size_t ContainerHeaderCmdSize = 2;
const size_t ContainerHeaderRetSize = 3;
//const size_t ContainerCmdSize = 4;
//const size_t ContainerRetSize = 5;
const size_t ContainerPayloadSizeMax = 16;
///////////////////////////////////////////////////////////////////////////////////////////////////
enum class tMsgId : uint8_t
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
///////////////////////////////////////////////////////////////////////////////////////////////////
enum class tMsgStatus : uint8_t
{
	None                = 0x00,
	CmdIsNotReceived    = 0x01,
	WrongPayloadSize    = 0x02,
	WrongPayloadFormat  = 0x03,
	CmdCannotBeExecuted = 0x04,
	CmdExecutionError   = 0x05,
};
///////////////////////////////////////////////////////////////////////////////////////////////////
const uint16_t REG_Address_Settings_SerialPort         = 0x0007;
const uint16_t REG_Address_Settings_SerialPort_UART_BR = 0x0008;
const uint16_t REG_Address_Settings_VideoResolution    = 0x0019;
const uint16_t REG_Address_Settings_VideoCompression   = 0x001A;
///////////////////////////////////////////////////////////////////////////////////////////////////

template <class TPayload, uint8_t stx, uint8_t containerHeaderSize>
struct tFormat
{
	enum : uint8_t { STX = stx, containerSizePosition = containerHeaderSize + 1, };

protected:
	static tVectorUInt8 TestPacket(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
	{
		const size_t Size = std::distance(cbegin, cend);

		if (Size > containerSizePosition && *cbegin == STX)
		{
			const uint8_t DataSize = *(cbegin + containerSizePosition) + containerHeaderSize;

			if (DataSize <= ContainerPayloadSizeMax && Size >= GetSize(DataSize))
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
			const uint8_t DataSize = *(packetVector.cbegin() + containerSizePosition) + containerHeaderSize;

			if (DataSize <= ContainerPayloadSizeMax && packetVector.size() == GetSize(DataSize))
			{
				payload = TPayload(packetVector.cbegin() + 1, packetVector.cend());//+1 for STX

				return true;
			}
		}

		return false;
	}

	static size_t GetSize(size_t payloadSize) { return ContainerSize + payloadSize; }

	void Append(tVectorUInt8& dst, const TPayload& payload) const
	{
		dst.reserve(GetSize(payload.size()));//[#]reserved a bit more

		dst.push_back(STX);

		for (const auto& i : payload)
		{
			dst.push_back(i);
		}
	}
};

template <class TPayload> struct tFormatCmd : public tFormat<TPayload, 'V', ContainerHeaderCmdSize> { };
template <class TPayload> struct tFormatRet : public tFormat<TPayload, 'v', ContainerHeaderRetSize> { };

template <class TData>
struct tPayloadCommon
{
	typedef TData value_type;

	class tIterator
	{
		friend struct tPayloadCommon;

		const tPayloadCommon* m_pObj = nullptr;

		const size_t m_DataSize = 0;
		size_t m_DataIndex = 0;

		tIterator() = delete;
		tIterator(const tPayloadCommon* obj, bool begin)
			:m_pObj(obj), m_DataSize(m_pObj->size())
		{
			if (m_DataSize > 0)
			{
				if (begin)
				{
					m_DataIndex = 0;
				}
				else
				{
					m_DataIndex = m_DataSize;
				}
			}
		}

	public:
		tIterator& operator ++ ()
		{
			if (m_DataIndex < m_DataSize)
			{
				++m_DataIndex;
			}

			return *this;
		}

		bool operator != (const tIterator& val) const
		{
			return m_DataIndex != val.m_DataIndex;
		}

		const uint8_t operator * () const
		{
			return m_pObj->Data[m_DataIndex];
		}
	};

	typedef tIterator iterator;

	value_type Data{};

	tPayloadCommon() = default;

	//explicit tPayloadCommon(const value_type& data)
	//	:Data(data)
	//{}

	tPayloadCommon(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
		:Data(cbegin, cend)
	{}

	size_t size() const
	{
		return Data.size();
	}

	iterator begin() const
	{
		return iterator(this, true);
	}

	iterator end() const
	{
		return iterator(this, false);
	}
};

struct tDataCmd
{
	uint8_t SerialNumber = 0;
	tMsgId MsgId = tMsgId::None;
	tVectorUInt8 Payload;

	tDataCmd() = default;
	tDataCmd(uint8_t serialNumber, tMsgId msgId, const tVectorUInt8& payload)
		:SerialNumber(serialNumber), MsgId(msgId), Payload(payload)
	{}

	tDataCmd(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
	{
		if (cbegin == cend)
			return;

		SerialNumber = *cbegin;

		if (++cbegin == cend)
			return;

		MsgId = static_cast<tMsgId>(*cbegin);

		if (++cbegin == cend)
			return;
		//Payload size
		if (++cbegin == cend)
			return;

		Payload = tVectorUInt8(cbegin, cend);
	}

	size_t size() const
	{
		return sizeof(SerialNumber) + sizeof(MsgId) + 1 + Payload.size();//+1 for PayloadSize
	}

	uint8_t operator[] (const size_t index) const
	{
		if (index >= size())
			return 0;

		switch (index)
		{
		case 0: return SerialNumber;
		case 1: return static_cast<uint8_t>(MsgId);
		case 2: return static_cast<uint8_t>(Payload.size());
		}
		return Payload[index - 3];
	}

	bool operator == (const tDataCmd& val) const
	{
		return
			SerialNumber == val.SerialNumber &&
			MsgId == val.MsgId &&
			Payload == val.Payload;
	}
	bool operator != (const tDataCmd& val) const
	{
		return !(*this == val);
	}
};

struct tDataRet
{
	uint8_t SerialNumber = 0;
	tMsgId MsgId = tMsgId::None;
	tMsgStatus MsgStatus = tMsgStatus::None;
	tVectorUInt8 Payload;

	tDataRet() = default;
	tDataRet(uint8_t serialNumber, tMsgId msgId, tMsgStatus msgStatus, const tVectorUInt8& payload)
		:SerialNumber(serialNumber), MsgId(msgId), MsgStatus(msgStatus), Payload(payload)
	{}

	tDataRet(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
	{
		if (cbegin == cend)
			return;

		SerialNumber = *cbegin;

		if (++cbegin == cend)
			return;

		MsgId = static_cast<tMsgId>(*cbegin);

		if (++cbegin == cend)
			return;

		MsgStatus = static_cast<tMsgStatus>(*cbegin);

		if (++cbegin == cend)
			return;

		//Payload size
		if (++cbegin == cend)
			return;

		Payload = tVectorUInt8(cbegin, cend);
	}

	size_t size() const
	{
		return sizeof(SerialNumber) + sizeof(MsgId) + sizeof(MsgStatus) + 1 + Payload.size();//+1 for PayloadSize
	}

	uint8_t operator[] (const size_t index) const
	{
		if (index >= size())
			return 0;

		switch (index)
		{
		case 0: return SerialNumber;
		case 1: return static_cast<uint8_t>(MsgId);
		case 2: return static_cast<uint8_t>(MsgStatus);
		case 3: return static_cast<uint8_t>(Payload.size());
		}
		return Payload[index - 4];
	}

	bool operator == (const tDataRet& val) const
	{
		return
			SerialNumber == val.SerialNumber &&
			MsgId == val.MsgId &&
			MsgStatus == val.MsgStatus &&
			Payload == val.Payload;
	}
	bool operator != (const tDataRet& val) const
	{
		return !(*this == val);
	}
};

struct tPayloadCommonCmd : public tPayloadCommon<tDataCmd>
{
	tPayloadCommonCmd() = default;
	tPayloadCommonCmd(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
		:tPayloadCommon(cbegin, cend)
	{}
};

struct tPayloadCommonRet : public tPayloadCommon<tDataRet>
{
	tPayloadCommonRet() = default;
	tPayloadCommonRet(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
		:tPayloadCommon(cbegin, cend)
	{}
};





//std::vector<char> FindPacketRet(std::deque<char>& receivedData);
std::vector<char> FindPacketRet(std::vector<char>& receivedData);

struct tPacketCmd
{
	unsigned char SerialNumber;
	unsigned char MsgId;//Command

	std::vector<char> Payload;

	tPacketCmd()
	{
		SerialNumber = 0;
		MsgId = 0;
	}

	static bool TryParse(std::vector<char>& receivedPacket, tPacketCmd& packet);

	std::vector<char> ToVector();//MakePacket in previous versions
};

struct tPacketRet
{
	unsigned char SerialNumber;
	unsigned char MsgId;//Command
	unsigned char Status;

	std::vector<char> Payload;

	tPacketRet()
	{
		SerialNumber = 0;
		MsgId = 0;
		Status = 0;
	}

	static bool TryParse(std::vector<char>& receivedPacket, tPacketRet& packet);

	std::vector<char> ToVector();//MakePacket in previous versions
};

enum tSerialCommInterface
{
	tSerialCommInterface_UART   = 1,
	tSerialCommInterface_HSUART = 2,
	tSerialCommInterface_SPI    = 3,
};

struct tSerialCommUartBaudrate
{
	char S1RELH;
	char S1RELL;	
};

const tSerialCommUartBaudrate SerialCommUartBaudrate_115200 = {	(char)0x0D, (char)0xA6 };

//const tSerialCommUartBaudrate SerialCommUartBaudrate[] =
//{
//	{0x0D, 0xA6},
//};
//const unsigned short SerialCommUartBaudrate[] =
//{
//	0xAEC8,//9600
//	0x56E4,//19200
//	0xA2F2,//38400
//	0x1C4C,//57600
//	0x0DA6,//115200
//};

//const unsigned short SerialCommUartBaudrate = 0x0DA6;//115200
////const unsigned short SerialCommUartBaudrate[] =
////{
////	0xAEC8,//9600
////	0x56E4,//19200
////	0xA2F2,//38400
////	0x1C4C,//57600
////	0x0DA6,//115200
////};

enum tMemoryDevice
{
	tMemoryDevice_Chip_Reg    = 1,
	tMemoryDevice_Sensor_Reg  = 2,
	tMemoryDevice_CCIR656_Reg = 3,
	tMemoryDevice_I2C_EEPROM  = 4,
	tMemoryDevice_SPI_EEPROM  = 5,
	tMemoryDevice_SPI_Flash   = 6,
};

enum tVideoResolution
{
	tVideoResolution_640x480 = 0x00,
	tVideoResolution_320x240 = 0x11,
	tVideoResolution_160x120 = 0x22,
};

union tPayload_READ_FBUF_ControlMode
{
	struct
	{
		char TRANSFER_MODE : 1;//0 - MCU, 1 - DMA
		char NONAME_1      : 2;//1
		char NONAME_2      : 1;//1
		char               : 4;
	}Field;

	char Value;
};

union tPayload_WRITE_FBUF_ControlMode
{
	struct
	{
		char TRANSFER_MODE : 1;//0 - MCU, 1 - DMA
		char NONAME_1      : 2;//1
		char NONAME_2      : 1;//1
		char FIRST_WRITE   : 1;//0 - no, 1 - yes
		char               : 3;
	}Field;

	char Value;
};

enum tPayload_FBUF_CTRL_ControlFlag
{
	tPayload_FBUF_CTRL_ControlFlag_StopCurrentFrame = 0,
	tPayload_FBUF_CTRL_ControlFlag_StopNextFrame    = 1,
	tPayload_FBUF_CTRL_ControlFlag_ResumeFrame      = 2,
	tPayload_FBUF_CTRL_ControlFlag_StepFrame        = 3,
};
//{
//	struct
//	{
//		char TRANSFER_MODE : 1;//0 - MCU, 1 - DMA
//		char NONAME_HI1 : 1;
//		char NONAME_HI2 : 1;
//		char NONAME_HI3 : 1;
//		char : 4;
//	}Field;
//
//	char Value;
//};

	}
}
