#ifndef _MATRIX3_HPP_INCLUDED_
#define _MATRIX3_HPP_INCLUDED_

#include <iostream>

#include "vector3.hpp"

namespace core
{

   namespace math
   {
      template <class T>
      class Matrix3
      {
      private:
         T m[3][3];

      public:
         static const Matrix3<T> ZERO;
         static const Matrix3<T> IDENTITY;
    
         Matrix3();
         Matrix3(const T arr[3][3]);
         Matrix3(
            const T m00, const T m01, const T m02,
            const T m10, const T m11, const T m12,
            const T m20, const T m21, const T m22);
         Matrix3(const Vector3<T> &v1, const Vector3<T> &v2, const Vector3<T> &v3, bool rowMajor);
         void Zero();
         void SetIdentity();
         void Set(
            const T m00, const T m01, const T m02,
            const T m10, const T m11, const T m12,
            const T m20, const T m21, const T m22);
         void SetDiagonal(const T d0, const T d1, const T d2);
         T operator()(const uint8 index1, const uint8 index2) const;
         T &operator()(const uint8 index1, const uint8 index2);
         T operator[](const uint8 index) const;
         T &operator[](const uint8 index);
         Vector3<T> operator*(const Vector3<T> &vec) const;
         Matrix3 operator*(const Matrix3 &other) const;
         Matrix3 operator*(const T scalar) const;
         Matrix3 &operator*=(const T scalar);
         Matrix3 operator+(Matrix3 const &other) const;
         Matrix3 operator-(Matrix3 const &other) const;
         Matrix3 &operator+=(Matrix3 const &other);
         Matrix3 &operator-=(Matrix3 const &other);
         void Swap(Matrix3 &other);
         Matrix3 Transpose() const;
         T Determinant() const;
         bool Inverse(Matrix3 &inv, const float tolerance) const;

         //friend Matrix3	operator*( float a, Matrix3 const &b );

         friend std::ostream& operator<<(std::ostream &out, Matrix3 &mat3)
         {
            out << "((" << mat3[0][0] << ", " << mat3[0][1] << ", " << mat3[0][2] << "), " <<
               "(" << mat3[1][0] << ", " << mat3[1][1] << ", " << mat3[1][2] << "), " <<
               "(" << mat3[2][0] << ", " << mat3[2][1] ", " << << mat3[2][2] << "))";
            return out;
         }
      };
         
      typedef Matrix3<int16> Matrix3s;
      typedef Matrix3<int32> Matrix3i;
      typedef Matrix3<uint16> Matrix3us;
      typedef Matrix3<uint32> Matrix3u;
      typedef Matrix3<float> Matrix3f;
      typedef Matrix3<double> Matrix3d;
      typedef Matrix3<long double> Matrix3ld;

      template <typename T> const Matrix3<T> Matrix3<T>::ZERO(0, 0, 0, 0, 0, 0, 0, 0, 0);
      template <typename T> const Matrix3<T> Matrix3<T>::IDENTITY(1, 0, 0, 0, 1, 0, 0, 0, 1);

      template <class T>
      inline Matrix3<T>::Matrix3()
      {
      }

      template <class T>
      inline Matrix3<T>::Matrix3(const T arr[3][3])
      {
         memcpy(m, arr, 9 * sizeof(T));
      }

      template <class T>
      inline Matrix3<T>::Matrix3(
         const T m00, const T m01, const T m02,
         const T m10, const T m11, const T m12,
         const T m20, const T m21, const T m22)
      {
         m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
         m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
         m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
      }

      template <class T>
      inline Matrix3<T>::Matrix3(const Vector3<T> &v1, const Vector3<T> &v2, const Vector3<T> &v3, bool rowMajor = true)
      {
         if (rowMajor)
         {
            m[0][0] = v1[0]; m[0][1] = v1[1]; m[0][2] = v1[2];
            m[1][0] = v2[0]; m[1][1] = v2[1]; m[1][2] = v2[2];
            m[2][0] = v3[0]; m[2][1] = v3[1]; m[2][2] = v3[2];
         }
         else
         {
            m[0][0] = v1[0]; m[0][1] = v2[0]; m[0][2] = v3[0];
            m[1][0] = v1[1]; m[1][1] = v2[1]; m[1][2] = v3[1];
            m[2][0] = v1[2]; m[2][1] = v2[2]; m[2][2] = v3[2];
         }
      }

