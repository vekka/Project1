#ifndef _VERTEXBUFFER_HPP_INCLUDED_
#define _VERTEXBUFFER_HPP_INCLUDED_

#include "hardwarebuffer.hpp"
#include "vertexformat.hpp"
#include "vertexstructs.hpp"
using vertexstructs::VertexPC;

#include "model/mesh2.hpp"
using mesh2::aiMesh;

#include <map>
using std::map;

using core::math::Point2f;
using core::math::Point3f;
using core::math::Vector2f;
using core::math::Vector3f;

//using vertexformat::Vertex;

using hardwarebuffer::ACCESS_READ_ONLY;
using hardwarebuffer::USAGE_STATIC_READ;
using hardwarebuffer::eUsageFlag;
using hardwarebuffer::eAccessFlag;
using hardwarebuffer::HardwareBuffer;
using hardwarebuffer::eBufferBindingTarget;
using hardwarebuffer::BBTARGET_ARRAY_BUFFER;
using vertexformat::eVertexFormat;
namespace vbo
{

template <typename TFace>
class VertexBuffer : public HardwareBuffer
{
private:
   // Keep a copy vertex data in system memory
   int32 m_count;//Number of attributes in each Vertex
   int32 m_stride;
   int32 m_dataSize;
   void *m_dataPtr;
   
   int32 m_allocated; //?
   int32 m_used; //?

   //int32 streamIndex;
   
   // VertexAttrib2T is hard coded as a temporary fix
   // vector<Vertex<float> > indexedVertexData;
   // vector<TFace> indexBuffer;
public:
   void Lock( int32 start, int32 end );
   void Unlock();
   //void SetComponents(eVertexFormat components) { m_format |= components; }
   //bool HasComponents(eVertexFormat components) { return m_format & components; }


   //VertexBuffer(eVertexFormat format, void *structData, int32 count, const eUsageFlag usageFlag, const eAccessFlag accessFlag, const eBufferBindingTarget bindTarget) // exclusive other bbtargets for vbo?

   //VertexBuffer(eVertexFormat format, int32 count, const eUsageFlag usageFlag, const eAccessFlag accessFlag, const eBufferBindingTarget bindTarget) // exclusive other bbtargets for vbo?
   //VertexBuffer( eVertexFormat format, int32 count, const eUsageFlag = USAGE_STATIC_READ, const eAccessFlag = ACCESS_READ_ONLY, const eBufferBindingTarget = BBTARGET_ARRAY_BUFFER ); // exclusive other bbtargets for vbo?
   virtual ~VertexBuffer();

   //must use correct vertex type based on what attributes were found in a model file(.obj,.dae etc)
 
   //void PrepareMesh( const Mesh<TFace> & );
   //void IndexVBO(const vector<Vertex<float> > &inVertices);

   //bool GetSimilarVertex(const Vertex<float>  &inVertex, const map<Vertex<float>, uint32> &mymap, uint32 index);

