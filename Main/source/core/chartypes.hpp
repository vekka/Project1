#ifndef _CHARTYPES_HPP_INCLUDED_
#define _CHARTYPES_HPP_INCLUDED_

#include "types.hpp"

namespace core
{

inline uint8 CharToInt( const char x )
{
   return (uint8)(x - (char)0x30);
}

inline bool IsADigit( const char x )
{
   return (x >= (char)0x30 && x <= (char)0x39);
}

inline uint8 _ToLower( const char x )
{
   return (uint8)(x >= 'A' && x <= 'Z' ? x + 0x20 : x);
}

inline uint8 _ToUpper( const char x )
{
   return (uint8)(x >= 'a' && x <= 'z' ? x + ( 'A' - 'a' ) : x);
}

} // namespace core

#endif