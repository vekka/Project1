#ifndef _VERTEXSTRUCTS_HPP_INCLUDED_
#define _VERTEXSTRUCTS_HPP_INCLUDED_

#include "core/math/vector2.hpp"
#include "core/math/vector3.hpp"
using core::math::Vector2;
using core::math::Vector3;
using core::math::Vector3f;

namespace vertexstructs
{

   class Vertex
   {

   };

// this source is intended for a more simplistic indexing of vertex data, than the more generic approach (vertexformat.hpp) of
// creating vertex formats arbitrarly

   template <typename T>
   struct VertexPC
   {
      Vector3<T> position;
      Vector3<T> color;
   };

   template <typename T>
   struct VertexPNT
   {
      Vector3<T> position;
      Vector3<T> normal;
      Vector2<T> tex2coord;
      bool operator<(const VertexPNT &other) const { return memcmp((void*)this, (void*)&other, sizeof(VertexPNT))>0; }
   };

   template <typename T>
   struct VertexPN
   {
      Vector3<T> position;
      Vector3<T> normal;
   };

   template <typename T>
   struct VertexPT
   {
      Vector3<T> position;
      Vector2<T> tex2coord;
   };

} // namespace vertexstructs

#endif