      template <class T>
      inline void Matrix3<T>::Zero()
      {
         m[0][0] = m[0][1] = m[0][2]
            = m[1][0] = m[1][1] = m[1][2]
            = m[2][0] = m[2][1] = m[2][2] = (T)0;
      }

      template <class T>
      inline void Matrix3<T>::SetIdentity()
      {
         m[0][1] = m[0][2] = m[1][0] = m[1][2]
            = m[2][0] = m[2][1] = (T)0;
         m[0][0] = m[1][1] = m[2][2] = (T)1;
      }

      template <class T>
      inline void Matrix3<T>::Set(
         const T m00, const T m01, const T m02,
         const T m10, const T m11, const T m12,
         const T m20, const T m21, const T m22)
      {
         m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
         m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
         m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
      }

      template <class T>
      inline void Matrix3<T>::SetDiagonal(const T d0, const T d1, const T d2)
      {
         Matrix3<T>(d0, 0, 0, 0, d1, 0, 0, 0, d2);
      }

      template <class T>
      inline T Matrix3<T>::operator()(const uint8 index1, const uint8 index2) const
      {
         assert((index1 < 3) && (index2 < 3));
         return m[index1][index2];
      }

      template <class T>
      inline T &Matrix3<T>::operator()(const uint8 index1, const uint8 index2)
      {
         assert((index1 < 3) && (index2 < 3));
         return m[index1][index2];
      }

      template <class T>
      inline T Matrix3<T>::operator[](const uint8 index) const
      {
         assert(index < 9);
         return (&x)[index];
      }

      template <class T>
      inline T &Matrix3<T>::operator[](const uint8 index)
      {
         assert(index < 9);
         return (&x)[index];
      }

      template <class T>
      inline Vector3<T> Matrix3<T>::operator*(const Vector3<T> &vec) const
      {
         return Vector3<T>(
            m[0][0] * vec[0] + m[1][0] * vec[1] + m[2][0] * vec[2],
            m[0][1] * vec[0] + m[1][1] * vec[1] + m[2][1] * vec[2],
            m[0][2] * vec[0] + m[1][2] * vec[1] + m[2][2] * vec[2]);
      }

      template <class T>
      inline Matrix3<T> Matrix3<T>::operator*(const Matrix3<T> &other) const
      {
         return Matrix3<T>(
            m[0][0] * m.other[0][0] + m[0][1] * m.other[1][0] + m[0][2] * m.other[2][0],
            m[0][0] * m.other[0][1] + m[0][1] * m.other[1][1] + m[0][2] * m.other[2][1],
            m[0][0] * m.other[0][2] + m[0][1] * m.other[1][2] + m[0][2] * m.other[2][2],

            m[1][0] * m.other[0][0] + m[1][1] * m.other[1][0] + m[1][2] * m.other[2][0],
            m[1][0] * m.other[0][1] + m[1][1] * m.other[1][1] + m[1][2] * m.other[2][1],
            m[1][0] * m.other[0][2] + m[1][1] * m.other[1][2] + m[1][2] * m.other[2][2],

            m[2][0] * m.other[0][0] + m[2][1] * m.other[1][0] + m[2][2] * m.other[2][0],
            m[2][0] * m.other[0][1] + m[2][1] * m.other[1][1] + m[2][2] * m.other[2][1],
            m[2][0] * m.other[0][2] + m[2][1] * m.other[1][2] + m[2][2] * m.other[2][2]);
      }

      template <class T>
      inline Matrix3<T> Matrix3<T>::operator*(const T scalar) const
      {
         return Matrix3<T>(
            m[0][0] * scalar, m[0][1] * scalar, m[0][2] * scalar,
            m[1][0] * scalar, m[1][1] * scalar, m[1][2] * scalar,
            m[2][0] * scalar, m[2][1] * scalar, m[2][2] * scalar);
      }

      template <class T>
      inline Matrix3<T> &Matrix3<T>::operator*=(const T scalar)
      {
         m[0][0] *= scalar; m[0][1] *= scalar; m[0][2] *= scalar;
         m[1][0] *= scalar; m[1][1] *= scalar; m[1][2] *= scalar;
         m[2][0] *= scalar; m[2][1] *= scalar; m[2][2] *= scalar;

         return *this;
      }

      template <class T>
      inline Matrix3<T> Matrix3<T>::operator+(const Matrix3<T> &other) const
      {
         return Matrix3<T>(
            m[0][0] + other.m[0][0], m[0][1] + other.m[0][1], m[0][2] + other.m[0][2],
            m[1][0] + other.m[1][0], m[1][1] + other.m[1][1], m[1][2] + other.m[1][2],
            m[2][0] + other.m[2][0], m[2][1] + other.m[2][1], m[2][2] + other.m[2][2]);
      }

