#ifndef _VECTOR4_HPP_INCLUDED_
#define _VECTOR4_HPP_INCLUDED_

#include "mathcommon.hpp"

namespace core
{

namespace math
{

enum eConstructorVector4
{ 
   VEC4_CONST_NOTHING,
   VEC4_CONST_ZERO
};

template <class T>
class Vector4
{
private:
   T x, y, z, w;
public:
   Vector4();
   Vector4( const eConstructorVector4 constructor );
   Vector4( const T x, const T y, const T z, const T w );
   Vector4( const T scaler );
   void Zero();
   void Set( const T x, const T y, const T z, const T w );
   void Set( const Vector4 &other );
   T operator[]( const uint8 index ) const;
   T &operator[]( const uint8 index );
   Vector4 operator-() const;
   Vector4 &operator=( const T scaler );
   T* Ptr();
   const T* Ptr() const;
   Vector4 operator+( const Vector4 &other ) const;
   Vector4 &operator+=( const Vector4 &other );
   Vector4 operator-( const Vector4 &other ) const;
   Vector4 &operator-=( const Vector4 &other );
   void Swap( Vector4 &other );
   bool operator==( const Vector4 &other ) const;
   bool operator!=( const Vector4 &other ) const;
   Vector4 operator*( const T scalar ) const;
   Vector4 &operator*=( const T scalar );
   Vector4 operator/( const T scalar ) const;
   Vector4 &operator/=( const T scalar );
   T operator*( const Vector4 &other ) const;
   Vector4 CrossProd( const Vector4 &other ) const;
   T SqLength() const;
   T Length() const;
   T Normalize();
};

typedef Vector4<int16> Vector4s;
typedef Vector4<int32> Vector4i;
typedef Vector4<uint16> Vector4us;
typedef Vector4<uint32> Vector4u;
typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;
typedef Vector4<long double> Vector4ld; // TODO: replace with new type

template <class T>
inline Vector4<T>::Vector4( const eConstructorVector4 constructor )
{
   if (constructor == VEC4_CONST_NOTHING)
   { }
   else if (constructor == VEC4_CONST_ZERO )
   {
      Zero();
   }
}

template <class T>
inline Vector4<T>::Vector4()
{
   x = y = z = w = (T)0;
}

template <class T>
inline Vector4<T>::Vector4( const T x, const T y, const T z, const T w )
{
   this->x = x;
	this->y = y;
	this->z = z;
   this->w = w;
}

template <class T>
inline Vector4<T>::Vector4( const T scaler )
{
   x = y = z = w = scaler;
}

template <class T>
inline void Vector4<T>::Zero()
{
	x = y = z = w = (T)0;
}

template <class T>
inline void Vector4<T>::Set( const T x, const T y, const T z, const T w )
{
	this->x = x;
	this->y = y;
	this->z = z;
   this->w = w;
}

template <class T>
inline void Vector4<T>::Set( const Vector4<T> &other )
{
	x = other.x;
	y = other.y;
	z = other.z;
   w = other.w;
}

template <class T>
inline T Vector4<T>::operator[]( const uint8 index ) const
{
   assert(index < 4);
	return (&x)[index];
}

template <class T>
inline T &Vector4<T>::operator[]( const uint8 index )
{
   assert(index < 4);
	return (&x)[index];
}

template <class T>
inline Vector4<T> Vector4<T>::operator-() const
{
	return Vector4( -x, -y, -z, -w );
}

template <class T>
inline Vector4<T> &Vector4<T>::operator=( const T scaler )
{
   x = y = z = w = scaler;
   return *this;
}

// Pointer accessor for direct copying
template <class T>
inline T* Vector4<T>::Ptr()
{
   return &x;
}

// Pointer accessor for direct copying
template <class T>
inline const T* Vector4<T>::Ptr() const
{
   return &x;
}

template <class T>
inline Vector4<T> Vector4<T>::operator+( const Vector4<T> &other ) const
{
	return Vector4<T>( x + other.x, y + other.y, z + other.z, w + other.w );
}

template <class T>
inline Vector4<T> Vector4<T>::operator-( const Vector4<T> &other ) const
{
	return Vector4<T>( x - other.x, y - other.y, z - other.z, w - other.w );
}

template <class T>
inline Vector4<T> &Vector4<T>::operator+=( const Vector4<T> &other )
{
	x += other.x;
	y += other.y;
	z += other.z;
   w += other.z;
	return *this;
}

template <class T>
inline Vector4<T> &Vector4<T>::operator-=( const Vector4<T> &other )
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
   z -= other.w;
	return *this;
}

template <class T>
inline void Vector4<T>::Swap( Vector4<T> &other )
{
   core::Swap( x, other.x );
   core::Swap( y, other.y );
   core::Swap( z, other.z );
   core::Swap( w, other.w );
}

template <class T>
inline bool Vector4<T>::operator==( const Vector4<T> &other ) const
{
	if ( !core::Equals(x, other.x) ) {
		return false;
	}
			
	if ( !core::Equals(y, other.y) ) {
		return false;
	}

	if ( !core::Equals(z, other.z) ) {
		return false;
	}

	if ( !core::Equals(w, other.w) ) {
		return false;
	}

	return true;
}

template <class T>
inline bool Vector4<T>::operator!=( const Vector4<T> &other ) const
{
	if ( !core::Equals(x, other.x) ) {
		return true;
	}
			
	if ( !core::Equals(y, other.y) ) {
		return true;
	}

	if ( !core::Equals(z, other.z) ) {
		return true;
	}

	if ( !core::Equals(w, other.w) ) {
		return true;
	}

	return false;
}

template <class T>
inline Vector4<T> Vector4<T>::operator*( const T scalar ) const
{
   return Vector4<T>( x * scalar, y * scalar, z * scalar, w * scalar );
}

template <class T>
inline Vector4<T> &Vector4<T>::operator*=( const T scalar )
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
   w *= scalar;
	return *this;
}

template <class T>
inline Vector4<T> Vector4<T>::operator/( const T scalar ) const
{
   assert(!core::IsZero(scalar));

   T inv = (T)1 / scalar;
   return Vector4<T>( x * inv, y * inv, z * inv, w * inv );
}

template <class T>
inline Vector4<T> &Vector4<T>::operator/=( const T scalar )
{
   assert(!core::IsZero(scalar));

   T inv = (T)1 / scalar;
   x *= inv;
   y *= inv;
   z *= inv;
   w *= inv;
   return *this;
}

template <class T>
inline T Vector4<T>::operator*( const Vector4<T> &other ) const
{
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

template <class T>
inline T Vector4<T>::SqLength() const
{
	return x * x + y * y + z * z + w * w;
}

template <class T>
inline T Vector4<T>::Length() const
{
	return sqrt( x * x + y * y + z * z + w * w );
}

template <class T>
inline T Vector4<T>::Normalize()
{
	float length;
	float invlength;

	length = this->Length();
	if ( length ) {
		invlength = 1.0f / length;
		x *= invlength;
		y *= invlength;
		z *= invlength;
      w *= invlength;
	}
		
	return length;
}

} // namespace math

} // namespace core

#endif