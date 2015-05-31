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


#ifndef ASSIMP_BUILD_NO_OBJ_IMPORTER

//#include "FileSys.h"
#include "OBJFileImporter.hpp"
#include "OBJParser.hpp"
using model::objparser::ObjParser;

#include "OBJFile.hpp"
using objfile::Material;

#include "material.inl"


using mesh2::Face;

#include "../core/memory/scopedptr.hpp"
using core::memory::ScopedPtr;

//#include "../include/assimp/Importer.hpp"
#include "../scene/scene.hpp"

#include "ImporterDesc.hpp"

using scene::Node;
using mesh2::PRIMITIVE_TYPE_LINE;
using mesh2::PRIMITIVE_TYPE_POLYGON;
using mesh2::PRIMITIVE_TYPE_POINT;
using mesh2::PRIMITIVE_TYPE_TRIANGLE;
//#include "../include/assimp/assert.h"
//#include "../include/assimp/DefaultLogger.hpp"

#include "../core/fileio/filesys.hpp"
using core::filesys::HasExtension;
//using core::filesys::

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

namespace objfileimporter
{

   using namespace std;

   ObjFileImporter::ObjFileImporter() :
      m_pBuffer(),
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
   void ObjFileImporter::InternReadFile(const std::string &pFileName, Scene* pScene, File* pFile)
   {
     
      pFile->Open(pFileName, true);
      ScopedPtr<File> file(pFile);
      if (!file.Get()) {
         //throw DeadlyImportError("Failed to open file " + pFile + ".");
      }

      // Get the file-size and validate it, throwing an exception when fails
      size_t fileSize = file->GetSize();
      if (fileSize < ObjMinSize) {
         //throw DeadlyImportError("OBJ-file is too small.");
      }

      // Allocate buffer and read file into it
      //TextFileToBuffer(file.Get(), m_pBuffer);
      file->CopyToBuffer(m_pBuffer);

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
      std::vector<char> ::iterator iter = m_pBuffer.begin();
      while (iter != m_pBuffer.end())
      {
         if (*iter == '\\')
         {
            // remove '\'
            iter = m_pBuffer.erase(iter);
            // remove next character
            while (*iter == '\r' || *iter == '\n')
               iter = m_pBuffer.erase(iter);
         }
         else
            ++iter;
      }

      // parse the file into a temporary representation
      ObjParser parser(m_pBuffer, strModelName, pFile);

      // And create the proper return structures out of it
      CreateDataFromImport(parser.GetModel(), pScene);

      // Clean up allocated storage for the next import 
      m_pBuffer.clear();
   }

   // ------------------------------------------------------------------------------------------------
   //	Create the data from parsed obj-file
   void ObjFileImporter::CreateDataFromImport(const objfile::Model* pModel, Scene* pScene) {
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

      // Create all materials
      CreateMaterials(pModel, pScene);
   }

   // ------------------------------------------------------------------------------------------------
   //	Creates all nodes of the model
   Node *ObjFileImporter::CreateNodes(const objfile::Model* pModel, const objfile::Object* pObject,
      Node *pParent, Scene* pScene,
      std::vector<Mesh*> &MeshArray)
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

   // ------------------------------------------------------------------------------------------------
   //	Create topology data
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
         objfile::Face *const inp = pObjMesh->m_faces[index];
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
         pMesh->m_pFaces = new Face[pMesh->m_numFaces];
         if (pObjMesh->m_uiMaterialIndex != objfile::Mesh::NoMaterial)
         {
            pMesh->m_materialIndex = pObjMesh->m_uiMaterialIndex;
         }

         uint32 outIndex(0);

