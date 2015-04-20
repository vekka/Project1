#ifndef _VERTEXSTRUCTS_HPP_INCLUDED_
#define _VERTEXSTRUCTS_HPP_INCLUDED_

#include "core/math/vector2.hpp"
#include "core/math/vector3.hpp"
using core::math::Vector2;
using core::math::Vector3;
using core::math::Vector3f;

#include "core/types.hpp"

namespace vertexstructs
{
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
   };

} // namespace vertexstructs

#endif