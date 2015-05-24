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

#ifndef _FILESYS_HPP_INCLUDED_
#define _FILESYS_HPP_INCLUDED_

#include "file.hpp"
using core::fileio::File;

namespace core
{

   namespace filesys
   {
      bool Exists(const char* pFile);
      //char GetDirectorySeparator() const; // OS specific
      //File* Open(const char* pFile, const char* pMode = "rb");
      //void Close(File* pFile);
      bool ComparePaths(const char* one, const char* second);

      // get the file name of a full filepath
      // example: /tmp/archive.tar.gz -> archive.tar.gz
      std::string GetFileName(const std::string &path);

      // get the complete base name of a full filepath
      // example: /tmp/archive.tar.gz -> archive.tar
      std::string GetCompleteBaseName(const std::string &path);

      // get the path of a full filepath
      // example: /tmp/archive.tar.gz -> /tmp/
      std::string GetAbsolutePath(const std::string &path);

      bool HasExtension(const std::string &path,
         const char* ext0,
         const char* ext1 = NULL,
         const char* ext2 = NULL);

      std::string GetExtension(const std::string &path);
   } // namespace filesys

} // namespace core

#endif