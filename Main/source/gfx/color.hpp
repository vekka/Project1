#ifndef _COLOR_HPP_INCLUDED_
#define _COLOR_HPP_INCLUDED_

#include <assert.h>

#include "core/math/mathcommon.hpp"

namespace gfx
{

   namespace color
   {
      enum eIdxColorComponent
      {
         IDX_RED, IDX_GREEN, IDX_BLUE, IDX_ALPHA
      };

      inline uint16 CreateA1R5G5B5(uint16 r, uint16 g, uint16 b, uint16 a = 0xFF)
      {
         return (uint16)(
            (a & 0x80) << 8 |
            (r & 0xf8) << 7 |
            (g & 0xf8) << 2 |
            (b & 0xf8) >> 3);
      }

      inline uint16 CreateA1R5G5B5(uint16 r, uint16 g, uint16 b)
      {
         return (0x8000 |
            (r & 0x1f) << 10 |
            (g & 0x1f) << 5 |
            (b & 0x1f));
      }

      inline uint16 X8R8G8B8toA1R5G5B5(uint32 color)
      {
         return (uint16)(0x8000 |
            (color & 0x00f80000) >> 9 |
            (color & 0x0000f800) >> 6 |
            (color & 0x000000f8) >> 3);
      }

      inline uint32 A8R8G8B8toA1R5G5B5(uint32 color)
      {
         return (uint16)(
            (color & 0x80000000) >> 16 |
            (color & 0x00f80000) >> 9 |
            (color & 0x0000f800) >> 6 |
            (color & 0x000000f8) >> 3);
      }

      inline uint16 A8R8G8B8toR5G6B5(uint32 color)
      {
         return (uint16)(
            (color & 0x00f80000) >> 8 |
            (color & 0x0000fc00) >> 5 |
            (color & 0x000000f8) >> 3);
      }

      inline uint32 A1R5G5B5toA8R8G8B8(uint16 color)
      {
         return 0;
         //return (((-((uint32)color & 0x00008000) >> (uint32)31) & 0xff000000) |
         //   ((color & 0x00007c00) << 9) | ((color & 0x00007000) << 4) |
         //   ((color & 0x000003e0) << 6) | ((color & 0x00000380) << 1) |
         //   ((color & 0x0000001f) << 3) | ((color & 0x0000001c) >> 2)
         //   );
      }

      inline uint32 R5G6B5toA8R8G8B8(uint16 color)
      {
         return 0xff000000 |
            ((color & 0xf800) << 8) |
            ((color & 0x07e0) << 5) |
            ((color & 0x001f) << 3);
      }

      inline uint16 R5G6B5toA1R5G5B5(uint16 color)
      {
         return 0x8000 | (((color & 0xffc0) >> 1) | (color & 0x1f));
      }

      inline uint16 A1R5G5B5toR5G6B5(uint16 color)
      {
         return (((color & 0x7fe0) << 1) | (color & 0x1f));
      }

      inline uint32 GetAlphaFromA1R5G5B5(uint16 color)
      {
         return ((color >> 15) & 0x1);
      }

      inline uint32 GetRedFromA1R5G5B5(uint16 color)
      {
         return ((color >> 10) & 0x1f);
      }

      inline uint32 GetGreenFromA1R5G5B5(uint16 color)
      {
         return ((color >> 5) & 0x1f);
      }

      inline uint32 GetBlueFromA1R5G5B5(uint16 color)
      {
         return (color & 0x1f);
      }

      inline int32 GetAverageFromA1R5G5B5(int16 color)
      {
         return (
            (GetRedFromA1R5G5B5(color) << 3) +
            (GetGreenFromA1R5G5B5(color) << 3) +
            (GetBlueFromA1R5G5B5(color) << 3)) / 3;
      }

      typedef int32 RGBA_t;
      typedef int32 ARGB_t;
      typedef int32 BGRA_t;
      typedef int32 ABGR_t;

