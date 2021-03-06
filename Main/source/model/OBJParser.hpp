
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
#ifndef _OBJPARSER_HPP_INCLUDED_
#define _OBJPARSER_HPP_INCLUDED_

// parser for a obj waveform file

#include <vector>
#include <map>

#include "mesh2.hpp"
using mesh2::ePrimitiveType;

//#include "core/string/string.hpp"
//using core::string::String_c;

#include "core/math/vector2.hpp"
using core::math::Point2f;
using core::math::Vector2f;

#include "core/math/vector3.hpp"
using core::math::Point3f;
using core::math::Vector3f;

#include "core/fileio/file.hpp"
using core::fileio::File;

#include "mesh2.hpp"

#include "OBJFile.hpp"

namespace model
{
  
  namespace objparser
  {
       //namespace objfile
       //{
       //   //struct Model;
       //   struct Object;
       //   struct Material;
       //   class Point3f;
       //   class Point2f;
       //}
       //class ObjFileImporter;
       //class File;
    
       class ObjParser
       {
       public:
          static const size_t BUFFERSIZE = 4096;
          static const std::string DEFAULT_MATERIAL_NAME;

          typedef std::vector<char> DataArray_t;
          typedef std::vector<char>::iterator DataArrayIterator_t;
          typedef std::vector<char>::const_iterator ConstDataArrayIterator_t;
       private:
          //DataArrayIterator_t m_dataIterator;
          ConstDataArrayIterator_t m_dataIterator;
          //DataArrayIterator_t m_dataIteratorEndOfBuffer;
          ConstDataArrayIterator_t m_dataIteratorEndOfBuffer;
          objfile::Model *m_pModelInstance;
          uint32 m_currentLine;

          //std::vector<char> m_data;
          
          // helper buffer
          char m_buffer[BUFFERSIZE];
          
          File *m_file;

          //	Parse the loaded file
          void ParseFile();
          //	Method to copy the new delimited word in the current line.
          void CopyNextWord(char *pBuffer, size_t length);
          //	Method to copy the new line.
          void CopyNextLine(char *pBuffer, size_t length);
          // Stores the vector 
          void GetVector(std::vector<Vector3f> &point3d_array);
          //	Stores the following 3d vector.
          void GetVector3(std::vector<Vector3f> &point3d_array);
          //	Stores the following 3d vector.
          void GetVector2(std::vector<Vector2f> &point2d_array);
          //	Stores the following face.
          void GetFace(ePrimitiveType type);
          // Reads the material description.
          void GetMaterialDesc();
          //	Gets a comment.
          void GetComment();
          // Gets a a material library.
          void GetMaterialLib();
          // Creates a new material.
          void GetNewMaterial();
          // Gets the group name from file.
          void GetGroupName();
          // Gets the group number from file.
          void GetGroupNumber();
          // Gets the group number and resolution from file.
          void GetGroupNumberAndResolution();
          // Returns the index of the material. Is -1 if not material was found.
          int32 GetMaterialIndex(const std::string &strMaterialName);
          // Parse object name
          void GetObjectName();
          // Creates a new object.
          void CreateObject(const std::string &strObjectName);
          //	Creates a new mesh.
          void CreateMesh();
          //	Returns true, if a new mesh instance must be created.
          bool NeedsNewMesh(const std::string &rMaterialName);
          //	Error report in token
          void ReportErrorTokenInFace();
       public:

          ObjParser::ObjParser(std::vector<char> &data, const std::string &strModelName, const File *file);

          ~ObjParser();
          objfile::Model *GetModel() const;
      };
       
    } // namespace objparser

} //namespace model

#endif
