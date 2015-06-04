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

#ifndef _OBJFILE_HPP_INCLUDED_
#define _OBJFILE_HPP_INCLUDED_

#include <vector>
#include <map>

#include "mesh2.hpp"
using mesh2::ePrimitiveType;
using mesh2::PRIMITIVE_TYPE_POLYGON;

namespace objfile
{
   struct Object;
   struct ObjFace;
   struct ObjMaterial;

   // data structure for a simple obj-face, describes discredit,l.ation and materials
   struct ObjFace
   {
   //protected:
      //friend void model::objparser::ObjParser::GetFace(ePrimitiveType type);
      
      typedef std::vector<uint32> IndexList;

      ePrimitiveType m_primitiveType;

      IndexList *m_pVertexIndices;
      IndexList *m_pNormalIndices;
      IndexList *m_pTexCoordIndices;

      ObjMaterial *m_pMaterial;

   //public:

      ObjFace(std::vector<uint32> *pVertices,
         std::vector<uint32> *pNormals,
         std::vector<uint32> *pTexCoords,
         ePrimitiveType pt = PRIMITIVE_TYPE_POLYGON) :
         m_primitiveType(pt),
         m_pVertexIndices(pVertices),
         m_pNormalIndices(pNormals),
         m_pTexCoordIndices(pTexCoords),
         m_pMaterial(0L)
      {
         // empty
      }
     
      ~ObjFace()
      {
         delete m_pVertexIndices;
         m_pVertexIndices = NULL;

         delete m_pNormalIndices;
         m_pNormalIndices = NULL;

         delete m_pTexCoordIndices;
         m_pTexCoordIndices = NULL;
      }
   };

   //	Stores all objects of an objfile object definition
   struct Object
   {
      enum eObjectType
      {
         OBJTYPE,
         GROUPTYPE
      };

      std::string m_strObjName;
      //!	Transformation matrix, stored in OpenGL format
      Matrix4f m_Transformation;
      //!	All sub-objects referenced by this object
      std::vector<Object*> m_SubObjects;
      std::vector<uint32> m_meshes;

      Object() : m_strObjName("") { }

      ~Object()
      {
         for (std::vector<Object*>::iterator it = m_SubObjects.begin();
            it != m_SubObjects.end(); ++it)
         {
            delete *it;
         }
         m_SubObjects.clear();
      }
   };

   // Data structure to store all material specific data
   struct ObjMaterial
   {
      std::string m_materialName;

      // m_texture names
      std::string m_texture;
      std::string textureSpecular;
      std::string m_textureAmbient;
      std::string m_textureEmissive;
      std::string m_textureBump;
      std::string m_textureNormal;
      std::string m_textureSpecularity;
      std::string m_textureOpacity;
      std::string m_textureDisplacement;
     
      enum eTextureType
      {
         TEXTURE_TYPE_DIFFUSE,
         TEXTURE_TYPE_SPECULAR,
         TEXTURE_TYPE_AMBIENT,
         TEXTURE_TYPE_EMISSIVE,
         TEXTURE_TYPE_BUMP,
         TEXTURE_TYPE_NORMALS,
         TEXTURE_TYPE_SHININESS,
         TEXTURE_TYPE_OPACITY,
         TEXTURE_TYPE_DISPLACEMENT,
         
         TEXTURE_TYPE_COUNT
      };

      bool m_clamp[TEXTURE_TYPE_COUNT];

      Color4f m_ambientColor;
      Color4f m_diffuseColor;
      Color4f m_specularColor;
      Color4f m_emissiveColor;

      float m_alpha;

      float m_shineness;

      int32 m_illuminationModel;

      float m_indexOfRefraction;

      ObjMaterial()
         : m_diffuseColor(0.6f, 0.6f, 0.6f)
         , m_alpha(1.f)
         , m_shineness(0.0f)
         , m_illuminationModel(1)
         , m_indexOfRefraction(1.f)
      {
         for (size_t i = 0; i < TEXTURE_TYPE_COUNT; i++)
         {
            m_clamp[i] = false;
         }
      }
   };

