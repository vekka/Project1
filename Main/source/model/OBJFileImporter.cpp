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

//#ifndef ASSIMP_BUILD_NO_OBJ_IMPORTER

//#include "FileSys.h"
#include "OBJFileImporter.hpp"
#include "OBJParser.hpp"
using model::objparser::ObjParser;

using mesh2::Face;

//#include "materialSystem.hpp"

#include "../core/memory/pointer.hpp"
using core::pointer::ScopedPtr;

using scene::Node;
using material::SHADING_MODE_FLAT;
using material::SHADING_MODE_NOSHADING;
using material::SHADING_MODE_GOURAUD;
using material::SHADING_MODE_PHONG;

using mesh2::PRIMITIVE_TYPE_LINE;
using mesh2::PRIMITIVE_TYPE_POLYGON;
using mesh2::PRIMITIVE_TYPE_POINT;
using mesh2::PRIMITIVE_TYPE_TRIANGLE;
//#include "../include/assimp/assert.h"
//#include "../include/assimp/DefaultLogger.hpp"

#include "../core/fileio/filesys.hpp"
using core::filesys::HasExtension;

namespace objfileimporter
{
   using namespace std;

   static const eImporterDesc desc = {
      "Wavefront Object Importer",
      "",
      "",
      "surfaces not supported",
      IMPORTERFLAGS_SUPPORT_TEXT_FLAVOUR,
      0,
      0,
      0,
      0,
      "obj"
   };

   static const uint32 ObjMinSize = 16;

   ObjFileImporter::ObjFileImporter() :
      m_pDataBuffer(),
      m_pRootObject(NULL),
      m_strAbsPath("")
   {
      //FileSys filesys;
      m_strAbsPath = '/'; //= io.getOsSeparator();
   }

   ObjFileImporter::~ObjFileImporter()
   {
      delete m_pRootObject;
      m_pRootObject = NULL;
   }

   bool ObjFileImporter::CanRead(const std::string &fileName, File* pFile, bool checkSig) const
   {
      if (!checkSig) //Check File Extension
      {
         return HasExtension(fileName, "obj");
      }
      else //Check file Header
      {
         //static const char *pTokens[] = { "mtllib", "usemtl", "v ", "vt ", "vn ", "o ", "g ", "s ", "f " };
         //return SearchFileHeaderForToken(pFile, fileName, pTokens, 9);
         return true;
      }
   }

   const eImporterDesc* ObjFileImporter::GetInfo() const
   {
      return &desc;
   }

   //	Obj-file import implementation
   void ObjFileImporter::InternReadFile(const std::string &pFileName, scene::Scene* pScene)
   {
     
      //pFile->Open(pFileName, true);
      //ScopedPtr<File> file(pFileName);

      File *file = new File();

      file->Open(pFileName, true);
      //if (!file->Get()) {
      //   //throw DeadlyImportError("Failed to open file " + pFile + ".");
      //}

      // Get the file-size and validate it, throwing an exception when fails
      size_t fileSize = file->GetSize();
      if (fileSize < ObjMinSize) {
         //throw DeadlyImportError("OBJ-file is too small.");
      }

      // Allocate buffer and read file into it
      //TextFileToBuffer(file.Get(), m_pDataBuffer);
      file->CopyToBuffer(m_pDataBuffer);

      // Get the model name
      std::string strModelName;
      std::string::size_type pos = pFileName.find_last_of("\\/");
      if (pos != std::string::npos)
      {
         strModelName = pFileName.substr(pos + 1, pFileName.size() - pos - 1);
      }
      else
      {
         strModelName = pFileName;
      }

      // process all '\'
      std::vector<char> ::iterator iter = m_pDataBuffer.begin();
      while (iter != m_pDataBuffer.end())
      {
         if (*iter == '\\')
         {
            // remove '\'
            iter = m_pDataBuffer.erase(iter);
            // remove next character
            while (*iter == '\r' || *iter == '\n')
               iter = m_pDataBuffer.erase(iter);
         }
         else
            ++iter;
      }

      // parse the file into a temporary representation
      ObjParser parser(m_pDataBuffer, strModelName, file);

      // And create the proper return structures out of it
      CreateDataFromImport(parser.GetModel(), pScene);

      // Clean up allocated storage for the next import 
      m_pDataBuffer.clear();
      file->Close();
   }