      class Colorf
      {
      private:
         float r, g, b, a;
      public:
         static const Colorf ZERO;
         static const Colorf ONE;
         static const Colorf BLACK;
         static const Colorf WHITE;
         static const Colorf RED;
         static const Colorf GREEN;
         static const Colorf BLUE;

         explicit Colorf(float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f)
            : r(red), g(green), b(blue), a(alpha) {}

         bool operator==(const Colorf &other) const;
         bool operator!=(const Colorf &other) const;
         float operator[](const eIdxColorComponent index) const;
         float &operator[](const eIdxColorComponent index);
         Colorf operator+(const Colorf &color) const;
         Colorf &operator+=(const Colorf &other);
         Colorf operator-(const Colorf &other) const;
         Colorf &operator-=(const Colorf &other);
         Colorf operator*(const float scalar) const;
         Colorf &operator*=(const float scalar);
         Colorf operator*(const Colorf &other) const;
         Colorf operator/(const float scalar) const;
         Colorf &operator/=(const float scalar);
         Colorf operator/(const Colorf &other) const;

         RGBA_t GetAsRGBA() const;
         ARGB_t GetAsARGB() const;
         BGRA_t GetAsBGRA() const;
         ABGR_t GetAsABGR() const;

         void SetAsRGBA(const RGBA_t color);
         void SetAsARGB(const ARGB_t color);
         void SetAsBGRA(const BGRA_t color);
         void SetAsABGR(const ABGR_t color);

         void SetHSB(const float hue, const float saturation, const float brightness);
         void GetHSB(float* hue, float* saturation, float* brightness) const;
      };

      inline float Colorf::operator[](const eIdxColorComponent index) const
      {
         assert(index < 4);
         return *(&r + index);
      }

      inline float &Colorf::operator[](const eIdxColorComponent index)
      {
         assert(index < 4);
         return *(&r + index);
      }

      inline Colorf Colorf::operator+(const Colorf &other) const
      {
         return Colorf(r + other.r, g + other.g, b + other.b, a + other.a);
      }

      inline Colorf &Colorf::operator+=(const Colorf &other)
      {
         r += other.r;
         g += other.g;
         b += other.b;
         a += other.a;
         return *this;
      }

      inline Colorf Colorf::operator-(const Colorf &other) const
      {
         return Colorf(r - other.r, g - other.g, b - other.b, a - other.a);
      }

      inline Colorf &Colorf::operator-=(const Colorf &other)
      {
         r -= other.r;
         g -= other.g;
         b -= other.b;
         a -= other.a;
         return *this;
      }

      inline Colorf Colorf::operator*(const float scalar) const
      {
         return Colorf(scalar * r, scalar * g, scalar * b, scalar * a);
      }

      inline Colorf &Colorf::operator*=(const float scalar)
      {
         r *= scalar;
         g *= scalar;
         b *= scalar;
         a *= scalar;
         return *this;
      }

      inline Colorf Colorf::operator*(const Colorf &other) const
      {
         return Colorf(r * other.r, g * other.g, b * other.b, a * other.a);
      }

      inline Colorf Colorf::operator/(const float scalar) const
      {
         assert(scalar != 0.0);

         float inv = 1.0f / scalar;
         return Colorf(r * inv, g * inv, b * inv, a * inv);
      }

      inline Colorf &Colorf::operator/=(const float scalar)
      {
         assert(scalar != 0.0);

         float inv = 1.0f / scalar;
         r *= inv;
         g *= inv;
         b *= inv;
         a *= inv;
         return *this;
      }

      inline Colorf Colorf::operator/(const Colorf &other) const
      {
         return Colorf(r / other.r, g / other.g, b / other.b, a / other.a);
      }

      class ColorARGB16i
      {
         uint32 color;

