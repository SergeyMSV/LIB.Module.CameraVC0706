#include "modCameraVC0706_Packet.h"

#include <string.h>

#include <iostream>
#include <iomanip>

namespace mod
{
	namespace CameraVC0706
	{
		namespace Packet
		{

void UnitTests()
{
	std::cout<<"\n\n""mod::CameraVC0706::Packet::UnitTests"<<std::endl;

	int SerialNumber = 0;

	char *DataArray = 0;
	utils::tVectorUInt8 DataVector;

	char *PacketArray = 0;
	char *PayloadArray = 0;
	utils::tVectorUInt8 PacketVector;
	utils::tVectorUInt8 Payload;
	bool Result = false;

	tPacketCmd PacketCmd;
	tPacketRet PacketRet;

	//Test Parse: Just a packet

	char DataParse_1[] = { 0x76, 0x00, 0x11, 0x00, 0x0B, "VC0703 1.00" };

	DataVector = utils::tVectorUInt8((uint8_t*)DataParse_1, (uint8_t*)DataParse_1 + sizeof(DataParse_1) - 1);
	PacketVector = FindPacketRet(DataVector);

	Result = tPacketRet::TryParse(PacketVector, PacketRet);

	Result = Result && PacketRet.SerialNumber == SerialNumber && PacketRet.MsgId == MsgId_GetVersion && PacketRet.Payload.size() == 0x0B;

	std::cout<<std::setw(70)<<std::setfill('.')<<std::setiosflags(std::ios::left)<<"Parse: Just a packet"<<(Result ? "OK" : "ERR")<<std::endl;

	////Test Parse: Rubbish

	//char DataParse_2[] = { 0x76, 0x76, 0x76, 0x76, 0x00, 0x11, 0x00, 0x0B, "VC0703 1.00""sdfsdfsfd" };

	//DataVector = utils::ToVector(DataParse_2, sizeof(DataParse_2) - 1);
	//PacketVector = FindPacketRet(DataVector);

	//Result = tPacketRet::TryParse(PacketVector, PacketRet);

	//Result = Result && PacketRet.SerialNumber == SerialNumber && PacketRet.MsgId == MsgId_GetVersion && PacketRet.Payload.size() == 0x0B;

	//std::cout << std::setw(70) << std::setfill('.') << std::setiosflags(std::ios::left) << "Parse: Rubbish" << (Result ? "OK" : "ERR") << std::endl;

	//Test Parse: Wrong SerialNumber

	char DataParse_3[] = { 0x76, 0x21, 0x11, 0x00, 0x0B, "VC0703 1.00" };

	DataVector = utils::tVectorUInt8((uint8_t*)DataParse_3, (uint8_t*)DataParse_3 + sizeof(DataParse_3) - 1);
	PacketVector = FindPacketRet(DataVector);

	Result = tPacketRet::TryParse(PacketVector, PacketRet);

	Result = Result && PacketRet.SerialNumber == 0x21 && PacketRet.MsgId == MsgId_GetVersion && PacketRet.Payload.size() == 0x0B;

	std::cout<<std::setw(70)<<std::setfill('.')<<std::setiosflags(std::ios::left)<<"Parse: Wrong SerialNumber"<<(Result ? "OK" : "ERR")<<std::endl;

	//WRONG PACKETS

	//76
	//76 35 73 9C 0F C4 53 C0 EB 90 0F 7C 0A 7A 14 ED 70 23 3B 7C
	//76 57 07 B1
	//76 B0 84 1C 0F 4E 28 07 8C 30 00 63 B1 E4 7E 14 D1 0B 6B 0A
	//76 A3 99 75 0E 51 79 60 40 E0 77 A0 21 0A 46 C3 B4 1E 94
	//76 A0 E7 92 06 47 4A 7B 0B 95 BD
	//76 E7 27 1D 05 26 4E 46 39 19
	//76 BB 71 8E
	//76 A6 86 C9 00
	//76 E2 8A 0E 01 3E
	//76 2E 09 20 0D BE B4 94 98 D7 71 81 55 80 DC 48 24 F5
	//76 56 A3 D1 00
	//76 E2 C1 94 0C 8A 69 93 82 36 93 81 9E 9D AA 79 75
	//76 23 48 A3
	//76 C9 24 9C 10 46 29 3F 32 B7 17 90 DF 32 8D C0 1F A5 03 71 52
	//76 BA E7 D7 07 96 1F 95 36 22 17 77

	//Test Parse: Wrong Packet 1

	char DataParse_WrongPacket_1[] = { 0x76 };

	DataVector = utils::tVectorUInt8((uint8_t*)DataParse_WrongPacket_1, (uint8_t*)DataParse_WrongPacket_1 + sizeof(DataParse_WrongPacket_1));
	PacketVector = FindPacketRet(DataVector);

	Result = PacketVector.size() == 0;

	//Result = tPacketRet::TryParse(PacketVector, PacketRet);

	//Result = Result && PacketRet.SerialNumber == 0x21 && PacketRet.MsgId == MsgId_GetVersion && PacketRet.Payload.size() == 0x0B;

	std::cout << std::setw(70) << std::setfill('.') << std::setiosflags(std::ios::left) << "Parse: Wrong Packet 1" << (Result ? "OK" : "ERR") << std::endl;

	//Test Parse: Wrong Packet 2

	char DataParse_WrongPacket_2[] = { 0x76, 0x35, 0x73, 0x9C, 0x0F, 0xC4, 0x53, 0xC0, 0xEB, 0x90, 0x0F, 0x7C, 0x0A, 0x7A, 0x14, 0xED, 0x70, 0x23, 0x3B, 0x7C };

	DataVector = utils::tVectorUInt8((uint8_t*)DataParse_WrongPacket_2, (uint8_t*)DataParse_WrongPacket_2 + sizeof(DataParse_WrongPacket_2));
	PacketVector = FindPacketRet(DataVector);

	//Result = PacketVector.size() == 0;

	Result = tPacketRet::TryParse(PacketVector, PacketRet);

	std::cout << std::setw(70) << std::setfill('.') << std::setiosflags(std::ios::left) << "Parse: Wrong Packet 2 (format is OK occasionally)" << (Result ? "OK" : "ERR") << std::endl;

	//Test Parse: Wrong Packet 3

	char DataParse_WrongPacket_3[] = { 0x76, 0x57, 0x07, 0xB1};

	DataVector = utils::tVectorUInt8((uint8_t*)DataParse_WrongPacket_3, (uint8_t*)DataParse_WrongPacket_3 + sizeof(DataParse_WrongPacket_3));
	PacketVector = FindPacketRet(DataVector);

	Result = PacketVector.size() == 0;

	//Result = tPacketRet::TryParse(PacketVector, PacketRet);

	//Result = Result && PacketRet.SerialNumber == 0x21 && PacketRet.MsgId == MsgId_GetVersion && PacketRet.Payload.size() == 0x0B;

	std::cout << std::setw(70) << std::setfill('.') << std::setiosflags(std::ios::left) << "Parse: Wrong Packet 3" << (Result ? "OK" : "ERR") << std::endl;


	//Test Make CMD: GET_VERSION

	//char DataMake_1[] = "SET CONTROL MUX 0";

	PacketCmd.SerialNumber = SerialNumber;
	PacketCmd.MsgId = MsgId_GetVersion;
	//Packet.Payload = utils::ToVector(DataMake_1, strlen(DataMake_1));

	PacketVector = PacketCmd.ToVector();

	Result = tPacketCmd::TryParse(PacketVector, PacketCmd);

	Result = Result && PacketCmd.SerialNumber == SerialNumber && PacketCmd.MsgId == MsgId_GetVersion && PacketCmd.Payload.size() == 0;

	std::cout << std::setw(70) << std::setfill('.') << std::setiosflags(std::ios::left) << "Make CMD: GET_VERSION" << (Result ? "OK" : "ERR") << std::endl;


	//Test Make RET: GET_VERSION

	//char DataMake_RET_Packet_1[] = { 0x76, 0x00, 0x11, 0x00, 0x0B, "VC0703 1.00" };
	char DataMake_RET_Payload_1[] = { "VC0703 1.00" };

	PacketRet.SerialNumber = SerialNumber;
	PacketRet.MsgId = MsgId_GetVersion;
	PacketRet.Status = 0;
	PacketRet.Payload = Payload = utils::tVectorUInt8((uint8_t*)DataMake_RET_Payload_1, (uint8_t*)DataMake_RET_Payload_1 + sizeof(DataMake_RET_Payload_1) - 1);

	PacketVector = PacketRet.ToVector();

	Result = tPacketRet::TryParse(PacketVector, PacketRet);

	Result = Result && PacketRet.SerialNumber == SerialNumber && PacketRet.MsgId == MsgId_GetVersion && PacketRet.Payload.size() == Payload.size() && PacketRet.Payload == Payload;

	std::cout << std::setw(70) << std::setfill('.') << std::setiosflags(std::ios::left) << "Make RET: GET_VERSION" << (Result ? "OK" : "ERR") << std::endl;


/*
	//Test Make: just make

	char DataMake_1[] = "SET CONTROL MUX 0";

	Packet.Link = 0xFF;
	Packet.Flags = 0x00;
	Packet.Payload = utils::ToVector(DataMake_1, strlen(DataMake_1));

	PacketVector = Packet.ToVector();

	Result = tPacket::TryParse(PacketVector, Packet);

	Result = Result && Packet.Link == 0xFF && Packet.Flags == 0x00 && Packet.Payload.size() == 17;
	///....
	std::cout<<std::setw(70)<<std::setfill('.')<<std::setiosflags(std::ios::left)<<"Make: just make"<<(Result ? "OK" : "ERR")<<std::endl;


	//Test Make: big payload

	//char DataMake_1[] = "SET CONTROL MUX 0";

	Packet.Link = 0xFF;
	Packet.Flags = 0x00;
	Packet.Payload = std::vector<char>();

	for (int i = 0; i < 1023; ++i)
		Packet.Payload.push_back((char)i);

	PacketVector = Packet.ToVector();

	Result = tPacket::TryParse(PacketVector, Packet);

	Result = Result && Packet.Link == 0xFF && Packet.Flags == 0x00 && Packet.Payload.size() == 1023;
	///....
	std::cout<<std::setw(70)<<std::setfill('.')<<std::setiosflags(std::ios::left)<<"Make: big payload"<<(Result ? "OK" : "ERR")<<std::endl;
*/

	std::cout<<std::endl;
}

		}
	}
}
