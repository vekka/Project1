#ifndef _VECTOR2_HPP_INCLUDED_
#define _VECTOR2_HPP_INCLUDED_

#include "mathcommon.hpp" // Swap(), Equals()

#include "point2.hpp"

namespace core
{

   namespace math
   {
      template <typename T>
      class Vector2 : public Point2 <T>
      {
      public:
         using Point2<T>::operator=;

         static const Vector2<T> ZERO;
         static const Vector2<T> UNIT_X;
         static const Vector2<T> UNIT_Y;
         static const Vector2<T> NEG_UNIT_X;
         static const Vector2<T> NEG_UNIT_Y;
         static const Vector2<T> UNIT_SCALE; 

         //----------- constructors
         Vector2();
         Vector2(const T x, const T y);
         Vector2(const T scaler);

         T operator*(const Vector2 &other) const;
         T DotProd(const Vector2 &other) const;
         T CrossProd(const Vector2 &other) const;
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

      template <typename T> const Vector2<T> Vector2<T>::ZERO(0, 0);
      template <typename T> const Vector2<T> Vector2<T>::UNIT_X(1, 0);
      template <typename T> const Vector2<T> Vector2<T>::UNIT_Y(0, 1);
      template <typename T> const Vector2<T> Vector2<T>::NEG_UNIT_X(-1, 0);
      template <typename T> const Vector2<T> Vector2<T>::NEG_UNIT_Y(0, -1);
      template <typename T> const Vector2<T> Vector2<T>::UNIT_SCALE(1, 1);

      template <typename T>
      inline Vector2<T>::Vector2()
      {
      }

      template <typename T>
      inline Vector2<T>::Vector2(const T x, const T y) : Point2<T>(x, y)
      {
      }

      template <typename T>
      inline Vector2<T>::Vector2(T scaler) : Point2<T>(scaler)
      {
      }

      template <typename T>
      inline T Vector2<T>::operator*(const Vector2<T> &other) const
      {
         return x * other.x + y * other.y;
      }

      template <typename T>
      inline T Vector2<T>::DotProd(const Vector2<T> &other) const
      {
         return x * other.x + y * other.y;
      }

      template <typename T>
      inline T Vector2<T>::CrossProd(const Vector2<T> &other) const
      {
         return x * other.y - y * other.x;
      }

      template <typename T>
      inline T Vector2<T>::SqLength() const
      {
         return x * x + y * y;
      }

      template <typename T>
      inline T Vector2<T>::Length() const
      {
         return sqrt(x * x + y * y);
      }

      template <typename T>
      inline T Vector2<T>::Normalize()
      {
         float length;
         float invlength;

         length = this->Length();
         if (length) {
            invlength = 1.0f / length;
            x *= invlength;
            y *= invlength;
            z *= invlength;
         }

         return length;
      }

      template <typename T>
      inline Vector2<T> Vector2<T>::CreatePerpendicularVec() const
      {
         return Vector2<T>(-y, x);
      }

   } // namespace math

} // namespace core

#endif
