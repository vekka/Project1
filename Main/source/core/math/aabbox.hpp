#ifndef _AABBOX_HPP_INCLUDED_
#define _AABBOX_HPP_INCLUDED_

#include "mathcommon.hpp"

#include "line3.hpp"
#include "plane.hpp"

namespace core
{

   namespace math
   {

      template <class T>
      class AABBox
      {
      private:
         Point3<T> minEdge;
         Point3<T> maxEdge;
      public:
         AABBox();
         AABBox(const Point3<T> &min, const Point3<T> &max);
         AABBox(const Point3<T> &); // one-point constructor
         AABBox(const T minX, const T minY, const T minZ, const T maxX, const T maxY, const T maxZ);

         bool operator==(const AABBox &other) const;
         bool operator!=(const AABBox &other) const;

         void Reset(const T x, const T y, const T z); // reset to one-point box
         void Reset(const Point3<T> &); // reset to one-point box
         void Reset(const AABBox &initValue);
         void AddInternalPoint(const T x, const T y, const T z);
         Point3<T> GetCenter() const;
         Point3<T> GetExtent() const; // get maximal distance of two points in the box
         bool IsEmpty() const;
         T GetVolume() const;
         T GetArea() const;
         void GetEdges(Point3<T> *edges) const;
         void Repair(); // necessary if for example MinEdge and MaxEdge are swapped.
         AABBox GetInterpolated(const AABBox &other, float d) const;
         bool IsPointInside(const Point3<T> &) const; // including edges!
         bool IsPointTotalInside(const Point3<T> &) const; // excluding edges!
         bool IsCompletelyInside(const AABBox &other) const;
         bool IntersectsWith(const AABBox &other) const;
         bool IntersectsWithLine(const Line3<T> &) const;
         // tests if the box intersects with a line with the center of the line as param
         bool IntersectsWithLine(const Point3<T> &lineMiddle, const Vector3<T> &lineVec, const T halfLength) const;

         // TODO: ????
         //eIntersectionRelation ClassifyPlaneRelation( const Plane3<T> & ) const;
      };

      typedef AABBox<int32> AABBox_i;
      typedef AABBox<float> AABBox_f;
      typedef AABBox<double> AABBox_d;

      template <class T>
      AABBox<T>::AABBox() : minEdge(-1, -1, -1), maxEdge(1, 1, 1)
      {
      }

      template <class T>
      AABBox<T>::AABBox(const Point3<T> &min, const Point3<T> &max)
         : minEdge(min), maxEdge(max)
      {
      }

      template <class T>
      AABBox<T>::AABBox(const Point3<T> &point)
         : minEdge(point), maxEdge(point)
      {
      }

      template <class T>
      AABBox<T>::AABBox(const T minX, const T minY, const T minZ, const T maxX, const T maxY, const T maxZ)
         : minEdge(minX, minY, minZ), maxEdge(maxX, maxY, maxZ)
      {
      }

      template <class T>
      inline bool AABBox<T>::operator==(const AABBox<T> &other) const
      {
         return (minEdge == other.minEdge && other.maxEdge == maxEdge);
      }

      template <class T>
      inline bool AABBox<T>::operator!=(const AABBox<T> &other) const
      {
         return !(minEdge == other.minEdge && other.maxEdge == maxEdge);
      }

      template <class T>
      void AABBox<T>::Reset(T x, T y, T z)
      {
         maxEdge.set(x, y, z);
         minEdge = maxEdge;
      }

      template <class T>
      void AABBox<T>::Reset(const Point3<T> &point)
      {
         MaxEdge = point;
         MinEdge = point;
      }

      template <class T>
      inline void AABBox<T>::Reset(const AABBox<T> &initValue)
      {
         *this = initValue;
      }

