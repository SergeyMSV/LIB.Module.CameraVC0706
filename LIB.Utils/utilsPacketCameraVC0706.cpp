/*#include "modCameraVC0706_Packet.h"

#include <stdio.h>
#include <stdlib.h>

namespace mod
{
	namespace CameraVC0706
	{
		namespace Packet
		{

const char ContainerCmdSTX = 0x56;
const char ContainerRetSTX = 0x76;

std::vector<char> FindPacketRet(std::vector<char>& receivedData)
{
	std::vector<char> Packet;

	unsigned int ReceivedStxPos = 0;

	while (1)
	{
		while (ReceivedStxPos < receivedData.size() && receivedData[ReceivedStxPos] != ContainerRetSTX)//[srg]2017-02-09
		{
			ReceivedStxPos++;
		}

		if (ReceivedStxPos >= receivedData.size())
		{
			break;
		}

		if (receivedData.size() - ReceivedStxPos >= ContainerRetSize)
		{
			unsigned int PayloadSize = receivedData[ReceivedStxPos + 4];
			unsigned int PacketSize = ContainerRetSize + PayloadSize;

			if (PayloadSize <= PayloadSizeMax && receivedData.size() - ReceivedStxPos >= PacketSize)
			{
				Packet = std::vector<char>(receivedData.begin() + ReceivedStxPos, receivedData.begin() + ReceivedStxPos + PacketSize);

				receivedData.erase(receivedData.begin(), receivedData.begin() + ReceivedStxPos + PacketSize);

				break;
			}
			else
			{
				ReceivedStxPos++;
			}
		}
		else
		{
			break;
		}
	}

	return Packet;
}

bool tPacketCmd::TryParse(std::vector<char>& receivedPacket, tPacketCmd& packet)
{
	if (receivedPacket.size() >= ContainerCmdSize)
	{
		int PayloadSize = receivedPacket[3];

		int PacketSize = ContainerCmdSize + PayloadSize;

		if (PayloadSize <= PayloadSizeMax && receivedPacket.size() == PacketSize)
		{
			packet.SerialNumber = receivedPacket[1];

			packet.MsgId = receivedPacket[2];

			packet.Payload = std::vector<char>(receivedPacket.begin() + ContainerCmdSize, receivedPacket.end());

			return true;
		}
	}

	return false;
}

std::vector<char> tPacketCmd::ToVector()
{
	if (Payload.size() > PayloadSizeMax)
	{
		return std::vector<char>();
	}

	std::vector<char> Packet(ContainerCmdSize + Payload.size());

	int Index = 0;

	Packet[Index++] = ContainerCmdSTX;

	Packet[Index++] = (char)SerialNumber;

	Packet[Index++] = (char)MsgId;

	Packet[Index++] = (char)Payload.size();

	for (unsigned int i = 0; i < Payload.size(); ++i)
	{
		Packet[Index++] = Payload[i];
	}

	return Packet;
}

bool tPacketRet::TryParse(std::vector<char>& receivedPacket, tPacketRet& packet)
{
	if (receivedPacket.size() >= ContainerRetSize)
	{
		int PayloadSize = receivedPacket[4];

		int PacketSize = ContainerRetSize + PayloadSize;

		if (PayloadSize <= PayloadSizeMax && receivedPacket.size() == PacketSize)
		{
			packet.SerialNumber = receivedPacket[1];
			packet.MsgId = receivedPacket[2];
			packet.Status = receivedPacket[3];

			packet.Payload = std::vector<char>(receivedPacket.begin() + ContainerRetSize, receivedPacket.end());

			return true;
		}
	}

	return false;
}

std::vector<char> tPacketRet::ToVector()
{
	if (Payload.size() > PayloadSizeMax)
	{
		return std::vector<char>();
	}

	std::vector<char> Packet(ContainerRetSize + Payload.size());

	int Index = 0;

	Packet[Index++] = ContainerRetSTX;

	Packet[Index++] = (char)SerialNumber;

	Packet[Index++] = (char)MsgId;

	Packet[Index++] = (char)Status;

	Packet[Index++] = (char)Payload.size();

	for (unsigned int i = 0; i < Payload.size(); ++i)
	{
		Packet[Index++] = Payload[i];
	}

	return Packet;
}

		}
	}
}
*/