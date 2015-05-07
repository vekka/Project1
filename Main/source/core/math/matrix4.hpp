#ifndef _MATRIX4_HPP_INCLUDED_
#define _MATRIX4_HPP_INCLUDED_

#include "vector3.hpp"
#include "vector4.hpp"

#include "matrix3.hpp"

namespace core
{

   namespace math
   {

      enum eConstructorMatrix4
      {
         MAT4_CONST_NOTHING,
         MAT4_CONST_ZERO,
         MAT4_CONST_IDENTITY
      };

      template <class T>
      class Matrix4
      {
      private:
         T m[4][4];
      public:
         Matrix4();
         Matrix4(const eConstructorMatrix4 constructor);
         Matrix4(const T arr[4][4]);
         Matrix4(
            const T m00, const T m01, const T m02, const T m03,
            const T m10, const T m11, const T m12, const T m13,
            const T m20, const T m21, const T m22, const T m23,
            const T m30, const T m31, const T m32, const T m33);
         void Zero();
         void SetIdentity();
         void Set(
            const T m00, const T m01, const T m02, const T m03,
            const T m10, const T m11, const T m12, const T m13,
            const T m20, const T m21, const T m22, const T m23,
            const T m30, const T m31, const T m32, const T m33);
         void SetDiagonal(const T d0, const T d1, const T d2, const T d3);
         T operator()(const uint8 index1, const uint8 index2) const;
         T &operator()(const uint8 index1, const uint8 index2);
         T operator[](const uint8 index) const;
         T &operator[](const uint8 index);
         // Pointer accessor for direct copying
         T* Ptr();
         const T* Ptr() const;
         Matrix4 operator*(const Matrix4 &other) const;
         Vector4<T> operator*(const Vector4<T> &vec) const;
         Matrix4 operator*(const T scalar) const;
         Matrix4 &operator*=(const T scalar);
         Matrix4 operator+(Matrix4 const &other) const;
         Matrix4 operator-(Matrix4 const &other) const;
         Matrix4 &operator+=(Matrix4 const &other);
         Matrix4 &operator-=(Matrix4 const &other);
         void Swap(Matrix4 &other);
         Matrix4 Transpose() const;
         void SetTranslation(const Vector3<T> &vec);
         void SetTranslation(const T x, const T y, const T z);
         Vector3<T> GetTranslationVec() const;
         void SetScale(const Vector3<T> &vec);
         void SetScale(const T scale);
         void SetScale(const T x, const T y, const T z);
         Vector3<T> GetScaleVec() const;
         void CreateTranslation(const Vector3<T> &vec);
         void CreateTranslation(const T x, const T y, const T z);
         Matrix3<T> GetMatrix3(const Matrix4 &) const;
         bool IsAffine(void) const;
      };

      typedef Matrix4<int16> Matrix4s;
      typedef Matrix4<int32> Matrix4i;
      typedef Matrix4<uint16> Matrix4us;
      typedef Matrix4<uint32> Matrix4u;
      typedef Matrix4<float> Matrix4f;
      typedef Matrix4<double> Matrix4d;
      typedef Matrix4<long double> Matrix4ld; // TODO: replace with new data type

      template <class T>
      inline Matrix4<T>::Matrix4()
      {
      }

      template <class T>
      inline Matrix4<T>::Matrix4(const eConstructorMatrix4 constructor)
      {
         switch (constructor)
         {
         case MAT4_CONST_NOTHING:
            break;
         case MAT4_CONST_ZERO:
            Zero();
            break;
         case MAT4_CONST_IDENTITY:
            SetIdentity();
            break;
         }
      }

      template <class T>
      inline Matrix4<T>::Matrix4(const T arr[4][4])
      {
         memcpy(m, arr, 16 * sizeof(T));
      }

      template <class T>
      inline Matrix4<T>::Matrix4(
         const T m00, const T m01, const T m02, const T m03,
         const T m10, const T m11, const T m12, const T m13,
         const T m20, const T m21, const T m22, const T m23,
         const T m30, const T m31, const T m32, const T m33)
      {
         m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
         m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
         m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
         m[3][0] = m30; m[3][1] = m21; m[3][2] = m22; m[3][3] = m33;
      }

