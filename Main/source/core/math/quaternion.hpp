#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <assert.h>
#include <cmath> // sqrt(), sin(), cos()

#include "vector3.hpp"

namespace core
{

namespace math
{

//extern const int W;

template <class T>
class Quaternion
{
private:
   T x, y, z, w;
public:
   /* inline methods */
   Quaternion();
   Quaternion( const T x, const T y, const T z, const T w );
   Quaternion( const Vector3<T> &v, const T w );
   Quaternion( const T* ptr );
   void Set( const T x, const T y, const T z, const T w );
   T operator[]( const int index ) const;
   T &operator[]( const int index );
   void operator=( const Quaternion other );
   T *Ptr();
   const T *Ptr() const;
   Quaternion operator+( const Quaternion &other ) const;
   Quaternion &operator+=( const Quaternion &other );
   Quaternion operator-( const Quaternion &other ) const;
   Quaternion &operator-=( const Quaternion &other );
   void Swap( Quaternion& other );
   bool operator==( const Quaternion &other ) const;
   bool operator!=( const Quaternion &other ) const;
   
   /* .cpp methods */
   void FromAngleAxis( const float angle, const Vector3<T> &vec );
   void ToAngleAxis( float &outAngle, Vector3<T> &outAxis ) const;

   //void FromRotationMatrix( const Matrix3 &rot );
   //void ToRotationMatrix( Matrix3 &rot ) const;
};

typedef Quaternion<int> Quaternion_i;
typedef Quaternion<short> Quaternion_s;
typedef Quaternion<unsigned int> Quaternion_u;
typedef Quaternion<unsigned short> Quaternion_us;
typedef Quaternion<float> Quaternion_f;
typedef Quaternion<double> Quaternion_d;
typedef Quaternion<long double> Quaternion_ld;

template <class T>
inline Quaternion<T>::Quaternion()
{
}

template <class T>
inline Quaternion<T>::Quaternion( const T* ptr )
{
   memcpy( &w, valptr, sizeof(T)*4 );
}

template <class T>
inline Quaternion<T>::Quaternion( const T x, const T y, const T z, const T w )
{
   this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

template <class T>
inline Quaternion<T>::Quaternion( const Vector3<T> &v, const T w )
{
   this->x = v[X];
	this->y = v[Y];
	this->z = v[Z];
	this->w = w;
}

template <class T>
inline void Quaternion<T>::Set( const T x, const T y, const T z, const T w )
{
   this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

template <class T>
inline void Quaternion<T>::operator=( const Quaternion<T> other )
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

template <class T>
inline T* Quaternion<T>::Ptr()
{
   return &x;
}

template <class T>
const inline T* Quaternion<T>::Ptr() const
{
   return &x;
}

template <class T>
inline T Quaternion<T>::operator[]( const int index ) const
{
	assert( ( index >= 0 ) && ( index < 4 ) );
	return (&x)[index];
}

template <class T>
inline T &Quaternion<T>::operator[]( const int index )
{
	assert( ( index >= 0 ) && ( index < 4 ) );
	return (&x)[index];
}

template <class T>
inline Quaternion<T> Quaternion<T>::operator+( const Quaternion<T> &other ) const
{
	return Quaternion<T>( x + other.x, y + other.y, z + other.z, w + other.w );
}

template <class T>
inline Quaternion<T> Quaternion<T>::operator-( const Quaternion<T> &other ) const
{
	return Quaternion<T>( x - other.x, y - other.y, z - other.z, w - other.w );
}

template <class T>
inline Quaternion<T> &Quaternion<T>::operator+=( const Quaternion<T> &other )
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;

	return *this;
}

template <class T>
inline Quaternion<T> &Quaternion<T>::operator-=( const Quaternion<T> &other )
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;

	return *this;
}

template <class T>
inline void Quaternion<T>::Swap( Quaternion& other )
{
   core::Swap( w, other.w );
   core::Swap( x, other.x );
   core::Swap( y, other.y );
   core::Swap( z, other.z );
}

template <class T>
inline bool Quaternion<T>::operator==( const Quaternion<T> &other ) const
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
inline bool Quaternion<T>::operator!=( const Quaternion<T> &other ) const
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
void Quaternion<T>::FromAngleAxis( const float angle, const Vector3<T> &axis )
{
   assert( axis.isUnit() );
   // The quaternion representing the rotation is
   //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

   float halfAngle ( 0.5*angle );
   float fSin = sin( halfAngle );
   w = cos( halfAngle );
   x = fSin * axis[0];
   y = fSin * axis[1];
   z = fSin * axis[2];
}

template <class T>
void Quaternion<T>::ToAngleAxis( float &outAngle, Vector3<T> &outAxis ) const
{
   // The quaternion representing the rotation is
   //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

   float sqrLength = x * x + y * y + z * z;
   if ( sqrLength > 0.0 )
   {
      outAngle = 2.0 * cos(w);
      float invLength = 1 / sqrt( sqrLength );
      outAxis[0] = x * invLength;
      outAxis[1] = y * invLength;
      outAxis[2] = z * invLength;
   }
   else
   {
      // angle is 0 (mod 2*pi), so any axis will do
      //angle = Radian(0.0);
      outAngle = 0.0f;
      outAxis[0] = (T)1;
      outAxis[1] = (T)0;
      outAxis[2] = (T)0;
   }
}

//template <class T>
//void Quaternion::FromAxes( const Vector3<T> *axis )
//{
//   Matrix3 rot;
//
//   for (int col = 0; iCol < 3; iCol++)
//   {
//      rot[0][col] = akAxis[col].x;
//      rot[1][col] = akAxis[col].y;
//      rot[2][col] = akAxis[col].z;
//   }
//
//   FromRotationMatrix(kRot);
//}

} // namespace math

} // namespace core

#endif