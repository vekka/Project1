/* irrlicht.h -- interface of the 'Irrlicht Engine'
Copyright (C) 2002-2012 Nikolaus Gebhardt
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:
1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
Please note that the Irrlicht Engine is based in part on the work of the
Independent JPEG Group, the zlib and the libPng. This means that if you use
the Irrlicht Engine in your product, you must acknowledge somewhere in your
documentation that you've m_used the IJG code. It would also be nice to mention
that you use the Irrlicht Engine, the zlib and libPng. See the README files
in the jpeglib, the zlib and libPng for further informations.
*/

// TODO: make << operator for stream, (cout etc.)

#ifndef _STRING_HPP_INCLUDED_
#define _STRING_HPP_INCLUDED_

#include <iostream>

#include <assert.h>

#include <iostream>

#include "core/memory/allocator.hpp"
#include "core/math/mathcommon.hpp" // IsZero

#include "core/chartypes.hpp"

namespace core
{

   namespace string
   {

      template <typename T, typename TAlloc = Allocator<T>>
      class String
      {
      private:
         T* m_strArray;
         uint32 m_allocated;
         uint32 m_used;
         TAlloc m_allocator;

         void Reallocate(const uint32 newSize);

      public:
         typedef T* iterator;
         typedef const T* const_iterator;

         String();
         String(const String &other);
         template <typename B> String(const B* const c, const uint32 length);
         template <typename B> String(const B* const c);
         explicit String(const int32 number);
         explicit String(const float number);
         template <typename TInputIterator> String(const TInputIterator first, const TInputIterator last);
         //template <typename TInputIterator> String(const TInputIterator first, const TInputIterator last);

         iterator begin();
         const_iterator begin() const;
         iterator end();
         const_iterator end() const;

         ~String();

         String &operator=(const String &other);
         template <typename B> String &operator=(const B* const c);
         String &operator=(const int32 number);

         int32 StringToInt() const; // return the first int32 value in a string or return INT32_MAX if not found
         float StringToFloat() const; // return the first float value in a string or return FLOAT_MAX if not found

         //TODO:
         char GetLastChar() const;

         T operator[](const uint32 index) const;
         T &operator[](const uint32 index);

         bool IsEmpty() const;
         uint32 GetSize() const;
         const T* CString() const;

         bool operator==(const T* const str) const;
         bool operator==(const String &other) const;
         bool operator==(const int32 number) const;
         bool operator<(const String &other) const;
         bool operator>(const String &other) const;
         bool operator<=(const String &other) const;
         bool operator>=(const String &other) const;
         bool operator!=(const T* const str) const;
         bool operator!=(const String &other) const;

         bool EqualSubstr(const uint32 index1, const uint32 index2, const T* const str);

         String &Erase(const uint32 index1, const uint32 index2); // erase index2-index1 number of char from index1 inclusive

         void Trim(const bool left = true, const bool right = true);

         String &RemoveChars(const String &chars);
         bool EqualsN(const String &other, const uint32 n) const;
         bool EqualsN(const T* const str, const uint32 n) const;
         String &ToUpper();
         String &ToLower();

         String &Append(const T character);
         String &Append(const T* const other, const uint32 length = 0xffffffff);
         String &Append(const String &other);

         String operator+(const String &other) const;
         String &operator+=(const char character);

         int32 FindFirst(const T c) const;
         int32 FindFirstNotOf(const T c) const;
         int32 FindFirstNotOf(const String<T, TAlloc> &chars) const;
         int32 FindLast(const T c) const;
         int32 FindLastNotOf(const T c) const;
         int32 FindLastNotOf(const String<T, TAlloc> &chars) const;
         int32 FindNext(const T c, const uint32 startPos) const;
         int32 FindNextNumber(const uint32 startPos) const;

         template <typename TContainer> int32 Tokenize(TContainer &ret, const T* const delimiter = " \t\r\n", const int32 count = 1,
            const bool ignoreEmptyTokens = true, const bool keepSeparators = false) const;
         String SubString(const uint32 begin, const uint32 length, const bool makeLower = false) const;
         String operator()(const uint32 index1, const uint32 index2);
         String operator()(const uint32 index);
         void Reserve(const uint32 count);