      template <class T>
      inline void Matrix4<T>::Zero()
      {
         m[0][0] = m[0][1] = m[0][2] = m[0][3]
            = m[1][0] = m[1][1] = m[1][2] = m[1][3]
            = m[2][0] = m[2][1] = m[2][2] = m[2][3]
            = m[3][0] = m[3][1] = m[3][2] = m[3][3] = (T)0;
      }

      template <class T>
      inline void Matrix4<T>::SetIdentity()
      {
         m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3]
            = m[2][0] = m[2][1] = m[2][3](T)0;
         m[0][0] = m[1][1] = m[2][2] = m[3][3] = (T)1;
      }

      template <class T>
      inline void Matrix4<T>::Set(
         const T m00, const T m01, const T m02, const T m03,
         const T m10, const T m11, const T m12, const T m13,
         const T m20, const T m21, const T m22, const T m23,
         const T m30, const T m31, const T m32, const T m33)
      {
         m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
         m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
         m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
         m[3][0] = m30; m[3][1] = m21; m[3][2] = m22; m[3][3] = m33;
      }

      template <class T>
      inline void Matrix4<T>::SetDiagonal(const T d0, const T d1, const T d2, const T d3)
      {
         Matrix4<T>(d0, 0, 0, 0, 0, d1, 0, 0, 0, 0, d2, 0, 0, 0, 0, d3);
      }

      template <class T>
      inline T Matrix4<T>::operator()(const uint8 index1, const uint8 index2) const
      {
         assert(index1 < 4 && index2 < 4);
         return m[index1][index2];
      }

      template <class T>
      inline T &Matrix4<T>::operator()(const uint8 index1, const uint8 index2)
      {
         assert(index1 < 4 && index2 < 4);
         return m[index1][index2];
      }

      template <class T>
      inline T Matrix4<T>::operator[](const uint8 index) const
      {
         assert(index < 16);
         return (&x)[index];
      }

      template <class T>
      inline T &Matrix4<T>::operator[](const uint8 index)
      {
         assert(index < 16);
         return (&x)[index];
      }

      template <class T>
      inline T* Matrix4<T>::Ptr()
      {
         return &m[0][0];
      }

      // Pointer accessor for direct copying
      template <class T>
      inline const T* Matrix4<T>::Ptr() const
      {
         return &m[0][0];
      }

      template <class T>
      inline Matrix4<T> Matrix4<T>::operator*(const Matrix4<T> &other) const
      {
         return Matrix4<T>(
            m[0][0] * other.m[0][0] + m[0][1] * other.m[1][0] + m[0][2] * other.m[2][0] + m[0][3] * other.m[3][0],
            m[0][0] * other.m[0][1] + m[0][1] * other.m[1][1] + m[0][2] * other.m[2][1] + m[0][3] * other.m[3][1],
            m[0][0] * other.m[0][2] + m[0][1] * other.m[1][2] + m[0][2] * other.m[2][2] + m[0][3] * other.m[3][2],
            m[0][0] * other.m[0][3] + m[0][1] * other.m[1][3] + m[0][2] * other.m[2][3] + m[0][3] * other.m[3][3],

            m[1][0] * other.m[0][0] + m[1][1] * other.m[1][0] + m[1][2] * other.m[2][0] + m[1][3] * other.m[3][0],
            m[1][0] * other.m[0][1] + m[1][1] * other.m[1][1] + m[1][2] * other.m[2][1] + m[1][3] * other.m[3][1],
            m[1][0] * other.m[0][2] + m[1][1] * other.m[1][2] + m[1][2] * other.m[2][2] + m[1][3] * other.m[3][2],
            m[1][0] * other.m[0][3] + m[1][1] * other.m[1][3] + m[1][2] * other.m[2][3] + m[1][3] * other.m[3][3],

            m[2][0] * other.m[0][0] + m[2][1] * other.m[1][0] + m[2][2] * other.m[2][0] + m[2][3] * other.m[3][0],
            m[2][0] * other.m[0][1] + m[2][1] * other.m[1][1] + m[2][2] * other.m[2][1] + m[2][3] * other.m[3][1],
            m[2][0] * other.m[0][2] + m[2][1] * other.m[1][2] + m[2][2] * other.m[2][2] + m[2][3] * other.m[3][2],
            m[2][0] * other.m[0][3] + m[2][1] * other.m[1][3] + m[2][2] * other.m[2][3] + m[2][3] * other.m[3][3],

            m[3][0] * other.m[0][0] + m[3][1] * other.m[1][0] + m[3][2] * other.m[2][0] + m[3][3] * other.m[3][0],
            m[3][0] * other.m[0][1] + m[3][1] * other.m[1][1] + m[3][2] * other.m[2][1] + m[3][3] * other.m[3][1],
            m[3][0] * other.m[0][2] + m[3][1] * other.m[1][2] + m[3][2] * other.m[2][2] + m[3][3] * other.m[3][2],
            m[3][0] * other.m[0][3] + m[3][1] * other.m[1][3] + m[3][2] * other.m[2][3] + m[3][3] * other.m[3][3]);
      }

