#ifndef _BITS_HPP_INCLUDED_
#define _BITS_HPP_INCLUDED_

#include "BasicTypes.hpp"

static int32 const LEADING_BIT_TABLE[32] = 
{
     0,  9,  1, 10, 13, 21,  2, 29,
    11, 14, 16, 18, 22, 25,  3, 30,
     8, 12, 20, 28, 15, 17, 24,  7,
    19, 27, 23,  6, 26,  5,  4, 31
};

static int32 const TRAILING_BIT_TABLE[32] = 
{
     0,  1, 28,  2, 29, 14, 24,  3,
    30, 22, 20, 15, 25, 17,  4,  8, 
    31, 27, 13, 23, 21, 19, 16,  7,
    26, 12, 18,  6, 11,  5, 10,  9
};

namespace core
{

namespace bits
{

namespace enumflags
{
// intended for enum bit operations:
template <typename TEnum>
inline TEnum operator|(TEnum a, TEnum b)
{
   return static_cast<TEnum>(static_cast<int32>(a) | static_cast<int32>(b));
}

template <typename TEnum>
inline TEnum &operator|=(TEnum &a, TEnum b)
{
   a = static_cast<TEnum>(a | b);
   return a;
}

template <typename TEnum>
inline TEnum operator&(TEnum a, TEnum b)
{
   return static_cast<TEnum>(static_cast<int32>(a)& static_cast<int32>(b));
}

template <typename TEnum>
inline TEnum operator^(TEnum a, TEnum b)
{
   return static_cast<TEnum>(static_cast<int32>(a) ^ static_cast<int32>(b));
}

template <typename TEnum>
inline TEnum operator~(TEnum a)
{
   return static_cast<TEnum>(~static_cast<int32>(a));
}

template <typename TEnum>
inline TEnum operator<<(TEnum a, TEnum b)
{
   return static_cast<TEnum>(static_cast<int32>(a) << static_cast<int32>(b));
}

} // namespace enumflags

inline void SetBit(uint32 &value, int32 bitNum)
{
   value |= (1 << bitNum);
}

inline void ClearBit(uint32 &value, int32 bitNum)
{
   value &= ~(1 << bitNum);
}

inline void ToggleBit(uint32 &value, int32 bitNum)
{
   value ^= (1 << bitNum);
}

inline bool CheckBit(int32 var, int32 pos)
{
   return static_cast<bool>(var & (1 << pos));
}

inline int32 GetLeadingBit( uint32 value )
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    uint32 key = (value * 0x07c4acddU) >> 27;
    return LEADING_BIT_TABLE[key];
}

inline int32 GetLeadingBit( int32 value )
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    uint32 key = (value * 0x07c4acddU) >> 27;
    return LEADING_BIT_TABLE[key];
}

inline int32 GetLeadingBit( const uint64 value )
{
    uint32 v1 = GetHighUint64(value);
    if (v1 != 0)
    {
        return GetLeadingBit(v1) + 32;
    }

    uint32 v0 = GetLowUint64(value);
    return GetLeadingBit(v0);
}

inline int32 GetLeadingBit( const int64 value )
{
    int32 v1 = GetHighInt64(value);
    if (v1 != 0)
    {
        return GetLeadingBit(v1) + 32;
    }

    int32 v0 = GetLowInt64(value);
    return GetLeadingBit(v0);
}

inline int32 GetTrailingBit( uint32 value )
{
#pragma warning(push)
#pragma warning(disable : 4146)  // do not warn on negation of uint32
    uint32 key = ((uint32)((value & -value) * 0x077cb531U)) >> 27;
    return TRAILING_BIT_TABLE[key];
#pragma warning(pop)
}

inline int32 GetTrailingBit( int32 value )
{
    uint32 key = ((uint32)((value & -value) * 0x077cb531U)) >> 27;
    return TRAILING_BIT_TABLE[key];
}

inline int32 GetTrailingBit( const uint64 value )
{
    uint32 v0 = GetLowUint64(value);
    if (v0 != 0)
    {
        return GetTrailingBit(v0);
    }

    uint32 v1 = GetHighUint64(value);
    return GetTrailingBit(v1) + 32;
}

inline int32 GetTrailingBit( const int64 value )
{
    int32 v0 = GetLowInt64(value);
    if (v0 != 0)
    {
        return GetTrailingBit(v0);
    }

    int32 v1 = GetHighInt64(value);
    return GetTrailingBit(v1) + 32;
}

// TODO:

// uint64_t RoundUpToPowerOfTwo(uint32_t value)
// {
    // if (value > 0)
    // {
        // int32_t leading = GetLeadingBit(value);
        // uint32_t mask = (1 << leading);
        // if ((value & ~mask) == 0)
        // {
            // // value is a power of two
            // return static_cast<uint64_t>(value);
        // }
        // else
        // {
            // // round up to a power of two
            // return (static_cast<uint64_t>(mask) << 1);
        // }

    // }
    // else
    // {
        // return GTE_U64(1);
    // }
// }

inline uint32 RoundDownToPowerOfTwo(uint32 value)
{
    if (value > 0)
    {
        int32 leading = GetLeadingBit(value);
        uint32 mask = (1 << leading);
        return mask;
    }
    else
    {
        return 0;
    }
}

} // namespace bits

} // namespace core

#endif