         //friend std::ostream &operator<<( std::ostream &out, const String &str )
         //{
         //   out << str->m_strArray;
         //   return out;
         //}
      };

      typedef String<char> String_c;
      typedef String<wchar_t> String_w;

      template <typename T, typename TAlloc>
      void String<T, TAlloc>::Reallocate(const uint32 newSize)
      {
         T* old_array = m_strArray;

         m_strArray = m_allocator.Allocate(newSize); //new T[new_size];
         m_allocated = newSize;

         int32 amount = m_used < newSize ? m_used : newSize;
         for (int32 i = 0; i < amount; i++)
            m_strArray[i] = old_array[i];

         if (m_allocated < m_used)
            m_used = m_allocated;

         m_allocator.Free(old_array); // delete [] old_array;
      }

      template <typename T, typename TAlloc>
      inline String<T, TAlloc>::String() : m_allocated(1), m_used(1)
      {
         m_strArray = m_allocator.Allocate(1);
         m_strArray[0] = 0;
      }

      template <typename T, typename TAlloc>
      inline String<T, TAlloc>::String(const String<T, TAlloc> &other) : m_strArray(NULL), m_allocated(0), m_used(0)
      {
         *this = other;
      }

      template <typename T, typename TAlloc>
      template <typename B>
      String<T, TAlloc>::String(const B* const c, const uint32 length) : m_strArray(NULL), m_allocated(0), m_used(0)
      {
         if (!c)
         {
            // correctly init the string to an empty one
            *this = "";
            return;
         }

         m_allocated = m_used = length + 1;
         m_strArray = m_allocator.Allocate(m_used); // new T[m_used];

         for (uint32 i = 0; i < length; i++)
            m_strArray[i] = (T)c[i];

         m_strArray[length] = 0;
      }

      template <typename T, typename TAlloc>
      template <typename B>
      String<T, TAlloc>::String(const B* const c) : m_strArray(NULL), m_allocated(0), m_used(0)
      {
         *this = c;
      }

      template <typename T, typename TAlloc>
      inline String<T, TAlloc>::String(const int32 number) : m_strArray(NULL), m_allocated(0), m_used(0)
      {
         int32 num = number;
         // store if negative and make positive
         bool negative = false;
         if (num < 0)
         {
            num *= -1;
            negative = true;
         }

         // temporary buffer for 16 numbers

         char tmpbuf[16] = { 0 };
         int32 idx = 15;

         // special case '0'

         if (!number)
         {
            tmpbuf[14] = '0';
            *this = &tmpbuf[14];
            return;
         }

         // add numbers

         while (num && idx)
         {
            idx--;
            tmpbuf[idx] = (char)('0' + (num % 10));
            num /= 10;
         }

         // add sign

         if (negative)
         {
            idx--;
            tmpbuf[idx] = '-';
         }

         *this = &tmpbuf[idx];
      }

      // not complete
      template <typename T, typename TAlloc>
      inline String<T, TAlloc>::String(const float number) : m_strArray(NULL), m_allocated(0), m_used(0)
      {
         int32 intNum = (int32)number;

         char tmpbuf[32] = { 0 };
         int32 idx = 31;

         if (core::IsZero(number))
         {
            tmpbuf[30] = '0';
            *this = &tmpbuf[14];
            return;
         }

         // store if negative and make positive
         bool negative = false;

         if (number < 0.0f)
         {
            intNum *= -1;
            negative = true;
         }

         //// temporary buffer for 16 numbers

         //char tmpbuf[16]={0};
         //int32 idx = 15;

         //// special case '0'

         //if (!number)
         //{
         //   tmpbuf[14] = '0';
         //   *this = &tmpbuf[14];
         //   return;
         //}

         //// add numbers

         //while(num && idx)
         //{
         //   idx--;
         //   tmpbuf[idx] = (char)('0' + (num % 10));
         //   num /= 10;
         //}

         //// add sign

         //if (negative)
         //{
         //   idx--;
         //   tmpbuf[idx] = '-';
         //}

         //*this = &tmpbuf[idx];
      }

