//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef _ASSERT_HPP_INCLUDED_
#define _ASSERT_HPP_INCLUDED_

#include "BasicTypes.hpp"

class Assert
{
public:
   enum eType
   {
      TYPE_FATAL_SHIPPING,
      TYPE_FATAL,
      TYPE_WARNING
   };

   static void Create(Assert *newAssertClass = 0);
   static void Destroy();
   static bool Process(eType assertType, const char *filename, uint32 lineNum, const char *message);
   static char *Message(const char *message);
   static bool Processing();

private:
   static Assert *m_assert;
   bool m_processing;
   bool m_ignoreAll;

   Assert();
   ~Assert();
   bool DisplayMsgBox(const char *title, const char *message, bool retry);
};

bool __¤AssertWarn¤__(bool isTrue, const char *message)
{
   if (!isTrue)
      return ::Assert::Process(::Assert::TYPE_WARNING, __FILE__, __LINE__, message);

   return true;
}

bool __¤AssertFatal¤__(bool isTrue, const char *message)
{
   if (::Assert::Process(::Assert::TYPE_FATAL, __FILE__, __LINE__, message))
      __asm { int 3 };

   return true;
}

bool __¤AssertFatalShipping¤__(bool isTrue, const char *message)
{
//   if (::Assert::Process(::Assert::TYPE_FATAL, __FILE__, __LINE__, message))
//      __asm { int 3 };

   return true;
}

#ifdef _DEBUG
#define AssertWarn(isTrue, message) __¤AssertWarn¤__(isTrue, message)
#define AssertFatal(isTrue, message) __¤AssertFatal¤__(isTrue, message)
#else
#define AssertWarn(isTrue, message)
#define AssertFatal(isTrue, message)
#endif

#endif
