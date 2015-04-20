#ifndef _VERTEXBUFFER_HPP_INCLUDED_
#define _VERTEXBUFFER_HPP_INCLUDED_

#include "hardwarebuffer.hpp"
#include "vertexformat.hpp"
#include "vertexstructs.hpp"
using vertexstructs::VertexPC;

#include "model/mesh.hpp"
using mesh::Mesh;

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

namespace vbo
{

 template <typename T>
 class Vertex
 {
 private:
    byte *vertex;
    int32 numBytes;
    char *formatDescriptor;
 public:
    Vertex(const char *formatDescriptor)
    { 
       strcpy(this->formatDescriptor, formatDescriptor);
       for (int32 i = 0, numBytes = 0; formatDescriptor[i]; i++)
       {
          if (formatDescriptor[i] == 'T')
            numBytes += strlen(formatDescriptor) * sizeof(T) * 2;
          else
            numBytes += strlen(formatDescriptor) * sizeof(T) * 3;
       }
    }
    //bool operator<(const VertexAttrib4 other) const
    //{
    //   return memcmp((void*)this, (void*)&other, sizeof(Vertex)) > 0;
    //}
 };

enum eVertexFormat
{
   VERTFORM_POSITION = 1<<0,
   VERTFORM_NORMAL = 1<<1,
   VERTFORM_TEXCOORD2 = 1<<2
};
 
template <typename TFace>
class VertexBuffer : public HardwareBuffer
{
private:
   // Keep a copy vertex data in system memory

   eVertexFormat m_format;
   int32 m_count;
   int32 m_stride;
   int32 m_dataSize;
   void *m_dataPtr;
   
   // VertexAttrib2T is hard coded as a temporary fix
   // vector<Vertex<float> > indexedVertexData;
   // vector<TFace> indexBuffer;
public:
   VertexBuffer( eVertexFormat format, int32 count, const eUsageFlag = USAGE_STATIC_READ, const eAccessFlag = ACCESS_READ_ONLY, const eBufferBindingTarget = BBTARGET_ARRAY_BUFFER ); // exclusive other bbtargets for vbo?
   virtual ~VertexBuffer();

   //must use correct vertex type based on what attributes were found in a model file(.obj,.dae etc)
 
   void PrepareMesh( const Mesh<TFace> & );
   void IndexVBO(const vector<Vertex<float> > &inVertices);

   bool GetSimilarVertex(const Vertex<float>  &inVertex, const map<Vertex<float>, uint32> &mymap, uint32 index);