   // data structure to store a mesh
   struct Mesh
   {
      static const uint32 NoMaterial = ~0u;

      //	Array with pointer to all stored faces
      std::vector<ObjFace*> m_faces;
      //	Assigned material
      ObjMaterial *m_pMaterial;
      //	Number of stored indices.
      uint32 m_uiNumIndices;
      // Number of UV
      uint32 m_uiUVCoordinates[AI_MAX_NUMBER_OF_TEXTURECOORDS];
      //	Material index.
      uint32 m_uiMaterialIndex;
      //	True, if normals are stored.
      bool m_hasNormals;

      Mesh() :
         m_pMaterial(NULL),
         m_uiNumIndices(0),
         m_uiMaterialIndex(NoMaterial),
         m_hasNormals(false)
      {
         memset(m_uiUVCoordinates, 0, sizeof(uint32) * AI_MAX_NUMBER_OF_TEXTURECOORDS);
      }

      ~Mesh()
      {
         for (std::vector<ObjFace*>::iterator it = m_faces.begin();
            it != m_faces.end(); ++it)
         {
            delete *it;
         }
      }
   };

   // data structure to store all obj-specific model datas
   struct Model
   {
      typedef std::map<std::string, std::vector<uint32>* > GroupMap;
      typedef std::map<std::string, std::vector<uint32>* >::iterator GroupMapIt;
      typedef std::map<std::string, std::vector<uint32>* >::const_iterator ConstGroupMapIt;

      std::string m_modelName;
      //	List ob assigned objects
      std::vector<Object*> m_objects;
      //	Pointer to current object
      objfile::Object *m_pCurrent;
      //	Pointer to current material
      objfile::ObjMaterial *m_pCurrentMaterial;
      //	Pointer to default material
      objfile::ObjMaterial *m_pDefaultMaterial;
      //	Vector with all generated materials
      std::vector<std::string> m_materialLib;
      //	Vector with all generated group
      std::vector<std::string> m_groupLib;
      //	Vector with all generated vertices
      std::vector<Vector3f> m_pVertices;
      //	vector with all generated normals
      std::vector<Vector3f> m_pNormals;
      //	Group map
      GroupMap m_groups;
      //	Group to face id assignment
      std::vector<uint32> *m_pGroupFaceIDs;
      //	Active group
      std::string m_strActiveGroup;
      //	Vector with generated m_texture coordinates
      //std::vector<Vector2f> m_textureCoord2;
      std::vector<Vector3f> m_textureCoord;

      Mesh *m_pCurrentMesh;

      std::vector<Mesh*> m_meshes;

      std::map<std::string, ObjMaterial*> m_materialMap;

      Model() :
         m_modelName(""),
         m_pCurrent(NULL),
         m_pCurrentMaterial(NULL),
         m_pDefaultMaterial(NULL),
         m_pGroupFaceIDs(NULL),
         m_strActiveGroup(""),
         m_pCurrentMesh(NULL)
      {
         // empty
      }

      //!	\brief	The class destructor
      ~Model()
      {
         // clear all stored object instances
         for (std::vector<Object*>::iterator it = m_objects.begin();
            it != m_objects.end(); ++it) {
            delete *it;
         }
         m_objects.clear();

         // clear all stored mesh instances
         for (std::vector<Mesh*>::iterator it = m_meshes.begin();
            it != m_meshes.end(); ++it) {
            delete *it;
         }
         m_meshes.clear();

         for (GroupMapIt it = m_groups.begin(); it != m_groups.end(); ++it) {
            delete it->second;
         }
         m_groups.clear();

         for (std::map<std::string, ObjMaterial*>::iterator it = m_materialMap.begin(); it != m_materialMap.end(); ++it) {
            delete it->second;
         }
      }
   };

} // namespace objfile


#endif