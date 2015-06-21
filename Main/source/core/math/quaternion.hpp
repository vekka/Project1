#ifndef _QUATERNION_HPP_INCLUDED_
#define _QUATERNION_HPP_INCLUDED_

#include <cassert>
#include <cmath> // sqrt(), sin(), cos()

#include "vector3.hpp"
#include "matrix4.hpp"
using core::math::Matrix4f;

namespace core
{
	namespace math
	{
	        template <class T>
	        class Quaternion
	        {
	        public:
	           T x, y, z, w;
	
		   static const Quaternion<T> ZERO;
		   static const Quaternion<T> IDENTITY;
		   
	           /* inline methods */
	           Quaternion();
	           Quaternion( T x, T y, T z, T w );
	           Quaternion( const Vector3<T> &v, T w );
	           Quaternion( const T* ptr );
	           void Set( T x, T y, T z, T w );
	           void operator=( const Quaternion other );
	
	           Quaternion Inverse(void);
	           


	           Quaternion operator+( const Quaternion &other ) const;
	           Quaternion &operator+=( const Quaternion &other );
	           Quaternion operator-( const Quaternion &other ) const;
	           Quaternion &operator-=( const Quaternion &other );
	           Quaternion operator*( const Quaternion &other ) const;
	           Vector3<T> operator*( const Vector3<T> &vec3 ) const;
	           Quaternion operator*( T scalar ) const;
	           Quaternion &operator*=( T scalar );
	           Quaternion operator/( T scalar ) const;
	           Quaternion &operator/=( T scalar );
	           
	           T Real(void) const;
	           Vector3<T> Imag(void) const;
	           
	           T DotProduct( const Quaternion &q1, const Quaternion &q2 );
	           T Magnitude(void) const;
	           
	           bool IsUnit(void) const;
	           
	           void Swap( Quaternion& other );
	           bool operator==( const Quaternion &other ) const;
	           bool operator!=( const Quaternion &other ) const;
	
		   //Matrix4 CreateMatrix() const;
	           void CreateMatrix( Matrix4<T> &dest, const Vector3<T> &center ) const;
	           /* .cpp methods */
	           //void Quaternion<T>::FromAngleAxis(const float angle, const Vector3<T> &axis);
	           void FromAngleAxis( const float angle, const Vector3<T> &vec );
	           void ToAngleAxis( float &outAngle, Vector3<T> &outAxis ) const;
	
              Quaternion &RotationFromTo(const Vector3<T> &from, const Vector3<T> &to);


	           void Normalize();
	           Quaternion Conjugate();
	
	           //void Quaternion<T>::FromAngleAxis(const float angle, const Vector3<T> &axis)
	           //void FromRotationMatrix( const Matrix3 &rot );
	           //void ToRotationMatrix( Matrix3 &rot ) const;
	        };
	
	        typedef Quaternion<int32> Quaternion_i;
	        typedef Quaternion<int16> Quaternion_s;
	        typedef Quaternion<uint32> Quaternion_u;
	        typedef Quaternion<uint16> Quaternion_us;
	        typedef Quaternion<float> Quaternion_f;
	        typedef Quaternion<double> Quaternion_d;
	        typedef Quaternion<long double> Quaternion_ld;
	
		template <typename T> const Quaternion<T> Quaternion<T>::ZERO(0, 0, 0, 0);
      		template <typename T> const Quaternion<T> Quaternion<T>::IDENTITY(0, 0, 0, 1); 

	        template <class T>
	        inline Quaternion<T>::Quaternion()
	        {
	        }
	
	        template <class T>
	        inline Quaternion<T>::Quaternion( const T* ptr )
	        {
	           assert(ptr != NULL);
	           memcpy( &w, valptr, sizeof(T)*4 );
	        }
	
	        template <class T>
	        inline Quaternion<T>::Quaternion( T x, T y, T z, T w )
	        {
	            this->x = x;
	            this->y = y;
	            this->z = z;
	            this->w = w;
	        }
	
	        template <class T>
	        inline void Quaternion<T>::Normalize()
	        {
	           float length = sqrt(x*x, y*y, z*z, w*w);
	
	           x /= length;
	           y /= length;
	           z /= length;
	           w /= length;
	        }
	
	        template <class T>
	        Quaternion<T> Quaternion<T>::Conjugate()
	        {
              return Quaternion<T>(-x, -y, -z, w);
	        }
	
	        template <class T>
	        inline Quaternion<T>::Quaternion( const Vector3<T> &v, T w )
	        {
	            this->x = v.x;
	            this->y = v.y;
	            this->z = v.z;
	            this->w = w;
	        }
	
