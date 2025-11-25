#pragma once

//from /usr/include/sysexits.h

namespace utils
{

namespace exit_code
{
constexpr int EX_OK = 0;			// successful termination
constexpr int EX__BASE = 64;		// base value for error messages
constexpr int EX_USAGE = 64;		// command line usage error
constexpr int EX_DATAERR = 65;		// data format error
constexpr int EX_NOINPUT = 66;		// cannot open input
constexpr int EX_NOUSER = 67;		// addressee unknown
constexpr int EX_NOHOST = 68;		// host name unknown
constexpr int EX_UNAVAILABLE = 69;	// service unavailable
constexpr int EX_SOFTWARE = 70;		// internal software error
constexpr int EX_OSERR = 71;		// system error (e.g.; can't fork)
constexpr int EX_OSFILE = 72;		// critical OS file missing
constexpr int EX_CANTCREAT = 73;	// can't create (user) output file
constexpr int EX_IOERR = 74;		// input/output error
constexpr int EX_TEMPFAIL = 75;		// temp failure; user is invited to retry
constexpr int EX_PROTOCOL = 76;		// remote error in protocol
constexpr int EX_NOPERM = 77;		// permission denied
constexpr int EX_CONFIG = 78;		// configuration error
constexpr int EX__MAX = 78;			// maximum listed value
}

}

/*enum class tExitCode : int
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
};*/