      template <typename T, typename TAlloc>
      template <typename TInputIterator>
      inline String<T, TAlloc>::String(const TInputIterator first, const TInputIterator last)
      {
         int32 length = 0;
         TInputIterator _first = first;
         TInputIterator _last = last;

         while (_first++ != _last)
         {
            length++;
         }

         m_allocated = m_used = length + 1;
         m_strArray = m_allocator.Allocate(m_used);
         _first = first;

         for (int32 i = 0; i < length; i++)
         {
            m_strArray[i] = _first[i];
         }
         //m_strArray[0] = 'a';
      }

      //template <typename T, typename TAlloc>
      //template <typename TInputIterator>
      //inline String<T, TAlloc>::String(const TInputIterator first, const TInputIterator last)
      //{
      //   int32 i = 0;
      //   int32 _first = first;
      //   int32 _last = last;
      //   while (_first != _last)
      //   {
      //      m_strArray[i++] = *(_first++);
      //   }
      //}

      template <typename T, typename TAlloc>
      inline String<T, TAlloc>::~String()
      {
         m_allocator.Free(m_strArray);
      }

      template <typename T, typename TAlloc>
      inline typename String<typename T, typename TAlloc>::iterator String<typename T, typename TAlloc>::begin()
      {
         return m_strArray;
      }

      template <typename T, typename TAlloc>
      inline typename String<typename T, typename TAlloc>::const_iterator String<typename T, typename TAlloc>::begin() const
      {
         return m_strArray;
      }

      template <typename T, typename TAlloc>
      inline typename String<typename T, typename TAlloc>::iterator String<typename T, typename TAlloc>::end()
      {
         return m_strArray + m_used;
      }

      template <typename T, typename TAlloc>
      inline typename String<typename T, typename TAlloc>::const_iterator String<typename T, typename TAlloc>::end() const
      {
         return m_strArray + m_used;
      }

      template <typename T, typename TAlloc>
      bool String<T, TAlloc>::operator==(const T* const str) const
      {
         if (!str)
            return false;

         int32 i;
         for (i = 0; m_strArray[i] && str[i]; i++)
         {
            if (m_strArray[i] != str[i])
               return false;
         }

         return (!m_strArray[i] && !str[i]);
      }

      template <typename T, typename TAlloc>
      bool String<T, TAlloc>::operator==(const String<T, TAlloc> &other) const
      {
         for (int32 i = 0; m_strArray[i] && other.m_strArray[i]; i++)
            if (m_strArray[i] != other.m_strArray[i])
               return false;

         return m_used == other.m_used;
      }

      template <typename T, typename TAlloc>
      bool String<T, TAlloc>::operator==(const int32 number) const
      {
         String_c str(number);

         int32 i;
         for (i = 0; m_strArray[i] && str[i]; i++)
         {
            if (m_strArray[i] != str[i])
               return false;
         }

         return (!m_strArray[i] && !str[i]);
      }

      template <typename T, typename TAlloc>
      bool String<T, TAlloc>::operator<(const String &other) const
      {
         for (int32 i = 0; m_strArray[i] && other.m_strArray[i]; i++)
         {
            const int32 diff = m_strArray[i] - other.m_strArray[i];
            if (diff)
               return (diff < 0);
         }

         return (m_used < other.m_used);
      }

      template <typename T, typename TAlloc>
      bool String<T, TAlloc>::operator>(const String &other) const
      {
         for (int32 i = 0; m_strArray[i] && other.m_strArray[i]; i++)
         {
            const int32 diff = m_strArray[i] - other.m_strArray[i];
            if (diff)
               return (diff > 0);
         }

         return (m_used > other.m_used);
      }

      template <typename T, typename TAlloc>
      bool String<T, TAlloc>::operator<=(const String &other) const
      {
         for (int32 i = 0; m_strArray[i] && other.m_strArray[i]; i++)
         {
            const int32 diff = m_strArray[i] - other.m_strArray[i];
            if (diff)
               return (diff <= 0);
         }

         return (m_used <= other.m_used);
      }

