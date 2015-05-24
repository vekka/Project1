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

//namespace model
//{
//   namespace objparser
//   {
//      class ObjParser;
//   };
//};

namespace objfile
{
 
   struct Object;
   struct Face;
   struct Material;

   // data structure for a simple obj-face, describes discredit,l.ation and materials
   struct Face
   {
   //protected:
      //friend void model::objparser::ObjParser::GetFace(ePrimitiveType type);
      
      typedef std::vector<uint32> IndexList;

      //!	Primitive type
      ePrimitiveType m_primitiveType;

      IndexList *m_pVertexIndices;
      IndexList *m_pNormalIndices;
      IndexList *m_pTexCoordIndices;
      //!	Pointer to assigned material
      Material *m_pMaterial;

      //!	\param	pVertices	Pointer to assigned vertex indexbuffer
      //!	\param	pNormals	Pointer to assigned normals indexbuffer
      //!	\param	pTexCoords	Pointer to assigned texture indexbuffer

   //public:

      Face(std::vector<uint32> *pVertices,
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
     
      ~Face()
      {
         delete m_pVertexIndices;
         m_pVertexIndices = NULL;

         delete m_pNormalIndices;
         m_pNormalIndices = NULL;

         delete m_pTexCoordIndices;
         m_pTexCoordIndices = NULL;
      }
   };

   // ------------------------------------------------------------------------------------------------
   //!	\struct	Object
   //!	\brief	Stores all objects of an objfile object definition
   struct Object
   {
      enum ObjectType
      {
         ObjType,
         GroupType
      };

      //!	Object name
      String_c m_strObjName;
      //!	Transformation matrix, stored in OpenGL format
      Matrix4f m_Transformation;
      //!	All sub-objects referenced by this object
      std::vector<Object*> m_SubObjects;
      ///	Assigned meshes
      std::vector<uint32> m_meshes;

      //!	\brief	Default constructor
      Object() :
         m_strObjName("")
      {
         // empty
      }

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

   //!	\brief	Data structure to store all material specific data
   struct Material
   {
      //!	Name of material description
      String_c MaterialName;

      //!	Texture names
      String_c texture;
      String_c textureSpecular;
      String_c textureAmbient;
      String_c textureEmissive;
      String_c textureBump;
      String_c textureNormal;
      String_c textureSpecularity;
      String_c textureOpacity;
      String_c textureDisp;
      enum eTextureType
      {
         TextureDiffuseType ,
         TextureSpecularType,
         TextureAmbientType,
         TextureEmissiveType,
         TextureBumpType,
         TextureNormalType,
         TextureSpecularityType,
         TextureOpacityType,
         TextureDispType,
         TextureTypeCount
      };

      bool clamp[TextureTypeCount];

      Colorf ambientColor;
      Colorf diffuseColor;
      Colorf specularColor;
      Colorf emissiveColor;

      float alpha;

      float shineness;

      int32 illuminationModel;

      float indexOfRefraction;

      Material()
         : diffuseColor(0.6f, 0.6f, 0.6f)
         , alpha(1.f)
         , shineness(0.0f)
         , illuminationModel(1)
         , indexOfRefraction(1.f)
      {
         for (size_t i = 0; i < TextureTypeCount; ++i)
         {
            clamp[i] = false;
         }
      }

      ~Material()
      {
      }
   };

   // data structure to store a mesh
   struct Mesh
   {
      static const uint32 NoMaterial = ~0u;

      //	Array with pointer to all stored faces
      std::vector<Face*> m_faces;
      //	Assigned material
      Material *m_pMaterial;
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
         for (std::vector<Face*>::iterator it = m_faces.begin();
            it != m_faces.end(); ++it)
         {
            delete *it;
         }
      }
   };

   // data structure to store all obj-specific model datas
   struct Model
   {
      typedef std::map<String_c, std::vector<uint32>* > GroupMap;
      typedef std::map<String_c, std::vector<uint32>* >::iterator GroupMapIt;
      typedef std::map<String_c, std::vector<uint32>* >::const_iterator ConstGroupMapIt;

      String_c m_modelName;
      //	List ob assigned objects
      std::vector<Object*> m_objects;
      //	Pointer to current object
      objfile::Object *m_pCurrent;
      //	Pointer to current material
      objfile::Material *m_pCurrentMaterial;
      //	Pointer to default material
      objfile::Material *m_pDefaultMaterial;
      //	Vector with all generated materials
      std::vector<String_c> m_materialLib;
      //	Vector with all generated group
      std::vector<String_c> m_groupLib;
      //	Vector with all generated vertices
      std::vector<Vector3f> m_pVertices;
      //	vector with all generated normals
      std::vector<Vector3f> m_pNormals;
      //	Group map
      GroupMap m_groups;
      //	Group to face id assignment
      std::vector<uint32> *m_pGroupFaceIDs;
      //	Active group
      String_c m_strActiveGroup;
      //	Vector with generated texture coordinates
      //std::vector<Vector2f> m_textureCoord2;
      std::vector<Vector3f> m_textureCoord;

      Mesh *m_pCurrentMesh;

      std::vector<Mesh*> m_meshes;

      std::map<String_c, Material*> m_materialMap;

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

         for (std::map<String_c, Material*>::iterator it = m_materialMap.begin(); it != m_materialMap.end(); ++it) {
            delete it->second;
         }
      }
   };

} // namespace objfile


#endif