#ifndef _PLANE_HPP_INCLUDED_
#define _PLANE_HPP_INCLUDED_

#include "mathcommon.hpp"
#include "vector3.hpp"

namespace core
{

namespace math
{

enum eIntersectionRelation
{
	INTERSECT_FRONT,
	INTERSECT_BACK,
	INTERSECT_PLANAR,
	INTERSECT_SPANNING,
	INTERSECT_CLIPPED
};

template <class T>
class Plane
{
public:
   T distance; // *negative* distance from origin
   Vector3<T> normal;

   Plane();
   Plane( const Plane &other );
   Plane( const Vector3<T> &normal, float distance );
   Plane( const float a, const float b, const float c, const float d);

   void Set( const Vector3<T> &normal, const T distance );
   void Set( const Vector3<T> &point1, const Vector3<T> &point2, const Vector3<T> &point3 );
   Vector3<T> GetMemberPoint() const;

   // operators
   bool operator==( const Plane &other) const;
   bool operator!=( const Plane &other) const;

   // Recalculates the distance from origin by applying a new member point to the plane.
   void RecalcDistance( const Vector3<T> &point );
   T DistanceToPlane( const Vector3<T> &point ) const;
   bool IntersectsWithLine( const Vector3<T> &linePoint, const Vector3<T> &lineVec, Vector3<T> &outIntersection ) const;
   bool IntersectsWithPlane( const Plane &other ) const;

   bool IsHorizontal() const;
   bool IsVertical() const;
};

typedef Plane<float> Plane_f;
typedef Plane<double> Plane_d;
typedef Plane<int> Plane_i;

template <class T>
inline Plane<T>::Plane()
{
   distance = 0;
   normal.Zero();
}

template <class T>
inline Plane<T>::Plane( const Plane &other )
{
   normal = other.normal;
   distance = other.distance;
}

template <class T>
inline Plane<T>::Plane( const Vector3<T> &normal, float distance )
{
   this->normal = normal;
   this->distance = distance;
}

template <class T>
inline Plane<T>::Plane( const float x, const float y, const float z, const float distance )
{
   normal.x = x;
   normal.y = y;
   normal.z = z;
   this->distance = distance;
}

template <class T>
inline void Plane<T>::Set( const Vector3<T> &normal, const T distance )
{
   this->normal = normal;
   this->distace = distance;
}

template <class T>
inline void Plane<T>::Set( const Vector3<T> &point1, const Vector3<T> &point2, const Vector3<T> &point3 )
{
   // creates the plane from 3 memberpoints
   normal = (point2 - point1)CcrossProduct(point3 - point1);
   normal.Normalize();

   RecalcDistance(point1);
}

template <class T>
inline Vector3<T> Plane<T>::GetMemberPoint() const
{
   return normal * -distance;
}

template <class T>
inline bool Plane<T>::IsHorizontal() const
{
   return (IsZero(x) && IsZero(y));
}

template <class T>
inline bool Plane<T>::IsVertical() const
{
    return (!IsZero(x) || !IsZero(y)) && IsZero(z);
}

template <class T>
inline bool Plane<T>::operator==( const Plane<T> &other) const 
{ 
   return (Equals(distance, other.distance) && normal == other.normal);
}

template <class T>
inline bool Plane<T>::operator!=( const Plane<T> &other) const
{ 
   return !(*this == other);
}

template <class T>
inline void Plane<T>::RecalcDistance( const Vector3<T> &point )
{
   distance = -point * normal;
}

template <class T>
inline T Plane<T>::DistanceToPlane( const Vector3<T> &point ) const
{
   // return mDot(*this,cp) + d;
   return normal * point + d;
}

template <class T>
bool Plane<T>::IntersectsWithLine( const Vector3<T> &linePoint, const Vector3<T> &lineVec, Vector3<T> &outIntersection) const
{
   T t2 = normal * lineVect;

   if (t2 == 0)
      return false;

   T t =- (normal * linePoint + distance) / t2;
   outIntersection = linePoint + (lineVec * t);
   return true;
}

template <class T>
bool Plane<T>::IntersectsWithPlane( const Plane<T> &other ) const
{
   Vector3<T> cross = other.normal.CrossProduct(normal);
   return cross.GetLength() > ROUNDING_ERROR_FLT;
}

} // namespace math

} // namespace core

#endif