      template <typename T, typename TAlloc>
      bool String<T, TAlloc>::operator>=(const String &other) const
      {
         for (int32 i = 0; m_strArray[i] && other.m_strArray[i]; i++)
         {
            const int32 diff = m_strArray[i] - other.m_strArray[i];
            if (diff)
               return (diff >= 0);
         }

         return (m_used >= other.m_used);
      }

      template <typename T, typename TAlloc>
      bool String<T, TAlloc>::operator!=(const T* const str) const
      {
         return !(*this == str);
      }

      template <typename T, typename TAlloc>
      bool String<T, TAlloc>::operator!=(const String<T, TAlloc> &other) const
      {
         return !(*this == other);
      }

      template <typename T, typename TAlloc>
      inline T String<T, TAlloc>::operator[](const uint32 index) const
      {
         assert(index < m_used);
         return m_strArray[index];
      }

      template <typename T, typename TAlloc>
      inline T &String<T, TAlloc>::operator[](const uint32 index)
      {
         assert(index < m_used);
         return m_strArray[index];
      }

      template <typename T, typename TAlloc>
      inline bool String<T, TAlloc>::IsEmpty() const
      {
         return (m_used == 1);
      }

      template <typename T, typename TAlloc>
      inline uint32 String<T, TAlloc>::GetSize() const
      {
         return m_used - 1;
      }

      template <typename T, typename TAlloc>
      inline int32 String<T, TAlloc>::StringToInt() const
      {
         int32 index;
         if ((index = FindNextNumber(0)) != -1)
            return atoi(&m_strArray[index]);
         return INT32_MAX;
      }

      template <typename T, typename TAlloc>
      inline float String<T, TAlloc>::StringToFloat() const
      {
         int32 index;
         if ((index = FindNextNumber(0)) != -1)
            return (float)atof(&m_strArray[index]);
         return FLOAT_MAX;
      }

      template <typename T, typename TAlloc>
      inline char String<T, TAlloc>::GetLastChar() const
      {
         return m_strArray[m_used - 2];
      }

      template <typename T, typename TAlloc>
      inline const T* String<T, TAlloc>::CString() const
      {
         return m_strArray;
      }

      template <typename T, typename TAlloc>
      template <typename B>
      String<T, TAlloc> &String<T, TAlloc>::operator=(const B* const c)
      {
         if (!c)
         {
            if (!m_strArray)
            {
               m_strArray = m_allocator.Allocate(1); //new T[1];
               m_allocated = 1;
            }
            m_used = 1;
            m_strArray[0] = 0x0;
            return *this;
         }

         if ((void*)c == (void*)m_strArray)
            return *this;

         int32 len = 0;
         const B* p = c;
         do
         {
            len++;
         } while (*p++);

         // we'll keep the old string for a while, because the new
         // string could be a part of the current string.
         T* oldArray = m_strArray;

         m_used = len;
         if (m_used > m_allocated)
         {
            m_allocated = m_used;
            m_strArray = m_allocator.Allocate(m_used); //new T[m_used];
         }

         // build the string array
         for (int32 i = 0; i < len; i++)
            m_strArray[i] = (T)c[i];

         if (oldArray != m_strArray)
            m_allocator.Free(oldArray); // delete [] oldArray;

         return *this;
      }

      // TODO: not well-functioned yet
      template <typename T, typename TAlloc>
      void String<T, TAlloc>::Trim(const bool left, const bool right)
      {
         int32 leftIdx = FindFirstNotOf(" \t\r");
         int32 rightIdx = FindLastNotOf(" \t\r");

         if (m_used == 1)
            return;

         if (left && leftIdx > 0)
            Erase(0, leftIdx - 1); // trim left

         if (right && rightIdx < m_used - 2)
            Erase(rightIdx + 1, GetSize() - 1); // trim right
      }