         static const ColorARGB16i ZERO;
         static const ColorARGB16i ONE;
         static const ColorARGB16i BLACK;
         static const ColorARGB16i WHITE;
         static const ColorARGB16i RED;
         static const ColorARGB16i GREEN;
         static const ColorARGB16i BLUE;

         ColorARGB16i(
            const uint32 r,
            const uint32 g,
            const uint32 b);
         ColorARGB16i(
            const uint32 a,
            const uint32 r,
            const uint32 g,
            const uint32 b);
         ColorARGB16i(const uint32 other);
         uint32 GetAlpha() const;
         uint32 GetRed() const;
         uint32 GetGreen() const;
         uint32 GetBlue() const;
         void SetAlpha(const uint32 b);
         void SetRed(const uint32 r);
         void SetGreen(const uint32 g);
         void SetBlue(const uint32 b);
         float GetLightness() const;
         float GetLuminance() const;
         uint32 GetAverage() const;
         uint16 ToA1R5G5B5() const;
         void ToOpenGLColor(uint8* dest) const;
         void Set(const uint32 color);
         void Set(const uint32 a, const uint32 r, const uint32 g, const uint32 b);
      };

      inline ColorARGB16i::ColorARGB16i(
         const uint32 r,
         const uint32 g,
         const uint32 b)
         : color((0xff << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff))
      {
      }

      inline ColorARGB16i::ColorARGB16i(
         const uint32 a,
         const uint32 r,
         const uint32 g,
         const uint32 b)
         : color(((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff))
      {
      }

      inline ColorARGB16i::ColorARGB16i(const uint32 other) : color(other)
      {
      }

      inline uint32 ColorARGB16i::GetAlpha() const
      {
         return color >> 24;
      }

      inline uint32 ColorARGB16i::GetRed() const
      {
         return (color >> 16) & 0xff;
      }

      inline uint32 ColorARGB16i::GetGreen() const
      {
         return (color >> 8) & 0xff;
      }

      inline uint32 ColorARGB16i::GetBlue() const
      {
         return color & 0xff;
      }

      inline void ColorARGB16i::SetAlpha(const uint32 a)
      {
         color = ((a & 0xff) << 24) | (color & 0x00ffffff);
      }

      inline void ColorARGB16i::SetRed(const uint32 r)
      {
         color = ((r & 0xff) << 16) | (color & 0xff00ffff);
      }

      inline void ColorARGB16i::SetGreen(const uint32 g)
      {
         color = ((g & 0xff) << 8) | (color & 0xffff00ff);
      }

      inline void ColorARGB16i::SetBlue(const uint32 b)
      {
         color = (b & 0xff) | (color & 0xffffff00);
      }

      inline float ColorARGB16i::GetLightness() const
      {
         return 0.5f*(
            core::math::Max(core::math::Max(GetRed(), GetGreen()), GetBlue()) +
            core::math::Min(core::math::Min(GetRed(), GetGreen()), GetBlue()));
      }

      inline float ColorARGB16i::GetLuminance() const
      {
         return 0.3f*GetRed() + 0.59f*GetGreen() + 0.11f*GetBlue();
      }

      inline uint32 ColorARGB16i::GetAverage() const
      {
         return (GetRed() + GetGreen() + GetBlue()) / 3;
      }

      inline uint16 ColorARGB16i::ToA1R5G5B5() const
      {
         return A8R8G8B8toA1R5G5B5(color);
      }

      inline void ColorARGB16i::ToOpenGLColor(uint8* dest) const
      {
         *dest = (uint8)GetRed();
         *++dest = (uint8)GetGreen();
         *++dest = (uint8)GetBlue();
         *++dest = (uint8)GetAlpha();
      }

      inline void ColorARGB16i::Set(const uint32 color)
      {
         this->color = color;
      }

      inline void ColorARGB16i::Set(
         const uint32 a,
         const uint32 r,
         const uint32 g,
         const uint32 b)
      {
         color = (((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff));
      }

   } // namespace color

} // namespace gfx

#endif