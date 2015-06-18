#ifndef _MATRIX4_HPP_INCLUDED_
#define _MATRIX4_HPP_INCLUDED_

#include <iostream>

#include <cstring>

#include "vector3.hpp"
#include "vector4.hpp"

#include "matrix3.hpp"

namespace core
{

   namespace math
   {

      template <typename T>
      class Matrix4
      {
      private:
         T m[4][4];

      public:
         static const Matrix4<T> ZERO;
         static const Matrix4<T> IDENTITY;

         Matrix4();
         Matrix4(const T arr[4][4]);
         Matrix4(
            const T m00, const T m01, const T m02, const T m03,
            const T m10, const T m11, const T m12, const T m13,
            const T m20, const T m21, const T m22, const T m23,
            const T m30, const T m31, const T m32, const T m33);
         //Matrix4 &Matrix4::operator=(const Matrix4 &other);
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
         Matrix4<T> &SetRotationRadians(const Vector3<T> &rotation);
         Matrix4<T> &SetRotationDegrees(const Vector3<T> &rotation);
         Vector3<T> GetRotationDegrees() const;
         void CreateTranslation(const Vector3<T> &vec);
         void CreateTranslation(const T x, const T y, const T z);
         Matrix3<T> GetMatrix3(const Matrix4 &) const;
         bool IsAffine(void) const;

         friend std::ostream& operator<<(std::ostream &out, Matrix4 &mat4)
         {
            out << "((" << mat4(0,0) << ", " << mat4(0,1) << ", " << mat4(0,2) << ", " << mat4(0,3) << "), " <<
               "(" << mat4(1,0) << ", " << mat4(1,1) << ", " << mat4(1,2) << ", " << mat4(1,3) << "), " <<
               "(" << mat4(2,0) << ", " << mat4(2,1) << ", " << mat4(2,2) << ", " << mat4(2,3) << "), " <<
               "(" << mat4(3,0) << ", " << mat4(3,1) << ", " << mat4(3,2) << ", " << mat4(3,3) << "))";
            return out;
         }
      };

      typedef Matrix4<int16> Matrix4s;
      typedef Matrix4<int32> Matrix4i;
      typedef Matrix4<uint16> Matrix4us;
      typedef Matrix4<uint32> Matrix4u;
      typedef Matrix4<float> Matrix4f;
      typedef Matrix4<double> Matrix4d;
      typedef Matrix4<long double> Matrix4ld; // TODO: replace with new data type

