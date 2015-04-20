#ifndef _TYPES_HPP_INCLUDED_
#define _TYPES_HPP_INCLUDED_

#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef unsigned __int8	uint8;
typedef unsigned __int8 byte;
#else
typedef unsigned char uint8;
typedef unsigned char byte;
#endif

#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef __int8	int8;
#else
typedef signed char int8;
#endif

#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef unsigned __int16 uint16;
#else
typedef unsigned short uint16;
#endif

#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef __int16 int16;
#else
typedef signed short	int16;
#endif

#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef unsigned __int32 uint32;
#else
typedef unsigned int	uint32;
#endif

#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef __int32 int32;
#else
typedef signed int int32;
#endif

#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef unsigned __int64 uint64;
#elif __GNUC__
#if __WORDSIZE == 64
typedef unsigned long int uint64;
#else
__extension__ typedef unsigned long long uint64;
#endif
#else
typedef unsigned long long	uint64;
#endif

#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef __int64 int64;
#elif __GNUC__
#if __WORDSIZE == 64
typedef long int int64;
#else
__extension__ typedef long long int64;
#endif
#else
typedef long long	int64;
#endif

static const int8 INT8_MIN = int8(-128);
static const int8 INT8_MAX = int8(127);
static const uint8 UINT8_MAX = uint8(255);
static const int16 INT16_MIN = int16(-32768);
static const int16 INT16_MAX = int16(32767);
static const uint16 UINT16_MAX = uint16(65535);
static const int32 INT32_MIN = int32(-2147483647 - 1);
static const int32 INT32_MAX = int32(2147483647);
static const uint32 UINT32_MAX = uint32(0xffffffff);
static const float FLOAT_MIN = float(1.175494351e-38F);
static const float FLOAT_MAX = float(3.402823466e+38F);

#undef NULL
#define NULL 0

namespace core
{

inline int32 GetLowInt64( const int64 value )
{
   return (int32)(value & 0x00000000ffffffffLL);
}

inline int32 GetHighInt64( const int64 value )
{
   return (int32)((value >> 32) & 0x00000000ffffffffLL);
}

inline uint32 GetLowUint64( const uint64 value )
{
   return (uint32)(value & 0x00000000ffffffffULL);
}

inline uint32 GetHighUint64( const uint64 value )
{
   return (uint32)((value >> 32) & 0x00000000ffffffffULL);
}

// inline int64 SetLowInt64( const int32 value ) const
// {
   // return (int64)((value & 0xffffffff00000000LL) | (int64_t)(lo))
// }

// inline int32 SetHighInt64( const double value ) const
// {
   // return (int32)((value >> 32) & 0x00000000ffffffffLL);
// }

// #define GTE_SET_LO_I64(v,lo) (((v) & 0xffffffff00000000LL) | (int64_t)(lo))
// #define GTE_SET_HI_I64(v,hi) (((v) & 0x00000000ffffffffLL) | ((int64_t)(hi) << 32))
// #define GTE_MAKE_I64(hi,lo)  ((int64_t)(lo) | ((int64_t)(hi) << 32))
// #define GTE_SET_LO_U64(v,lo) (((v) & 0xffffffff00000000ULL) | (uint64_t)(lo))
// #define GTE_SET_HI_U64(v,hi) (((v) & 0x00000000ffffffffULL) | ((uint64_t)(hi) << 32))
// #define GTE_MAKE_U64(hi,lo)  ((uint64_t)(lo) | ((uint64_t)(hi) << 32))

} // namespace core

#endif