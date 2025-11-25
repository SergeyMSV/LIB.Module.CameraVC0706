///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacket
// 2019-06-20
// C++14
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <algorithm>
#include <vector>

#include <cstdint>

namespace utils
{
namespace packet
{

template
<
	template <class> class TFormat,
	class TPayload
>
class tPacket : private TFormat<TPayload>, private TPayload
{
public:
	typedef TPayload payload_type;
	typedef typename TPayload::value_type payload_value_type;

	tPacket() = default;

	explicit tPacket(const TPayload& payload)
	{
		TPayload::Value = payload.Value;
	}

	explicit tPacket(const payload_value_type& payloadValue)
	{
		TPayload::Value = payloadValue;
	}

	static std::size_t Find(std::vector<std::uint8_t>& receivedData, tPacket& packet)
	{
		std::vector<std::uint8_t>::const_iterator Begin = receivedData.cbegin();

		for (;;)
		{
			Begin = std::find(Begin, receivedData.cend(), TFormat<TPayload>::STX);

			if (Begin == receivedData.cend())
				break;

			std::vector<std::uint8_t> PacketVector = TFormat<TPayload>::TestPacket(Begin, receivedData.cend());

			if (PacketVector.size() > 0)
			{
				if (TFormat<TPayload>::TryParse(PacketVector, packet))
				{
					std::size_t EraseSize = std::distance(receivedData.cbegin(), Begin);

					EraseSize += PacketVector.size();

					receivedData.erase(receivedData.begin(), receivedData.begin() + EraseSize);

					return PacketVector.size();
				}
			}

			Begin++;
		}

		return 0;
	}

	const payload_value_type& GetPayloadValue() const
	{
		return TPayload::Value;
	}

	std::vector<std::uint8_t> ToVector() const
	{
		std::vector<std::uint8_t> PacketVector;

		TFormat<TPayload>::Append(PacketVector, *this);

		return PacketVector;
	}
};

template <class TValue>
struct tPayload
{
	typedef TValue value_type;

	class tIterator
	{
		friend struct tPayload;

		const tPayload* m_pObj = nullptr;

		const std::size_t m_DataSize = 0;
		std::size_t m_DataIndex = 0;

		tIterator() = delete;
		tIterator(const tPayload* obj, bool begin)
			:m_pObj(obj), m_DataSize(m_pObj->size())
		{
			if (!m_DataSize)
				return;

			m_DataIndex = begin ? 0 : m_DataSize;
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

		const std::uint8_t operator * () const
		{
			return m_pObj->Value[m_DataIndex];
		}
	};

	typedef tIterator iterator;

	value_type Value{};

	tPayload() = default;

	tPayload(std::vector<std::uint8_t>::const_iterator cbegin, std::vector<std::uint8_t>::const_iterator cend)
		:Value(cbegin, cend)
	{}

	std::size_t size() const
	{
		return Value.size();
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

}
}