   float *GetIndexedVertexData() { return &indexedVertexData[0][0]; }
   TFace *GetIndexBuffer() { return &indexBuffer[0][0]; }
};


//VertexBuffer(eVertexFormat format, void *structData, int32 count, const eUsageFlag usageFlag, const eAccessFlag accessFlag, const eBufferBindingTarget bindTarget) // exclusive other bbtargets for vbo?

// ctor for structs (see vertexstructs.hpp)
//template <typename TFace>
//VertexBuffer<TFace>::VertexBuffer(eVertexFormat format, void *structData, int32 count, const eUsageFlag usageFlag, const eAccessFlag accessFlag, const eBufferBindingTarget bindTarget) // exclusive other bbtargets for vbo?
//{
//   m_count = count;
//   m_stride = 0;
//   this->usageFlag = usageFlag;
//   this->accessFlag = accessFlag;
//   vector<int32>  
//  
//   bufferBindingTarget = BBTARGET_ARRAY_BUFFER;
//   bufferBindingTarget = bindTarget; // exclusive other bbtargets for vbo?
//   glGenBuffers(1, &handle);
//   glBindBuffer(bufferBindingTarget, handle);
//
//   for (int32 i = 0; i < format; i++)
//   {
//      m_stride += GetVertexComponentSize(static_cast<eVertexFormat>(format & (1 << pos)));
//   }
//
//
//  
//   for (int32 i = 0; i < m_count; i++)
//   {
//      glEnableVertexAttribArray(i);
//   }
//   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_stride, (void*)0);
//}
//
//
//// ctor for FVF (Flexible Vertex Format) (For later allocation of Vertex Class placeholders)
//template <typename TFace>
//VertexBuffer<TFace>::VertexBuffer(eVertexFormat format, int32 count, const eUsageFlag usageFlag, const eAccessFlag accessFlag, const eBufferBindingTarget bindTarget) // exclusive other bbtargets for vbo?
//{
//   m_count = count;
//   m_stride = 0;
//   this->usageFlag = usageFlag;
//   this->accessFlag = accessFlag;
////http://www.gamedev.net/topic/367617-flexible-vertex-format-on-the-fly/
//    if( format & VERTFORM_POSITION )
//       m_stride += 3 * sizeof(float);
//    if( format & VERTFORM_NORMAL )
//       m_stride += 3 * sizeof(float);    
//     if( format & VERTFORM_TEXCOORD2 )
//       m_stride += 2 * sizeof(float);   
//
//   bufferBindingTarget = BBTARGET_ARRAY_BUFFER;
//   bufferBindingTarget = bindTarget; // exclusive other bbtargets for vbo?
//
//    glGenBuffers(1, &handle);
//    glBindBuffer(bufferBindingTarget, handle);
//    for (int i = 0; i < m_count; i++)
//     {
//        glEnableVertexAttribArray(i);
//     }
//    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, m_stride, (void*)0 );

   // glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
   // glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)12);
   // glVertexAttribPointer(texLocation, 2 , GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)24);

   // calculate offset
   //for (i = 0; i < strlen(formatDescriptor); i++)
   //{
   //   if (Vertex::IsPacked())
   //   {
   //      temp = dummy.GetNumBytes();
   //      glVertexAttribPointer(i, temp / sizeof(float), GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
   //      offset += temp;
   //   }
   //   else
   //   { }
   //}