         // Copy all data from all stored meshes
         for (size_t index = 0; index < pObjMesh->m_faces.size(); index++)
         {
            objfile::Face* const inp = pObjMesh->m_faces[index];
            if (inp->m_primitiveType == PRIMITIVE_TYPE_LINE) {
               for (size_t i = 0; i < inp->m_pVertexIndices->size() - 1; ++i) {
                  Face& f = pMesh->m_pFaces[outIndex++];
                  uiIdxCount += f.m_numIndices = 2;
                  f.m_pIndexArray = new uint32[2];
               }
               continue;
            }
            else if (inp->m_primitiveType == PRIMITIVE_TYPE_POINT) {
               for (size_t i = 0; i < inp->m_pVertexIndices->size(); ++i) {
                  Face& f = pMesh->m_pFaces[outIndex++];
                  uiIdxCount += f.m_numIndices = 1;
                  f.m_pIndexArray = new uint32[1];
               }
               continue;
            }

            Face *pFace = &pMesh->m_pFaces[outIndex++];
            const uint32 uiNumIndices = (uint32)pObjMesh->m_faces[index]->m_pVertexIndices->size();
            uiIdxCount += pFace->m_numIndices = (uint32)uiNumIndices;
            if (pFace->m_numIndices > 0) {
               pFace->m_pIndexArray = new uint32[uiNumIndices];
            }
         }
      }

      // Create mesh vertices
      CreateVertexArray(pModel, pData, meshIndex, pMesh, uiIdxCount);

      return pMesh;
   }

   // ------------------------------------------------------------------------------------------------
   //	Creates a vertex array
   void ObjFileImporter::CreateVertexArray(const objfile::Model* pModel,
      const objfile::Object* pCurrentObject,
      uint32 uiMeshIndex,
      Mesh* pMesh,
      uint32 numIndices)
   {
      // Checking preconditions
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

      // Allocate buffer for texture coordinates
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
         objfile::Face *pSourceFace = pObjMesh->m_faces[index];

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

            // Copy all texture coordinates
            if (!pModel->m_textureCoord.empty() && vertexIndex < pSourceFace->m_pTexCoordIndices->size())
            {
               const uint32 tex = pSourceFace->m_pTexCoordIndices->at(vertexIndex);
               assert(tex < pModel->m_textureCoord.size());

               //if (tex >= pModel->m_textureCoord.size())
               //   throw DeadlyImportError("OBJ: texture coordinate index out of range");

               const Vector3f &coord3d = pModel->m_textureCoord[tex];
               pMesh->m_pTextureCoords[0][newIndex] = Vector3f(coord3d[0], coord3d[1], coord3d[2]);
            }

            assert(pMesh->m_numVertices > newIndex);

            // Get destination face
            Face *pDestFace = &pMesh->m_pFaces[outIndex];

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

   // ------------------------------------------------------------------------------------------------
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

   // this member is missing ....addTextureMappingModeProperty

   // ------------------------------------------------------------------------------------------------
   //	 Add clamp mode property to material if necessary 
   //void ObjFileImporter::addTextureMappingModeProperty(aiMaterial* mat, aiTextureType type, int32 clampMode)
   //{
   //   assert(NULL != mat);
   //   mat->AddProperty<int32>(&clampMode, 1, AI_MATKEY_MAPPINGMODE_U(type, 0));
   //   mat->AddProperty<int32>(&clampMode, 1, AI_MATKEY_MAPPINGMODE_V(type, 0));
   //}

   // ------------------------------------------------------------------------------------------------
   //	Creates the material 
   void ObjFileImporter::CreateMaterials(const objfile::Model* pModel, Scene* pScene)
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

      pScene->m_ppMaterials = new Material*[numMaterials];
      for (uint32 matIndex = 0; matIndex < numMaterials; matIndex++)
      {
         // Store material name
         std::map<std::string, objfile::Material*>::const_iterator it;
         it = pModel->m_materialMap.find(pModel->m_materialLib[matIndex]);

         // No material found, use the default material
         if (pModel->m_materialMap.end() == it)
            continue;

     
         Material* mat = new Material;
         Material *pCurrentMaterial = (*it).second;
         // AddProperty can be found in Assimp file called material.inl(several versions, must be overloaded...)
         mat->AddProperty(&pCurrentMaterial->MaterialName, AI_MATKEY_NAME);

         // convert illumination model
         int32 sm = 0;
         switch (pCurrentMaterial->illuminationModel)
         {
         case 0:
            sm = aiShadingMode_NoShading;
            break;
         case 1:
            sm = aiShadingMode_Gouraud;
            break;
         case 2:
            sm = aiShadingMode_Phong;
            break;
         default:
            sm = aiShadingMode_Gouraud;
         //   DefaultLogger::get()->error("OBJ: unexpected illumination model (0-2 recognized)");
         }

         mat->AddProperty<int32>(&sm, 1, AI_MATKEY_SHADING_MODEL);

         // multiplying the specular exponent with 2 seems to yield better results
         pCurrentMaterial->shineness *= 4.f;

         // Adding material colors
         mat->AddProperty(&pCurrentMaterial->ambient, 1, AI_MATKEY_COLOR_AMBIENT);
         mat->AddProperty(&pCurrentMaterial->diffuse, 1, AI_MATKEY_COLOR_DIFFUSE);
         mat->AddProperty(&pCurrentMaterial->specular, 1, AI_MATKEY_COLOR_SPECULAR);
         mat->AddProperty(&pCurrentMaterial->emissive, 1, AI_MATKEY_COLOR_EMISSIVE);
         mat->AddProperty(&pCurrentMaterial->shineness, 1, AI_MATKEY_SHININESS);
         mat->AddProperty(&pCurrentMaterial->alpha, 1, AI_MATKEY_OPACITY);

         // Adding refraction index
         mat->AddProperty(&pCurrentMaterial->ior, 1, AI_MATKEY_REFRACTI);

         // Adding textures
         if (0 != pCurrentMaterial->texture.length)
         {
            mat->AddProperty(&pCurrentMaterial->texture, AI_MATKEY_TEXTURE_DIFFUSE(0));
            if (pCurrentMaterial->clamp[objfile::Material::TextureDiffuseType])
            {
               addTextureMappingModeProperty(mat, aiTextureType_DIFFUSE);
            }
         }

         if (0 != pCurrentMaterial->textureAmbient.length)
         {
            mat->AddProperty(&pCurrentMaterial->textureAmbient, AI_MATKEY_TEXTURE_AMBIENT(0));
            if (pCurrentMaterial->clamp[objfile::Material::TextureAmbientType])
            {
               addTextureMappingModeProperty(mat, aiTextureType_AMBIENT);
            }
         }

         if (0 != pCurrentMaterial->textureEmissive.length)
            mat->AddProperty(&pCurrentMaterial->textureEmissive, AI_MATKEY_TEXTURE_EMISSIVE(0));

         if (0 != pCurrentMaterial->textureSpecular.length)
         {
            mat->AddProperty(&pCurrentMaterial->textureSpecular, AI_MATKEY_TEXTURE_SPECULAR(0));
            if (pCurrentMaterial->clamp[objfile::Material::TextureSpecularType])
            {
               addTextureMappingModeProperty(mat, aiTextureType_SPECULAR);
            }
         }

         if (0 != pCurrentMaterial->textureBump.length)
         {
            mat->AddProperty(&pCurrentMaterial->textureBump, AI_MATKEY_TEXTURE_HEIGHT(0));
            if (pCurrentMaterial->clamp[objfile::Material::TextureBumpType])
            {
               addTextureMappingModeProperty(mat, aiTextureType_HEIGHT);
            }
         }

         if (0 != pCurrentMaterial->textureNormal.length)
         {
            mat->AddProperty(&pCurrentMaterial->textureNormal, AI_MATKEY_TEXTURE_NORMALS(0));
            if (pCurrentMaterial->clamp[objfile::Material::TextureNormalType])
            {
               addTextureMappingModeProperty(mat, aiTextureType_NORMALS);
            }
         }

         if (0 != pCurrentMaterial->textureDisp.length)
         {
            mat->AddProperty(&pCurrentMaterial->textureDisp, AI_MATKEY_TEXTURE_DISPLACEMENT(0));
            if (pCurrentMaterial->clamp[objfile::Material::TextureDispType])
            {
               addTextureMappingModeProperty(mat, aiTextureType_DISPLACEMENT);
            }
         }

         if (0 != pCurrentMaterial->textureOpacity.length)
         {
            mat->AddProperty(&pCurrentMaterial->textureOpacity, AI_MATKEY_TEXTURE_OPACITY(0));
            if (pCurrentMaterial->clamp[objfile::Material::TextureOpacityType])
            {
               addTextureMappingModeProperty(mat, aiTextureType_OPACITY);
            }
         }

         if (0 != pCurrentMaterial->textureSpecularity.length)
         {
            mat->AddProperty(&pCurrentMaterial->textureSpecularity, AI_MATKEY_TEXTURE_SHININESS(0));
            if (pCurrentMaterial->clamp[objfile::Material::TextureSpecularityType])
            {
               addTextureMappingModeProperty(mat, aiTextureType_SHININESS);
            }
         }

         // Store material property info in material array in scene
         pScene->mMaterials[pScene->mNumMaterials] = mat;
         pScene->mNumMaterials++;
      }

      // Test number of created materials.
      assert(pScene->m_numMaterials == numMaterials);
   }

   // ------------------------------------------------------------------------------------------------
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

   // ------------------------------------------------------------------------------------------------

}	// Namespace Assimp

#endif // !! ASSIMP_BUILD_NO_OBJ_IMPORTER