   //	Create the data from parsed obj-file
   void ObjFileImporter::CreateDataFromImport(const objfile::Model* pModel, scene::Scene* pScene) {
      if (0L == pModel) {
         return;
      }

      // Create the root node of the scene
      pScene->m_pRootNode = new Node;
      if (!pModel->m_modelName.empty())
      {
         // Set the name of the scene
         pScene->m_pRootNode->m_name = pModel->m_modelName;
      }
      else
      {
         // This is a fatal error, so break down the application
         assert(false);
      }

      // Create nodes for the whole scene	
      std::vector<Mesh*> MeshArray;
      for (size_t index = 0; index < pModel->m_objects.size(); index++)
      {
         CreateNodes(pModel, pModel->m_objects[index], pScene->m_pRootNode, pScene, MeshArray);
      }

      // Create mesh pointer buffer for this scene
      if (pScene->m_numMeshes > 0)
      {
         pScene->m_ppMeshes = new Mesh*[MeshArray.size()];
         for (size_t index = 0; index < MeshArray.size(); index++)
         {
            pScene->m_ppMeshes[index] = MeshArray[index];
         }
      }

      CreateMaterials(pModel, pScene); // Create all materials
   }

   //	Creates all nodes of the model
   Node *ObjFileImporter::CreateNodes(const objfile::Model* pModel, const objfile::Object* pObject,
      Node *pParent, scene::Scene* pScene, std::vector<Mesh*> &MeshArray)
   {
      assert(NULL != pModel);
      if (NULL == pObject) {
         return NULL;
      }

      // Store older mesh size to be able to computes mesh offsets for new mesh instances
      const size_t oldMeshSize = MeshArray.size();
      Node *pNode = new Node;

      pNode->m_name = pObject->m_strObjName;

      // If we have a parent node, store it
      if (pParent != NULL) {
         AppendChildToParentNode(pParent, pNode);
      }

      for (size_t i = 0; i< pObject->m_meshes.size(); i++)
      {
         uint32 meshId = pObject->m_meshes[i];
         Mesh *pMesh = CreateTopology(pModel, pObject, meshId);
         if (pMesh && pMesh->m_numFaces > 0) {
            MeshArray.push_back(pMesh);
         }
      }

      // Create all nodes from the sub-objects stored in the current object
      if (!pObject->m_SubObjects.empty())
      {
         size_t numChilds = pObject->m_SubObjects.size();
         pNode->m_numChildren = static_cast<uint32>(numChilds);
         pNode->m_ppChildren = new Node*[numChilds];
         pNode->m_numMeshes = 1;
         pNode->m_ppMeshes = new uint32[1];
      }

      // Set mesh instances into scene- and node-instances
      const size_t meshSizeDiff = MeshArray.size() - oldMeshSize;
      if (meshSizeDiff > 0)
      {
         pNode->m_ppMeshes = new uint32[meshSizeDiff];
         pNode->m_numMeshes = static_cast<uint32>(meshSizeDiff);
         size_t index = 0;
         for (size_t i = oldMeshSize; i < MeshArray.size(); i++)
         {
            pNode->m_ppMeshes[index] = pScene->m_numMeshes;
            pScene->m_numMeshes++;
            index++;
         }
      }

      return pNode;
   }