      template <class T>
      inline Matrix3<T> Matrix3<T>::operator-(const Matrix3<T> &other) const
      {
         return Matrix3<T>(
            m[0][0] - other.m[0][0], m[0][1] - other.m[0][1], m[0][2] - other.m[0][2],
            m[1][0] - other.m[1][0], m[1][1] - other.m[1][1], m[1][2] - other.m[1][2],
            m[2][0] - other.m[2][0], m[2][1] - other.m[2][1], m[2][2] - other.m[2][2]);
      }

      template <class T>
      inline Matrix3<T> &Matrix3<T>::operator+=(const Matrix3<T> &other)
      {
         m[0][0] += other.m[0][0]; m[0][1] += other.m[0][1]; m[0][2] += other.m[0][2];
         m[1][0] += other.m[1][0]; m[1][1] += other.m[1][1]; m[1][2] += other.m[1][2];
         m[2][0] += other.m[2][0]; m[2][1] += other.m[2][1]; m[2][2] += other.m[2][2];

         return *this;
      }

      template <class T>
      inline Matrix3<T> &Matrix3<T>::operator-=(const Matrix3<T> &other)
      {
         m[0][0] -= other.m[0][0]; m[0][1] -= other.m[0][1]; m[0][2] -= other.m[0][2];
         m[1][0] -= other.m[1][0]; m[1][1] -= other.m[1][1]; m[1][2] -= other.m[1][2];
         m[2][0] -= other.m[2][0]; m[2][1] -= other.m[2][1]; m[2][2] -= other.m[2][2];

         return *this;
      }

      template <class T>
      inline void Matrix3<T>::Swap(Matrix3<T> &other)
      {
         core::Swap(m[0][0], other.m[0][0]);
         core::Swap(m[0][1], other.m[0][1]);
         core::Swap(m[0][2], other.m[0][2]);
         core::Swap(m[1][0], other.m[1][0]);
         core::Swap(m[1][1], other.m[1][1]);
         core::Swap(m[1][2], other.m[1][2]);
         core::Swap(m[2][0], other.m[2][0]);
         core::Swap(m[2][1], other.m[2][1]);
         core::Swap(m[2][2], other.m[2][2]);
      }

      template <class T>
      inline Matrix3<T> Matrix3<T>::Transpose() const
      {
         return Matrix3(
            m[0][0], m[1][0], m[2][0],
            m[0][1], m[1][1], m[2][1],
            m[0][2], m[1][2], m[2][2]);
      }

      template <class T>
      T Matrix3<T>::Determinant() const
      {
         float cofactor00 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
         float cofactor10 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
         float cofactor20 = m[1][0] * m[2][1] - m[1][1] * m[2][0];

         return m[0][0] * cofactor00 + m[0][1] * cofactor10 + m[0][2] * cofactor20;
      }

      template <class T>
      bool Matrix3<T>::Inverse(Matrix3<T> &inv, const float tolerance) const
      {
         // Invert a 3x3 using cofactors.  This is about 8 times faster than
         // the Numerical Recipes code which uses Gaussian elimination.
         inv[0][0] = m[1][1] * m[2][2] - m[1][2] * m[2][1];
         inv[0][1] = m[0][2] * m[2][1] - m[0][1] * m[2][2];
         inv[0][2] = m[0][1] * m[1][2] - m[0][2] * m[1][1];
         inv[1][0] = m[1][2] * m[2][0] - m[1][0] * m[2][2];
         inv[1][1] = m[0][0] * m[2][2] - m[0][2] * m[2][0];
         inv[1][2] = m[0][2] * m[1][0] - m[0][0] * m[1][2];
         inv[2][0] = m[1][0] * m[2][1] - m[1][1] * m[2][0];
         inv[2][1] = m[0][1] * m[2][0] - m[0][0] * m[2][1];
         inv[2][2] = m[0][0] * m[1][1] - m[0][1] * m[1][0];

         T det =
            m[0][0] * inv[0][0] +
            m[0][1] * inv[1][0] +
            m[0][2] * inv[2][0];

         if (fabs(det) <= tolerance)
            return false;

         float invDet = 1.0f / det;
         for (int32 row = 0; row < 3; row++)
         {
            for (int32 col = 0; col < 3; col++)
               inv[row][col] *= invDet;
         }

         return true;
      }

   } // namespace math

} // namespace core

#endif
