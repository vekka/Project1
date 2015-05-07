#ifndef _DIMENSION_HPP_INCLUDED_
#define _DIMENSION_HPP_INCLUDED_

#include "mathcommon.hpp"

namespace core
{

   namespace math
   {

      enum eConstructorDimension2
      {
         DIM2_CONST_NOTHING,
         DIM2_CONST_ZERO,
         DIM2_CONST_ONE,
         DIM2_CONST_MIN,
         DIM2_CONST_MAX
      };

      template <class T>
      class Dimension2
      {
      public:
         T x, y;

         //----------- constructors
         Dimension2();
         Dimension2(const eConstructorDimension2 constructor);
         Dimension2(const Dimension2 &other);
         Dimension2(const T x, const T y);

         //----------- mutators
         void Set(const T x, const T y);
         void Zero();
         void SetMin(const Dimension2 &test);
         void SetMax(const Dimension2 &test);
         void Convolve(const Dimension2 &other);

         //----------- operators
         bool operator==(const Dimension2 &other) const;
         bool operator!=(const Dimension2 &other) const;

         Dimension2 operator+(const Dimension2 &other) const;
         Dimension2 &operator+=(const Dimension2 &other);
         Dimension2 operator-(const Dimension2 &other) const;
         Dimension2 &operator-=(const Dimension2 &other);

         Dimension2 operator*(const T scalar) const;
         Dimension2 &operator*=(const T scalar);
         Dimension2 operator/(const T scalar) const;
         Dimension2 &operator/=(const T scalar);

         Dimension2 operator*(const Dimension2 &other) const;
         Dimension2 &operator*=(const Dimension2 &other);
         Dimension2 operator/(const Dimension2 &other) const;
         Dimension2 &operator/=(const Dimension2 &other);

         Dimension2 operator-() const;

         //----------- queries
         bool IsZero() const;
         T Length() const;
         T LenghtSq() const;
         T GetArea() const;
         Dimension2 GetInterpolated(const Dimension2 &other, const float d) const;
      };

      typedef Dimension2<int16> Dimension2s;
      typedef Dimension2<uint16> Dimension2us;
      typedef Dimension2<int32> Dimension2i;
      typedef Dimension2<uint32> Dimension2u;
      typedef Dimension2<float> Dimension2f;
      typedef Dimension2<double> Dimension2d;

      template <class T>
      inline Dimension2<T>::Dimension2()
      {
      }

      template <class T>
      inline Dimension2<T>::Dimension2(const eConstructorDimension2 constructor)
      {
         if (constructor == DIM2_CONST_NOTHING) {}
         else if (constructor == DIM2_CONST_ZERO)
         {
            Zero();
         }
         else if (constructor == DIM2_CONST_ONE)
         {
            x = y = (T)1;
         }
         else if (constructor == DIM2_CONST_MIN)
         {
         }
         else if (constructor == DIM2_CONST_MAX)
         {
         }
         else
         {
            Zero();
         }
      }

      template <class T>
      inline Dimension2<T>::Dimension2(const T x, const T y)
      {
         this->x = x;
         this->y = y;
      }

      template <class T>
      inline void Dimension2<T>::Set(const T x, const T y)
      {
         this->x = x;
         this->y = y;
      }

      template <class T>
      inline void Dimension2<T>::Zero()
      {
         x = y = (T)0;
      }

      template <class T>
      inline void Dimension2<T>::SetMin(const Dimension2<T> &test)
      {
         this = Min(this, test);
      }

      template <class T>
      inline void Dimension2<T>::SetMax(const Dimension2<T> &test)
      {
         this = Max(this, test);
      }

      template <class T>
      inline void Dimension2<T>::Convolve(const Dimension2<T> &other)
      {
         x *= other.x;
         y *= other.y;
      }

      template <class T>
      inline bool Dimension2<T>::operator==(const Dimension2<T> &other) const
      {
         if (!core::Equals(x, other.x)) {
            return false;
         }

         if (!core::Equals(y, other.y)) {
            return false;
         }

         return true;
      }

      template <class T>
      inline bool Dimension2<T>::operator!=(const Dimension2<T> &other) const
      {
         if (!core::Equals(x, other.x)) {
            return true;
         }

         if (!core::Equals(y, other.y)) {
            return true;
         }

         return false;
      }

      template <class T>
      inline Dimension2<T> Dimension2<T>::operator+(const Dimension2<T> &other) const
      {
         return Dimension2<T>(x + other.x, y + other.y);
      }

      template <class T>
      inline Dimension2<T> Dimension2<T>::operator-(const Dimension2<T> &other) const
      {
         return Dimension2<T>(x - other.x, y - other.y);
      }

      template <class T>
      inline Dimension2<T> &Dimension2<T>::operator+=(const Dimension2<T> &other)
      {
      }

      template <class T>
      inline Dimension2<T> &Dimension2<T>::operator-=(const Dimension2<T> &other)
      {
      }

      template <class T>
      inline Dimension2<T> Dimension2<T>::operator*(const T scalar) const
      {
         return Dimension2<T>(x * scalar, y * scalar);
      }

      template <class T>
      inline Dimension2<T> &Dimension2<T>::operator*=(const T scalar)
      {
         x *= scalar;
         y *= scalar;
         return *this;
      }

      template <class T>
      inline Dimension2<T> Dimension2<T>::operator/(const T scalar) const
      {
         assert(!core::IsZero(scalar));

         T inv = (T)1 / scalar;
         return Dimension2<T>(x * inv, y * inv);
      }

      template <class T>
      inline Dimension2<T> &Dimension2<T>::operator/=(const T scalar)
      {
         assert(!core::IsZero(scalar));

         T inv = (T)1 / scalar;
         x *= inv;
         y *= inv;
         z *= inv;
         return *this;
      }

      template <class T>
      inline Dimension2<T> Dimension2<T>::operator-() const
      {
         return Dimension2<T>(-x, -y);
      }

      template <class T>
      bool Dimension2<T>::IsZero() const
      {
         return core::IsZero(x) && core::IsZero(y);
      }

      template <class T>
      T Dimension2<T>::Length() const
      {
         return sqrt(x*x + y*y + z*z);
      }

      template <class T>
      T Dimension2<T>::LenghtSq() const
      {
         return x*x + y*y + z*z;
      }

      template <class T>
      T Dimension2<T>::GetArea() const
      {
         return x*y;
      }

      template <class T>
      Dimension2<T> Dimension2<T>::GetInterpolated(const Dimension2<T> &other, const float d) const
      {
         float inv = (1.0f - d);
         return Dimension2<T>((T)(other.x*inv + x*d), (T)(other.y*inv + y*d));
      }

   } // namespace math

} // namespace core

#endif