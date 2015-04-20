#ifndef _LINE3_HPP_INCLUDED_
#define _LINE3_HPP_INCLUDED_

#include "vector3.hpp"

namespace core
{

namespace math
{

template <class T>
class Line3
{
private:
   Vector3<T> start;
   Vector3<T> end;
public:
   Line3();
   Line3( const T startX, const T startY, const T startZ,
      const T endX, const T endY, const T endZ );
   Line3( const Vector3<T> &start, const Vector3<T> &end );
   Vector3<T> GetMiddle() const;

   /* operators */
   Line3 operator+( const Vector3<T> &point ) const;
   Line3 &operator+=( const Vector3<T> &point );
   Line3 operator-( const Vector3<T> &point ) const;
   Line3 &operator-=( const Vector3<T> &point );
   bool operator==( const Line3 &other ) const;
   bool operator!=( const Line3 &other ) const;

   Vector3<T> GetVector() const;
   Vector3<T> GetUnitVector() const;
   void Set( const T &xa, const T &ya, const T &za, const T &xb, const T &yb, const T &zb );
   void Set( const Vector3<T> &start, const Vector3<T> &end );
   void Set( const Line3 &line );
};

typedef Line3<float> Line3f;
typedef Line3<double> Line3d;

template <class T>
inline Line3<T>::Line3()
{
}

template <class T>
inline Line3<T>::Line3( const T startX, const T startY, const T startZ,
   const T endX, const T endY, const T endZ )
{
   start( startX, startY, startZ );
   end( endX, endY, endZ );
}

template <class T>
inline Line3<T>::Line3( const Vector3<T> &start, const Vector3<T> &end )
{
   this->start = start;
   this->end = end;
}

template <class T>
inline Line3<T> Line3<T>::operator+( const Vector3<T> &point ) const
{ 
   return Line3<T>( start + point, end + point );
}

template <class T>
inline Line3<T> &Line3<T>::operator+=( const Vector3<T> &point )
{ 
   start += point;
   end += point;
   return *this;
}

template <class T>
inline Line3<T> Line3<T>::operator-( const Vector3<T> &point ) const
{ 
   return Line3<T>( start - point, end - point );
}

template <class T>
inline Line3<T> &Line3<T>::operator-=( const Vector3<T> &point )
{ 
   start -= point;
   end -= point;
   return *this;
}

template <class T>
inline bool Line3<T>::operator==( const Line3<T> &other ) const
{ 
   return ( start == other.start && end == other.end ) ||
      ( end == other.start && start == other.end );
}

template <class T>
inline bool Line3<T>::operator!=( const Line3<T> &other ) const
{ 
   return !( start == other.start && end == other.end ) ||
      ( end == other.start && start == other.end );
}

template <class T>
inline Vector3<T> Line3<T>::GetVector() const
{ 
   return Vector3<T>( end.X - start.X, end.Y - start.Y );
}

template <class T>
inline Vector3<T> Line3<T>::GetUnitVector() const
{
   T len = (T)(1 / Length());
   return Vector3<T>( (end.X - start.X) * len, (end.Y - start.Y) * len, (end.Z - start.Z) * len );
}

template <class T>
inline Vector3<T> Line3<T>::GetMiddle() const
{
   return (start + end)/(T)2;
}

template <class T>
inline void Set( const T &xa, const T &ya, const T &za, const T &xb, const T &yb, const T &zb )
{
   start.Set(xa, ya, za);
   end.set(xb, yb, zb);
}

template <class T>
inline void Set( const Vector3<T> &start, const Vector3<T> &end )
{
   start.Set(start);
   end.Set(end);
}

template <class T>
inline void Set( const Line3<T> &other )
{
   start.Set(other.start);
   end.Set(line.end);
}

} // namespace math

} // namespace core

#endif