/*
---------------------------------------------------------------------------
Open Asset Import Library (assimp)
---------------------------------------------------------------------------

Copyright (c) 2006-2012, assimp team

All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the following
conditions are met:

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
---------------------------------------------------------------------------
*/
/** @file Default implementation of IOSystem using the standard C file functions */

//#include "FileSys.h"
//#include "DefaultIOStream.h"
//#include "StringComparison.h"

#include "filesys.hpp"

//#include "../include/assimp/DefaultLogger.hpp"
//#include "../include/assimp/assert.h"
#include <stdlib.h>

#include <algorithm>

#include <assert.h>

#include <cctype>

//#ifdef __unix__
//#include <sys/param.h>
//#include <stdlib.h>
//#endif

namespace core
{

   namespace filesys
   {

      bool Exists(const char* pFileName)
      {
         FILE* file = ::fopen(pFileName, "rb");
         if (!file)
            return false;

         ::fclose(file);
         return true;
      }

      //File* FileSys::Open(const char* strFile, const char* strMode)
      //{
      //   assert(NULL != strFile);
      //   assert(NULL != strMode);

      //   FILE* file = ::fopen(strFile, strMode);
      //   if (NULL == file)
      //      return NULL;

      //   return new DefaultIOStream(file, (std::string) strFile);
      //}

      //// ------------------------------------------------------------------------------------------------
      //// Closes the given file and releases all resources associated with it.
      //void FileSys::Close(IOStream* pFileName)
      //{
      //   delete pFileName;
      //}

      // ------------------------------------------------------------------------------------------------
      // Returns the operation specific directory separator
//      char FileSys::GetDirectorySeparator() const
//      {
//#ifndef _WIN32
//         return '/';
//#else
//         return '\\';
//#endif
//      }
//
      //bool FileSys::ComparePaths(const std::string &one, const std::string &second) const
      //{
      //   return (one == second);
      //}

      // maximum path length
      // XXX http://insanecoding.blogspot.com/2007/11/pathmax-simply-isnt.html 
#ifdef PATH_MAX
#	define PATHLIMIT PATH_MAX
#else
#	define PATHLIMIT 4096
#endif

      // ------------------------------------------------------------------------------------------------
      // Convert a relative path into an absolute path
      inline void MakeAbsolutePath(const char* in, char* _out)
      {
         assert(in && _out);
         char* ret;
#ifdef _WIN32
         ret = ::_fullpath(_out, in, PATHLIMIT);
#else
         // use realpath
         ret = realpath(in, _out);
#endif  
         if (!ret) {
            // preserve the input path, maybe someone else is able to fix
            // the path before it is accessed (e.g. our file system filter)
            //DefaultLogger::get()->warn("Invalid path: " + std::string(in));
            strcpy(_out, in);
         }
      }

      // ------------------------------------------------------------------------------------------------
      // FileSys's more specialized implementation
      bool ComparePaths(const char* one, const char* second)
      {
         // chances are quite good both paths are formatted identically,
         // so we can hopefully return here already
         if (!strcmp(one, second))
            return true;

         char temp1[PATHLIMIT];
         char temp2[PATHLIMIT];

         MakeAbsolutePath(one, temp1);
         MakeAbsolutePath(second, temp2);

         return !strcmp(temp1, temp2);
      }

      std::string GetFileName(const std::string &path)
      {
         std::string ret = path;
         std::size_t last =  ret.find_last_of("\\/");
         if (last != std::string::npos) ret = ret.substr(last + 1);
         return ret;
      }


      std::string GetCompleteBaseName(const std::string &path)
      {
         std::string ret = GetFileName(path);
         std::size_t pos = ret.find_last_of('.');
         if (pos != ret.npos) ret = ret.substr(0, pos);
         return ret;
      }


      std::string GetAbsolutePath(const std::string &path)
      {
         std::string ret = path;
         std::size_t last = ret.find_last_of("\\/");
         if (last != std::string::npos) ret = ret.substr(0, last);
         return ret;
      }

      bool HasExtension(const std::string &path,
         const char* ext0,
         const char* ext1,
         const char* ext2)
      {
         std::string::size_type pos = path.find_last_of('.');

         // no file extension - can't read
         if (pos == std::string::npos)
            return false;

         const char* ext_real = &path[pos + 1];
         if (!strcmp(ext_real, ext0))
            return true;

         // check for other, optional, file extensions
         if (ext1 && !strcmp(ext_real, ext1))
            return true;

         if (ext2 && !strcmp(ext_real, ext2))
            return true;

         return false;
      }

      std::string GetExtension(const std::string &path)
      {
         std::string::size_type pos = path.find_last_of('.');

         // no file extension at all
         if (pos == std::string::npos)
            return "";

         std::string ret = path.substr(pos + 1);
         std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower); // thanks to Andy Maloney for the hint
         return ret;
      }

      //bool SearchFileHeaderForToken(File* pFile,
      //   const std::string &pFileName,
      //   const char** tokens,
      //   uint32 numTokens,
      //   uint32 searchBytes /* = 200 */,
      //   bool tokensSol /* false */)
      //{
      //   assert(NULL != tokens && 0 != numTokens && 0 != searchBytes);
      //   if (!pFile)
      //      return false;

      //   //boost::scoped_ptr<IOStream> pStream(pFile->Open(pFileName));
      //   pFile->Open(pFileName);

      //   if (pStream.get())	{
      //      // read 200 characters from the file
      //      boost::scoped_array<char> _buffer(new char[searchBytes + 1 /* for the '\0' */]);
      //      char* buffer = _buffer.get();

      //      const uint32 read = pStream->Read(buffer, 1, searchBytes);
      //      if (!read)
      //         return false;

      //      for (uint32 i = 0; i < read; ++i)
      //         buffer[i] = ::tolower(buffer[i]);

      //      // It is not a proper handling of unicode files here ...
      //      // ehm ... but it works in most cases.
      //      char* cur = buffer, *cur2 = buffer, *end = &buffer[read];
      //      while (cur != end)	{
      //         if (*cur)
      //            *cur2++ = *cur;
      //         ++cur;
      //      }
      //      *cur2 = '\0';

      //      for (uint32 i = 0; i < numTokens; ++i)	{
      //         assert(NULL != tokens[i]);


      //         const char* r = strstr(buffer, tokens[i]);
      //         if (!r)
      //            continue;
      //         // We got a match, either we don't care where it is, or it happens to
      //         // be in the beginning of the file / line
      //         if (!tokensSol || r == buffer || r[-1] == '\r' || r[-1] == '\n') {
      //            //DefaultLogger::get()->debug(std::string("Found positive match for header keyword: ") + tokens[i]);
      //            return true;
      //         }
      //      }
      //   }
      //   return false;
      //}

   } // namespace filesys

} // namespace core

#undef PATHLIMIT