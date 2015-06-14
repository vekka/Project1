#ifndef _MATHCOMMON_HPP_INCLUDED_
#define _MATHCOMMON_HPP_INCLUDED_

#include "core/BasicTypes.hpp"

#include <cmath>
#include <cassert>

namespace core
{

   namespace math
   {
      enum { X, Y, Z, W };

      const float ROUNDING_ERR_FLT = 0.000001f;
      const double ROUNDING_ERR_DBL = 0.00000001;

#ifdef PI
#undef PI
#endif

      const float PI = 3.14159265358979323846f;
      const float _2PI = 6.28318530718f;
      const float RECIPROCAL_PI = 0.31830988618f;
      const float HALF_PI = 1.570796326795f;
      const float SQRT2 = 1.41421356237309504880f;
      const float SQRTHALF = 0.707106781187f;
      const float CONST_E = 2.718281828459f;

#ifdef PI64
#undef PI64
#endif

      const double PI64 = 3.1415926535897932384626433832795028841971693993751;
      const double RECIPROCAL_PI64 = 1.0 / PI64;

      const float DEGTORAD = 0.01745329252f;
      const float RADTODEG = 57.29577951307f;
      const double DEGTORAD64 = PI64 / 180.0;
      const double RADTODEG64 = 180.0 / PI64;

      inline float RadToDeg(const float rad)
      {
         return RADTODEG * rad;
      }

      inline double RadToDeg(const double rad)
      {
         return RADTODEG64 * rad;
      }

      inline float DegToRad(const float deg)
      {
         return DEGTORAD * deg;
      }

      inline double DegToRad(const double deg)
      {
         return DEGTORAD64 * deg;
      }

      template<class T>
      inline const T &Min(const T &a, const T &b)
      {
         return a < b ? a : b;
      }

      template<class T>
      inline const T &Min(const T &a, const T &b, const T &c)
      {
         return a < b ? Min(a, c) : Min(b, c);
      }

      template<class T>
      inline const T &Max(const T &a, const T &b)
      {
         return a < b ? b : a;
      }

      template<class T>
      inline const T &Max(const T &a, const T &b, const T &c)
      {
         return a < b ? Max(b, c) : Max(a, c);
      }

      template<class T>
      inline T Abs(const T &a)
      {
         return a < (T)0 ? -a : a;
      }

      template <class T>
      inline const T Clamp(const T &val, const T &low, const T &high)
      {
         return Min(Max(val, low), high);
      }

      inline float ClampToZero(float &val)
      {
         if (val < ROUNDING_ERR_FLT && val > -ROUNDING_ERR_FLT)
            val = 0.0f;

         return val;
      }

      inline double ClampToZero(double &val)
      {
         if (val < ROUNDING_ERR_DBL && val > -ROUNDING_ERR_DBL)
            val = 0.0;

         return val;
      }

      template <class T1, class T2>
      inline void Swap(T1 &a, T2 &b)
      {
         T1 c(a);
         a = b;
         b = c;
      }

      // for better compability for templates
      inline bool Equals(const int8 a, const int8 b)
      {
         return (a == b);
      }

      // for better compability for templates
      inline bool Equals(const int16 a, const int16 b)
      {
         return (a == b);
      }

      // for better compability for templates
      inline bool Equals(const int32 a, const int32 b)
      {
         return (a == b);
      }

      // for better compability for templates
      inline bool Equals(const uint8 a, const uint8 b)
      {
         return (a == b);
      }

      // for better compability for templates
      inline bool Equals(const uint16 a, const uint16 b)
      {
         return (a == b);
      }

      // for better compability for templates
      inline bool Equals(const uint32 a, const uint32 b)
      {
         return (a == b);
      }

      inline bool Equals(const float a, const float b,
         const float tolerance = ROUNDING_ERR_FLT)
      {
         return (a + tolerance >= b) && (a - tolerance <= b);
      }

      inline bool Equals(const double a, const double b,
         const double tolerance = ROUNDING_ERR_DBL)
      {
         return (a + tolerance >= b) && (a - tolerance <= b);
      }

      // for better compability for templates
      inline bool IsZero(const int8 a)
      {
         return (a == 0);
      }

      // for better compability for templates
      inline bool IsZero(const int16 a)
      {
         return (a == 0);
      }

      // for better compability for templates
      inline bool IsZero(const int32 a)
      {
         return (a == 0);
      }

      inline bool IsZero(const float a, const float tolerance = ROUNDING_ERR_FLT)
      {
         return (a >= -tolerance) && (a <= tolerance);
      }

      inline bool IsZero(const double a, const double tolerance = ROUNDING_ERR_DBL)
      {
         return (a >= -tolerance) && (a <= tolerance);
      }

      union FloatIntUnion32
      {
         FloatIntUnion32(float f1 = 0.0f) : f(f1) {}
         // Portable sign-extraction
         bool sign() const { return (i >> 31) != 0; }

         uint32 i;
         float f;
      };

      template <class T1, class T2>
      inline T1 AlignToMultiple(const T1 val, const T2 mul)
      {
         T1 rem = val % mul;
         return (rem ? val + mul - rem : val);
      }

      inline bool IsPowerOfTwo(const uint32 value)
      {
         return (value > 0) && ((value & (value - 1)) == 0);
      }

      inline bool IsPowerOfTwo(const int32 value)
      {
         return (value > 0) && ((value & (value - 1)) == 0);
      }

      inline uint32 Log2OfPowerOfTwo(uint32 powerOfTwo)
      {
         uint32 log2 = (powerOfTwo & 0xaaaaaaaau) != 0;
         log2 |= ((powerOfTwo & 0xffff0000u) != 0) << 4;
         log2 |= ((powerOfTwo & 0xff00ff00u) != 0) << 3;
         log2 |= ((powerOfTwo & 0xf0f0f0f0u) != 0) << 2;
         log2 |= ((powerOfTwo & 0xccccccccu) != 0) << 1;
         return log2;
      }

      inline int32 Log2OfPowerOfTwo(int32 powerOfTwo)
      {
         uint32 log2 = (powerOfTwo & 0xaaaaaaaau) != 0;
         log2 |= ((powerOfTwo & 0xffff0000u) != 0) << 4;
         log2 |= ((powerOfTwo & 0xff00ff00u) != 0) << 3;
         log2 |= ((powerOfTwo & 0xf0f0f0f0u) != 0) << 2;
         log2 |= ((powerOfTwo & 0xccccccccu) != 0) << 1;
         return static_cast<int32>(log2);
      }

      template <typename T>
      inline T Lerp(const T &v1, const T &v2, float factor)
      {
         return (v1 * (1.0f - factor)) + (v2 * factor);
      }

   } // namespace math

} // namespace core

#endif