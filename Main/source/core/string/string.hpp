// TODO: make << operator for stream, (cout etc.)

#ifndef _STRING_HPP_INCLUDED_
#define _STRING_HPP_INCLUDED_

#include <iostream>

#include <assert.h>

#include <iostream>

#include "core/memory/allocator.hpp"
#include "core/math/mathcommon.hpp" // IsZero

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

namespace string
{

template <class T, class TAlloc = Allocator<T>>
class String
{
private:
	T* strArray;
	uint32 allocated;
	uint32 used;
   TAlloc allocator;

	void Reallocate( const uint32 newSize );

public:
   String();
   String( const String &other );
   template <class B> String( const B* const c, const uint32 length );
	template <class B> String( const B* const c );
	explicit String( const int32 number );
   explicit String( const float number );

   ~String();

   String &operator=( const String &other );
   template <class B> String &operator=( const B* const c );
   String &operator=( const int32 number );

   int32 StringToInt() const; // return the first int32 value in a string or return INT32_MAX if not found
   float StringToFloat() const; // return the first float value in a string or return FLOAT_MAX if not found

   //TODO:
   char GetLastChar() const;

   T operator[]( const uint32 index ) const;
   T &operator[]( const uint32 index );

   bool IsEmpty() const;
   uint32 GetSize() const;
   const T* CString() const;

	bool operator==( const T* const str ) const;
	bool operator==( const String &other ) const;
	bool operator==( const int32 number ) const;
   bool operator<( const String &other ) const;
   bool operator>( const String &other ) const;
   bool operator<=( const String &other ) const;
   bool operator>=( const String &other ) const;
   bool operator!=( const T* const str ) const;
	bool operator!=( const String &other ) const;

   bool EqualSubstr( const uint32 index1, const uint32 index2, const T* const str );

   String &Erase( const uint32 index1, const uint32 index2 ); // erase index2-index1 number of char from index1 inclusive

   void Trim( const bool left = true, const bool right = true );

   String &RemoveChars( const String &chars );
   bool EqualsN( const String &other, const uint32 n ) const;
   bool EqualsN( const T* const str, const uint32 n ) const;
   String &ToUpper();
   String &ToLower();

   String &Append( const T character );
   String &Append( const T* const other, const uint32 length = 0xffffffff );
   String &Append( const String &other);

   String operator+( const String &other ) const;
   String &operator+=( const char character );

   int32 FindFirst( const T c ) const;
   int32 FindFirstNotOf( const T c ) const; 
   int32 FindFirstNotOf( const String<T, TAlloc> &chars ) const;
   int32 FindLast( const T c ) const;
   int32 FindLastNotOf( const T c ) const;
   int32 FindLastNotOf( const String<T, TAlloc> &chars ) const;
   int32 FindNext( const T c, const uint32 startPos ) const;
   int32 FindNextNumber( const uint32 startPos ) const;
   
   template <class TContainer> int32 Tokenize( TContainer &ret, const T* const delimiter = " \t\r\n", const int32 count = 1,
      const bool ignoreEmptyTokens = true, const bool keepSeparators = false ) const;
   String SubString( const uint32 begin, const uint32 length, const bool makeLower = false ) const;
   String operator()( const uint32 index1, const uint32 index2 );
   String operator()( const uint32 index );
   void Reserve( const uint32 count );

   //friend std::ostream &operator<<( std::ostream &out, const String &str )
   //{
   //   out << str->strArray;
   //   return out;
   //}
};

typedef String<char> String_c;
typedef String<wchar_t> String_w;

template <class T, class TAlloc>
void String<T, TAlloc>::Reallocate( const uint32 newSize )
{
   T* old_array = strArray;

   strArray = allocator.Allocate(newSize); //new T[new_size];
   allocated = newSize;

   int32 amount = used < newSize ? used : newSize;
   for (int32 i = 0; i < amount; i++)
      strArray[i] = old_array[i];

   if (allocated < used)
      used = allocated;

   allocator.Free(old_array); // delete [] old_array;
}

template <class T, class TAlloc>
inline String<T, TAlloc>::String() : allocated(1), used(1)
{
   strArray = allocator.Allocate(1);
   strArray[0] = 0;
}

template <class T, class TAlloc>
inline String<T, TAlloc>::String( const String<T, TAlloc> &other ) : strArray(NULL), allocated(0), used(0)
{
   *this = other;
}

template <class T, class TAlloc>
template <class B>
String<T, TAlloc>::String( const B* const c, const uint32 length ) : strArray(NULL), allocated(0), used(0)
{
   if (!c)
   {
      // correctly init the string to an empty one
      *this="";
      return;
   }

   allocated = used = length+1;
   strArray = allocator.Allocate(used); // new T[used];

   for (uint32 i = 0; i < length; i++)
      strArray[i] = (T)c[i];

   strArray[length] = 0;
}

template <class T, class TAlloc>
template <class B>
String<T, TAlloc>::String( const B* const c ) : strArray(NULL), allocated(0), used(0)
{
   *this = c;
}

template <class T, class TAlloc>
inline String<T, TAlloc>::String( const int32 number ) : strArray(NULL), allocated(0), used(0)
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

