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

#ifndef _OBJIMPORTER_HPP_INCLUDED_
#define _OBJIMPORTER_HPP_INCLUDED_

#include "BaseImporter.h"
#include "../include/assimp/material.h"
#include <vector>

struct aiMesh;
struct aiNode;

namespace objimporter
{

  namespace ObjFile
  {
    struct Object;
    struct Model;
  }
  
  // ------------------------------------------------------------------------------------------------
  ///	\class	ObjFileImporter
  ///	\brief	Imports a waveform obj file
  // ------------------------------------------------------------------------------------------------
  class ObjFileImporter : public BaseImporter
  {
  private:
      //!	Data buffer
      std::vector<char> m_Buffer;
      //!	Pointer to root object instance
      ObjFile::Object *m_pRootObject;
      //!	Absolute pathname of model in file system
      String_c m_strAbsPath;
  private:
  
      //! \brief	Appends the supported extension.
      const aiImporterDesc* GetInfo () const;
  
      //!	\brief	File import implementation.
      void InternReadFile(const String_c &pFile, aiScene* pScene, IOSystem* pIOHandler);
      
      //!	\brief	Create the data from imported content.
      void CreateDataFromImport(const ObjFile::Model* pModel, aiScene* pScene);
      
      //!	\brief	Creates all nodes stored in imported content.
      aiNode *CreateNodes(const ObjFile::Model* pModel, const ObjFile::Object* pData,
          aiNode *pParent, aiScene* pScene, std::vector<aiMesh*> &MeshArray);
  
      //!	\brief	Creates topology data like faces and meshes for the geometry.
      aiMesh *CreateTopology( const ObjFile::Model* pModel, const ObjFile::Object* pData,
          uint32 uiMeshIndex );	
      
      //!	\brief	Creates vertices from model.
      void CreateVertexArray(const ObjFile::Model* pModel, const ObjFile::Object* pCurrentObject,
          uint32 uiMeshIndex, aiMesh* pMesh, uint32 numIndices );
  
      //!	\brief	Object counter helper method.
      void CountObjects(const std::vector<ObjFile::Object*> &rObjects, int32 &iNumMeshes);
  
      //!	\brief	Material creation.
      void CreateMaterials(const ObjFile::Model* pModel, aiScene* pScene);
      
      ///	@brief  Adds special property for the used texture mapping mode of the model.
      void AddTextureMappingModeProperty(aiMaterial* mat, aiTextureType type, int32 clampMode = 1);
  
      //!	\brief	Appends a child node to a parent node and updates the data structures.
      void AppendChildToParentNode(aiNode *pParent, aiNode *pChild);      
  public:
      ObjFileImporter();
      ~ObjFileImporter();
  
      /// \brief	Returns whether the class can handle the format of the given file. 
      /// \remark	See BaseImporter::CanRead() for details.
      bool CanRead( const String_c &pFile, IOSystem* pIOHandler, bool checkSig) const;
  };
  
  // ------------------------------------------------------------------------------------------------

} // namespace objimporter

#endif
