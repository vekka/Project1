#ifndef _VECTOR3_HPP_INCLUDED_
#define _VECTOR3_HPP_INCLUDED_

#include "point3.hpp"

// cannot assign or return expressions made of Vector3<T> types and its operators directly
// possibly somehow due to inheritance from Point3<T>

namespace core
{

   namespace math
   {
      template <typename T>
      class Vector3 : public Point3 <T>
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
         void Rotate(float angle, const Vector3<T> &axis);
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
      inline Vector3<T>::Vector3() : Point3<T>()
      {
      }

      template <typename T>
      inline Vector3<T>::Vector3(const T x, const T y, const T z) : Point3<T>(x, y, z)
      {
      }
      
      template <typename T>
      inline Vector3<T>::Vector3(const T scaler) : Point3<T>(scaler)
      {    
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

      // angle in radians
      template <typename T>
      void Vector3<T>::Rotate(float angle, const Vector3<T> &axis)
      {
         const T sinHalfAngle = sinf(angle / 2);
         const T cosHalfAngle = cosf(angle / 2);

         const T rx = axis.x * sinHalfAngle;
         const T ry = axis.y * sinHalfAngle;
         const T rz = axis.z * sinHalfAngle;
         const T rw = cosHalfAngle;

         Quaternion<T> rotationQ(rx, ry, rz, rw);

         Quaternion<T> conjugateQ = rotationQ.Conjugate();

         //Quaternion_f vectorAsQuat(vector.x, vector.y, vector.z, 0);
         Vector3<T> w = rotationQ * (*this); // *conjugateQ;

         x = w.x;
         y = w.y;
         z = w.z;
      }

   } // namespace math

} // namespace core

#endif
