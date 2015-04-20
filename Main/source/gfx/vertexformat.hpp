#ifndef _VERTEXFORMATS_HPP_INCLUDED_
#define _VERTEXFORMATS_HPP_INCLUDED_

#include "core/math/vector2.hpp"
#include "core/math/vector3.hpp"
#include "color.hpp"
#include "core/string/string.hpp"

#include <vector>

using namespace std;

namespace vertexformat
{

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

class VertexComponent
{
private:
   char componentType;
   //int32 offset; // offset of vertex component in vertex
   int32 size;
   static eDataType dataType; // possibly not possible to have the vbo with mixed basic types
   byte *data;
public:
   VertexComponent(char vertexComponentType, eDataType dataType, byte *data);
   virtual ~VertexComponent();
   int32 GetSizeInBytes();
   char GetComponentType() { return componentType; };
};

//inline int32 GetComponentSize(char component) const
//{
//   switch (component)
//   {
//   case 'T': // texcoord (2 dimensional)
//      return sizeof(float) * 2;
//   case 'P': // position
//   case 'N': // normal
//   case 'C': // color
//   case 'X': // texcoord (3 dimensional)
//   case 'B': // binormal
//      return sizeof(float) * 3;
//   default:
//      return 0;
//   }
//}

class Vertex
{
private:
   //byte *vertex; // generic vertex format ptr for allocating an arbitrary vertex format

   vector<VertexComponent> components; // vertex configuration and data
   static int32 numBytes;
   static bool isPacked;
   static int32 *vertCompOffsets; // offset for each component
   static eDataType dataType;
   //const static int32 FORMAT_DESCRIPTOR_LEN;
   static core::string::String_c formatDescriptor;
public:
   // format description is composed of characters used for vertex formats: e.g "PNT" for position, normal, texture, respectively
   // this inits shared (static) vars only, and should be called only once
   static void Init(/*const char *formatDescriptor,*/ eDataType dataType = DATATYPE_FLOAT​​);


   // add component, the class will make a format descriptor based on the creation
   void AddComponent( VertexComponent & );
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
//   String_c formatDescriptor;
//   uint32 texCoordCount;
//   uint32 sizeInBytes;
//   vector<VertexComponent> components; // list of components for each vertex
//public:
//   Vertex();
//   Vertex( const String_c &formatDescription );
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

