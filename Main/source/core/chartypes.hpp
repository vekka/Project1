/*
Open Asset Import Library (assimp)
----------------------------------------------------------------------

Copyright (c) 2006-2012, assimp team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above
copyright notice, this list of conditions and the
following disclaimer.

* Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the
following disclaimer in the documentation and/or other
materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
contributors may be used to endorse or promote products
derived from this software without specific prior
written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------
*/

#ifndef _CHARTYPES_HPP_INCLUDED_
#define _CHARTYPES_HPP_INCLUDED_

#include "BasicTypes.hpp"

#if (_MSC_VER >= 1300)
   typedef __int8 utf8;
   typedef unsigned __int16 utf16;
   typedef unsigned __int32 utf32;
#endif

namespace core
{

   template <typename TChar>
   inline uint8 CharToInt(TChar x)
   {
      return (uint8)(x - (TChar)0x30);
   }

   template <typename TChar>
   inline bool IsADigit(const TChar x)
   {
      return (x >= (char)0x30 && x <= (TChar)0x39);
   }

   template <typename TChar>
   inline TChar _ToLower(const TChar x)
   {
      return (TChar)(x >= 'A' && x <= 'Z' ? x + 0x20 : x);
   }

   template <typename TChar>
   inline TChar _ToUpper(const TChar x)
   {
      return (TChar)(x >= 'a' && x <= 'z' ? x + ('A' - 'a') : x);
   }

   template <typename TChar>
   inline bool IsUpper(TChar in)
   {
      return (in >= (TChar)'A' && in <= (TChar)'Z');
   }

   template <typename TChar>
   inline bool IsLower(TChar in)
   {
      return (in >= (TChar)'a' && in <= (TChar)'z');
   }

   template <typename TChar>
   inline bool IsSpace(TChar in)
   {
      return (in == (TChar)' ' || in == (TChar)'\t');
   }

   template <typename TChar>
   inline bool IsLineEnd(TChar in)
   {
      return (in == (TChar)'\r' || in == (TChar)'\n' || in == (TChar)'\0' || in == (TChar)'\f');
   }

   template <typename TChar>
   inline bool IsSpaceOrNewLine(TChar in)
   {
      return IsSpace<TChar>(in) || IsLineEnd<TChar>(in);
   }

   template <typename TChar>
   inline bool SkipSpaces(const TChar* in, const TChar** out)
   {
      while (*in == (TChar)' ' || *in == (TChar)'\t') {
         ++in;
      }
      *out = in;
      return !IsLineEnd<TChar>(*in);
   }

   template <typename TChar>
   inline bool SkipSpaces(const TChar** inout)
   {
      return SkipSpaces<TChar>(*inout, inout);
   }

   inline void SkipToken(const char*& in)
   {
      SkipSpaces(&in);
      while (!IsSpaceOrNewLine(*in))++in;
   }

   template<typename TChar>
   inline TChar GetNextToken(TChar pBuffer, TChar pEnd)
   {
      while (!IsEndOfBuffer(pBuffer, pEnd))
      {
         if (IsSpaceOrNewLine(*pBuffer))
            break;
         pBuffer++;
      }
      return GetNextWord(pBuffer, pEnd);
   }

} // namespace core

#endif