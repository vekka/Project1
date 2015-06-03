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

/** @file Definition of platform independent string workers:

ASSIMP_itoa10
ASSIMP_stricmp
strncmp

These functions are not consistently available on all platforms,
or the provided implementations behave too differently.
*/
#ifndef INCLUDED_AI_STRING_WORKERS_H
#define INCLUDED_AI_STRING_WORKERS_H

#include <string.h>
#include <stdint.h>
#include <string>
#include <cctype>

#include <assert.h>

#include "BasicTypes.hpp"

namespace assimp	{

   /** @brief itoa with a fixed base 10
   * 'itoa' is not consistently available on all platforms so it is quite useful
   * to have a small replacement function here. No need to use a full sprintf()
   * if we just want to print a number ...
   * @param out Output buffer
   * @param max Maximum number of characters to be written, including '\0'.
   *   This parameter may not be 0.
   * @param number Number to be written
   * @return Length of the output string, excluding the '\0'
   */
   inline uint32 ASSIMP_itoa10(char* out, uint32 max, int32 number)
   {
      assert(NULL != out);

      // write the unary minus to indicate we have a negative number
      uint32 written = 1u;
      if (number < 0 && written < max)	{
         *out++ = '-';
         ++written;
         number = -number;
      }

      // We begin with the largest number that is not zero. 
      int32_t cur = 1000000000; // 2147483648
      bool mustPrint = false;
      while (written < max)	{

         const uint32 digit = number / cur;
         if (mustPrint || digit > 0 || 1 == cur)	{
            // print all future zeroes from now
            mustPrint = true;

            *out++ = '0' + static_cast<char>(digit);

            ++written;
            number -= digit*cur;
            if (1 == cur) {
               break;
            }
         }
         cur /= 10;
      }

      // append a terminal zero
      *out++ = '\0';
      return written - 1;
   }

   /** @brief itoa with a fixed base 10 (Secure template overload)
   *  The compiler should choose this function if he or she is able to determine the
   *  size of the array automatically.
   */
   template <size_t length>
   inline uint32 ASSIMP_itoa10(char(&out)[length], int32 number)
   {
      return ASSIMP_itoa10(out, length, number);
   }

   /** @brief Helper function to do platform independent string comparison.
   *
   *  This is required since stricmp() is not consistently available on
   *  all platforms. Some platforms use the '_' prefix, others don't even
   *  have such a function.
   *
   *  @param s1 First input string
   *  @param s2 Second input string
   *  @return 0 if the given strings are identical
   */
   inline int32 ASSIMP_stricmp(const char *s1, const char *s2)
   {
      assert(NULL != s1 && NULL != s2);

#if (defined _MSC_VER)

      return ::_stricmp(s1, s2);
#elif defined( __GNUC__ )

      return ::strcasecmp(s1, s2);
#else

      char c1, c2;
      do	{
         c1 = tolower(*s1++);
         c2 = tolower(*s2++);
      } while (c1 && (c1 == c2));
      return c1 - c2;
#endif
   }

   /** @brief Case independent comparison of two std::strings
   *
   *  @param a First  string
   *  @param b Second string
   *  @return 0 if a == b
   */
   inline int32 ASSIMP_stricmp(const std::string& a, const std::string& b)
   {
      int32 i = (int32)b.length() - (int32)a.length();
      return (i ? i : ASSIMP_stricmp(a.c_str(), b.c_str()));
   }

   /** @brief Helper function to do platform independent string comparison.
   *
   *  This is required since strincmp() is not consistently available on
   *  all platforms. Some platforms use the '_' prefix, others don't even
   *  have such a function.
   *
   *  @param s1 First input string
   *  @param s2 Second input string
   *  @param n Macimum number of characters to compare
   *  @return 0 if the given strings are identical
   */
   inline int32 ASSIMP_strncmp(const char *s1, const char *s2, uint32 n)
   {
      assert(NULL != s1 && NULL != s2);
      if (!n)return 0;

#if (defined _MSC_VER)

      return ::_strnicmp(s1, s2, n);

#elif defined( __GNUC__ )

      return ::strncasecmp(s1, s2, n);

#else
      char c1, c2;
      uint32 p = 0;
      do
      {
         if (p++ >= n)return 0;
         c1 = tolower(*s1++);
         c2 = tolower(*s2++);
      } while (c1 && (c1 == c2));

      return c1 - c2;
#endif
   }

   /**
   *
   * todo: move somewhere where it fits better in than here
   */
   inline uint32 integer_pow(uint32 base, uint32 power)
   {
      uint32 res = 1;
      for (uint32 i = 0; i < power; ++i)
         res *= base;

      return res;
   }
} // end of namespace

#endif // !  AI_STRINGCOMPARISON_H_INC