   ////this code can be simplified
   //if ( format == "PN" || format == "PT" || format == "PC" )
   //{
   //   glEnableVertexAttribArray(1);
   //   glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f),(void*)(3*sizeof(float)) );
   //}
   //else if( format == "PTT" )
   //{
   //   glEnableVertexAttribArray(1);
   //   glEnableVertexAttribArray(2);
   //   glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector3f) + 2*sizeof(Vector2f), (void*)(3*sizeof(float)) );
   //   glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector3f) + 2*sizeof(Vector2f), (void*)(5*sizeof(float)) );
   //}
   //else if( format == "PCT" )
   //{
   //   glEnableVertexAttribArray(1);
   //   glEnableVertexAttribArray(2);
   //   glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)(3*sizeof(float)) );
   //   glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), (void*)(6*sizeof(float)) );
   //}
   //else if ( format == "PTTT" )
   //{
   //   glEnableVertexAttribArray(1);
   //   glEnableVertexAttribArray(2);
   //   glEnableVertexAttribArray(3);
   //   glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), (void*)(3*sizeof(float)) );
   //   glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), (void*)(5*sizeof(float)) );
   //   glVertexAttribPointer( 3, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), (void*)(7*sizeof(float)) );
   //}
   //else if ( format == "PCTT" || format == "PNTT" )
   //{
   //   glEnableVertexAttribArray(1);
   //   glEnableVertexAttribArray(2);
   //   glEnableVertexAttribArray(3);
   //   glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)(3*sizeof(float)) );
   //   glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), (void*)(6*sizeof(float)) );
   //   glVertexAttribPointer( 3, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), (void*)(8*sizeof(float)) );
   //}
   //else if ( format == "PNT" )
   //{
   //   glEnableVertexAttribArray(1);
   //   glEnableVertexAttribArray(2);
   //   glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)(3*sizeof(float)) );
   //   glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), (void*)(5*sizeof(float)) );
   //}
   //else if ( format == "PCNT" )
   //{
   //   glEnableVertexAttribArray(1);
   //   glEnableVertexAttribArray(2);
   //   glEnableVertexAttribArray(3);
   //   glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)(3*sizeof(float)) );
   //   glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)(6*sizeof(float)) );
   //   glVertexAttribPointer( 3, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), (void*)(8*sizeof(float)) );
   //}
//};


template <typename TFace>
VertexBuffer<TFace >::~VertexBuffer()
{
};
//
//
//template <typename TFace>
//void VertexBuffer<TFace>::PrepareMesh(const Mesh<TFace> &mesh)
//{
//   uint32 vPos, vtPos, vnPos; // assume uint32 for time being
//   int32 i, face, a, b, c;
//   vector<VertexAttrib2T<float> > temp;
//   VertexAttrib2T<float> newVertex;
//   face = a = b = c = 0;
//
//   //PNT - P - PN - PT
//   for (i = 0; i < mesh.vertexList.size(); i++)
//   {
//      //Look up index for each attribute
//      vPos = faceList[face].vertexIndex[a];
//      newVertex.position = vertexList[vPos];
//      if (mesh.HasTexture())
//      {
//         vtPos = faceList[face].textureIndex[b];
//         newVertex.tex2Coord = textureList[vtPos];
//      }
//      if (mesh.HasNormal())
//      {
//         vnPos = faceList[face].normalIndex[c];
//         newVertex.normal = normalList[vnPos];
//      }
//
//      temp.push_back(newVertex);
//
//      a++; b++; c++;
//      if ((i + 1) % 3 == 0)
//      {
//         face++;
//         a = b = c = 0;
//      }
//   }
//   IndexVBO(temp);
//   //this->vertexArrayByteSize = numVertices * sizeof(1111111); // getvertexsize()
//   //this->indexArrayByteSize = numFaces * 3 * sizeof(TFace);
//   return 0;
//}
//
//template <typename TFace>
//void VertexBuffer<TFace>::IndexVBO(const vector<Vertex<float> > &inVertices)
//{
//   int32 index = 0;
//   Vertex<float> currentVertex;
//
//   map<Vertex<float>, uint32> mymap;
//   int32 face = 0, indexA = 0, indexB = 0, indexC = 0;
//   for (int32 i = 0; inVertices.size(); i++)
//   {
//
//      if (GetSimilarVertex(currentVertex, mymap, index))
//      {
//         indexBuffer.push_back(index);
//      }
//      else
//      {
//         VertexAttrib2T<float> newVertex;
//         newVertex.normal = inVertices[i].normal;
//         newVertex.position = inVertices[i].position;
//         newVertex.uv = inVertices[i].uv;
//         outVertices.push_back(newVertex);
//
//         uint32 newIndex = (uint32)outVertices.size() - 1;
//         outIndices.push_back(newIndex);
//         mymap[currentVertex] = newIndex;
//      }
//   }
//}
//
//
//template <typename TFace>
//bool  VertexBuffer<TFace>::GetSimilarVertex(const Vertex<float>  &inVertex, const map<Vertex<float>, uint32> &mymap, uint32 index)
//{
//   map<Vertex<float>, uint32>::iterator it = mymap.find(inVertex);
//
//   if (it == mymap.end())
//   {
//      return false;
//   }
//
//   index = it->second;
//   return true;
//}

} // namespace vbo

#endif