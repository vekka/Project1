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

/**	@file	ObjTools.h
*	@brief	Some helpful templates for text parsing
*/
#ifndef _OBJTOOLS_HPP_INCLUDED_
#define _OBJTOOLS_HPP_INCLUDED_

#include "fast_atof.h"
//#include "ParsingUtils.h"
#include <vector>

#include "core/string/string.hpp"
using core::string::String_c;

namespace objtools
{
   /**	@brief	Returns true, if the last entry of the buffer is reached.
   *	@param	it	Iterator of current position.
   *	@param	end	Iterator with end of buffer.
   *	@return	true, if the end of the buffer is reached.
   */
   template<typename TChar>
   inline bool IsEndOfBuffer(TChar it, TChar end)
   {
      if (it == end)
      {
         return true;
      }
      else
      {
         end--;
      }
      return (it == end);
   }

   /**	@brief	Returns next word separated by a space
   *	@param	pBuffer	Pointer to data buffer
   *	@param	pEnd	Pointer to end of buffer
   *	@return	Pointer to next space
   */
   template<typename TCHAR>
   inline TCHAR GetNextWord(TCHAR pBuffer, TCHAR pEnd)
   {
      while (!isEndOfBuffer(pBuffer, pEnd))
      {
         if (!IsSpaceOrNewLine(*pBuffer) || IsLineEnd(*pBuffer))
            break;
         pBuffer++;
      }
      return pBuffer;
   }

   /**	@brief	Returns pointer a next token
   *	@param	pBuffer	Pointer to data buffer
   *	@param	pEnd	Pointer to end of buffer
   *	@return	Pointer to next token
   */
   template<typename TCHAR>
   inline TCHAR GetNextToken(TCHAR pBuffer, TCHAR pEnd)
   {
      while (!IsEndOfBuffer(pBuffer, pEnd))
      {
         if (IsSpaceOrNewLine(*pBuffer))
            break;
         pBuffer++;
      }
      return GetNextWord(pBuffer, pEnd);
   }

   /**	@brief	Skips a line
   *	@param	it		Iterator set to current position
   *	@param	end		Iterator set to end of scratch buffer for readout
   *	@param	uiLine	Current line number in format
   *	@return	Current-iterator with new position
   */
   template<typename TChar>
   inline TChar SkipLine(TChar it, TChar end, uint32 &uiLine) {
      while (!IsEndOfBuffer(it, end) && !IsLineEnd(*it)) {
         ++it;
      }
      if (it != end)
      {
         ++it;
         ++uiLine;
      }
      // fix .. from time to time there are spaces at the beginning of a material line
      while (it != end && (*it == '\t' || *it == ' '))
         ++it;
      return it;
   }

   /**	@brief	Get a name from the current line. Preserve space in the middle,
   *    but trim it at the end.
   *	@param	it		set to current position
   *	@param	end		set to end of scratch buffer for readout
   *	@param	name	Separated name
   *	@return	Current-iterator with new position
   */
   template<typename TChar>
   inline TChar GetName(TChar it, TChar end, String_c &name)
   {
      name = "";
      if (IsEndOfBuffer(it, end)) {
         return end;
      }

      char *pStart = &(*it);
      while (!IsEndOfBuffer(it, end) && !IsLineEnd(*it)) {
         ++it;
      }

      while (IsEndOfBuffer(it, end) || IsLineEnd(*it) || IsSpaceOrNewLine(*it)) {
         --it;
      }
      ++it;

      // Get name
      // if there is no name, and the previous char is a separator, come back to start
      while (&(*it) < pStart) {
         ++it;
      }
      String_c strName(pStart, &(*it));
      if (strName.empty())
         return it;
      else
         name = strName;

      return it;
   }

   /**	@brief	Get next word from given line
   *	@param	it		set to current position
   *	@param	end		set to end of scratch buffer for readout
   *	@param	pBuffer	Buffer for next word
   *	@param	length	Buffer length
   *	@return	Current-iterator with new position
   */
   template<typename TChar>
   inline TChar CopyNextWord(TChar it, TChar end, char *pBuffer, size_t length)
   {
      size_t index = 0;
      it = GetNextWord<TChar>(it, end);
      while (!IsSpaceOrNewLine(*it) && !IsEndOfBuffer(it, end))
      {
         pBuffer[index] = *it;
         index++;
         if (index == length - 1)
            break;
         ++it;
      }
      pBuffer[index] = '\0';
      return it;
   }

   /**	@brief	Get next float from given line
   *	@param	it		set to current position
   *	@param	end		set to end of scratch buffer for readout
   *	@param	value	Separated float value.
   *	@return	Current-iterator with new position
   */
   template<typename TChar>
   inline TChar GetFloat(TChar it, TChar end, float &value)
   {
      static const size_t BUFFERSIZE = 1024;
      char buffer[BUFFERSIZE];
      it = CopyNextWord<TChar>(it, end, buffer, BUFFERSIZE);
      value = (float)fast_atof(buffer);

      return it;
   }

   /**	@brief	Will perform a simple tokenize.
   *	@param	str			String to tokenize.
   *	@param	tokens		Array with tokens, will be empty if no token was found.
   *	@param	delimiters	Delimiter for tokenize.
   *	@return	Number of found token.
   */

} // namespace objtools

#endif