      template <typename T, typename TAlloc>
      String<T, TAlloc> &String<T, TAlloc>::Erase(const uint32 index1, const uint32 index2)
      {
         assert(index1 < m_used && index2 < m_used);

         uint32 idx1 = index1;
         uint32 idx2 = index2;

         if (idx1 > idx2)
         {
            uint32 tmp;
            tmp = idx1;
            idx1 = idx2;
            idx2 = tmp;
         }

         for (uint32 i = idx1 + 1; i < m_used; i++)
         {
            uint32 index = i + (idx2 - idx1);
            m_strArray[i - 1] = m_strArray[index];
         }

         m_used -= (idx2 - idx1 + 1);

         return *this;
      }

      // not working yet
      template <typename T, typename TAlloc>
      bool String<T, TAlloc>::EqualSubstr(const uint32 index1, const uint32 index2, const T* const str)
      {
         assert(index1 < m_used && index2 < m_used);

         if (!str)
            return false;

         uint32 idx1 = index1;
         uint32 idx2 = index2;

         if (idx1 > idx2)
         {
            uint32 tmp;
            tmp = idx1;
            idx1 = idx2;
            idx2 = tmp;
         }

         int32 i;
         for (i = idx1; m_strArray[i] && i <= idx2; i++)
         {
            if (m_strArray[i] != str[i])
               return false;
         }

         return (!m_strArray[i] && !str[i]);
      }

      template <typename T, typename TAlloc>
      bool String<T, TAlloc>::EqualsN(const String<T, TAlloc> &other, const uint32 n) const
      {
         uint32 i;
         for (i = 0; m_strArray[i] && other[i] && i < n; i++)
            if (m_strArray[i] != other[i])
               return false;

         // if one (or both) of the strings was smaller then they
         // are only equal if they have the same length
         return (i == n) || (m_used == other.m_used);
      }

      template <typename T, typename TAlloc>
      bool String<T, TAlloc>::EqualsN(const T* const str, const uint32 n) const
      {
         if (!str)
            return false;
         int32 i;
         for (i = 0; m_strArray[i] && str[i] && i < n; i++)
            if (m_strArray[i] != str[i])
               return false;

         // if one (or both) of the strings was smaller then they
         // are only equal if they have the same length
         return (i == n) || (m_strArray[i] == 0 && str[i] == 0);
      }

      template <typename T, typename TAlloc>
      inline String<T, TAlloc> &String<T, TAlloc>::ToLower()
      {
         for (int32 i = 0; m_strArray[i]; i++)
            m_strArray[i] = _ToLower(m_strArray[i]);
         return *this;
      }

      // TODO: implement later
      //template <typename T, typename TAlloc>
      //String<T, TAlloc> &String<T, TAlloc>::operator=( const float number )
      //{
      //   return *this;
      //}

      template <typename T, typename TAlloc>
      inline String<T, TAlloc> &String<T, TAlloc>::ToUpper()
      {
         for (int32 i = 0; m_strArray[i]; i++)
            m_strArray[i] = _ToUpper(m_strArray[i]);
         return *this;
      }

      template <typename T, typename TAlloc>
      inline String<T, TAlloc> &String<T, TAlloc>::Append(const T character)
      {
         if (m_used + 1 > m_allocated)
            Reallocate(m_used + 1);

         m_used++;

         m_strArray[m_used - 2] = character;
         m_strArray[m_used - 1] = 0;

         return *this;
      }

      template <typename T, typename TAlloc>
      String<T, TAlloc> &String<T, TAlloc>::Append(const T* const other, const uint32 length)
      {
         if (!other)
            return *this;

         uint32 len = 0;
         const T* p = other;
         while (*p)
         {
            len++;
            p++;
         }
         if (len > length)
            len = length;

         if (m_used + len > m_allocated)
            Reallocate(m_used + len);

         m_used--;
         len++;

         for (uint32 i = 0; i < len; i++)
            m_strArray[i + m_used] = *(other + i);

         m_used += len;

         return *this;
      }

      template <typename T, typename TAlloc>
      String<T, TAlloc> &String<T, TAlloc>::Append(const String<T, TAlloc> &other)
      {
         if (other.GetSize() == 0)
            return *this;

         m_used--;
         int32 len = other.GetSize() + 1;

         if (m_used + len > m_allocated)
            Reallocate(m_used + len);

         for (int32 i = 0; i < len; i++)
            m_strArray[m_used + i] = other[i];

         m_used += len;

         return *this;
      }

