#ifndef _VECTOR3_HPP_INCLUDED_
#define _VECTOR3_HPP_INCLUDED_

#include "point3.hpp"

namespace core
{

namespace math
{

enum eConstructorVector3
{ 
   VEC3_CONST_NOTHING,
   VEC3_CONST_ZERO,
   VEC3_CONST_UNIT_X,
   VEC3_CONST_UNIT_Y,
   VEC3_CONST_UNIT_Z,
   VEC3_CONST_NEG_UNIT_X,
   VEC3_CONST_NEG_UNIT_Y,
   VEC3_CONST_NEG_UNIT_Z,
   VEC3_CONST_UNIT_SCALE
};

template <class T>
class Vector3 : public Point3<T>
{
public:
   using Point3<T>::operator=;

   //----------- constructors
   Vector3();
   Vector3( const eConstructorVector3 constructor );
   Vector3( const T x, const T y, const T z );
   Vector3( const T scaler );

   void SetUnitX();
   void SetUnitY();
   void SetUnitZ();
   void SetNegUnitX();
   void SetNegUnitY();
   void SetNegUnitZ();
   void SetUnitScale();

   T operator*( const Vector3<T> &other ) const;
   T DotProd( const Vector3<T> &other ) const;
   Vector3 CrossProd( const Vector3 &other ) const;
   T SqLength() const;
   T Length() const;
   T Normalize();
   bool IsUnit();
};

typedef Vector3<int16> Vector3s;
typedef Vector3<int32> Vector3i;
typedef Vector3<uint16> Vector3us;
typedef Vector3<uint32> Vector3u;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
typedef Vector3<long double> Vector3ld; // TODO: replace with new type

template <class T>
inline Vector3<T>::Vector3()
{
}

template <class T>
inline Vector3<T>::Vector3( const eConstructorVector3 constructor )
{
   switch ( constructor )
   {
   case VEC3_CONST_NOTHING:
      break;
   case VEC3_CONST_ZERO:
      Zero();
      break;
   case VEC3_CONST_UNIT_X:
      SetUnitX();
      break;
   case VEC3_CONST_UNIT_Y:
      SetUnitY();
      break;
   case VEC3_CONST_UNIT_Z:
      SetUnitZ();
      break;
   case VEC3_CONST_NEG_UNIT_X:
      SetNegUnitX();
      break;
   case VEC3_CONST_NEG_UNIT_Y:
      SetNegUnitY();
      break;
   case VEC3_CONST_NEG_UNIT_Z:
      SetNegUnitZ();
      break;
   case VEC3_CONST_UNIT_SCALE:
      SetUnitScale();
      break;
   default:
      break;
   }
}

template <class T>
inline Vector3<T>::Vector3( const T x, const T y, const T z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

template <class T>
inline void Vector3<T>::SetUnitX()
{
   x = (T)1; y = (T)0; z = (T)0;
}

template <class T>
inline void Vector3<T>::SetUnitY()
{
   x = (T)0; y = (T)1; z = (T)0;
}

template <class T>
inline void Vector3<T>::SetUnitZ()
{
   x = (T)0; y = (T)0; z = (T)1;
}

template <class T>
inline void Vector3<T>::SetNegUnitX()
{
   x = (T)-1; y = (T)0; z = (T)0;
}

template <class T>
inline void Vector3<T>::SetNegUnitY()
{
   x = (T)0; y = (T)-1; z = (T)0;
}

template <class T>
inline void Vector3<T>::SetNegUnitZ()
{
   x = (T)0; y = (T)0; z = (T)-1;
}

template <class T>
inline void Vector3<T>::SetUnitScale()
{
   x = (T)1; y = (T)1; z = (T)1;
}

template <class T>
inline T Vector3<T>::operator*( const Vector3<T> &other ) const
{
	return x * other.x + y * other.y + z * other.z;
}

template <class T>
inline T Vector3<T>::DotProd( const Vector3<T> &other ) const
{
	return x * other.x + y * other.y + z * other.z;
}

template <class T>
inline Vector3<T> Vector3<T>::CrossProd( const Vector3<T> &other ) const
{
	return Vector3<T>( 
      y * other.z - z * other.y,
      z * other.x - x * other.z,
      x * other.y - y * other.x );
}

template <class T>
inline T Vector3<T>::SqLength() const
{
	return x * x + y * y + z * z;
}

template <class T>
inline T Vector3<T>::Length() const
{
	return (T)sqrt( (float)(x * x + y * y + z * z) );
}

template <class T>
inline T Vector3<T>::Normalize()
{
	T length;
	T invlength;

	length = this->Length();
	if ( length ) {
		invlength = (T)1 / length;
		x *= invlength;
		y *= invlength;
		z *= invlength;
	}
		
	return length;
}

template <class T>
bool Vector3<T>::IsUnit()
{
   return core::Equals( (T)(x * x + y * y + z * z), (T)1 );
}

} // namespace math

} // namespace core

#endif