   float *GetIndexedVertexData() { return &indexedVertexData[0][0]; }
   TFace *GetIndexBuffer() { return &indexBuffer[0][0]; }
};


// for formats, these are the supported symbols at the time being: 'P' = position, 'N' = normal, 'C' = color, 'B' = binormal, 'T' = texcoord (2 dim),
// 'X' = texcoord (3 dim)
template <typename TFace>
VertexBuffer<TFace>::VertexBuffer(eVertexFormat format, int32 count, const eUsageFlag usageFlag, const eAccessFlag accessFlag, const eBufferBindingTarget bindTarget) // exclusive other bbtargets for vbo?
{
   m_format = format;
   m_count = count;
   m_stride = 0;
   // this->usageFlag = usageFlag;
   // this->accessFlag = accessFlag;
//http://www.gamedev.net/topic/367617-flexible-vertex-format-on-the-fly/
    if( m_format & VERTFORM_POSITION )
       m_stride += 3 * sizeof(float);
    if( m_format & VERTFORM_NORMAL )
       m_stride += 3 * sizeof(float);    
     if( m_format & VERTFORM_TEXCOORD2 )
       m_stride += 2 * sizeof(float);   
   //strcpy(Vertex::formatDescriptor, formatDescriptor);
   //Vertex::Init(0);

//   bufferBindingTarget = BBTARGET_ARRAY_BUFFER;
   //bufferBindingTarget = bindTarget; // exclusive other bbtargets for vbo?

   // glGenBuffers(1, &handle);
   // glBindBuffer(bufferBindingTarget, handle);

   // minimum a Position needed in VBO: Point3f or Vector3f type
   //glEnableVertexAttribArray(0); 
   // sizeof(Point3f) and sizeof(Vector3f) gives wrong stride value ....
   //glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)0 );
   // prealloc a vertex to find the static vars of num bytes ;D
   //dummy.Init(formatDescriptor);
   //dummy.CreateVertex(vertexformat::DATATYPE_FLOAT, 0); //assume the DATATYPE_FLOAT for time being
   // assume IsPacked == true for now
   // uint32 i = 0;
   // int32 stride = 0, offset = 0;
   // int32 vertexLocation = 0;
   // int32 normalLocation = 1;
   // int32 texLocation = 2;
 
   // // set positions of attributes
   // //while (format[i])
   // //{
   // //   glEnableVertexAttribArray(i);
   // //}

   // //temporary solution?
   // glEnableVertexAttribArray(vertexLocation);
   // glEnableVertexAttribArray(normalLocation);
   // glEnableVertexAttribArray(texLocation);

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
};


template <typename TFace>
VertexBuffer<TFace >::~VertexBuffer()
{
};


template <typename TFace>
void VertexBuffer<TFace>::PrepareMesh(const Mesh<TFace> &mesh)
{
   uint32 vPos, vtPos, vnPos; // assume uint32 for time being
   int32 i, face, a, b, c;
   vector<VertexAttrib2T<float> > temp;
   VertexAttrib2T<float> newVertex;
   face = a = b = c = 0;

   //PNT - P - PN - PT
   for (i = 0; i < mesh.vertexList.size(); i++)
   {
      //Look up index for each attribute
      vPos = faceList[face].vertexIndex[a];
      newVertex.position = vertexList[vPos];
      if (mesh.HasTexture())
      {
         vtPos = faceList[face].textureIndex[b];
         newVertex.tex2Coord = textureList[vtPos];
      }
      if (mesh.HasNormal())
      {
         vnPos = faceList[face].normalIndex[c];
         newVertex.normal = normalList[vnPos];
      }

      temp.push_back(newVertex);

      a++; b++; c++;
      if ((i + 1) % 3 == 0)
      {
         face++;
         a = b = c = 0;
      }
   }
   IndexVBO(temp);
   //this->vertexArrayByteSize = numVertices * sizeof(1111111); // getvertexsize()
   //this->indexArrayByteSize = numFaces * 3 * sizeof(TFace);
   return 0;
}

template <typename TFace>
void VertexBuffer<TFace>::IndexVBO(const vector<Vertex<float> > &inVertices)
{
   int32 index = 0;
   Vertex<float> currentVertex;

   map<Vertex<float>, uint32> mymap;
   int32 face = 0, indexA = 0, indexB = 0, indexC = 0;
   for (int32 i = 0; inVertices.size(); i++)
   {

      if (GetSimilarVertex(currentVertex, mymap, index))
      {
         indexBuffer.push_back(index);
      }
      else
      {
         VertexAttrib2T<float> newVertex;
         newVertex.normal = inVertices[i].normal;
         newVertex.position = inVertices[i].position;
         newVertex.uv = inVertices[i].uv;
         outVertices.push_back(newVertex);

         uint32 newIndex = (uint32)outVertices.size() - 1;
         outIndices.push_back(newIndex);
         mymap[currentVertex] = newIndex;
      }
   }
}


template <typename TFace>
bool  VertexBuffer<TFace>::GetSimilarVertex(const Vertex<float>  &inVertex, const map<Vertex<float>, uint32> &mymap, uint32 index)
{
   map<Vertex<float>, uint32>::iterator it = mymap.find(inVertex);

   if (it == mymap.end())
   {
      return false;
   }

   index = it->second;
   return true;
}

} // namespace vbo

#endif