   Mesh *ObjFileImporter::CreateTopology(const objfile::Model* pModel, const objfile::Object* pData,
      uint32 meshIndex)
   {
      // Checking preconditions
      assert(NULL != pModel);

      if (NULL == pData) {
         return NULL;
      }

      // Create faces
      objfile::Mesh *pObjMesh = pModel->m_meshes[meshIndex];
      if (!pObjMesh) {
         return NULL;
      }
      assert(NULL != pObjMesh);
      Mesh* pMesh = new Mesh;
      for (size_t index = 0; index < pObjMesh->m_faces.size(); index++)
      {
         objfile::ObjFace *const inp = pObjMesh->m_faces[index];
         assert(NULL != inp);

         if (inp->m_primitiveType == PRIMITIVE_TYPE_LINE) {
            pMesh->m_numFaces += inp->m_pVertexIndices->size() - 1;
            pMesh->m_primitiveTypes |= PRIMITIVE_TYPE_LINE;
         }
         else if (inp->m_primitiveType == PRIMITIVE_TYPE_POINT) {
            pMesh->m_numFaces += inp->m_pVertexIndices->size();
            pMesh->m_primitiveTypes |= PRIMITIVE_TYPE_POINT;
         }
         else {
            ++pMesh->m_numFaces;
            if (inp->m_pVertexIndices->size() > 3) {
               pMesh->m_primitiveTypes |= PRIMITIVE_TYPE_POLYGON;
            }
            else {
               pMesh->m_primitiveTypes |= PRIMITIVE_TYPE_POLYGON;
            }
         }
      }

      uint32 uiIdxCount(0u);
      if (pMesh->m_numFaces > 0)
      {
         pMesh->m_pFaces = new mesh2::Face[pMesh->m_numFaces];
         if (pObjMesh->m_uiMaterialIndex != objfile::Mesh::NoMaterial)
         {
            pMesh->m_materialIndex = pObjMesh->m_uiMaterialIndex;
         }

         uint32 outIndex(0);

         // Copy all data from all stored meshes
         for (size_t index = 0; index < pObjMesh->m_faces.size(); index++)
         {
            objfile::ObjFace* const inp = pObjMesh->m_faces[index];
            if (inp->m_primitiveType == PRIMITIVE_TYPE_LINE) {
               for (size_t i = 0; i < inp->m_pVertexIndices->size() - 1; ++i) {
                  mesh2::Face &f = pMesh->m_pFaces[outIndex++];
                  uiIdxCount += f.m_numIndices = 2;
                  f.m_pIndexArray = new uint32[2];
               }
               continue;
            }
            else if (inp->m_primitiveType == PRIMITIVE_TYPE_POINT) {
               for (size_t i = 0; i < inp->m_pVertexIndices->size(); ++i) {
                  mesh2::Face &f = pMesh->m_pFaces[outIndex++];
                  uiIdxCount += f.m_numIndices = 1;
                  f.m_pIndexArray = new uint32[1];
               }
               continue;
            }

            mesh2::Face *pFace = &pMesh->m_pFaces[outIndex++];
            const uint32 uiNumIndices = (uint32)pObjMesh->m_faces[index]->m_pVertexIndices->size();
            uiIdxCount += pFace->m_numIndices = (uint32)uiNumIndices;
            if (pFace->m_numIndices > 0) {
               pFace->m_pIndexArray = new uint32[uiNumIndices];
            }
         }
      }

      // Create mesh vertices
      CreateVertexArrayFromModel(pModel, pData, meshIndex, pMesh, uiIdxCount);

      return pMesh;
   }

