#ifndef _COLOR3F_HPP_INCLUDED_
#define _COLOR3F_HPP_INCLUDED_

#include <assert.h>

#include "color.hpp"

namespace gfx
{

   namespace color3f
   {

      class Color3f
      {
      public:
         float r, g, b;

         static const Color3f ZERO;
         static const Color3f ONE;
         static const Color3f BLACK;
         static const Color3f WHITE;
         static const Color3f RED;
         static const Color3f GREEN;
         static const Color3f BLUE;

         explicit Color3f(float red = 0.0f, float green = 0.0f, float blue = 0.0f) : r(red), g(green), b(blue) {}

         bool operator==(const Color3f &other) const;
         bool operator!=(const Color3f &other) const;
         Color3f operator+(const Color3f &color) const;
         Color3f &operator+=(const Color3f &other);
         Color3f operator-(const Color3f &other) const;
         Color3f &operator-=(const Color3f &other);
         Color3f operator*(const float scalar) const;
         Color3f &operator*=(const float scalar);
         Color3f operator*(const Color3f &other) const;
         Color3f operator/(const float scalar) const;
         Color3f &operator/=(const float scalar);
         Color3f operator/(const Color3f &other) const;

         color::RGB_t GetAsRGB() const;
         color::BGR_t GetAsBGR() const;

         void SetAsRGB(const color::RGBA_t color);
         void SetAsBGR(const color::ABGR_t color);

         void SetHSB(const float hue, const float saturation, const float brightness);
         void GetHSB(float* hue, float* saturation, float* brightness) const;
      };

      inline Color3f Color3f::operator+(const Color3f &other) const
      {
         return Color3f(r + other.r, g + other.g, b + other.b);
      }

      inline Color3f &Color3f::operator+=(const Color3f &other)
      {
         r += other.r;
         g += other.g;
         b += other.b;
         return *this;
      }

      inline Color3f Color3f::operator-(const Color3f &other) const
      {
         return Color3f(r - other.r, g - other.g, b - other.b);
      }

      inline Color3f &Color3f::operator-=(const Color3f &other)
      {
         r -= other.r;
         g -= other.g;
         b -= other.b;
         return *this;
      }

      inline Color3f Color3f::operator*(const float scalar) const
      {
         return Color3f(scalar * r, scalar * g, scalar * b);
      }

      inline Color3f &Color3f::operator*=(const float scalar)
      {
         r *= scalar;
         g *= scalar;
         b *= scalar;
         return *this;
      }

      inline Color3f Color3f::operator*(const Color3f &other) const
      {
         return Color3f(r * other.r, g * other.g, b * other.b);
      }

      inline Color3f Color3f::operator/(const float scalar) const
      {
         assert(scalar != 0.0);

         float inv = 1.0f / scalar;
         return Color3f(r * inv, g * inv, b * inv);
      }

      inline Color3f &Color3f::operator/=(const float scalar)
      {
         assert(scalar != 0.0);

         float inv = 1.0f / scalar;
         r *= inv;
         g *= inv;
         b *= inv;
         return *this;
      }

      inline Color3f Color3f::operator/(const Color3f &other) const
      {
         return Color3f(r / other.r, g / other.g, b / other.b, a / other.a);
      }

   } // namespace color4f

} // namespace gfx