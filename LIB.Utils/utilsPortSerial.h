///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPortSerial
// 2021-11-02
// C++20
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <libConfig.h>

#include <mutex>
#include <queue>
#include <vector>

#include <boost/asio.hpp>

namespace utils
{
namespace port
{
namespace serial
{

using tCharSize = boost::asio::serial_port_base::character_size;
using tStopBits = boost::asio::serial_port_base::stop_bits::type;
using tParity = boost::asio::serial_port_base::parity::type;
using tFlowControl = boost::asio::serial_port_base::flow_control::type;

template<int DataSizeRecv = 1024>
class tPortSerialAsync
{
	boost::asio::serial_port m_Port;

	std::uint8_t m_Data[DataSizeRecv];

	mutable std::mutex m_Mtx;
	std::queue<std::vector<std::uint8_t>> m_DataSent;

	boost::system::error_code m_ErrorCode;

public:
	tPortSerialAsync(boost::asio::io_context& io, const std::string& id, std::uint32_t baudRate, tCharSize charSize, tStopBits stopBits, tParity parity, tFlowControl flowControl)
		: m_Port(io)
	{
		m_Port.open(id);

		if (m_Port.is_open())
		{
			m_Port.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
			m_Port.set_option(boost::asio::serial_port_base::character_size(charSize));
			m_Port.set_option(boost::asio::serial_port_base::stop_bits(stopBits));
			m_Port.set_option(boost::asio::serial_port_base::parity(parity));
			m_Port.set_option(boost::asio::serial_port_base::flow_control(flowControl));
		}

		Receive();
	}

	tPortSerialAsync(boost::asio::io_context& io, const std::string& id, std::uint32_t baudRate)
		:tPortSerialAsync(io, id, baudRate, tCharSize(8), tStopBits::one, tParity::none, tFlowControl::none)
	{
	}

	virtual ~tPortSerialAsync()
	{
		m_Port.close();
	}

	bool IsReady()
	{
		return !m_ErrorCode;
	}

	boost::system::error_code GetError()
	{
		return m_ErrorCode;
	}

	std::uint32_t GetBaudRate() const
	{
		if (!m_Port.is_open())
			return 0;
		boost::asio::serial_port_base::baud_rate Br{};
		boost::system::error_code Ec{};
		m_Port.get_option(Br, Ec);
		if (Ec)
			return 0;
		return Br.value();
	}

	void SetBaudRate(std::uint32_t val)
	{
		if (!m_Port.is_open())
			return;
		m_Port.set_option(boost::asio::serial_port_base::baud_rate(val));
	}

	bool Send(const std::vector<std::uint8_t>& data)
	{
		if (data.empty())
			return false;

		// [TBD] Something shall limit incoming data for sending here.

		std::lock_guard<std::mutex> Lock(m_Mtx);

		bool StartSending = m_DataSent.empty();

		m_DataSent.push(data);

		if (StartSending)
			Send();

		return true;
	}

private:
    void Receive()
    {
        m_Port.async_read_some(boost::asio::buffer(m_Data, sizeof(m_Data)),
            [this](boost::system::error_code ec, std::size_t bytes_recvd)
            {
                if (!ec && bytes_recvd > 0)
                {
					std::vector<std::uint8_t> Data(m_Data, m_Data + bytes_recvd);
					OnReceived(Data);
                }

				if (ec && !m_ErrorCode) // error list is in file C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\ucrt\errno.h
				{
					m_ErrorCode = ec;
					return;
				}

				Receive();
            });
    }

protected:
    virtual void OnReceived(std::vector<std::uint8_t>& data) = 0;

	void Send()
	{
		m_Port.async_write_some(boost::asio::buffer(m_DataSent.front(), m_DataSent.front().size()),
			[this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/)
			{
				std::lock_guard<std::mutex> Lock(m_Mtx);

				m_DataSent.pop();

				if (!m_DataSent.empty())
					Send();
			});
	}
};

class tPortOneWireSync
{
	class tPort : public tPortSerialAsync<64>
	{
		mutable std::mutex m_Mtx;
		std::vector<std::uint8_t> m_DataRcv;

	public:
		tPort(boost::asio::io_context& io, const std::string& id, std::uint32_t baudRate, tCharSize charSize, tStopBits stopBits, tParity parity, tFlowControl flowControl);

		bool IsReceived(std::size_t qty) const;
		std::vector<std::uint8_t> GetReceived();
		void ResetReceived();

	protected:
		void OnReceived(std::vector<std::uint8_t>& data) override;
	};

	struct tGuardBR
	{
		tPort* m_pPort;
		std::uint32_t m_BR = 0;

		tGuardBR() = delete;
		tGuardBR(tPort* port, std::uint32_t baudRate);
		~tGuardBR();
	};

	tPort m_Port;

public:
	enum class tStatus
	{
		Success,
		NoRsp,
		PortDamaged,
		ShortCircuit,
		Presence,
	};

	enum class tSpeed // Supported baudrates: 56000, 57600, 76800, 115200, 128000, 230400, 256000
	{
		Slow, // 57600
		Norm, // 115200
		Fast, // 230400
	};

public:
	tPortOneWireSync() = delete;
	tPortOneWireSync(boost::asio::io_context& io, const std::string& id, tSpeed speed = tSpeed::Norm);

	bool IsReady();
	boost::system::error_code GetError();

	tStatus Reset();
	std::vector<std::uint8_t> Transaction(std::vector<std::uint8_t> tx, std::size_t rxSize);

protected:
	void SendBit(bool tx);
	std::vector<bool> ReceiveBits(std::size_t rxSize);

private:
	std::vector<std::uint8_t> OW_Transaction(const std::vector<std::uint8_t>& tx);

	std::vector<std::uint8_t> GetReceived(std::size_t rxSize);

protected:
	static std::vector<std::uint8_t> ByteToBit(const std::vector<std::uint8_t>& data);
	static std::vector<std::uint8_t> BitToByte(const std::vector<std::uint8_t>& data);
	static std::uint8_t BitToByte(std::uint8_t val) { return val == 0xFF ? 1 : 0; }

	static std::uint32_t ToBaudRate(tSpeed val);
};

}
}
}