	        template <class T>
	        inline void Quaternion<T>::Set( T x, T y, T z, T w )
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
	        inline Quaternion<T> Quaternion<T>::operator+( const Quaternion<T> &other ) const
	        {
	            return Quaternion<T>( x + other.x, y + other.y, z + other.z, w + other.w );
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
	        inline Quaternion<T> Quaternion<T>::operator-( const Quaternion<T> &other ) const
	        {
	            return Quaternion<T>( x - other.x, y - other.y, z - other.z, w - other.w );
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
	        inline Quaternion<T> Quaternion<T>::operator*( const Quaternion<T> &other ) const
	        {
		   return Quaternion<T>(
			w*other.x + x*other.w + y*other.z - z*other.y,
			w*other.y - x*other.z + y*other.w + z*other.x,
			w*other.z + x*other.y - y*other.x + z*other.w,
			w*other.w - x*other.x - y*other.y - z*other.z);
	        }

	        template <class T>
	        inline Vector3<T> Quaternion<T>::operator*( const Vector3<T> &vec3 ) const
	        {
	           // nvidia SDK implementation
	           Vector3<T> uv, uuv; 
		   Vector3<T> qvec(x, y, z);
		   uv = qvec.CrossProduct(v); 
		   uuv = qvec.CrossProduct(uv);
		   uv *= (2.0f * w);
		   uuv *= 2.0f;
		   return v + uv + uuv;
	        }
	        
	        template <class T>
	        inline Quaternion<T> Quaternion<T>::operator*( T scalar ) const
	        {
	           return Quaternion<T>(x*scalar, y*scalar, z*scalar, w*scalar);
	        }
	        
	        template <class T>
	        inline Quaternion<T> &Quaternion<T>::operator*=( T scalar )
	        {
	            x *= scalar;
	            y *= scalar;
	            z *= scalar;
	            w *= scalar;
	
	            return *this;
	        }
	        
	        template <class T>
	        inline Quaternion<T> Quaternion<T>::operator/( T scalar ) const
	        {
	           return Quaternion<T>(x/scalar, y/scalar, z/scalar, w/scalar);
	        }
	        
	        template <class T>
	        inline Quaternion<T> &Quaternion<T>::operator/=( T scalar )
	        {
	            x /= scalar;
	            y /= scalar;
	            z /= scalar;
	            w /= scalar;
	
	            return *this;
	        }
	        
	        template <class T>
	        inline T Quaternion<T>::Real(void) const
	        {
	           return w;
	        }
	        
	        template <class T>
	        inline Vector3<T> Quaternion<T>::Imag(void) const
	        {
	           return Vector3<T>(x, y, z);
	        }
	        
	        template <class T>
	        inline T Quaternion<T>::DotProduct( const Quaternion<T> &q1, const Quaternion<T> &q2 )
	        {
	            return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
	        }
	        


           template <class T>
	   Quaternion<T> Quaternion<T>::Inverse(void)
  		{
  		   float magnitude = x*x + y*y + z*z + w*w;
  		   assert(!core::IsZero(magnitude));
  		   float invMagnitude;
  		   if( core::Equals(magnitude, T(1)) )    // special case: unit quaternion
  		   {
  		      x = -x;
  		      y = -y;
  		      z = -z;
  		   }
  		   else // else scale
  		   {
  		      if( core::IsZero(magnitude) )
  		         invMagnitude = T(1);
  		      else
  		         invMagnitude = T(1) / magnitude;
  		      x *= -invMagnitude;
  		      y *= -invMagnitude;
  		      z *= -invMagnitude;
  		      w *= invMagnitude;
  		   }
  		   return *this;
  		}
  		
	        template <class T>
	        T Quaternion<T>::Magnitude(void) const
	        {
	            return std::sqrt(DotProduct(*this, *this));
	        }
	        
	        template <class T>
	        bool Quaternion<T>::IsUnit(void) const
	        {
	            return core::Equals(DotProduct(*this, *this), T(1));
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
		Quaternion<T> &Quaternion<T>::RotationFromTo(const Vector3<T> &from, const Vector3<T> &to)
		{
		   // Based on Stan Melax's article in Game Programming Gems
		   // Copy, since cannot modify local
		   Vector3<T> v0 = from;
		   Vector3<T> v1 = to;
		   v0.Normalize();
		   v1.Normalize();
	
		   const float d = v0.DotProduct(v1);
		   if (d >= 1.0f) // If dot == 1, vectors are the same
		   {
			return IDENTITY;
		   }
		  else if (d <= -1.0f) // exactly opposite
		  {
			Vector3<T> axis(1.0f, 0.0f, 0.0f);
			axis = axis.CrossProduct(Vector3<T>(x, y, z));
			if (IsEqual(axis.Length(), 0))
			{
				axis.Set(0.0f, 1.0f, 0.0f);
				axis.CrossProduct(Vector3<T>(x, y, z));
			}
			return this->FromAngleAxis(PI, axis);
		}
	
		const f32 s = sqrtf( (1+d)*2 ); // optimize inv_sqrt
		const f32 invs = 1.f / s;
		const vector3df c = v0.crossProduct(v1)*invs;
		X = c.X;
		Y = c.Y;
		Z = c.Z;
		W = s * 0.5f;
	
		return *this;

		}

		
		template <class T>
	        void Quaternion<T>::CreateMatrix( Matrix4<T> &dest, const Vector3<T> &center ) const
	        {
	        	
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
	        //   for (int32 col = 0; iCol < 3; iCol++)
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
