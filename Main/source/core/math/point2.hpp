#ifndef _POINT2_HPP_INCLUDED_
#define _POINT2_HPP_INCLUDED_

namespace core
{

   namespace math
   {

      template <class T>
      class Point2
      {
      protected:
         T x, y;
      public:
         //----------- constructors
         Point2();
         Point2(const T x, const T y);
         Point2(const T scaler);

         ~Point2();

         void Zero();

         void Set(const T x, const T y);
         void Set(const Point2 &other);

         T *Ptr();
         const T *Ptr() const;

         void Swap(Point2 &other);

         //----------- operators
         Point2 &operator=(const T scaler);

         T operator[](const uint8 index) const;
         T &operator[](const uint8 index);

         bool operator==(const Point2 &other) const;
         bool operator!=(const Point2 &other) const;

         Point2 operator+(const Point2 &other) const;
         Point2 &operator+=(const Point2 &other);
         Point2 operator-(const Point2 &other) const;
         Point2 &operator-=(const Point2 &other);

         Point2 operator*(const T scalar) const;
         Point2 &operator*=(const T scalar);
         Point2 operator/(const T scalar) const;
         Point2 &operator/=(const T scalar);

         Point2 operator-() const;
      };

      typedef Point2<int16> Point2s;
      typedef Point2<int32> Point2i;
      typedef Point2<uint16> Point2us;
      typedef Point2<uint32> Point2u;
      typedef Point2<float> Point2f;
      typedef Point2<double> Point2d;
      typedef Point2<long double> Point2ld; // TODO: replace with new type

      template <class T>
      Point2<T>::Point2()
      {
      }

      template <class T>
      Point2<T>::~Point2()
      {
      }

      template <class T>
      inline Point2<T>::Point2(const T x, const T y)
      {
         this->x = x;
         this->y = y;
      }

      template <class T>
      inline Point2<T>::Point2(const T scaler)
      {
         x = y = scaler;
      }

      template <class T>
      inline void Point2<T>::Zero()
      {
         x = y = (T)0;
      }

      template <class T>
      inline void Point2<T>::Set(const T x, const T y)
      {
         this->x = x;
         this->y = y;
      }

      template <class T>
      inline void Point2<T>::Set(const Point2<T> &other)
      {
         x = other.x;
         y = other.y;
      }

      template <class T>
      inline T Point2<T>::operator[](const uint8 index) const
      {
         assert(index < 2);
         return (&x)[index];
      }

      template <class T>
      inline T &Point2<T>::operator[](const uint8 index)
      {
         assert(index < 2);
         return (&x)[index];
      }

      template <class T>
      inline Point2<T> Point2<T>::operator-() const
      {
         return Point2<T>(-x, -y);
      }

      template <class T>
      inline Point2<T> &Point2<T>::operator=(const T scaler)
      {
         x = y = scaler;
         return *this;
      }

      // pointer accessor for direct copying
      template <class T>
      inline T* Point2<T>::Ptr()
      {
         return &x;
      }

      // pointer accessor for direct copying
      template <class T>
      inline const T* Point2<T>::Ptr() const
      {
         return &x;
      }

      template <class T>
      inline Point2<T> Point2<T>::operator+(const Point2<T> &other) const
      {
         return Point2<T>(x + other.x, y + other.y);
      }

      template <class T>
      inline Point2<T> &Point2<T>::operator+=(const Point2<T> &other)
      {
         x += other.x;
         y += other.y;
         return *this;
      }

      template <class T>
      inline Point2<T> Point2<T>::operator-(const Point2<T> &other) const
      {
         return Point2<T>(x - other.x, y - other.y);
      }

      template <class T>
      inline Point2<T> &Point2<T>::operator-=(const Point2<T> &other)
      {
         x -= other.x;
         y -= other.y;
         return *this;
      }

      template <class T>
      inline void Point2<T>::Swap(Point2<T> &other)
      {
         core::Swap(x, other.x);
         core::Swap(y, other.y);
      }

      template <class T>
      inline bool Point2<T>::operator==(const Point2<T> &other) const
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
      inline bool Point2<T>::operator!=(const Point2<T> &other) const
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
      inline Point2<T> Point2<T>::operator*(const T scalar) const
      {
         return Point2<T>(x * scalar, y * scalar);
      }

      template <class T>
      inline Point2<T> &Point2<T>::operator*=(const T scalar)
      {
         x += scalar;
         y += scalar;
         return *this;
      }

      template <class T>
      inline Point2<T> Point2<T>::operator/(const T scalar) const
      {
         assert(!core::IsZero(scalar));

         T inv = (T)1 / scalar;
         return Point2<T>(x * inv, y * inv);
      }

      template <class T>
      inline Point2<T> &Point2<T>::operator/=(const T scalar)
      {
         assert(!core::IsZero(scalar));

         T inv = (T)1 / scalar;
         x *= inv;
         y *= inv;
         return *this;
      }

   } // namespace math

} // namespace core

#endif