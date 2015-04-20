#ifndef _VECTOR2_HPP_INCLUDED_
#define _VECTOR2_HPP_INCLUDED_

#include "mathcommon.hpp" // Swap(), Equals()

#include "point2.hpp"

namespace core
{

namespace math
{

enum eConstructorVector2
{ 
   VEC2_CONST_NOTHING,
   VEC2_CONST_ZERO,
   VEC2_CONST_UNIT_X,
   VEC2_CONST_UNIT_Y,
   VEC2_CONST_NEG_UNIT_X,
   VEC2_CONST_NEG_UNIT_Y,
   VEC2_CONST_UNIT_SCALE
};

template <class T>
class Vector2 : public Point2<T>
{
public:
   using Point2<T>::operator=;

   //----------- constructors
   Vector2();
   Vector2( const eConstructorVector2 constructor );
   Vector2( const T x, const T y );
   Vector2( const T scaler );
   
   //----------- mutators
   void SetUnitX();
   void SetUnitY();
   void SetNegUnitX();
   void SetNegUnitY();
   void SetUnitScale();

   T operator*( const Vector2 &other ) const;
   T DotProd( const Vector2 &other ) const;
   T CrossProd( const Vector2 &other ) const;
   T SqLength() const;
   T Length() const;
   T Normalize();
   Vector2 CreatePerpendicularVec() const;
};

typedef Vector2<int16> Vector2s;
typedef Vector2<int32> Vector2i;
typedef Vector2<uint16> Vector2us;
typedef Vector2<uint32> Vector2u;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<long double> Vector2ld; // TODO: replace with new type

template <class T>
inline Vector2<T>::Vector2()
{
}

template <class T>
inline Vector2<T>::Vector2( const eConstructorVector2 constructor )
{
   switch ( eConstructorVector2 )
   {
   case CONST_NOTHING:
      break;
   case CONST_ZERO:
      Zero();
      break;
   case CONST_UNIT_X:
      SetUnitX();
      break;
   case CONST_UNIT_Y:
      SetUnitY();
      break;
   case CONST_NEG_UNIT_X:
      SetNegUnitX();
      break;
   case CONST_NEG_UNIT_Y:
      SetNegUnitY();
      break;
   case CONST_UNIT_SCALE:
      SetUnitScale();
      break;
   default:
      break;
   }
}

template <class T>
inline Vector2<T>::Vector2( const T x, const T y )
{
	this->x = x;
	this->y = y;
}

template <class T>
inline void Vector2<T>::SetUnitX()
{
   x = (T)1; y = (T)0;
}

template <class T>
inline void Vector2<T>::SetUnitY()
{
   x = (T)0; y = (T)1;
}

template <class T>
inline void Vector2<T>::SetNegUnitX()
{
   x = (T)-1; y = (T)0;
}

template <class T>
inline void Vector2<T>::SetNegUnitY()
{
   x = (T)0; y = (T)-1;
}

template <class T>
inline void Vector2<T>::SetUnitScale()
{
   x = (T)1; y = (T)1;
}

template <class T>
inline T Vector2<T>::operator*( const Vector2<T> &other ) const
{
	return x * other.x + y * other.y;
}

template <class T>
inline T Vector2<T>::DotProd( const Vector2<T> &other ) const
{
	return x * other.x + y * other.y;
}

template <class T>
inline T Vector2<T>::CrossProd( const Vector2<T> &other ) const
{
	return x * other.y - y * other.x;
}

template <class T>
inline T Vector2<T>::SqLength() const
{
	return x * x + y * y;
}

template <class T>
inline T Vector2<T>::Length() const
{
	return sqrt( x * x + y * y );
}

template <class T>
inline T Vector2<T>::Normalize()
{
	float length;
	float invlength;

	length = this->Length();
	if ( length ) {
		invlength = 1.0f / length;
		x *= invlength;
		y *= invlength;
		z *= invlength;
	}
		
	return length;
}

template <class T>
inline Vector2<T> Vector2<T>::CreatePerpendicularVec() const
{
   return Vector2<T>( -y, x );
}
 
} // namespace math

} // namespace core

#endif