      template <typename T> const Matrix4<T> Matrix4<T>::ZERO(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      template <typename T> const Matrix4<T> Matrix4<T>::IDENTITY(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

      template <typename T>
      inline Matrix4<T>::Matrix4()
      {
      }

      template <typename T>
      inline Matrix4<T>::Matrix4(const T arr[4][4])
      {
         memcpy(m, arr, 16 * sizeof(T));
      }

      template <typename T>
      inline Matrix4<T>::Matrix4(
         const T m00, const T m01, const T m02, const T m03,
         const T m10, const T m11, const T m12, const T m13,
         const T m20, const T m21, const T m22, const T m23,
         const T m30, const T m31, const T m32, const T m33)
      {
         m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
         m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
         m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
         m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
      }

      //template <typename T>
      //Matrix4<T> &Matrix4<T>::operator=(const Matrix4<T> &other)
      //{
      //   if (this == &other)
      //      return *this;
      //   std::memcpy(m, other.Ptr(), 16*sizeof(T));
      //   return *this;
      //}

      template <typename T>
      inline void Matrix4<T>::Zero()
      {
         m[0][0] = m[0][1] = m[0][2] = m[0][3]
            = m[1][0] = m[1][1] = m[1][2] = m[1][3]
            = m[2][0] = m[2][1] = m[2][2] = m[2][3]
            = m[3][0] = m[3][1] = m[3][2] = m[3][3] = (T)0;
      }

      template <typename T>
      inline void Matrix4<T>::SetIdentity()
      {
         m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] =
             m[2][0] = m[2][1] = m[2][3] = (T)0;

         m[0][0] = m[1][1] = m[2][2] = m[3][3] = (T)1;
      }

      /*
         0,0   0,1   0,2   0,3
         1,0   1,1   1,2   1,3
         2,0   2,1   2,2   2,3
         3,0   3,1   3,2   3,3

      */

      template <typename T>
      inline void Matrix4<T>::Set(
         const T m00, const T m01, const T m02, const T m03,
         const T m10, const T m11, const T m12, const T m13,
         const T m20, const T m21, const T m22, const T m23,
         const T m30, const T m31, const T m32, const T m33)
      {
         m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
         m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
         m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
         m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
      }

      template <typename T>
      inline void Matrix4<T>::SetDiagonal(const T d0, const T d1, const T d2, const T d3)
      {
         Matrix4<T>(d0, 0, 0, 0, 0, d1, 0, 0, 0, 0, d2, 0, 0, 0, 0, d3);
      }

      template <typename T>
      inline T Matrix4<T>::operator()(const uint8 index1, const uint8 index2) const
      {
         assert(index1 < 4 && index2 < 4);
         return m[index1][index2];
      }

      template <typename T>
      inline T &Matrix4<T>::operator()(const uint8 index1, const uint8 index2)
      {
         assert(index1 < 4 && index2 < 4);
         return m[index1][index2];
      }

      template <typename T>
      inline T Matrix4<T>::operator[](const uint8 index) const
      {
         assert(index < 16);
         return (&x)[index];
      }

      template <typename T>
      inline T &Matrix4<T>::operator[](const uint8 index)
      {
         assert(index < 16);
         return (m)[index];
      }

      template <typename T>
      inline T* Matrix4<T>::Ptr()
      {
         return &m[0][0];
      }

      // Pointer accessor for direct copying
      template <typename T>
      inline const T* Matrix4<T>::Ptr() const
      {
         return &m[0][0];
      }

      template <typename T>
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

      template <typename T>
      inline Vector4<T> Matrix4<T>::operator*(const Vector4<T> &vec) const
      {
         return Vector4<T>(
            m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3] * vec.w,
            m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3] * vec.w,
            m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3] * vec.w,
            m[3][0] * vec.x + m[3][1] * vec.y + m[3][2] * vec.z + m[3][3] * vec.w);
      }

      template <typename T>
      inline Matrix4<T> Matrix4<T>::operator*(const T scalar) const
      {
         return Matrix4(
            scalar*m[0][0], scalar*m[0][1], scalar*m[0][2], scalar*m[0][3],
            scalar*m[1][0], scalar*m[1][1], scalar*m[1][2], scalar*m[1][3],
            scalar*m[2][0], scalar*m[2][1], scalar*m[2][2], scalar*m[2][3],
            scalar*m[3][0], scalar*m[3][1], scalar*m[3][2], scalar*m[3][3]);
      }

      template <typename T>
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

      template <typename T>
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

      template <typename T>
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

      template <typename T>
      inline Matrix4<T> Matrix4<T>::Transpose() const
      {
         return Matrix4(
            m[0][0], m[1][0], m[2][0], m[3][0],
            m[0][1], m[1][1], m[2][1], m[3][1],
            m[0][2], m[1][2], m[2][2], m[3][2],
            m[0][3], m[1][3], m[2][3], m[3][3]);
      }

      template <typename T>
      inline void Matrix4<T>::SetTranslation(const Vector3<T> &vec)
      {
         m[0][3] = vec.x;
         m[1][3] = vec.y;
         m[2][3] = vec.z;
      }

      template <typename T>
      inline void Matrix4<T>::SetTranslation(const T x, const T y, const T z)
      {
         m[0][3] = x;
         m[1][3] = y;
         m[2][3] = z;
      }

      template <typename T>
      inline Vector3<T> Matrix4<T>::GetTranslationVec() const
      {
         return Vector3<T>(m[0][3], m[1][3], m[2][3]);
      }

      template <typename T>
      inline void Matrix4<T>::SetScale(const Vector3<T> &vec)
      {
         m[0][0] = vec.x;
         m[1][1] = vec.y;
         m[2][2] = vec.z;
      }

      template <typename T>
      inline void Matrix4<T>::SetScale(const T x, const T y, const T z)
      {
         m[0][0] = x;
         m[1][1] = y;
         m[2][2] = z;
      }

      template <typename T>
      inline void Matrix4<T>::SetScale(const T scale)
      {
         m[0][0] = scalingFactor;
         m[1][1] = scalingFactor;
         m[2][2] = scalingFactor;
      }

      template <typename T>
      Vector3<T> Matrix4<T>::GetScaleVec() const
      {
         return Vector3<T>(m[0][0], m[1][1], m[2][2]);
      }

      template <typename T>
      inline void Matrix4<T>::CreateTranslation(const Vector3<T> &vec)
      {
         m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = vec.x;
         m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = vec.y;
         m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = vec.z;
         m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
      }

      template <typename T>
      inline void Matrix4<T>::CreateTranslation(const T x, const T y, const T z)
      {
         m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = x;
         m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = y;
         m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = z;
         m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
      }

      template <typename T>
      Matrix4<T> &Matrix4<T>::SetRotationRadians(const Vector3<T> &rotation)
      {
         const double cr = cos(rotation.x);
         const double sr = sin(rotation.x);
         const double cp = cos(rotation.y);
         const double sp = sin(rotation.y);
         const double cy = cos(rotation.z);
         const double sy = sin(rotation.z);

         m[0][0] = (T)(cp*cy);
         m[0][1] = (T)(cp*sy);
         m[0][2] = (T)(-sp);

         const double srsp = sr*sp;
         const double crsp = cr*sp;

         m[1][0] = (T)(srsp*cy - cr*sy);
         m[1][1] = (T)(srsp*sy + cr*cy);
         m[1][2] = (T)(sr*cp);

         m[2][0] = (T)(crsp*cy + sr*sy);
         m[2][1] = (T)(crsp*sy - sr*cy);
         m[2][2] = (T)(cr*cp);

         return *this;
      }

      template <typename T>
      Matrix4<T> &Matrix4<T>::SetRotationDegrees(const Vector3<T> &rotation)
      {
          return SetRotationRadians( rotation * core::DEGTORAD );
      }

      template <typename T>
      Vector3<T> Matrix4<T>::GetRotationDegrees() const
      {
        const Matrix4<T> &mat = *this;
		const Vector3f<T> scale = GetScale();
		const Vector3f<double> invScale(Reciprocal(scale.x), Reciprocal(scale.y), Reciprocal(scale.z));

		//double y = -asin(m[2]*invScale.X);
		//const double c = cos(Y);
		//y *= RADTODEG64;

		float rotx, roty, x, z;

		if (!IsZero(c))
		{
			const double invC = Reciprocal(c);
			rotx = mat[2][2] * invC * invScale.z;
			roty = mat[1][2] * invC * invScale.y;
			x = atan2( roty, rotx ) * RADTODEG64;
			rotx = mat[0][0] * invC * invScale.x;
			roty = mat[0][1] * invC * invScale.x;
			z = atan2( roty, rotx ) * RADTODEG64;
		}
		else
		{
			x = 0.0;
			rotx = mat[5] * invScale.y;
			roty = -mat[4] * invScale.y;
			z = atan2( roty, rotx ) * RADTODEG64;
		}

		// fix values that get below zero
		// before it would set (!) values to 360
		// that were above 360:
		if (x < 0.0) x += 360.0;
		if (y < 0.0) y += 360.0;
		if (z < 0.0) z += 360.0;

		return Vector3f<T>((T)x,(T)y,(T)z);
      }

      // Extracts the rotation / scaling part of the Matrix as a 3x3 matrix.
      template <typename T>
      inline Matrix3<T> Matrix4<T>::GetMatrix3(const Matrix4<T> &mat4) const
      {
         return Matrix3<T>(
            m[0][0], m[0][1], m[0][2],
            m[1][0], m[1][1], m[1][2],
            m[2][0], m[2][1], m[2][2]);
      }

      template <typename T>
      inline bool Matrix4<T>::IsAffine(void) const
      {
         return m[3][0] == 0 && m[3][1] == 0 && m[3][2] == 0 && m[3][3] == 1;
      }

   } // namespace math

} // namespace core

#endif