      template <typename T, typename TAlloc>
      inline String<T, TAlloc> String<T, TAlloc>::operator+(const String<T, TAlloc> &other) const
      {
         String<T, TAlloc> str(*this);
         str.Append(other);

         return str;
      }

      template <typename T, typename TAlloc>
      inline String<T, TAlloc> &String<T, TAlloc>::operator+=(const char character)
      {
         // Append the given string into a new string

         //char *sub = new (m_used + 1) char(NULL); // temporarily commented

         //memcpy(sub, m_strArray, len);  // temporarily commented

         //sub[len] = c;
         //sub[len+1] = 0;

         //_string->release();
         //_string = sub;

         return *this;
      }

      template <typename T, typename TAlloc>
      String<T, TAlloc> &String<T, TAlloc>::operator=(const int32 number)
      {
         *this = String<T, TAlloc>(number);
         return *this;
      }

      template <typename T, typename TAlloc>
      String<T, TAlloc> &String<T, TAlloc>::RemoveChars(const String<T, TAlloc> &chars)
      {
         if (chars.GetSize() == 0)
            return *this;

         int32 pos = 0;
         int32 found = 0;
         for (uint32 i = 0; i < m_used - 1; i++)
         {
            // Don't use characters.findFirst as it finds the \0,
            // causing m_used to become incorrect.
            bool docontinue = false;
            for (uint32 j = 0; j < chars.GetSize(); j++)
            {
               if (chars[j] == m_strArray[i])
               {
                  found++;
                  docontinue = true;
                  break;
               }
            }
            if (docontinue)
               continue;

            m_strArray[pos++] = m_strArray[i];
         }

         m_used -= found;
         m_strArray[m_used - 1] = 0;

         return *this;
      }

      template <typename T, typename TAlloc>
      String<T, TAlloc> &String<T, TAlloc>::operator=(const String<T, TAlloc> &other)
      {
         if (this == &other)
            return *this;

         m_used = other.GetSize() + 1;
         if (m_used > m_allocated)
         {
            m_allocator.Free(m_strArray);
            m_allocated = m_used;
            m_strArray = m_allocator.Allocate(m_used); //new T[m_used];
         }

         const T* p = other.CString();
         for (uint32 i = 0; i < m_used; i++, p++)
            m_strArray[i] = *p;

         return *this;
      }

      template <typename T, typename TAlloc>
      int32 String<T, TAlloc>::FindFirst(const T c) const
      {
         for (uint32 i = 0; i < m_used - 1; i++)
         {
            if (m_strArray[i] == c)
               return i;
         }

         return -1;
      }

      template <typename T, typename TAlloc>
      int32 String<T, TAlloc>::FindFirstNotOf(const T character) const
      {
         if (m_used == 1)
            return -1;

         for (int32 i = 0; i < m_used - 1; i++)
         {
            if (m_strArray[i] != character)
               return i;
         }

         return -1;
      }

      template <typename T, typename TAlloc>
      int32 String<T, TAlloc>::FindFirstNotOf(const String<T, TAlloc> &chars) const
      {
         // TODO: some test fix? case : findfirstnotof "", string = ""
         if (chars.GetSize() == 0)
            return 0;
         if (m_used == 1)
            return -1;
         for (int32 i = 0; i < m_used - 1; i++)
         {
            {
               if (chars.FindFirst(m_strArray[i]) == -1)
                  return i;
            }
         }

         return -1;
      }

      template <typename T, typename TAlloc>
      int32 String<T, TAlloc>::FindLast(const T c) const
      {
         for (int32 i = m_used - 2; i >= 0; i--)
         {
            if (m_strArray[i] == c)
               return i;
         }

         return -1;
      }

      template <typename T, typename TAlloc>
      int32 String<T, TAlloc>::FindLastNotOf(const T c) const
      {
         for (int32 i = m_used - 2; i >= 0; i--)
         {
            if (m_strArray[i] != c)
               return i;
         }

         return -1;
      }

