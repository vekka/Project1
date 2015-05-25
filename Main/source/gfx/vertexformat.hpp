#ifndef _VERTEXFORMATS_HPP_INCLUDED_
#define _VERTEXFORMATS_HPP_INCLUDED_

#include "core/math/vector2.hpp"
#include "core/math/vector3.hpp"
#include "color.hpp"
//#include "core/string/string.hpp"

#include <vector>
using namespace std;

#include "../core/bits.hpp"
using core::bits::enumflags::operator&;

namespace vertexformat
{

   enum eVertexFormat
   {
      VF_EMPTY,
      VF_POSITION = 1 << 0, // vertex position 3 floats x,y,z
      VF_NORMAL = 1 << 1, // normal 3 floats x,y,z
      VF_TEXCOORD2D_1 = 1 << 2, // texcoord 1 2 floats u,v
      VF_TEXCOORD2D_2 = 1 << 3, // texcoord 2 2 floats u,v
      VF_TEXCOORD2D_3 = 1 << 4, // texcoord 3 2 floats u,v
      VF_COLOR = 1 << 5,
      VF_BINORMAL = 1 << 6,
      VF_TEXCOORD3D_1 = 1 << 7
      // and so on
   };

    //P   N   T1   T2  T3
    //0   0   0    0    0


    //VF_POSITION | VF_NORMAL

    //P   N   T1   T2  T3
    //1   1   0    0    0

    //bool HasComponent(eVertexFormat comp)




// types used in GLSL shaders, prefixed with GL instead of DATATYPE
enum eDataType
{
   DATATYPE_HALF_FLOAT​​,
   DATATYPE_FLOAT​​,
   DATATYPE_DOUBLE​​,
   DATATYPE_BYTE​​,
   DATATYPE_UNSIGNED_BYTE​​,
   DATATYPE_SHORT​​,
   DATATYPE_UNSIGNED_SHORT​​,
   DATATYPE_INT,
   DATATYPE_UNSIGNED_INT,
   DATATYPE_INT_2_10_10_10_REV​​,
   DATATYPE_UNSIGNED_INT_2_10_10_10_REV,
   DATATYPE_UNSIGNED_INT_10F_11F_11F_REV​,

