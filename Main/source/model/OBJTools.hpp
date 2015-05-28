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

#include "../core/fast_atof.hpp"
//#include "ParsingUtils.h"
#include <vector>

//#include "core/string/string.hpp"
//using core::string::String_c;
#include "core/charTypes.hpp"

using core::IsSpaceOrNewLine;

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
      while (!IsEndOfBuffer(pBuffer, pEnd))
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
   *	@param	currentPos		Iterator set to current position
   *	@param	end		Iterator set to end of scratch buffer for readout
   *	@param	currentLine	Current line number in format
   *	@return	Current-iterator with new position
   */
   template<typename TIterator>
   inline TIterator SkipLine(TIterator currentPos, TIterator end, uint32 &currentLine) {
      while (!IsEndOfBuffer(currentPos, end) && !IsLineEnd(*currentPos)) {
         currentPos++;
      }
      if (currentPos != end)
      {
         currentPos++;
         currentLine++;
      }
      // fix .. from time to time there are spaces at the beginning of a material line
      while (currentPos != end && (*currentPos == '\t' || *currentPos == ' '))
         currentPos++;
      return currentPos;
   }

   /**	@brief	Get a name from the current line. Preserve space in the middle,
   *    but trim it at the end.
   *	@param	it		set to current position
   *	@param	end		set to end of scratch buffer for readout
   *	@param	name	Separated name
   *	@return	Current-iterator with new position
   */
   template<typename TChar>
   inline TChar GetName(TChar it, TChar end, std::string &name)
   {
      name = "";
      if (IsEndOfBuffer(it, end)) {
         return end;
      }

      const char *pStart = &(*it);
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
      std::string strName(pStart, &(*it));
      if (strName.IsEmpty())
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
      value = (float)assimp::fast_atof(buffer);

      return it;
   }

   /**	@brief	Will perform a simple tokenize.
   *	@param	str			string to tokenize.
   *	@param	tokens		Array with tokens, will be empty if no token was found.
   *	@param	delimiters	Delimiter for tokenize.
   *	@return	Number of found token.
   */

   template<class string_type>
   unsigned int tokenize(const string_type& str, std::vector<string_type>& tokens,
      const string_type& delimiters)
   {
      // Skip delimiters at beginning.
      typename string_type::size_type lastPos = str.find_first_not_of(delimiters, 0);

      // Find first "non-delimiter".
      typename string_type::size_type pos = str.find_first_of(delimiters, lastPos);
      while (string_type::npos != pos || string_type::npos != lastPos)
      {
         // Found a token, add it to the vector.
         string_type tmp = str.substr(lastPos, pos - lastPos);
         if (!tmp.empty() && ' ' != tmp[0])
            tokens.push_back(tmp);

         // Skip delimiters.  Note the "not_of"
         lastPos = str.find_first_not_of(delimiters, pos);

         // Find next "non-delimiter"
         pos = str.find_first_of(delimiters, lastPos);
      }

      return static_cast<unsigned int>(tokens.size());
   }

} // namespace objtools

#endif
