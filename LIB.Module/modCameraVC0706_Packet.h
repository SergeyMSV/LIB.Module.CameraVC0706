///////////////////////////////////////////////////////////////////////////////////////////////////
// modCameraVC0706_Packet
//
// Created by Sergey Maslennikov
// Tel.:   +7-916-540-09-19
// E-mail: maslennikovserge@yandex.ru
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2017 02 01  |
// |            |               |
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <libConfig.h>

#include <utilsBase.h>

//#include <deque>
#include <string>
#include <vector>

namespace mod
{
	namespace CameraVC0706
	{
		namespace Packet
		{

const size_t ContainerCmdSize = 4;
const size_t ContainerRetSize = 5;
const size_t PayloadSizeMax = 16;
///////////////////////////////////////////////////////////////////////////////////////////////////
const unsigned char MsgId_GetVersion         = 0x11;
const unsigned char MsgId_SetSerialNumber    = 0x21;
const unsigned char MsgId_SetPort            = 0x24;
const unsigned char MsgId_SystemReset        = 0x26;
const unsigned char MsgId_ReadDataReg        = 0x30;
const unsigned char MsgId_WriteDataReg       = 0x31;
const unsigned char MsgId_ReadFBuf           = 0x32;
const unsigned char MsgId_WriteFBuf          = 0x33;
const unsigned char MsgId_GetFBufLength      = 0x34;//get image size in frame buffer
const unsigned char MsgId_SetFBufLength      = 0x35;//set image size in frame buffer
const unsigned char MsgId_FBufCtrl           = 0x36;
const unsigned char MsgId_CommMotionCtrl     = 0x37;
const unsigned char MsgId_CommMotionStatus   = 0x38;
const unsigned char MsgId_CommMotionDetected = 0x39;
const unsigned char MsgId_MirrorCtrl         = 0x3A;
const unsigned char MsgId_MirrorStatus       = 0x3B;
const unsigned char MsgId_ColourCtrl         = 0x3C;
const unsigned char MsgId_ColourStatus       = 0x3D;
const unsigned char MsgId_PowerSaveCtrl      = 0x3E;
const unsigned char MsgId_PowerSaveStatus    = 0x3F;
const unsigned char MsgId_AeCtrl             = 0x40;
const unsigned char MsgId_AeStatus           = 0x41;
const unsigned char MsgId_MotionCtrl         = 0x42;
const unsigned char MsgId_MotionStatus       = 0x43;
const unsigned char MsgId_TvOutCtrl          = 0x44;
const unsigned char MsgId_OsdAddChar         = 0x45;
const unsigned char MsgId_DownsizeCtrl       = 0x54;
const unsigned char MsgId_DownsizeStatus     = 0x55;
const unsigned char MsgId_GetFlashSize       = 0x60;
const unsigned char MsgId_EraseFlashSector   = 0x61;
const unsigned char MsgId_EraseFlashAll      = 0x62;
const unsigned char MsgId_ReadLogo           = 0x70;
const unsigned char MsgId_SetBitmap          = 0x71;
const unsigned char MsgId_BatchWrite         = 0x80;
///////////////////////////////////////////////////////////////////////////////////////////////////
const unsigned char CERR_None                = 0x00;
const unsigned char CERR_CmdIsNotReceived    = 0x01;
const unsigned char CERR_WrongPayloadSize    = 0x02;
const unsigned char CERR_WrongPayloadFormat  = 0x03;
const unsigned char CERR_CmdCannotBeExecuted = 0x04;
const unsigned char CERR_CmdExecutionError   = 0x05;
///////////////////////////////////////////////////////////////////////////////////////////////////
const unsigned short REG_Address_Settings_SerialPort         = 0x0007;
const unsigned short REG_Address_Settings_SerialPort_UART_BR = 0x0008;
const unsigned short REG_Address_Settings_VideoResolution    = 0x0019;
const unsigned short REG_Address_Settings_VideoCompression   = 0x001A;
///////////////////////////////////////////////////////////////////////////////////////////////////

//utils::tVectorUInt8 FindPacketRet(std::deque<char>& receivedData);
utils::tVectorUInt8 FindPacketRet(utils::tVectorUInt8& receivedData);

struct tPacketCmd
{
	uint8_t SerialNumber = 0;
	uint8_t MsgId = 0;//Command

	utils::tVectorUInt8 Payload;

	//tPacketCmd()
	//{
	//	SerialNumber = 0;
	//	MsgId = 0;
	//}

	static bool TryParse(const utils::tVectorUInt8& receivedPacket, tPacketCmd& packet);

	utils::tVectorUInt8 ToVector();//MakePacket in previous versions
};

struct tPacketRet
{
	uint8_t SerialNumber = 0;
	uint8_t MsgId = 0;//Command
	uint8_t Status = 0;

	utils::tVectorUInt8 Payload;

	//tPacketRet()
	//{
	//	SerialNumber = 0;
	//	MsgId = 0;
	//	Status = 0;
	//}

	static bool TryParse(const utils::tVectorUInt8& receivedPacket, tPacketRet& packet);

	utils::tVectorUInt8 ToVector();//MakePacket in previous versions
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
}