   DATATYPE_ENUM_SIZE
};

inline int32 GetDataTypeSizeInBytes(eDataType type)
{
   assert(type < DATATYPE_ENUM_SIZE);

   switch (type)
   {
   case DATATYPE_BYTE​​:
   case DATATYPE_UNSIGNED_BYTE​​:
      return 1;
   case DATATYPE_SHORT​​:
   case DATATYPE_UNSIGNED_SHORT​​:
   case DATATYPE_HALF_FLOAT​​:
      return 2;
   case DATATYPE_FLOAT​​:
   case DATATYPE_INT:
   case DATATYPE_UNSIGNED_INT:
   case DATATYPE_INT_2_10_10_10_REV​​:
   case DATATYPE_UNSIGNED_INT_2_10_10_10_REV:
   case DATATYPE_UNSIGNED_INT_10F_11F_11F_REV​:
      return 4;
   case DATATYPE_DOUBLE​​:
      return 8;
   default:
      return 0;
   };
}

//enum eVertexComponentType
//{
//   VCOMP_POSITION,
//   VCOMP_NORMAL,
//   VCOMP_TEXCOORD2,
//   VCOMP_TEXCOORD3,
//   VCOMP_COLOR,
//   VCOMP_BINORMAL,
//   VCOMP_TANGENT,
//   VCOMP_TANGENTW,
//
//   VCOMP_ENUM_SIZE
//};

//class VertexComponent
//{ 
//public:
//   //friend class Vertex<T>;
//
//   VertexComponent();
//   VertexComponent( const VertexComponent &other );
//   uint32 GetSizeInBytes() const;
//   uint32 GetStreamIndex() const { return streamIndex; }
//   void SetStreamIndex( uint32 streamIndex ) { this->streamIndex = streamIndex;}
//protected: 
//   uint32 streamIndex; // the stream index when rendering from multiple vertex streams.  In most cases this is 0.
//   uint32 semanticIndex; // the semantic index is used where there are multiple semantics of the same type. For instance with texcoords.
//   char semantic;
//   eVertexType type;
//};

//class VertexComponent
//{
//private:
//   char componentType;
//   //int32 offset; // offset of vertex component in vertex
//   int32 size;
//   static eDataType dataType; // possibly not possible to have the vbo with mixed basic types
//   byte *data;
//public:
//   VertexComponent(char vertexComponentType, eDataType dataType, byte *data);
//   virtual ~VertexComponent();
//   int32 GetSizeInBytes();
//   char GetComponentType() { return componentType; };
//};


inline int32 GetVertexComponentSize(eVertexFormat component)
{
   switch (component)
   {
   case VF_EMPTY:
      return 0;
   case VF_POSITION:
   case VF_NORMAL:
   case VF_COLOR:
   case VF_BINORMAL:
   case VF_TEXCOORD3D_1:
      return sizeof(float) * 3;
   case VF_TEXCOORD2D_1:
   case VF_TEXCOORD2D_2:
   case VF_TEXCOORD2D_3:
      return sizeof(float) * 2;
   default:
      return 0;
   }
}

class Vertex
{
private:
   //byte *vertex; // generic vertex format ptr for allocating an arbitrary vertex format
   //vector<VertexComponent> components; // vertex configuration and data
   static int32 numBytes;
   static bool isPacked;
   static eVertexFormat format;
   static int32 numComponents;
   static int32 *vertCompOffsets; //assume offset 0 equals size of furst component, offset 1 is size of first component + the second, and so eond

   int32 GetVertexFormatSize(eVertexFormat format)
   {
      for (int32 pos = 0; pos < sizeof(eVertexFormat); pos++)
      {
         numBytes += GetVertexComponentSize(static_cast<eVertexFormat>(format & (1 << pos)));
      }
   }
public:
   // format description is composed of characters used for vertex formats: e.g "PNT" for position, normal, texture, respectively
   // this inits shared (static) vars only, and should be called only once
   static void Init(/*const char *formatDescriptor,*/ eDataType dataType = DATATYPE_FLOAT​​);


   // create a vertex containing the basic type and corresponding to the vertex format
   //void CreateVertexFormat(eDataType dataType); 

   ~Vertex();
   static bool IsPacked() { return isPacked; }
   
   bool operator<(const Vertex &other) const { return memcmp((void*)this, (void*)&other, sizeof(Vertex))>0; }  // this
   //bool operator<(const Vertex other) const { return memcmp((void*)this, (void*)&other, sizeof(Vertex))>0; }  // or that
   int32 GetNumBytes();
  
   int32 *GetOffsets();
   //void *operator[](char vertexComponentType) const;
   //void *&operator[](char vertexComponentType);

   //T operator[](const uint8 index) const;
   //T &operator[](const uint8 index);
};

//class Vertex
//{
//protected:
//   bool isDirty;
//   bool hasNormal;
//   bool hasColor;
//   bool hasTangent;
//   void updateDirty();
//   std::string formatDescriptor;
//   uint32 texCoordCount;
//   uint32 sizeInBytes;
//   vector<VertexComponent> components; // list of components for each vertex
//public:
//   Vertex();
//   Vertex( const std::string &formatDescription );
//   Vertex( const Vertex &other );
//   void Append( const Vertex &other, const int32 streamIndex = -1 );
//   void Clear();
//   const VertexComponent &GetComponent( uint32 index ) const { return components[index]; }
//   void UpdateDirty();
//
//   void CopyFrom( const Vertex &other );
//   uint32 GetSizeInBytes() const;
//};

} // namespace vertexformat

#endif

