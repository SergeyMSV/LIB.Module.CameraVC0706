#include "modCameraVC0706_Packet.h"

#include <stdio.h>
#include <stdlib.h>

namespace mod
{
	namespace CameraVC0706
	{
		namespace Packet
		{

const uint8_t ContainerCmdSTX = 0x56;
const uint8_t ContainerRetSTX = 0x76;

utils::tVectorUInt8 FindPacketRet(utils::tVectorUInt8& receivedData)
{
	unsigned int ReceivedStxPos = 0;

	while (true)
	{
		while (ReceivedStxPos < receivedData.size() && receivedData[ReceivedStxPos] != ContainerRetSTX)//[srg]2017-02-09
		{
			ReceivedStxPos++;
		}

		if (ReceivedStxPos >= receivedData.size())
			break;

		if (receivedData.size() - ReceivedStxPos < ContainerRetSize)
			break;

		const size_t PayloadSize = receivedData[ReceivedStxPos + 4];
		const size_t PacketSize = ContainerRetSize + PayloadSize;

		if (PayloadSize <= PayloadSizeMax && receivedData.size() - ReceivedStxPos >= PacketSize)
		{
			const utils::tVectorUInt8 Packet(receivedData.begin() + ReceivedStxPos, receivedData.begin() + ReceivedStxPos + PacketSize);

			receivedData.erase(receivedData.begin(), receivedData.begin() + ReceivedStxPos + PacketSize);

			return Packet;
		}
		else
		{
			ReceivedStxPos++;
		}
	}
	return {};
}

bool tPacketCmd::TryParse(const utils::tVectorUInt8& receivedPacket, tPacketCmd& packet)
{
	if (receivedPacket.size() >= ContainerCmdSize)
	{
		const size_t PayloadSize = receivedPacket[3];
		const size_t PacketSize = ContainerCmdSize + PayloadSize;

		if (PayloadSize <= PayloadSizeMax && receivedPacket.size() == PacketSize)
		{
			packet.SerialNumber = receivedPacket[1];
			packet.MsgId = receivedPacket[2];
			packet.Payload = utils::tVectorUInt8(receivedPacket.begin() + ContainerCmdSize, receivedPacket.end());
			return true;
		}
	}
	return false;
}

utils::tVectorUInt8 tPacketCmd::ToVector()
{
	if (Payload.size() > PayloadSizeMax)
		return {};

	utils::tVectorUInt8 Packet(ContainerCmdSize + Payload.size());

	int Index = 0;
	Packet[Index++] = ContainerCmdSTX;
	Packet[Index++] = SerialNumber;
	Packet[Index++] = MsgId;
	Packet[Index++] = Payload.size();
	for (const auto& i : Payload)
	{
		Packet[Index++] = i;
	}
	return Packet;
}

bool tPacketRet::TryParse(const utils::tVectorUInt8& receivedPacket, tPacketRet& packet)
{
	if (receivedPacket.size() >= ContainerRetSize)
	{
		const size_t PayloadSize = receivedPacket[4];
		const size_t PacketSize = ContainerRetSize + PayloadSize;

		if (PayloadSize <= PayloadSizeMax && receivedPacket.size() == PacketSize)
		{
			packet.SerialNumber = receivedPacket[1];
			packet.MsgId = receivedPacket[2];
			packet.Status = receivedPacket[3];
			packet.Payload = utils::tVectorUInt8(receivedPacket.begin() + ContainerRetSize, receivedPacket.end());
			return true;
		}
	}
	return false;
}

utils::tVectorUInt8 tPacketRet::ToVector()
{
	if (Payload.size() > PayloadSizeMax)
		return {};

	utils::tVectorUInt8 Packet(ContainerRetSize + Payload.size());

	int Index = 0;
	Packet[Index++] = ContainerRetSTX;
	Packet[Index++] = SerialNumber;
	Packet[Index++] = MsgId;
	Packet[Index++] = Status;
	Packet[Index++] = Payload.size();
	for (const auto& i : Payload)
	{
		Packet[Index++] = i;
	}
	return Packet;
}

		}
	}
}
