
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

#include <vector>
#include <map>

#include "mesh2.hpp"
using mesh2::aiPrimitiveType;

#include "core/string/string.hpp"
using core::string::String_c;

#include "core/math/vector2.hpp"
using core::math::Point2f;
using core::math::Vector2f;

#include "core/math/vector3.hpp"
using core::math::Point3f;
using core::math::Vector3f;

#include "ObjFile.hpp"
using objfile::Model;

namespace model
{
   //namespace objfile
   //{
   //   //struct Model;
   //   struct Object;
   //   struct Material;
   //   class Point3f;
   //   class Point2f;
   //}
   class ObjFileImporter;
   class IOSystem;

   ///	\class	ObjFileParser
   ///	\brief	Parser for a obj waveform file
   class ObjFileParser
   {
   public:
      static const size_t BUFFERSIZE = 4096;
      typedef std::vector<char> DataArray;
      typedef std::vector<char>::iterator DataArrayIt;
      typedef std::vector<char>::const_iterator ConstDataArrayIt;

   public:
      ///	\brief	Constructor with data array.
      ObjFileParser(std::vector<char> &Data, const String_c &strModelName, IOSystem* io);
      ///	\brief	Destructor
      ~ObjFileParser();
      ///	\brief	Model getter.
      objfile::Model *GetModel() const;

   private:
      ///	Parse the loaded file
      void parseFile();
      ///	Method to copy the new delimited word in the current line.
      void copyNextWord(char *pBuffer, size_t length);
      ///	Method to copy the new line.
      void copyNextLine(char *pBuffer, size_t length);
      /// Stores the vector 
      void getVector(std::vector<Vector3f> &point3d_array);
      ///	Stores the following 3d vector.
      void getVector3(std::vector<Vector3f> &point3d_array);
      ///	Stores the following 3d vector.
      void getVector2(std::vector<Vector2f> &point2d_array);
      ///	Stores the following face.
      void getFace(aiPrimitiveType type);
      /// Reads the material description.
      void getMaterialDesc();
      ///	Gets a comment.
      void getComment();
      /// Gets a a material library.
      void getMaterialLib();
      /// Creates a new material.
      void getNewMaterial();
      /// Gets the group name from file.
      void getGroupName();
      /// Gets the group number from file.
      void getGroupNumber();
      /// Gets the group number and resolution from file.
      void getGroupNumberAndResolution();
      /// Returns the index of the material. Is -1 if not material was found.
      int32 getMaterialIndex(const String_c &strMaterialName);
      /// Parse object name
      void getObjectName();
      /// Creates a new object.
      void createObject(const String_c &strObjectName);
      ///	Creates a new mesh.
      void createMesh();
      ///	Returns true, if a new mesh instance must be created.
      bool needsNewMesh(const String_c &rMaterialName);
      ///	Error report in token
      void reportErrorTokenInFace();

   private:
      ///	Default material name
      static const String_c DEFAULT_MATERIAL;
      //!	Iterator to current position in buffer
      DataArrayIt m_DataIt;
      //!	Iterator to end position of buffer
      DataArrayIt m_DataItEnd;
      //!	Pointer to model instance
      objfile::Model *m_pModel;
      //!	Current line (for debugging)
      uint32 m_uiLine;
      //!	Helper buffer
      char m_buffer[BUFFERSIZE];
      ///	Pointer to IO system instance.
      IOSystem *m_pIO;
   };

} //namespace model

#endif
