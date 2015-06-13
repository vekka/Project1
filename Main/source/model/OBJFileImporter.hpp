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
#ifndef _OBJFILEIMPORTER_HPP_INCLUDED_
#define _OBJFILEIMPORTER_HPP_INCLUDED_

#include <vector>
#include "mesh2.hpp"
struct mesh2::Mesh;
#include "../core/fileio/file.hpp"
using core::fileio::File;

#include "OBJFile.hpp"
#include "material.hpp"
#include "scene/scene.hpp"
#include "ImporterDesc.hpp"

namespace objfileimporter
{
   class ObjFileImporter
   {
   private:
      std::vector<char> m_pDataBuffer;
      objfile::Object *m_pRootObject;
      
      std::string m_strAbsPath; //	Absolute pathname of model in file system

      const eImporterDesc* GetInfo() const; // Appends the supported extension.

      // Create the data from imported content.
      void CreateDataFromImport(const objfile::Model* pModel, scene::Scene* pScene);

      // Creates all nodes stored in imported content.
      scene::Node *CreateNodes(const objfile::Model* pModel, const objfile::Object* pData,
         scene::Node *pParent, scene::Scene* pScene, std::vector<mesh2::Mesh*> &MeshArray);

      // Creates topology data like faces and meshes for the geometry.
      mesh2::Mesh *CreateTopology(const objfile::Model* pModel, const objfile::Object* pData,
         uint32 uiMeshIndex);

      void CreateVertexArrayFromModel(const objfile::Model* pModel, const objfile::Object* pCurrentObject,
         uint32 uiMeshIndex, mesh2::Mesh* pMesh, uint32 numIndices);

      // Object counter helper method.
      void CountObjects(const std::vector<objfile::Object*> &rObjects, int32 &iNumMeshes);

      void ObjFileImporter::AddTextureMappingModeProperty(material::Material* mat, material::eTextureType type, int32 clampMode = 1);

      //	Material creation.
      void CreateMaterials(const objfile::Model* pModel, scene::Scene* pScene);

      // Adds special property for the used m_texture mapping mode of the model.
      //void addTextureMappingModeProperty(Material* mat, aiTextureType type, int32 clampMode = 1);

      //	Appends a child node to a parent node and updates the data structures.
      void AppendChildToParentNode(scene::Node *pParent, scene::Node *pChild);

   public:
      ObjFileImporter();
      ~ObjFileImporter();
      // Returns whether the class can handle the format of the given file. 
      //	See BaseImporter::CanRead() for details.
      bool CanRead(const std::string &fileName, File* file, bool checkSig) const;
      //TODO: implement later, we need the scene.h code here
      void InternReadFile(const std::string &filePath, scene::Scene* pScene);
   };
} // namespace objfileimporter

#endif