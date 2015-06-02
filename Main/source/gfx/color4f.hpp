#ifndef _COLOR4F_HPP_INCLUDED_
#define _COLOR4F_HPP_INCLUDED_

#include <assert.h>

#include "color.hpp"

namespace gfx
{

   namespace color4f
   {

      class Color4f
      {
      public:
         float r, g, b, a;

         static const Color4f ZERO;
         static const Color4f ONE;
         static const Color4f BLACK;
         static const Color4f WHITE;
         static const Color4f RED;
         static const Color4f GREEN;
         static const Color4f BLUE;

         explicit Color4f(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 0.0f)
            : r(red), g(green), b(blue), a(alpha) {}

         bool operator==(const Color4f &other) const;
         bool operator!=(const Color4f &other) const;
         Color4f operator+(const Color4f &color) const;
         Color4f &operator+=(const Color4f &other);
         Color4f operator-(const Color4f &other) const;
         Color4f &operator-=(const Color4f &other);
         Color4f operator*(const float scalar) const;
         Color4f &operator*=(const float scalar);
         Color4f operator*(const Color4f &other) const;
         Color4f operator/(const float scalar) const;
         Color4f &operator/=(const float scalar);
         Color4f operator/(const Color4f &other) const;

         color::RGBA_t GetAsRGBA() const;
         color::ARGB_t GetAsARGB() const;
         color::BGRA_t GetAsBGRA() const;
         color::ABGR_t GetAsABGR() const;

         void SetAsRGBA(const color::RGBA_t color);
         void SetAsARGB(const color::ARGB_t color);
         void SetAsBGRA(const color::BGRA_t color);
         void SetAsABGR(const color::ABGR_t color);

         void SetHSB(const float hue, const float saturation, const float brightness);
         void GetHSB(float* hue, float* saturation, float* brightness) const;
      };

      inline Color4f Color4f::operator+(const Color4f &other) const
      {
         return Color4f(r + other.r, g + other.g, b + other.b, a + other.a);
      }

      inline Color4f &Color4f::operator+=(const Color4f &other)
      {
         r += other.r;
         g += other.g;
         b += other.b;
         a += other.a;
         return *this;
      }

      inline Color4f Color4f::operator-(const Color4f &other) const
      {
         return Color4f(r - other.r, g - other.g, b - other.b, a - other.a);
      }

      inline Color4f &Color4f::operator-=(const Color4f &other)
      {
         r -= other.r;
         g -= other.g;
         b -= other.b;
         a -= other.a;
         return *this;
      }

      inline Color4f Color4f::operator*(const float scalar) const
      {
         return Color4f(scalar * r, scalar * g, scalar * b, scalar * a);
      }

      inline Color4f &Color4f::operator*=(const float scalar)
      {
         r *= scalar;
         g *= scalar;
         b *= scalar;
         a *= scalar;
         return *this;
      }

      inline Color4f Color4f::operator*(const Color4f &other) const
      {
         return Color4f(r * other.r, g * other.g, b * other.b, a * other.a);
      }

      inline Color4f Color4f::operator/(const float scalar) const
      {
         assert(scalar != 0.0);

         float inv = 1.0f / scalar;
         return Color4f(r * inv, g * inv, b * inv, a * inv);
      }

      inline Color4f &Color4f::operator/=(const float scalar)
      {
         assert(scalar != 0.0);

         float inv = 1.0f / scalar;
         r *= inv;
         g *= inv;
         b *= inv;
         a *= inv;
         return *this;
      }

      inline Color4f Color4f::operator/(const Color4f &other) const
      {
         return Color4f(r / other.r, g / other.g, b / other.b, a / other.a);
      }

   } // namespace color4f

} // namespace gfx