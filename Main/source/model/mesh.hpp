#ifndef _MESH_HPP_INCLUDED_
#define _MESH_HPP_INCLUDED_

// this is the interface for all mesh loaders

#include <iostream>
#include <map>
#include <vector>

#include "core/bits.hpp"
using core::bits::enumflags::operator|;
using core::bits::enumflags::operator&;

#include "core/fileio/file.hpp"
#include "core/math/vector3.hpp"
#include "core/math/vector2.hpp"
#include "core/string/string.hpp"

using core::math::Vector2f;
using core::math::Vector3f;

#include "gfx/vertexformat.hpp"
using vertexformat::eVertexFormat;
using vertexformat::VF_EMPTY;
using vertexformat::VF_POSITION;
using vertexformat::VF_NORMAL;
using vertexformat::VF_TEXCOORD2D_1;
using vertexformat::VF_TEXCOORD2D_2;
using vertexformat::VF_TEXCOORD2D_3;

using namespace std;

namespace mesh
{

template <typename T>
class Face
{
private:
   int32 numVertices;
   // for trifaces
   vector<T> vertexIndex;
   vector<T> normalIndex;
   vector<T> textureIndex;
  
   //uint32 materialIndex; ?

public:
   Face( const int32 numVertices = 3 ) { this->numVertices = numVertices; }

   int32 GetNumVertices(void) const { return numVertices; }

   void AddVertexIndex( const T idx ) { vertexIndex.push_back( idx ); }
   void AddNormalIndex( const T idx ) { normalIndex.push_back( idx ); }
   void AddTextureIndex( const T idx ) { textureIndex.push_back( idx ); }

   T *GetVertexIdxPtr() { return &vertexIndex[0]; }
   T *GetNormalIdxPtr() { return &normalIndex[0]; }
   T *GetTextureIdxPtr() { return &textureIndex[0]; }
};

typedef Face<uint16> Face16;
typedef Face<uint32> Face32;

template <typename TFace = uint32>
class Mesh
{
private:
   int32 numGroups;
   eVertexFormat vertexFormat;
   // generic basic types should be used (?)
   // vector<Vector<generic type> > vertexList;

   // these are the unindexed lists!
   vector<Vector3f> vertexList;
   vector<Vector3f> normalList;
   vector<Vector2f> texture2List;
   vector<Vector3f> texture3List;
   vector<Vector3f> tangentList;
   vector<Vector3f> colorList;
   vector<Vector3f> binormalList;

   vector<Face<TFace> > faceList;
public:
   Mesh::Mesh()
   {
      vertexList.clear();
      faceList.clear();
      numGroups = 0;
      vertexFormat = VF_EMPTY;
   };

   Mesh::~Mesh(){};

   eVertexFormat GetVertexFormat() const { return vertexFormat; }
   void AddVertex( const Vector3f vertex ) { vertexList.push_back(vertex); }
   void AddVertexNormal( const Vector3f vertexNormal ) { normalList.push_back(vertexNormal); }
   void AddVertexTexture2( const Vector2f vertexTexture ) { texture2List.push_back(vertexTexture); }
   void AddVertexTexture3( const Vector3f vertexTexture ) { texture3List.push_back(vertexTexture); }
   void AddTangent(const Vector3f tangent) { tangentList.push_back(tangent); }
   void AddBinormal(const Vector3f binormal) { binormalList.push_back(binormal); }
   void AddFace( const Face<TFace> &face ) { faceList.push_back(face); }

   float *GetVertexListPtr() { return &vertexList[0][0]; }
   float *GetNormalListPtr() { return &normalList[0][0]; }
   float *GetTexture2ListPtr() { return &texture2List[0][0]; }
   float *GetTexture3ListPtr() { return &texture3List[0][0]; }

   void SetComponents(eVertexFormat components) { vertexFormat = vertexFormat | components; }

   bool HasComponents(eVertexFormat components) const { return vertexFormat ^ components;  }
   
   uint32 GetNumElemVertexList() { return vertexList.size(); }
   uint32 GetNumElemNormalList() { return normalList.size(); }
   uint32 GetNumElemTexture2List() { return texture2List.size(); }
   uint32 GetNumElemTexture3List() { return texture3List.size(); }

   Face<TFace> *GetFaceListPtr() { return &faceList[0]; }

   bool HasComponents(eVertexFormat components) { return vertexFormat & components; }
   bool HasNormal() { return !normalList.empty();; }
   bool HasTexture2() { return !texture2List.empty(); }
   bool HasTexture3() { return !texture3List.empty(); }
   bool HasTangent() { return !tangentList.empty(); }
   bool HasColor() { return !colorList.empty(); }
   bool HasBinormal() { return !binormal.empty(); }
};

typedef Mesh<uint16> Mesh16;
typedef Mesh<uint32> Mesh32;

} // namespace mesh

#endif