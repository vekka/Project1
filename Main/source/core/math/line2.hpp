#ifndef _LINE2_HPP_INCLUDED_
#define _LINE2_HPP_INCLUDED_

#include "vector2.hpp"

namespace core
{

namespace math
{

template <class T>
class Line2
{
private:
   Vector2<T> start;
   Vector2<T> end;
public:
   Line2();
   Line2( const T startX, const T startY, const T endX, const T endY );
   Line2( const Vector2<T> &start, const Vector2<T> &end );
   Vector2<T> GetMiddle() const;

   /* operators */
   Line2 operator+( const Vector2<T> &point ) const;
   Line2 &operator+=( const Vector2<T> &point );
   Line2 operator-( const Vector2<T> &point ) const;
   Line2 &operator-=( const Vector2<T> &point );
   bool operator==( const Line2 &other ) const;
   bool operator!=( const Line2 &other ) const;

   Vector2<T> GetVector() const;
   Vector2<T> GetUnitVector() const;
   void Set( const T &xa, const T &ya, const T &xb, const T &yb );
   void Set( const Vector2<T> &start, const Vector2<T> &end );
   void Set( const Line2 &other );
};

typedef Line2<float> Line2f;
typedef Line2<double> Line2d;

template <class T>
inline Line2<T>::Line2()
{
}

template <class T>
inline Line2<T>::Line2( const T startX, const T startY, const T endX, const T endY )
{
   start( startX, startY );
   end( endX, endY );
}

template <class T>
inline Line2<T>::Line2( const Vector2<T> &start, const Vector2<T> &end )
{
   this->start = start;
   this->end = end;
}

template <class T>
inline Line2<T> Line2<T>::operator+( const Vector2<T> &point ) const
{ 
   return Line2<T>( start + point, end + point );
}

template <class T>
inline Line2<T> &Line2<T>::operator+=( const Vector2<T> &point )
{ 
   start += point;
   end += point;
   return *this;
}

template <class T>
inline Line2<T> Line2<T>::operator-( const Vector2<T> &point ) const
{ 
   return Line2<T>( start - point, end - point );
}

template <class T>
inline Line2<T> &Line2<T>::operator-=( const Vector2<T> &point )
{ 
   start -= point;
   end -= point;
   return *this;
}

template <class T>
inline bool Line2<T>::operator==( const Line2<T> &other ) const
{ 
   return ( start == other.start && end == other.end ) ||
      ( end == other.start && start == other.end );
}

template <class T>
inline bool Line2<T>::operator!=( const Line2<T> &other ) const
{ 
   return !( start == other.start && end == other.end ) ||
      ( end == other.start && start == other.end );
}

template <class T>
inline Vector2<T> Line2<T>::GetVector() const
{ 
   return Vector2<T>( end.X - start.X, end.Y - start.Y );
}

template <class T>
inline Vector2<T> Line2<T>::GetUnitVector() const
{
   T len = (T)(1 / Length());
   return Vector2<T>( (end.X - start.X) * len, (end.Y - start.Y) * len );
}

template <class T>
Vector2<T> Line2<T>::GetMiddle() const
{
   return (start + end)/(T)2;
}

template <class T>
inline void Line2<T>::Set( const T &xa, const T &ya, const T &xb, const T &yb )
{
   start.Set(xa, ya);
   end.set(xb, yb);
}

template <class T>
inline void Line2<T>::Set( const Vector2<T> &start, const Vector2<T> &end )
{
   start.Set(start);
   end.Set(end);
}

template <class T>
inline void Line2<T>::Set( const Line2<T> &other )
{
   start.Set(other.start);
   end.Set(line.end);
}

} // namespace math

} // namespace core

#endif