   void ObjFileImporter::CreateVertexArrayFromModel(const objfile::Model* pModel,
      const objfile::Object* pCurrentObject,
      uint32 uiMeshIndex,
      Mesh* pMesh,
      uint32 numIndices)
   {
      assert(NULL != pCurrentObject);

      // Break, if no faces are stored in object
      if (pCurrentObject->m_meshes.empty())
         return;

      // Get current mesh
      objfile::Mesh *pObjMesh = pModel->m_meshes[uiMeshIndex];
      if (NULL == pObjMesh || pObjMesh->m_uiNumIndices < 1)
         return;

      // Copy vertices of this mesh instance
      pMesh->m_numVertices = numIndices;
      pMesh->m_pVertices = new Vector3f[pMesh->m_numVertices];

      // Allocate buffer for normal vectors
      if (!pModel->m_pNormals.empty() && pObjMesh->m_hasNormals)
         pMesh->m_pNormals = new Vector3f[pMesh->m_numVertices];

      // Allocate buffer for m_texture coordinates
      if (!pModel->m_textureCoord.empty() && pObjMesh->m_uiUVCoordinates[0])
      {
         pMesh->m_numUVComponents[0] = 2;
         pMesh->m_pTextureCoords[0] = new Vector3f[pMesh->m_numVertices];
      }

      // Copy vertices, normals and textures into Mesh instance
      uint32 newIndex = 0, outIndex = 0;
      for (size_t index = 0; index < pObjMesh->m_faces.size(); index++)
      {
         // Get source face
         objfile::ObjFace *pSourceFace = pObjMesh->m_faces[index];

         // Copy all index arrays
         for (size_t vertexIndex = 0, outVertexIndex = 0; vertexIndex < pSourceFace->m_pVertexIndices->size(); vertexIndex++)
         {
            const uint32 vertex = pSourceFace->m_pVertexIndices->at(vertexIndex);

            // Where is DeadlyImportError?
       /*     if (vertex >= pModel->m_pVertices.size())
               throw DeadlyImportError("OBJ: vertex index out of range");*/

            pMesh->m_pVertices[newIndex] = pModel->m_pVertices[vertex];

            // Copy all normals 
            if (!pModel->m_pNormals.empty() && vertexIndex < pSourceFace->m_pNormalIndices->size())
            {
               const uint32 normal = pSourceFace->m_pNormalIndices->at(vertexIndex);
  /*             if (normal >= pModel->m_pNormals.size())
                  throw DeadlyImportError("OBJ: vertex normal index out of range");*/

               pMesh->m_pNormals[newIndex] = pModel->m_pNormals[normal];
            }

            // Copy all m_texture coordinates
            if (!pModel->m_textureCoord.empty() && vertexIndex < pSourceFace->m_pTexCoordIndices->size())
            {
               const uint32 tex = pSourceFace->m_pTexCoordIndices->at(vertexIndex);
               assert(tex < pModel->m_textureCoord.size());

               //if (tex >= pModel->m_textureCoord.size())
               //   throw DeadlyImportError("OBJ: m_texture coordinate index out of range");

               const Vector3f &coord3d = pModel->m_textureCoord[tex];
               pMesh->m_pTextureCoords[0][newIndex] = Vector3f(coord3d[0], coord3d[1], coord3d[2]);
            }

            assert(pMesh->m_numVertices > newIndex);

            // Get destination face
            mesh2::Face *pDestFace = &pMesh->m_pFaces[outIndex];

            const bool last = (vertexIndex == pSourceFace->m_pVertexIndices->size() - 1);
            if (pSourceFace->m_primitiveType != PRIMITIVE_TYPE_LINE || !last)
            {
               pDestFace->m_pIndexArray[outVertexIndex] = newIndex;
               outVertexIndex++;
            }

            if (pSourceFace->m_primitiveType == PRIMITIVE_TYPE_POINT)
            {
               outIndex++;
               outVertexIndex = 0;
            }
            else if (pSourceFace->m_primitiveType == PRIMITIVE_TYPE_LINE)
            {
               outVertexIndex = 0;

               if (!last)
                  outIndex++;

               if (vertexIndex) {
                  if (!last) {
                     pMesh->m_pVertices[newIndex + 1] = pMesh->m_pVertices[newIndex];
                     if (!pSourceFace->m_pNormalIndices->empty() && !pModel->m_pNormals.empty()) {
                        pMesh->m_pNormals[newIndex + 1] = pMesh->m_pNormals[newIndex];
                     }
                     if (!pModel->m_textureCoord.empty()) {
                        for (size_t i = 0; i < pMesh->GetNumUVChannels(); i++) {
                           pMesh->m_pTextureCoords[i][newIndex + 1] = pMesh->m_pTextureCoords[i][newIndex];
                        }
                     }
                     ++newIndex;
                  }

                  pDestFace[-1].m_pIndexArray[1] = newIndex;
               }
            }
            else if (last) {
               outIndex++;
            }
            ++newIndex;
         }
      }
   }

   //	Counts all stored meshes 
   void ObjFileImporter::CountObjects(const std::vector<objfile::Object*> &rObjects, int32 &iNumMeshes)
   {
      iNumMeshes = 0;
      if (rObjects.empty())
         return;

      iNumMeshes += static_cast<uint32>(rObjects.size());
      for (std::vector<objfile::Object*>::const_iterator it = rObjects.begin();
         it != rObjects.end();
         ++it)
      {
         if (!(*it)->m_SubObjects.empty())
         {
            CountObjects((*it)->m_SubObjects, iNumMeshes);
         }
      }
   }

   //	 Add m_clamp mode property to material if necessary 
   void ObjFileImporter::AddTextureMappingModeProperty(material::Material* mat, material::eTextureType type, int32 clampMode)
   {
      assert(NULL != mat);
      mat->AddProperty<int32>(&clampMode, 1, material::Material::KEYNAME_MAPPINGMODE_U_BASE, type, 0);
      mat->AddProperty<int32>(&clampMode, 1, material::Material::KEYNAME_MAPPINGMODE_V_BASE, type, 0);
   }

