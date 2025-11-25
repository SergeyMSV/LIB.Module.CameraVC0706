///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsBase
// 2014-09-24
// C++17
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <libConfig.h>

#include <cassert>
#include <cstring>

#include <algorithm>
#include <string>
#include <vector>

namespace utils
{

#ifdef LIB_UTILS_BASE_DEPRECATED
typedef std::vector<std::uint8_t> tVectorUInt8;
#endif // LIB_UTILS_BASE_DEPRECATED

template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value, void>::type Append(std::vector<std::uint8_t>& dst, const T& value)
{
	const std::uint8_t* Begin = reinterpret_cast<const std::uint8_t*>(&value);

	dst.insert<const std::uint8_t*>(dst.end(), Begin, Begin + sizeof(value));
}

template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value, std::vector<std::uint8_t>>::type ToVector(const T& value)
{
	std::vector<std::uint8_t> Data;

	Data.reserve(sizeof(value));

	const std::uint8_t* Begin = reinterpret_cast<const std::uint8_t*>(&value);

	Data.insert(Data.end(), Begin, Begin + sizeof(value));

	return Data;
}

template<typename T, typename Iterator>
typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type Read(Iterator first, Iterator last)
{
	T Data = 0;

	const auto Size = std::distance(first, last);

	if (Size > 0 && Size <= static_cast<std::size_t>(sizeof(T)))
	{
		std::copy(first, last, reinterpret_cast<std::uint8_t*>(&Data));
	}

	return Data;
}

template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type Read(const char* data, std::size_t dataSize)
{
	return Read<T, const char*>(data, data + dataSize);
}

template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type Read(const unsigned char* data, std::size_t dataSize)
{
	const char* Begin = reinterpret_cast<const char*>(data);

	return Read<T, const char*>(Begin, Begin + dataSize);
}

enum class tRadix : std::uint8_t
{
	//oct = 8,//it's just for tests
	dec = 10,
	hex = 16,
};

template<typename T, typename Iterator, tRadix Radix>
struct tRead
{
	typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type operator()(Iterator first, Iterator last)
	{
		assert(false);
		return 0;//T();//T{};//[TBD] - C++20
	}
};

template<typename T, typename Iterator>
struct tRead<T, Iterator, tRadix::dec>
{
	typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type operator()(Iterator first, Iterator last)
	{
		std::string ValStr(first, last);
		ValStr.erase(ValStr.begin(), std::find_if(ValStr.begin(), ValStr.end(), [](char ch) { return std::isdigit(ch) || ch == '-'; }));
		return static_cast<T>(strtol(ValStr.c_str(), nullptr, 10));
	}
};

template<typename T, typename Iterator>
struct tRead<T, Iterator, tRadix::hex>
{
	typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type operator()(Iterator first, Iterator last)
	{
		std::string ValStr(first, last);
		ValStr.erase(ValStr.begin(), std::find_if(ValStr.begin(), ValStr.end(), [](char ch) { return std::isxdigit(ch); }));
		return static_cast<T>(strtoul(ValStr.c_str(), nullptr, 16));
	}
};

template<typename T, typename Iterator, int N = 20>
typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type Read(Iterator first, Iterator last, tRadix radix)
{
	switch (radix)
	{
	//case tRadix::oct: return tRead<T, Iterator, tRadix::oct>()(first, last);
	case tRadix::dec: return tRead<T, Iterator, tRadix::dec>()(first, last);
	case tRadix::hex: return tRead<T, Iterator, tRadix::hex>()(first, last);
	default:
		assert(false);
		return 0;
	}
}

template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type Read(const char* data, tRadix radix)
{
	const std::size_t DataSize = strlen(data);

	return Read<T, const char*>(data, data + DataSize, radix);
}

template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type Reverse(T value)
{
	std::uint8_t* Begin = reinterpret_cast<std::uint8_t*>(&value);

	std::reverse<std::uint8_t*>(Begin, Begin + sizeof(value));

	return value;
}

#ifdef LIB_UTILS_BASE_DEPRECATED
class tEmptyAble
{
protected:
	bool m_Empty = true;

public:
	tEmptyAble() = default;
	explicit tEmptyAble(bool empty) :m_Empty(empty) {}

	bool Empty() const { return m_Empty; }

protected:
	~tEmptyAble() {}
};

enum class tDevStatus : std::uint8_t
{
	Init,
	Operation,
	Deinit,
	Halted,
	Error,
	Unknown = 0xFF,
};

enum class tGNSSCode : std::uint8_t
{
	Unknown = 0,
	GLONASS,
	GPS,
	GALILEO,
	BEIDOU,
	WAAS = 20,
	//END = 31, in this case it's possible to use this GNSSCode as an index for a bitfield when navigation solution based on two or more GNSS systems
};

//from /usr/include/sysexits.h
enum class tExitCode : int // DEPRECATED, use from utilsExits.h
{
	EX_OK = 0,			// successful termination
	EX__BASE = 64,		// base value for error messages
	EX_USAGE = 64,		// command line usage error
	EX_DATAERR = 65,	// data format error
	EX_NOINPUT = 66,	// cannot open input
	EX_NOUSER = 67,		// addressee unknown
	EX_NOHOST = 68,		// host name unknown
	EX_UNAVAILABLE = 69,// service unavailable
	EX_SOFTWARE = 70,	// internal software error
	EX_OSERR = 71,		// system error (e.g., can't fork)
	EX_OSFILE = 72,		// critical OS file missing
	EX_CANTCREAT = 73,	// can't create (user) output file
	EX_IOERR = 74,		// input/output error
	EX_TEMPFAIL = 75,	// temp failure; user is invited to retry
	EX_PROTOCOL = 76,	// remote error in protocol
	EX_NOPERM = 77,		// permission denied
	EX_CONFIG = 78,		// configuration error
	EX__MAX = 78,		// maximum listed value
};
#endif // LIB_UTILS_BASE_DEPRECATED
}