   char tmpbuf[16]={0};
   int32 idx = 15;

   // special case '0'

   if (!number)
   {
      tmpbuf[14] = '0';
      *this = &tmpbuf[14];
      return;
   }

   // add numbers

   while(num && idx)
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
template <class T, class TAlloc>
inline String<T, TAlloc>::String( const float number ) : strArray(NULL), allocated(0), used(0)
{
   int32 intNum = (int32)number;
  
   char tmpbuf[32]={0};
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

template <class T, class TAlloc>
inline String<T, TAlloc>::~String()
{
   allocator.Free( strArray );
}

template <class T, class TAlloc>
bool String<T, TAlloc>::operator==( const T* const str ) const
{
   if (!str)
      return false;

   int32 i;
   for ( i = 0; strArray[i] && str[i]; i++ )
   {
      if ( strArray[i] != str[i] )
         return false;
   }

   return (!strArray[i] && !str[i]);
}

template <class T, class TAlloc>
bool String<T, TAlloc>::operator==( const String<T, TAlloc> &other ) const
{
   for (int32 i = 0; strArray[i] && other.strArray[i]; i++)
      if (strArray[i] != other.strArray[i])
         return false;

   return used == other.used;
}

template <class T, class TAlloc>
bool String<T, TAlloc>::operator==( const int32 number ) const
{
   String_c str(number);

   int32 i;
   for ( i = 0; strArray[i] && str[i]; i++ )
   {
      if ( strArray[i] != str[i] )
         return false;
   }

   return (!strArray[i] && !str[i]);
}

template <class T, class TAlloc>
bool String<T, TAlloc>::operator<( const String &other ) const
{
   for (int32 i = 0; strArray[i] && other.strArray[i]; i++)
   {
      const int32 diff = strArray[i] - other.strArray[i];
      if (diff)
         return (diff < 0);
   }

   return (used < other.used);
}

template <class T, class TAlloc>
bool String<T, TAlloc>::operator>( const String &other ) const
{
   for (int32 i = 0; strArray[i] && other.strArray[i]; i++)
   {
      const int32 diff = strArray[i] - other.strArray[i];
      if (diff)
         return (diff > 0);
   }

   return (used > other.used);
}

template <class T, class TAlloc>
bool String<T, TAlloc>::operator<=( const String &other ) const
{
   for (int32 i = 0; strArray[i] && other.strArray[i]; i++)
   {
      const int32 diff = strArray[i] - other.strArray[i];
      if (diff)
         return (diff <= 0);
   }

   return (used <= other.used);
}

template <class T, class TAlloc>
bool String<T, TAlloc>::operator>=( const String &other ) const
{
   for (int32 i = 0; strArray[i] && other.strArray[i]; i++)
   {
      const int32 diff = strArray[i] - other.strArray[i];
      if (diff)
         return (diff >= 0);
   }

   return (used >= other.used);
}

template <class T, class TAlloc>
bool String<T, TAlloc>::operator!=( const T* const str ) const
{
   return !(*this == str);
}

template <class T, class TAlloc>
bool String<T, TAlloc>::operator!=( const String<T, TAlloc> &other ) const
{
   return !(*this == other);
}

template <class T, class TAlloc>
inline T String<T, TAlloc>::operator[]( const uint32 index ) const
{
   assert( index < used );
   return strArray[index];
}

template <class T, class TAlloc>
inline T &String<T, TAlloc>::operator[]( const uint32 index )
{
   assert( index < used );
   return strArray[index];
}

template <class T, class TAlloc>
inline bool String<T, TAlloc>::IsEmpty() const
{
   return ( used == 1 );
}

template <class T, class TAlloc>
inline uint32 String<T, TAlloc>::GetSize() const
{
   return used - 1;
}

template <class T, class TAlloc>
inline int32 String<T, TAlloc>::StringToInt() const
{
   int32 index;
   if ( (index = FindNextNumber(0)) != -1 )
      return atoi( &strArray[index] );
   return INT32_MAX;
}

template <class T, class TAlloc>
inline float String<T, TAlloc>::StringToFloat() const
{
   int32 index;
   if ( (index = FindNextNumber(0)) != -1 )
      return (float)atof( &strArray[index] );
   return FLOAT_MAX;
}

template <class T, class TAlloc>
inline char String<T, TAlloc>::GetLastChar() const
{
   return strArray[used-2];
}

template <class T, class TAlloc>
inline const T* String<T, TAlloc>::CString() const
{
   return strArray;
}

template <class T, class TAlloc>
template <class B>
String<T, TAlloc> &String<T, TAlloc>::operator=( const B* const c )
{
   if (!c)
   {
      if (!strArray)
      {
         strArray = allocator.Allocate(1); //new T[1];
         allocated = 1;
      }
      used = 1;
      strArray[0] = 0x0;
      return *this;
   }

   if ((void*)c == (void*)strArray)
      return *this;

   int32 len = 0;
   const B* p = c;
   do
   {
      len++;
   } while(*p++);

   // we'll keep the old string for a while, because the new
   // string could be a part of the current string.
   T* oldArray = strArray;

   used = len;
   if (used>allocated)
   {
      allocated = used;
      strArray = allocator.Allocate(used); //new T[used];
   }

   // build the string array
   for (int32 i = 0; i < len; i++)
      strArray[i] = (T)c[i];

   if (oldArray != strArray)
      allocator.Free(oldArray); // delete [] oldArray;

   return *this;
}

// TODO: not well-functioned yet
template <class T, class TAlloc>
void String<T, TAlloc>::Trim( const bool left, const bool right )
{
   int32 leftIdx = FindFirstNotOf(" \t\r");
   int32 rightIdx = FindLastNotOf(" \t\r");

   if ( used == 1 )
      return;
   
   if( left && leftIdx > 0 )
      Erase(0, leftIdx-1); // trim left

   if( right && rightIdx < used - 2 )
      Erase(rightIdx+1, GetSize()-1); // trim right
}

template <class T, class TAlloc>
String<T, TAlloc> &String<T, TAlloc>::Erase( const uint32 index1, const uint32 index2 )
{
   assert( index1 < used && index2 < used );

   uint32 idx1 = index1;
   uint32 idx2 = index2;

   if ( idx1 > idx2 )
   {
      uint32 tmp;
      tmp = idx1;
      idx1 = idx2;
      idx2 = tmp;
   }

   for (uint32 i = idx1 + 1; i < used; i++)
   {
      uint32 index = i+(idx2-idx1);
      strArray[i-1] = strArray[index];
   }

   used -= (idx2 - idx1 + 1);

   return *this;
}

// not working yet
template <class T, class TAlloc>
bool String<T, TAlloc>::EqualSubstr( const uint32 index1, const uint32 index2, const T* const str )
{
   assert( index1 < used && index2 < used );

   if (!str)
      return false;

   uint32 idx1 = index1;
   uint32 idx2 = index2;

   if ( idx1 > idx2 )
   {
      uint32 tmp;
      tmp = idx1;
      idx1 = idx2;
      idx2 = tmp;
   }

   int32 i;
   for ( i = idx1; strArray[i] && i <= idx2 ; i++ )
   {
      if ( strArray[i] != str[i] )
         return false;
   }

   return (!strArray[i] && !str[i]);
}

template <class T, class TAlloc>
bool String<T, TAlloc>::EqualsN( const String<T, TAlloc> &other, const uint32 n ) const
{
   uint32 i;
   for( i = 0; strArray[i] && other[i] && i < n; i++ )
      if (strArray[i] != other[i])
         return false;

   // if one (or both) of the strings was smaller then they
   // are only equal if they have the same length
   return (i == n) || (used == other.used);
}

template <class T, class TAlloc>
bool String<T, TAlloc>::EqualsN( const T* const str, const uint32 n ) const
{
   if (!str)
      return false;
   int32 i;
   for(i = 0; strArray[i] && str[i] && i < n; i++)
      if (strArray[i] != str[i])
         return false;

   // if one (or both) of the strings was smaller then they
   // are only equal if they have the same length
   return (i == n) || (strArray[i] == 0 && str[i] == 0);
}

template <class T, class TAlloc>
inline String<T, TAlloc> &String<T, TAlloc>::ToLower()
{
   for ( int32 i = 0; strArray[i]; i++ )
      strArray[i] = _ToLower( strArray[i] );
   return *this;
}

// TODO: implement later
//template <class T, class TAlloc>
//String<T, TAlloc> &String<T, TAlloc>::operator=( const float number )
//{
//   return *this;
//}

template <class T, class TAlloc>
inline String<T, TAlloc> &String<T, TAlloc>::ToUpper()
{
   for ( int32 i = 0; strArray[i]; i++ )
      strArray[i] = _ToUpper( strArray[i] );
   return *this;
}

template <class T, class TAlloc>
inline String<T, TAlloc> &String<T, TAlloc>::Append( const T character )
{
   if (used + 1 > allocated)
      Reallocate(used + 1);

   used++;

   strArray[used-2] = character;
   strArray[used-1] = 0;

   return *this;
}

template <class T, class TAlloc>
String<T, TAlloc> &String<T, TAlloc>::Append( const T* const other, const uint32 length )
{
   if (!other)
      return *this;

   uint32 len = 0;
   const T* p = other;
   while(*p)
   {
      len++;
      p++;
   }
   if (len > length)
      len = length;

   if (used + len > allocated)
      Reallocate(used + len);

   used--;
   len++;

   for (uint32 i = 0; i < len; i++ )
      strArray[i+used] = *(other+i);

   used += len;

   return *this;
}

template <class T, class TAlloc>
String<T, TAlloc> &String<T, TAlloc>::Append( const String<T, TAlloc> &other )
{
   if (other.GetSize() == 0)
      return *this;

   used--;
   int len = other.GetSize()+1;

   if (used + len > allocated)
      Reallocate(used + len);

   for ( int i = 0; i < len; i++ )
      strArray[used+i] = other[i];

   used += len;

   return *this;
}

template <class T, class TAlloc>
inline String<T, TAlloc> String<T, TAlloc>::operator+( const String<T, TAlloc> &other ) const
{
   String<T, TAlloc> str(*this);
   str.Append(other);

   return str;
}

template <class T, class TAlloc>
inline String<T, TAlloc> &String<T, TAlloc>::operator+=( const char character )
{
   // Append the given string into a new string
   char *sub = new ( used + 1 ) char( NULL );

   memcpy( sub, strArray, len );
   //sub[len] = c;
   //sub[len+1] = 0;

   //_string->release();
   //_string = sub;

   return *this;
}

template <class T, class TAlloc>
String<T, TAlloc> &String<T, TAlloc>::operator=( const int32 number )
{
   *this = String<T, TAlloc>(number);
   return *this;
}

template <class T, class TAlloc>
String<T, TAlloc> &String<T, TAlloc>::RemoveChars( const String<T, TAlloc> &chars )
{
   if (chars.GetSize() == 0)
      return *this;

   int32 pos = 0;
   int32 found = 0;
   for ( uint32 i = 0; i < used-1; i++ )
   {
      // Don't use characters.findFirst as it finds the \0,
      // causing used to become incorrect.
      bool docontinue = false;
      for ( uint32 j = 0; j < chars.GetSize(); j++ )
      {
         if (chars[j] == strArray[i])
         {
            found++;
            docontinue = true;
            break;
         }
      }
      if (docontinue)
         continue;

      strArray[pos++] = strArray[i];
   }

   used -= found;
   strArray[used-1] = 0;

   return *this;
}

template <class T, class TAlloc>
String<T, TAlloc> &String<T, TAlloc>::operator=( const String<T, TAlloc> &other )
{
   if (this == &other)
      return *this;

   used = other.GetSize()+1;
   if (used>allocated)
   {
      allocator.Free(strArray);
      allocated = used;
      strArray = allocator.Allocate(used); //new T[used];
   }

   const T* p = other.CString();
   for (uint32 i = 0; i < used; i++, p++)
      strArray[i] = *p;

   return *this;
}

template <class T, class TAlloc>
int32 String<T, TAlloc>::FindFirst( const T c ) const
{
   for (uint32 i = 0; i < used - 1; i++ )
   {
      if (strArray[i] == c)
         return i;
   }

   return -1;
}

template <class T, class TAlloc>
int32 String<T, TAlloc>::FindFirstNotOf( const T character ) const
{
   if (used == 1)
      return -1;

   for (int32 i = 0; i < used - 1; i++ )
   {
      if (strArray[i] != character)
         return i;
   }

   return -1;
}

template <class T, class TAlloc>
int32 String<T, TAlloc>::FindFirstNotOf( const String<T, TAlloc> &chars ) const
{
   // TODO: some test fix? case : findfirstnotof "", string = ""
   if (chars.GetSize() == 0)
      return 0;
   if (used == 1)
      return -1;
   for (int32 i = 0; i < used - 1; i++)
   {
      {
         if (chars.FindFirst(strArray[i]) == -1)
            return i;
      }
   }

   return -1;
}

template <class T, class TAlloc>
int32 String<T, TAlloc>::FindLast( const T c ) const
{
   for (int32 i = used - 2; i >= 0; i-- )
   {
      if (strArray[i] == c)
         return i;
   }

   return -1;
}

template <class T, class TAlloc>
int32 String<T, TAlloc>::FindLastNotOf( const T c ) const
{
   for (int32 i = used - 2; i >= 0; i-- )
   {
      if (strArray[i] != c)
         return i;
   }

   return -1;
}

template <class T, class TAlloc>
int32 String<T, TAlloc>::FindLastNotOf( const String<T, TAlloc> &chars ) const
{
   // TODO: some test fix? case : findfirstnotof "", string = ""
   if (chars.GetSize() == 0)
      return 0;
   if (used == 1)
      return -1;

   for (int32 i = used - 2; i >= 0; i--)
   {
      if (chars.FindFirst(strArray[i]) == -1)
         return i;
   }

   return -1;
}

template <class T, class TAlloc>
int32 String<T, TAlloc>::FindNext( const T c, const uint32 startPos ) const
{
   for (uint32 i = startPos; i < used - 1; i++)
   {
      if (strArray[i] == c)
      {
         return i;
      }
   }
   return -1;
}

template <class T, class TAlloc>
int32 String<T, TAlloc>::FindNextNumber( const uint32 startPos ) const
{
   for (uint32 i = startPos; i < used - 1; i++)
   {
      if (IsADigit(strArray[i]))
      {
         if (i != 0)
         {
            if (strArray[i-1] == '-')
               return i - 1;
         }
         return i;
      }
   }
   return -1;
}

template <class T, class TAlloc>
template <class TContainer>
int32 String<T, TAlloc>::Tokenize( TContainer &ret, const T* const delimiter, const int32 count,
   const bool ignoreEmptyTokens, const bool keepSeparators ) const
{
   if (!delimiter)
      return 0;

   const int32 oldSize=ret.size();
   uint32 lastpos = 0;
   bool lastWasSeparator = false;
   for (uint32 i = 0; i < used; i++ )
   {
      bool foundSeparator = false;
      for (int32 j = 0; j < count; j++ )
      {
         if (strArray[i] == delimiter[j])
         {
            if ((!ignoreEmptyTokens || i - lastpos != 0) &&
               !lastWasSeparator)
               ret.push_back(String<T, TAlloc>(&strArray[lastpos], i - lastpos));
            foundSeparator = true;
            lastpos = (keepSeparators ? i : i + 1);
            break;
         }
      }
      lastWasSeparator = foundSeparator;
   }
   if ((used - 1) > lastpos)
      ret.push_back(String<T, TAlloc>(&strArray[lastpos], (used - 1) - lastpos));
   return ret.size()-oldSize;
}

template <class T, class TAlloc>
String<T, TAlloc> String<T, TAlloc>::SubString( const uint32 begin, const uint32 length, const bool makeLower ) const
{
   uint32 size = used - 1;
   uint32 len = length;
   // if start after string
   // or no proper substring length
   if ((len <= 0) || (begin >= size))
      return String<T, TAlloc>("", 0);
   // clamp length to maximal value
   if ((len+begin) > size)
      len = size - begin;

   String<T, TAlloc> out;
   out.Reserve(len+1);

   uint32 i;
   if ( !makeLower )
   {
      for (i = 0; i < len; i++)
         out.strArray[i] = strArray[i+begin];
   }
   else
   {
      for (i = 0; i < len; i++)
         out.strArray[i] = _ToLower( strArray[i+begin] );
   }

   out.strArray[len] = 0;
   out.used = len + 1;

   return out;
}

template <class T, class TAlloc>
String<T, TAlloc> String<T, TAlloc>::operator()( const uint32 index1, const uint32 index2 )
{
   return SubString(index1, index2 - index1);
}

template <class T, class TAlloc>
String<T, TAlloc> String<T, TAlloc>::operator()( const uint32 index )
{
   return SubString(index, this->GetSize() );
}

template <class T, class TAlloc>
void String<T, TAlloc>::Reserve( const uint32 count )
{
   if (count < allocated)
      return;

   Reallocate(count);
}

} // namespace string

} // namespace core

#endif