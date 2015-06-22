#ifndef _POINT3_HPP_INCLUDED_
#define _POINT3_HPP_INCLUDED_

#include <iostream>

#include "mathcommon.hpp"

namespace core
{

   namespace math
   {

      template <typename T>
      class Point3
      {
      public:
         T x, y, z;

         Point3();
         Point3(const T x, const T y, const T z);
         Point3(const T scaler);

         ~Point3();

         void Zero();
         void Set(const T x, const T y, const T z);
         void Set(const Point3 &other);

         T* Ptr();
         const T* Ptr() const;

         T operator[](const uint8 index) const;
         T &operator[](const uint8 index);

         Point3 operator-() const;
         Point3 &operator=(const T scaler);

         Point3 operator+(const Point3 &other) const;
         Point3 &operator+=(const Point3 &other);
         Point3 operator-(const Point3 &other) const;
         Point3 &operator-=(const Point3 &other);

         Point3 operator*(const T scalar) const;
         Point3 &operator*=(const T scalar);
         Point3 operator/(const T scalar) const;
         Point3 &operator/=(const T scalar);

         void Swap(Point3 &other);
         bool operator==(const Point3 &other) const;
         bool operator!=(const Point3 &other) const;

         T GetDistanceFrom(const Point3 &other) const;
         
         friend std::ostream& operator<<(std::ostream &out, Point3 &point3)
         {
           out << "(" << point3.x << ", " <<
               point3.y << ", " <<
               point3.z << ")";
           return out;
         }          
      };

      typedef Point3<int16> Point3s;
      typedef Point3<int32> Point3i;
      typedef Point3<uint16> Point3us;
      typedef Point3<uint32> Point3u;
      typedef Point3<float> Point3f;
      typedef Point3<double> Point3d;
      typedef Point3<long double> Point3ld; // TODO: replace with new type

      template <typename T>
      inline Point3<T>::Point3()
      {
      }

      template <typename T>
      inline Point3<T>::Point3(const T x, const T y, const T z)
      {
         this->x = x;
         this->y = y;
         this->z = z;
      }

      template <typename T>
      inline Point3<T>::Point3(const T scaler)
      {
         x = y = z = scaler;
      }

      template <typename T>
      inline Point3<T>::~Point3()
      {
      }

      template <typename T>
      inline void Point3<T>::Zero()
      {
         x = y = z = (T)0;
      }

      template <typename T>
      inline void Point3<T>::Set(const T x, const T y, const T z)
      {
         this->x = x;
         this->y = y;
         this->z = z;
      }

      template <typename T>
      void Point3<T>::Set(const Point3<T> &other)
      {
         x = other.x;
         y = other.y;
         z = other.z;
      }

      template <typename T>
      inline Point3<T> Point3<T>::operator-() const
      {
         return Point3<T>(-x, -y, -z);
      }

      template <typename T>
      inline Point3<T> &Point3<T>::operator=(const T scaler)
      {
         x = y = z = scaler;

         return *this;
      }

      // Pointer accessor for direct copying
      template <typename T>
      inline T* Point3<T>::Ptr()
      {
         return &x;
      }

      // Pointer accessor for direct copying
      template <typename T>
      inline const T* Point3<T>::Ptr() const
      {
         return &x;
      }

      template <typename T>
      inline Point3<T> Point3<T>::operator+(const Point3<T> &other) const
      {
         return Point3<T>(x + other.x, y + other.y, z + other.z);
      }

      template <typename T>
      inline Point3<T> &Point3<T>::operator+=(const Point3<T> &other)
      {
         x += other.x;
         y += other.y;
         z += other.z;
         return *this;
      }

      template <typename T>
      inline Point3<T> Point3<T>::operator-(const Point3<T> &other) const
      {
         return Point3<T>(x - other.x, y - other.y, z - other.z);
      }

      template <typename T>
      inline Point3<T> &Point3<T>::operator-=(const Point3<T> &other)
      {
         x -= other.x;
         y -= other.y;
         z -= other.z;
         return *this;
      }

      template <typename T>
      inline void Point3<T>::Swap(Point3<T> &other)
      {
         core::Swap(x, other.x);
         core::Swap(y, other.y);
         core::Swap(z, other.z);
      }

      template <typename T>
      inline bool Point3<T>::operator==(const Point3<T> &other) const
      {
         if (!core::Equals(x, other.x)) {
            return false;
         }

         if (!core::Equals(y, other.y)) {
            return false;
         }

         if (!core::Equals(z, other.z)) {
            return false;
         }

         return true;
      }

      template <typename T>
      inline bool Point3<T>::operator!=(const Point3<T> &other) const
      {
         if (!core::Equals(x, other.x)) {
            return true;
         }

         if (!core::Equals(y, other.y)) {
            return true;
         }

         if (!core::Equals(z, other.z)) {
            return true;
         }

         return false;
      }

      template <typename T>
      inline Point3<T> Point3<T>::operator*(const T scalar) const
      {
         return Point3<T>(x * scalar, y * scalar, z * scalar);
      }

      template <typename T>
      inline Point3<T> &Point3<T>::operator*=(const T scalar)
      {
         x *= scalar;
         y *= scalar;
         z *= scalar;
         return *this;
      }

      template <typename T>
      inline Point3<T> Point3<T>::operator/(const T scalar) const
      {
         assert(!core::IsZero(scalar));

         T inv = (T)1 / scalar;
         return Point3<T>(x * inv, y * inv, z * inv);
      }

      template <typename T>
      inline Point3<T> &Point3<T>::operator/=(const T scalar)
      {
         assert(!core::IsZero(scalar));

         T inv = (T)1 / scalar;
         x *= inv;
         y *= inv;
         z *= inv;
         return *this;
      }

      template <typename T>
      T Point3<T>::GetDistanceFrom(const Point3<T> &other) const
      {
         return Point3<T>(x - other.x, y - other.y, z - other.z).Length();
      }

   } // namespace math

} // namespace core

#endif