      template <class T>
      inline void AABBox<T>::AddInternalPoint(const T x, const T y, const T z)
      {
         if (x > maxEdge[0]) maxEdge[0] = x;
         if (y > maxEdge[1]) maxEdge[1] = y;
         if (z > maxEdge[2]) maxEdge[2] = z;

         if (x < minEdge[0]) minEdge[0] = x;
         if (y < minEdge[1]) minEdge[1] = y;
         if (z < minEdge[2]) minEdge[2] = z;
      }

      template <class T>
      inline Point3<T> AABBox<T>::GetCenter() const
      {
         return (minEdge + maxEdge) / 2;
      }

      template <class T>
      inline Point3<T> AABBox<T>::GetExtent() const
      {
         return maxEdge - minEdge;
      }

      template <class T>
      inline bool AABBox<T>::IsEmpty() const
      {
         return minEdge.Equals(maxEdge);
      }

      template <class T>
      inline T AABBox<T>::GetVolume() const
      {
         const Point3<T> e = GetExtent();
         return e[0] * e[1] * e[2];
      }

      template <class T>
      inline T AABBox<T>::GetArea() const
      {
         const Point3<T> e = GetExtent();
         return 2 * (e[0] * e[1] + e[0] * e[2] + e[1] * e[2]);
      }

      template <class T>
      void AABBox<T>::GetEdges(Point3<T> *edges) const
      {
         const Vector3<T> middle = GetCenter();
         const Vector3<T> diag = middle - maxEdge;

         /*
         Edges are stored in this way:
         Hey, am I an ascii artist, or what? :) niko.
         /3--------/7
         / |       / |
         /  |      /  |
         1---------5   |
         |  /2- - -|- -6
         | /       |  /
         |/        | /
         0---------4/
         */

         edges[0].Set(middle[0] + diag[0], middle[1] + diag[1], middle[2] + diag[2]);
         edges[1].Set(middle[0] + diag[0], middle[1] - diag[1], middle[2] + diag[2]);
         edges[2].Set(middle[0] + diag[0], middle[1] + diag[1], middle[2] - diag[2]);
         edges[3].Set(middle[0] + diag[0], middle[1] - diag[1], middle[2] - diag[2]);
         edges[4].Set(middle[0] - diag[0], middle[1] + diag[1], middle[2] + diag[2]);
         edges[5].Set(middle[0] - diag[0], middle[1] - diag[1], middle[2] + diag[2]);
         edges[6].Set(middle[0] - diag[0], middle[1] + diag[1], middle[2] - diag[2]);
         edges[7].Set(middle[0] - diag[0], middle[1] - diag[1], middle[2] - diag[2]);
      }

      template <class T>
      void AABBox<T>::Repair()
      {
         if (minEdge[0] > maxEdge[0])
            core::Swap(minEdge[0], maxEdge[0]);
         if (minEdge[1] > maxEdge[1])
            core::Swap(minEdge[1], maxEdge[1]);
         if (minEdge[2] > maxEdge[2])
            core::Swap(minEdge[2], maxEdge[2]);
      }

      // Calculates a new interpolated bounding box.
      // d=0 returns other, d=1 returns this, all other values blend between
      template <class T>
      AABBox<T> AABBox<T>::GetInterpolated(const AABBox<T> &other, const float d) const
      {
         assert(d >= 0.0f && d < 1.0f);

         float inv = 1.0f - d;
         return AABBox<T>((other.minEdge*inv) + (minEdge*d),
            (other.maxEdge*inv) + (maxEdge*d));
      }

      template <class T>
      bool AABBox<T>::IsPointInside(const Point3<T> &point) const
      {
         return (
            point[0] >= minEdge[0] && point[0] <= maxEdge[0] &&
            point[1] >= minEdge[1] && point[1] <= maxEdge[1] &&
            point[2] >= minEdge[2] && point[2] <= maxEdge[2]);
      }

