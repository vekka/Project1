#include "vertexformat.hpp"

using core::math::Point3f;
using core::math::Vector2f;
using core::math::Vector3f;

namespace vertexformat
{

//eDataType VertexComponent::dataType;
//
//VertexComponent::VertexComponent(char vertexComponentType, eDataType dataType, byte *data)
//{
//   assert(dataType < DATATYPE_ENUM_SIZE);
//
//   componentType = vertexComponentType;
//   this->dataType = dataType;
//
//   if (componentType == 'T') // 2d texture contains two half_floats,floats, double... etc.
//      size = GetDataTypeSizeInBytes(dataType) * 2;
//   else
//      size = GetDataTypeSizeInBytes(dataType) * 3;
//
//   this->data = new byte[size];
//
//   std::memmove(this->data, data, size);
//
//   //vertexComponentType = componentType; this->dataType = dataType;
//}
//
//int32 VertexComponent::GetSizeInBytes()
//{
//   return size;
//}
//
//VertexComponent::~VertexComponent()
//{
//   //delete[] this->data;  // where should delete
//}


//void Vertex::AddComponent(VertexComponent &c)
//{
//   components.push_back(c);
//   formatDescriptor.Append(c.GetComponentType());
//}

// int32 Vertex::GetNumBytes()
// {
//    if (numBytes == 0)
//    {
//       for (int32 i = 0; Vertex::formatDescriptor[i]; i++)
//       {
//          if (Vertex::isPacked) // is vertex packed?
//          {
//             //assert(temp != 0);
//             numBytes += components[i].GetSizeInBytes();
//          }
//          else
//          {
//          }
//       }
//    }
//   return numBytes;
//}

 //int32 *Vertex::GetOffsets()
 //{
 //   if (vertCompOffsets == 0)
 //   {
 //      vertCompOffsets = new int32[strlen(formatDescriptor)];
 //      if (Vertex::isPacked)
 //      {
 //         vertCompOffsets[0] = 0;
 //         int32 temp = 0;
 //         for (int32 i = 1; Vertex::formatDescriptor[i]; i++)
 //         {
 //            //assert(temp != 0);
 //            temp += components[i].GetSizeInBytes();
 //            vertCompOffsets[i] = temp;
 //         }
 //      }
 //      else
 //      {
 //      }
 //   }
 //   return vertCompOffsets;
 //}

////void Vertex::CreateVertexFormat(eDataType dataType)
//void Vertex::CreateVertex(eDataType dataType, byte* data)
//{
//   components.reserve(strlen(formatDescriptor));
//   for (int32 i = 0; formatDescriptor[i]; i++)
//   {
//      components.push_back(VertexComponent(formatDescriptor[i], dataType, data+i));
//   }
//}

//void *Vertex::operator[](char vertexComponentType) const
//{
//}
//
//void *&Vertex::operator[](char vertexComponentType)
//{
//   for (int32 i = 0; formatDescriptor[i]; i++)
//   {
//      if (formatDescriptor[i] == 'T')
//
//   }
//}

Vertex::~Vertex()
{
   if (vertCompOffsets != 0)
      delete[] vertCompOffsets;
}

} // namespace vertexformat

//VertexComponent::VertexComponent() 
//{
//
//}
//
//VertexComponent::VertexComponent( const VertexComponent &other ) 
//{
//   *this = other;
//}
//
//
//Vertex::Vertex()
//   :isDirty(true), hasColor(false), hasNormal(false), texCoordCount(0), hasTangent(false), sizeInBytes(0)
//{
//}
//
//Vertex::Vertex( const String_c &formatDescriptor )
//{
//   this->formatDescriptor = formatDescriptor;
//}
//
//Vertex::Vertex( const Vertex &other )
//{
//   *this = other;
//}
//
//void Vertex::Append( const Vertex &other, const int32 inStreamIndex ) 
//{
//   // TODO: find more about this later
//   // I think this function is about connecting vertices together. Agree?
//   for( uint32 i = 0; i < components.size(); i ++ )
//   {
//      if( !components.empty() )
//         components.back() = other.GetComponent( i );
//      if( inStreamIndex != -1 )
//         components.back().SetStreamIndex( inStreamIndex );
//   }
//   this->isDirty = true;
//}
//
//void Vertex::Clear()
//{
//   isDirty = true;
//   components.clear();
//}
//
//void Vertex::CopyFrom( const Vertex &other )
//{
//   isDirty = other.isDirty;
//   sizeInBytes = other.sizeInBytes;
//   components = other.components;
//}
//
//void Vertex::UpdateDirty()
//{
//
//   for( uint32 i = 0; i < components.size(); i++ )
//   {
//      const VertexComponent &component = components[i];
//      sizeInBytes += component.GetSizeInBytes();
//   }
//   isDirty = false;
//}
//
//uint32 Vertex::GetSizeInBytes() const
//{
//   if( isDirty )
//      const_cast<Vertex*>(this)->UpdateDirty();
//   return sizeInBytes;
//}