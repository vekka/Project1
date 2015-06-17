#ifndef _VECTOR3_HPP_INCLUDED_
#define _VECTOR3_HPP_INCLUDED_

#include "point3.hpp"

namespace core
{

   namespace math
   {
      template <typename T>
      class Vector3 : public Point3 < T >
      {
      public:
         using Point3<T>::operator=;

         static const Vector3<T> ZERO;
         static const Vector3<T> UNIT_X;
         static const Vector3<T> UNIT_Y;
         static const Vector3<T> UNIT_Z;
         static const Vector3<T> NEG_UNIT_X;
         static const Vector3<T> NEG_UNIT_Y;
         static const Vector3<T> NEG_UNIT_Z;
         static const Vector3<T> UNIT_SCALE;
         //----------- constructors
         Vector3();
         Vector3(const T x, const T y, const T z);
         Vector3(const T scaler);

         T operator*(const Vector3<T> &other) const;
         T DotProd(const Vector3<T> &other) const;
         Vector3 CrossProd(const Vector3 &other) const;
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

      template <typename T> const Vector3<T> Vector3<T>::ZERO(0, 0, 0);
      template <typename T> const Vector3<T> Vector3<T>::UNIT_X(1, 0, 0);
      template <typename T> const Vector3<T> Vector3<T>::UNIT_Y(0, 1, 0);
      template <typename T> const Vector3<T> Vector3<T>::UNIT_Z(0, 0, 1);
      template <typename T> const Vector3<T> Vector3<T>::NEG_UNIT_X(-1, 0, 0);
      template <typename T> const Vector3<T> Vector3<T>::NEG_UNIT_Y(0, -1, 0);
      template <typename T> const Vector3<T> Vector3<T>::NEG_UNIT_Z(0, 0, -1);
      template <typename T> const Vector3<T> Vector3<T>::UNIT_SCALE(1, 1, 1);

      template <typename T>
      inline Vector3<T>::Vector3()
      {
      }

      template <typename T>
      inline Vector3<T>::Vector3(const T x, const T y, const T z)  : Point3<T>(x, y, z)
      {
      }
      
      template <typename T>
      inline Vector3<T>::Vector3(const T scaler)
      {
         this->x = scaler;
         this->y = scaler;
         this->z = scaler;        
      }

      template <typename T>
      inline T Vector3<T>::operator*(const Vector3<T> &other) const
      {
         return x * other.x + y * other.y + z * other.z;
      }

      template <typename T>
      inline T Vector3<T>::DotProd(const Vector3<T> &other) const
      {
         return x * other.x + y * other.y + z * other.z;
      }

      template <typename T>
      inline Vector3<T> Vector3<T>::CrossProd(const Vector3<T> &other) const
      {
         return Vector3<T>(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x);
      }

      template <typename T>
      inline T Vector3<T>::SqLength() const
      {
         return x * x + y * y + z * z;
      }

      template <typename T>
      inline T Vector3<T>::Length() const
      {
         return (T)sqrt((float)(x * x + y * y + z * z));
      }

      template <typename T>
      inline T Vector3<T>::Normalize()
      {
         T length;
         T invlength;

         length = this->Length();
         if (length) {
            invlength = (T)1 / length;
            x *= invlength;
            y *= invlength;
            z *= invlength;
         }

         return length;
      }

      template <typename T>
      bool Vector3<T>::IsUnit()
      {
         return core::Equals((T)(x * x + y * y + z * z), (T)1);
      }

   } // namespace math

} // namespace core

#endif