      template <class T>
      bool AABBox<T>::IsPointTotalInside(const Point3<T> &point) const
      {
         return (
            point[0] > minEdge[0] && point[0] < maxEdge[0] &&
            point[1] > minEdge[1] && point[1] < maxEdge[1] &&
            point[2] > minEdge[2] && point[2] < maxEdge[2]);
      }

      template <class T>
      bool AABBox<T>::IsCompletelyInside(const AABBox<T> &other) const
      {
         return (
            minEdge[0] >= other.minEdge[0] &&
            minEdge[1] >= other.minEdge[1] &&
            minEdge[2] >= other.minEdge[2] &&
            maxEdge[0] <= other.maxEdge[0] &&
            maxEdge[1] <= other.maxEdge[1] &&
            maxEdge[2] <= other.maxEdge[2]);
      }

      template <class T>
      bool AABBox<T>::IntersectsWith(const AABBox<T> &other) const
      {
         return (
            minEdge[0] <= other.maxEdge[0] &&
            minEdge[1] <= other.maxEdge[1] &&
            minEdge[2] <= other.maxEdge[2] &&
            maxEdge[0] >= other.minEdge[0] &&
            maxEdge[1] >= other.minEdge[1] &&
            maxEdge[2] >= other.minEdge[2]);
      }

      template <class T>
      bool AABBox<T>::IntersectsWithLine(const Line3<T> &line) const
      {
         return IntersectsWithLine(
            line.GetMiddle(), line.GetVector().Normalize(), (T)(line.GetLength() * 0.5));
      }

      template <class T>
      bool AABBox<T>::IntersectsWithLine(const Point3<T> &lineMiddle,
         const Vector3<T> &lineVec, const T halfLength) const
      {
         const Point3<T> extent = GetExtent() * (T)0.5;
         const Point3<T> center = GetCenter() - lineMiddle;

         if (
            (fabs(center[0]) > extent[0] + halfLength * fabs(lineVec[0])) ||
            (fabs(center[1]) > extent[1] + halfLength * fabs(lineVec[1])) ||
            (fabs(center[2]) > extent[2] + halfLength * fabs(lineVec[2])))
            return false;

         T r = extent[1] * (T)fabs(lineVec[2]) + extent[2] * (T)fabs(lineVec[1]);
         if (fabs(center[1] * lineVec[2] - center[2] * lineVec[1]) > r)
            return false;

         r = extent[0] * (T)fabs(lineVec[2]) + extent[2] * (T)fabs(lineVec[0]);
         if (fabs(center[2] * lineVec[0] - center[0] * lineVec[2]) > r)
            return false;

         r = extent[0] * (T)fabs(lineVec[1]) + extent[1] * (T)fabs(lineVec[0]);
         if (fabs(center[0] * lineVec[1] - center[1] * lineVec[0]) > r)
            return false;

         return true;
      }

      // TODO: ????? enum problem

      //template <class T, class TAlloc>
      //eIntersectionRelation AABBox<T>::ClassifyPlaneRelation( const Plane3<T> &plane ) const
      //{
      //   Point3<T> nearPoint(maxEdge);
      //   Point3<T> farPoint(minEdge);
      //
      //   if (plane.normal[0] > (T)0)
      //   {
      //      nearPoint[0] = minEdge[0];
      //      farPoint[0] = maxEdge[0];
      //   }
      //
      //   if (plane.normal[1] > (T)0)
      //   {
      //      nearPoint[1] = minEdge[1];
      //      farPoint[1] = maxEdge[1];
      //   }
      //
      //   if (plane.normal[2] > (T)0)
      //   {
      //      nearPoint[2] = minEdge[0];
      //      farPoint[2] = maxEdge[0];
      //   }
      //
      //   if (plane.normal * nearPoint + plane.distance > (T)0)
      //      return INTERSECT_FRONT;
      //
      //   if (plane.normal * farPoint + plane.distance > (T)0)
      //      return INTERSECT_CLIPPED;
      //
      //   return INTERSECT_BACK;
      //}

   } // namespace math

} // namespace core

#endif