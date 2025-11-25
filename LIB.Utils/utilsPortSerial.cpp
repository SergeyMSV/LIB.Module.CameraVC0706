#include "utilsPortSerial.h"

#include <chrono>
#include <thread>

#include <cmath>

#ifdef LIB_UTILS_TEST_LOG
#include "utilsTest.h"
#include <iostream>
#endif // LIB_UTILS_TEST_LOG

namespace utils
{
namespace port
{
namespace serial
{

tPortOneWireSync::tPort::tPort(boost::asio::io_context& io, const std::string& id, std::uint32_t baudRate, tCharSize charSize, tStopBits stopBits, tParity parity, tFlowControl flowControl)
	: tPortSerialAsync(io, id, baudRate, charSize, stopBits, parity, flowControl)
{
}

bool tPortOneWireSync::tPort::IsReceived(std::size_t qty) const
{
	std::lock_guard<std::mutex> Lock(m_Mtx);
	return m_DataRcv.size() >= qty;
}

std::vector<std::uint8_t> tPortOneWireSync::tPort::GetReceived()
{
	std::lock_guard<std::mutex> Lock(m_Mtx);
	std::vector<std::uint8_t> Data = std::move(m_DataRcv);
	return Data;
}

void tPortOneWireSync::tPort::ResetReceived()
{
	std::lock_guard<std::mutex> Lock(m_Mtx);
	m_DataRcv.clear();
}

void tPortOneWireSync::tPort::OnReceived(std::vector<std::uint8_t>& data)
{
	std::lock_guard<std::mutex> Lock(m_Mtx);
	m_DataRcv.insert(m_DataRcv.end(), data.begin(), data.end());
}

tPortOneWireSync::tGuardBR::tGuardBR(tPort* port, std::uint32_t baudRate) :m_pPort(port)
{
	if (!m_pPort)
		return;
	m_BR = m_pPort->GetBaudRate();
	m_pPort->SetBaudRate(baudRate);
}
tPortOneWireSync::tGuardBR::~tGuardBR()
{
	if (!m_pPort)
		return;
	m_pPort->SetBaudRate(m_BR);
}

tPortOneWireSync::tPortOneWireSync(boost::asio::io_context& io, const std::string& id, tSpeed speed)
	: m_Port(io, id, ToBaudRate(speed), tCharSize(8), tStopBits::one, tParity::none, tFlowControl::none)
{}

bool tPortOneWireSync::IsReady()
{
	return m_Port.IsReady();
}

boost::system::error_code tPortOneWireSync::GetError()
{
	return m_Port.GetError();
}

tPortOneWireSync::tStatus tPortOneWireSync::Reset()
{
	m_Port.ResetReceived();
	tGuardBR BR(&m_Port, 9600);

	m_Port.Send({ 0x0F }); // Pull DQ line low, then up. UART transmits LSB first.

	constexpr std::size_t RspSize = 2;
	std::vector<std::uint8_t> Data = GetReceived(RspSize);
	if (Data.size() != RspSize)
		return tStatus::NoRsp;
	if (Data[0] != 0x0F)
		return tStatus::PortDamaged;
	if (Data[1] == 0x00)
		return tStatus::ShortCircuit;
	if (Data[1] == 0xF0)
		return tStatus::Presence;
	return tStatus::Success;
}

std::vector<std::uint8_t> tPortOneWireSync::Transaction(std::vector<std::uint8_t> tx, std::size_t rxSize)
{
	if (tx.empty())
		return {};
	tx.resize(tx.size() + rxSize, 0xFF);
	std::vector<std::uint8_t> Rx = OW_Transaction(tx);
	if (Rx.size() != tx.size())
		return {};
#ifdef LIB_UTILS_TEST_LOG
	std::cout << '\n'
		<< "Transaction TX: " << utils::test::ToStringHEX(tx, true) << '\n'
		<< " <>         RX: " << utils::test::ToStringHEX(Rx, true) << '\n';
#endif // LIB_UTILS_TEST_LOG
	Rx.erase(Rx.begin(), Rx.begin() + 1);
	return Rx;
}

void tPortOneWireSync::SendBit(bool tx)
{
	const std::vector<std::uint8_t> DataSend(1, tx ? 0xFF : 0x00);
	m_Port.Send(DataSend);
	std::vector<std::uint8_t> Rx = GetReceived(DataSend.size());
#ifdef LIB_UTILS_TEST_LOG
	std::cout << '\n'
		<< "SendBit     TX: " << utils::test::ToStringHEX(DataSend, true) << '\n'
		<< " <>         RX: " << utils::test::ToStringHEX(Rx, true) << '\n';
#endif // LIB_UTILS_TEST_LOG
}

std::vector<bool> tPortOneWireSync::ReceiveBits(std::size_t rxSize)
{
	std::vector<std::uint8_t> DataSend(rxSize, 0xFF);
	m_Port.Send(DataSend);
	std::vector<std::uint8_t> Rx = GetReceived(DataSend.size());
#ifdef LIB_UTILS_TEST_LOG
	std::cout << '\n'
		<< "ReceiveBits TX: " << utils::test::ToStringHEX(DataSend, true) << '\n'
		<< " <>         RX: " << utils::test::ToStringHEX(Rx, true) << '\n';
#endif // LIB_UTILS_TEST_LOG
	std::vector<bool> Res;
	for (std::uint32_t i = 0; i < Rx.size(); ++i)
	{
		Res.push_back(BitToByte(Rx[i]));
	}
	return Res;
}

std::vector<std::uint8_t> tPortOneWireSync::OW_Transaction(const std::vector<std::uint8_t>& tx)
{
	const std::vector<std::uint8_t> DataSend = ByteToBit(tx);
	m_Port.Send(DataSend);
	return BitToByte(GetReceived(DataSend.size()));
}

std::vector<std::uint8_t> tPortOneWireSync::GetReceived(std::size_t rxSize)
{
	double BytesPerSecond = m_Port.GetBaudRate() / 10; // 10 -> [1 Start Bit] + [8 DataBits] + [1 StopBit]
	std::uint32_t Bytes_in_10us = static_cast<std::uint32_t>(std::ceil(100000 / BytesPerSecond)) + 100; // + 100us is a waiting for asynchronous reading operation

	for (std::size_t i = 0; i < rxSize * Bytes_in_10us; ++i)
	{
		if (m_Port.IsReceived(rxSize))
			break;
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
	return m_Port.GetReceived();
}

std::vector<std::uint8_t> tPortOneWireSync::ByteToBit(const std::vector<std::uint8_t>& data)
{
	std::vector<std::uint8_t> DataBits;
	for (auto& k : data)
	{
		for (int i = 0; i < 8; ++i)
		{
			DataBits.push_back((k & (1 << i)) ? 0xFF : 0x00);
		}
	}
	return DataBits;
}

std::vector<std::uint8_t> tPortOneWireSync::BitToByte(const std::vector<std::uint8_t>& data)
{
	std::vector<std::uint8_t> DataBytes;
	std::uint8_t Byte = 0;
	for (std::size_t i = 0; i < data.size(); ++i)
	{
		std::uint8_t bitIdx = i % 8;
		Byte |= BitToByte(data[i]) << bitIdx;

		if (bitIdx == 7)
		{
			DataBytes.push_back(Byte);
			Byte = 0;
		}
	}
	return DataBytes;
}

std::uint32_t tPortOneWireSync::ToBaudRate(tSpeed val)
{
	switch (val)
	{
	case tSpeed::Slow: return 57600;
	case tSpeed::Norm: return 115200;
	case tSpeed::Fast: return 230400;
	}
	return 57600;
}

}
}
}