   //	Creates the material 
   void ObjFileImporter::CreateMaterials(const objfile::Model* pModel, scene::Scene* pScene)
   {
      assert(NULL != pScene);
      if (NULL == pScene)
         return;

      const uint32 numMaterials = (uint32)pModel->m_materialLib.size();
      pScene->m_numMaterials = 0;
      //if (pModel->m_materialLib.empty()) {
      //   DefaultLogger::get()->debug("OBJ: no materials specified");
      //   return;
      //}

      pScene->m_ppMaterials = new material::Material*[numMaterials];
      for (uint32 matIndex = 0; matIndex < numMaterials; matIndex++)
      {
         // Store material name
         std::map<std::string, objfile::ObjMaterial*>::const_iterator it;
         it = pModel->m_materialMap.find(pModel->m_materialLib[matIndex]);

         // No material found, use the default material
         if (pModel->m_materialMap.end() == it)
            continue;

         material::Material* mat = new material::Material;
         objfile::ObjMaterial *pCurrentMaterial = (*it).second;

         mat->AddProperty(pCurrentMaterial->m_materialName, material::Material::KEY_NAME);

         // convert illumination model
         int32 sm = 0;
         switch (pCurrentMaterial->m_illuminationModel)
         {
         case 0:
            sm = SHADING_MODE_NOSHADING;
            break;
         case 1:
            sm = SHADING_MODE_GOURAUD;
            break;
         case 2:
            sm = SHADING_MODE_PHONG;
            break;
         default:
            sm = SHADING_MODE_GOURAUD;
         //   DefaultLogger::get()->error("OBJ: unexpected illumination model (0-2 recognized)");
         }

         mat->AddProperty<int32>(&sm, 1, material::Material::KEY_SHADING_MODEL);

         // multiplying the specular exponent with 2 seems to yield better results
         pCurrentMaterial->m_shineness *= 4.f;
         
         // Adding material colors 
         mat->AddProperty(&pCurrentMaterial->m_ambientColor, 1, material::Material::KEY_COLOR_AMBIENT);
         mat->AddProperty(&pCurrentMaterial->m_diffuseColor, 1, material::Material::KEY_COLOR_DIFFUSE);
         mat->AddProperty(&pCurrentMaterial->m_specularColor, 1, material::Material::KEY_COLOR_SPECULAR);
         mat->AddProperty(&pCurrentMaterial->m_emissiveColor, 1, material::Material::KEY_COLOR_EMISSIVE);
         mat->AddProperty(&pCurrentMaterial->m_shineness, 1, material::Material::KEY_SHININESS);
         mat->AddProperty(&pCurrentMaterial->m_alpha, 1, material::Material::KEY_OPACITY);

         // Adding refraction index
         mat->AddProperty(&pCurrentMaterial->m_indexOfRefraction, 1, material::Material::KEY_REFRACTI);

         // Adding textures
         if (0 != pCurrentMaterial->m_texture.size())
         {
            mat->AddProperty(pCurrentMaterial->m_texture, material::Material::KEYNAME_TEXTURE_BASE, material::TEXTURE_TYPE_DIFFUSE, 0);
            if (pCurrentMaterial->m_clamp[objfile::ObjMaterial::TEXTURE_TYPE_DIFFUSE])
            {
               AddTextureMappingModeProperty(mat, material::TEXTURE_TYPE_DIFFUSE);
            }
         }

         if (0 != pCurrentMaterial->m_textureAmbient.size())
         {
            mat->AddProperty(pCurrentMaterial->m_textureAmbient, material::Material::KEYNAME_TEXTURE_BASE, material::TEXTURE_TYPE_AMBIENT, 0);
            if (pCurrentMaterial->m_clamp[objfile::ObjMaterial::TEXTURE_TYPE_AMBIENT])
            {
               AddTextureMappingModeProperty(mat, material::TEXTURE_TYPE_AMBIENT);
            }
         }

         if (0 != pCurrentMaterial->m_textureEmissive.size())
            mat->AddProperty(pCurrentMaterial->m_textureEmissive, material::Material::KEYNAME_TEXTURE_BASE, material::TEXTURE_TYPE_EMISSIVE, 0);

         if (0 != pCurrentMaterial->textureSpecular.size())
         {
            mat->AddProperty(pCurrentMaterial->textureSpecular, material::Material::KEYNAME_TEXTURE_BASE, material::TEXTURE_TYPE_SPECULAR, 0);
            if (pCurrentMaterial->m_clamp[objfile::ObjMaterial::TEXTURE_TYPE_SPECULAR])
            {
               AddTextureMappingModeProperty(mat, material::TEXTURE_TYPE_SPECULAR);
            }
         }

         if (0 != pCurrentMaterial->m_textureBump.size())
         {
            mat->AddProperty(pCurrentMaterial->m_textureBump, material::Material::KEYNAME_TEXTURE_BASE, material::TEXTURE_TYPE_HEIGHTMAP, 0);
            if (pCurrentMaterial->m_clamp[objfile::ObjMaterial::TEXTURE_TYPE_BUMP])
            {
               AddTextureMappingModeProperty(mat, material::TEXTURE_TYPE_HEIGHTMAP);
            }
         }

         if (0 != pCurrentMaterial->m_textureNormal.size())
         {
            mat->AddProperty(pCurrentMaterial->m_textureNormal, material::Material::KEYNAME_TEXTURE_BASE, material::TEXTURE_TYPE_NORMALS, 0);
            if (pCurrentMaterial->m_clamp[objfile::ObjMaterial::TEXTURE_TYPE_NORMALS])
            {
               AddTextureMappingModeProperty(mat, material::TEXTURE_TYPE_NORMALS);
            }
         }

         if (0 != pCurrentMaterial->m_textureDisplacement.size())
         {
            mat->AddProperty(pCurrentMaterial->m_textureDisplacement, material::Material::KEYNAME_TEXTURE_BASE, material::TEXTURE_TYPE_DISPLACEMENT, 0);
            if (pCurrentMaterial->m_clamp[objfile::ObjMaterial::TEXTURE_TYPE_DISPLACEMENT])
            {
               AddTextureMappingModeProperty(mat, material::TEXTURE_TYPE_DISPLACEMENT);
            }
         }

         if (0 != pCurrentMaterial->m_textureOpacity.size())
         {
            mat->AddProperty(pCurrentMaterial->m_textureOpacity, material::Material::KEYNAME_TEXTURE_BASE, material::TEXTURE_TYPE_OPACITY, 0);
            if (pCurrentMaterial->m_clamp[objfile::ObjMaterial::TEXTURE_TYPE_OPACITY])
            {
               AddTextureMappingModeProperty(mat, material::TEXTURE_TYPE_OPACITY);
            }
         }

         if (0 != pCurrentMaterial->m_textureSpecularity.size())
         {
            mat->AddProperty(pCurrentMaterial->m_textureSpecularity, material::Material::KEYNAME_TEXTURE_BASE, material::TEXTURE_TYPE_SHININESS, 0);
            if (pCurrentMaterial->m_clamp[objfile::ObjMaterial::TEXTURE_TYPE_SHININESS])
            {
               AddTextureMappingModeProperty(mat, material::TEXTURE_TYPE_SHININESS);
            }
         }

         // Store material property info in material array in scene
         pScene->m_ppMaterials[pScene->m_numMaterials] = mat;
         pScene->m_numMaterials++;
      }

      // Test number of created materials.
      assert(pScene->m_numMaterials == numMaterials);
   }

   //	Appends this node to the parent node
   void ObjFileImporter::AppendChildToParentNode(Node *pParent, Node *pChild)
   {
      // Checking preconditions
      assert(NULL != pParent);
      assert(NULL != pChild);

      // Assign parent to child
      pChild->m_pParentNode = pParent;

      // If already children was assigned to the parent node, store them in a 
      std::vector<Node*> temp;
      if (pParent->m_ppChildren != NULL)
      {
         assert(0 != pParent->m_numChildren);
         for (size_t index = 0; index < pParent->m_numChildren; index++)
         {
            temp.push_back(pParent->m_ppChildren[index]);
         }
         delete[] pParent->m_ppChildren;
      }

      // Copy node instances into parent node
      pParent->m_numChildren++;
      pParent->m_ppChildren = new Node*[pParent->m_numChildren];
      for (size_t index = 0; index < pParent->m_numChildren - 1; index++)
      {
         pParent->m_ppChildren[index] = temp[index];
      }
      pParent->m_ppChildren[pParent->m_numChildren - 1] = pChild;
   }

}	// namespace objfileimporter

//#endif // !! ASSIMP_BUILD_NO_OBJ_IMPORTER