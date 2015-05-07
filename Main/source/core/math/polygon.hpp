#ifndef _POLYGON_HPP_INCLUDED_
#define _POLYGON_HPP_INCLUDED_

#include "vector3.hpp"

namespace core
{

   namespace math
   {

      template <class T>
      class Polygon
      {
      protected:
         // vertexList;
      public:
         Polygon();
         Polygon(const Polygon &other);
         ~Polygon();

         void InsertVertex(const Point3<T> &vertex, const uint32 pos);
         void AppendVertex(const Point3<T> &vertex);

      };

   } // namespace math

} // namespace core

#endif