      template <typename T, typename TAlloc>
      int32 String<T, TAlloc>::FindLastNotOf(const String<T, TAlloc> &chars) const
      {
         // TODO: some test fix? case : findfirstnotof "", string = ""
         if (chars.GetSize() == 0)
            return 0;
         if (m_used == 1)
            return -1;

         for (int32 i = m_used - 2; i >= 0; i--)
         {
            if (chars.FindFirst(m_strArray[i]) == -1)
               return i;
         }

         return -1;
      }

      template <typename T, typename TAlloc>
      int32 String<T, TAlloc>::FindNext(const T c, const uint32 startPos) const
      {
         for (uint32 i = startPos; i < m_used - 1; i++)
         {
            if (m_strArray[i] == c)
            {
               return i;
            }
         }
         return -1;
      }

      template <typename T, typename TAlloc>
      int32 String<T, TAlloc>::FindNextNumber(const uint32 startPos) const
      {
         for (uint32 i = startPos; i < m_used - 1; i++)
         {
            if (IsADigit(m_strArray[i]))
            {
               if (i != 0)
               {
                  if (m_strArray[i - 1] == '-')
                     return i - 1;
               }
               return i;
            }
         }
         return -1;
      }

      template <typename T, typename TAlloc>
      template <typename TContainer>
      int32 String<T, TAlloc>::Tokenize(TContainer &ret, const T* const delimiter, const int32 count,
         const bool ignoreEmptyTokens, const bool keepSeparators) const
      {
         if (!delimiter)
            return 0;

         const int32 oldSize = ret.size();
         uint32 lastpos = 0;
         bool lastWasSeparator = false;
         for (uint32 i = 0; i < m_used; i++)
         {
            bool foundSeparator = false;
            for (int32 j = 0; j < count; j++)
            {
               if (m_strArray[i] == delimiter[j])
               {
                  if ((!ignoreEmptyTokens || i - lastpos != 0) &&
                     !lastWasSeparator)
                     ret.push_back(String<T, TAlloc>(&m_strArray[lastpos], i - lastpos));
                  foundSeparator = true;
                  lastpos = (keepSeparators ? i : i + 1);
                  break;
               }
            }
            lastWasSeparator = foundSeparator;
         }
         if ((m_used - 1) > lastpos)
            ret.push_back(String<T, TAlloc>(&m_strArray[lastpos], (m_used - 1) - lastpos));
         return ret.size() - oldSize;
      }

      template <typename T, typename TAlloc>
      String<T, TAlloc> String<T, TAlloc>::SubString(const uint32 begin, const uint32 length, const bool makeLower) const
      {
         uint32 size = m_used - 1;
         uint32 len = length;
         // if start after string
         // or no proper substring length
         if ((len <= 0) || (begin >= size))
            return String<T, TAlloc>("", 0);
         // clamp length to maximal value
         if ((len + begin) > size)
            len = size - begin;

         String<T, TAlloc> out;
         out.Reserve(len + 1);

         uint32 i;
         if (!makeLower)
         {
            for (i = 0; i < len; i++)
               out.m_strArray[i] = m_strArray[i + begin];
         }
         else
         {
            for (i = 0; i < len; i++)
               out.m_strArray[i] = _ToLower(m_strArray[i + begin]);
         }

         out.m_strArray[len] = 0;
         out.m_used = len + 1;

         return out;
      }

      template <typename T, typename TAlloc>
      String<T, TAlloc> String<T, TAlloc>::operator()(const uint32 index1, const uint32 index2)
      {
         return SubString(index1, index2 - index1);
      }

      template <typename T, typename TAlloc>
      String<T, TAlloc> String<T, TAlloc>::operator()(const uint32 index)
      {
         return SubString(index, this->GetSize());
      }

      template <typename T, typename TAlloc>
      void String<T, TAlloc>::Reserve(const uint32 count)
      {
         if (count < m_allocated)
            return;

         Reallocate(count);
      }

   } // namespace string

} // namespace core

#endif