      template <class T>
      inline Vector4<T> Matrix4<T>::operator*(const Vector4<T> &vec) const
      {
         return Vector4<T>(
            m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3] * vec.w,
            m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3] * vec.w,
            m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3] * vec.w,
            m[3][0] * vec.x + m[3][1] * vec.y + m[3][2] * vec.z + m[3][3] * vec.w
            );
      }

      template <class T>
      inline Matrix4<T> Matrix4<T>::operator*(const T scalar) const
      {
         return Matrix4(
            scalar*m[0][0], scalar*m[0][1], scalar*m[0][2], scalar*m[0][3],
            scalar*m[1][0], scalar*m[1][1], scalar*m[1][2], scalar*m[1][3],
            scalar*m[2][0], scalar*m[2][1], scalar*m[2][2], scalar*m[2][3],
            scalar*m[3][0], scalar*m[3][1], scalar*m[3][2], scalar*m[3][3]);
      }

      template <class T>
      inline Matrix4<T> Matrix4<T>::operator+(const Matrix4<T> &other) const
      {
         return Matrix4<T>(
            m[0][0] = m[0][0] + other.m[0][0],
            m[0][1] = m[0][1] + other.m[0][1],
            m[0][2] = m[0][2] + other.m[0][2],
            m[0][3] = m[0][3] + other.m[0][3],

            m[1][0] = m[1][0] + other.m[1][0],
            m[1][1] = m[1][1] + other.m[1][1],
            m[1][2] = m[1][2] + other.m[1][2],
            m[1][3] = m[1][3] + other.m[1][3],

            m[2][0] = m[2][0] + other.m[2][0],
            m[2][1] = m[2][1] + other.m[2][1],
            m[2][2] = m[2][2] + other.m[2][2],
            m[2][3] = m[2][3] + other.m[2][3],

            m[3][0] = m[3][0] + other.m[3][0],
            m[3][1] = m[3][1] + other.m[3][1],
            m[3][2] = m[3][2] + other.m[3][2],
            m[3][3] = m[3][3] + other.m[3][3]);
      }

      template <class T>
      inline Matrix4<T> Matrix4<T>::operator-(const Matrix4<T> &other) const
      {
         return Matrix4<T>(
            m[0][0] = m[0][0] - other.m[0][0],
            m[0][1] = m[0][1] - other.m[0][1],
            m[0][2] = m[0][2] - other.m[0][2],
            m[0][3] = m[0][3] - other.m[0][3],

            m[1][0] = m[1][0] - other.m[1][0],
            m[1][1] = m[1][1] - other.m[1][1],
            m[1][2] = m[1][2] - other.m[1][2],
            m[1][3] = m[1][3] - other.m[1][3],

            m[2][0] = m[2][0] - other.m[2][0],
            m[2][1] = m[2][1] - other.m[2][1],
            m[2][2] = m[2][2] - other.m[2][2],
            m[2][3] = m[2][3] - other.m[2][3],

            m[3][0] = m[3][0] - other.m[3][0],
            m[3][1] = m[3][1] - other.m[3][1],
            m[3][2] = m[3][2] - other.m[3][2],
            m[3][3] = m[3][3] - other.m[3][3]);
      }

      template <class T>
      inline void Matrix4<T>::Swap(Matrix4<T> &other)
      {
         Swap(m[0][0], other.m[0][0]);
         Swap(m[0][1], other.m[0][1]);
         Swap(m[0][2], other.m[0][2]);
         Swap(m[0][3], other.m[0][3]);
         Swap(m[1][0], other.m[1][0]);
         Swap(m[1][1], other.m[1][1]);
         Swap(m[1][2], other.m[1][2]);
         Swap(m[1][3], other.m[1][3]);
         Swap(m[2][0], other.m[2][0]);
         Swap(m[2][1], other.m[2][1]);
         Swap(m[2][2], other.m[2][2]);
         Swap(m[2][3], other.m[2][3]);
         Swap(m[3][0], other.m[3][0]);
         Swap(m[3][1], other.m[3][1]);
         Swap(m[3][2], other.m[3][2]);
         Swap(m[3][3], other.m[3][3]);
      }

      template <class T>
      inline Matrix4<T> Matrix4<T>::Transpose() const
      {
         return Matrix4(
            m[0][0], m[1][0], m[2][0], m[3][0],
            m[0][1], m[1][1], m[2][1], m[3][1],
            m[0][2], m[1][2], m[2][2], m[3][2],
            m[0][3], m[1][3], m[2][3], m[3][3]);
      }

      template <class T>
      inline void Matrix4<T>::SetTranslation(const Vector3<T> &vec)
      {
         m[0][3] = vec.x;
         m[1][3] = vec.y;
         m[2][3] = vec.z;
      }

      template <class T>
      inline void Matrix4<T>::SetTranslation(const T x, const T y, const T z)
      {
         m[0][3] = x;
         m[1][3] = y;
         m[2][3] = z;
      }

      template <class T>
      inline Vector3<T> Matrix4<T>::GetTranslationVec() const
      {
         return Vector3<T>(m[0][3], m[1][3], m[2][3]);
      }

      template <class T>
      inline void Matrix4<T>::SetScale(const Vector3<T> &vec)
      {
         m[0][0] = vec.x;
         m[1][1] = vec.y;
         m[2][2] = vec.z;
      }

      template <class T>
      inline void Matrix4<T>::SetScale(const T x, const T y, const T z)
      {
         m[0][0] = x;
         m[1][1] = y;
         m[2][2] = z;
      }

      template <class T>
      inline void Matrix4<T>::SetScale(const T scale)
      {
         m[0][0] = scalingFactor;
         m[1][1] = scalingFactor;
         m[2][2] = scalingFactor;
      }

      template <class T>
      Vector3<T> Matrix4<T>::GetScaleVec() const
      {
         return Vector3<T>(m[0][0], m[1][1], m[2][2]);
      }

      template <class T>
      inline void Matrix4<T>::CreateTranslation(const Vector3<T> &vec)
      {
         m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = vec.x;
         m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = vec.y;
         m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = vec.z;
         m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
      }

      template <class T>
      inline void Matrix4<T>::CreateTranslation(const T x, const T y, const T z)
      {
         m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = x;
         m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = y;
         m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = z;
         m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
      }

      // Extracts the rotation / scaling part of the Matrix as a 3x3 matrix. 
      template <class T>
      inline Matrix3<T> Matrix4<T>::GetMatrix3(const Matrix4<T> &mat4) const
      {
         return Matrix3<T>(
            m[0][0], m[0][1], m[0][2],
            m[1][0], m[1][1], m[1][2],
            m[2][0], m[2][1], m[2][2]);
      }

      template <class T>
      inline bool Matrix4<T>::IsAffine(void) const
      {
         return m[3][0] == 0 && m[3][1] == 0 && m[3][2] == 0 && m[3][3] == 1;
      }

   } // namespace math

} // namespace core

#endif