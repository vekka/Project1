#ifndef _COLOR_HPP_INCLUDED_
#define _COLOR_HPP_INCLUDED_

#include <assert.h>

#include "core/math/mathcommon.hpp"

using namespace core;

inline unsigned short CreateA1R5G5B5(unsigned int r, unsigned int g, unsigned int b, unsigned int a=0xFF)
{
   return (unsigned short)(
      (a & 0x80) << 8 |
      (r & 0xf8) << 7 |
      (g & 0xf8) << 2 |
      (b & 0xf8) >> 3);
}

inline unsigned short CreateA1R5G5B5(unsigned short r, unsigned short g, unsigned short b)
{
   return (0x8000 |
      (r & 0x1f) << 10 |
      (g & 0x1f) << 5  |
      (b & 0x1f));
}

inline unsigned short X8R8G8B8toA1R5G5B5(unsigned int color)
{
   return (unsigned short)(0x8000 |
      ( color & 0x00f80000) >> 9 |
      ( color & 0x0000f800) >> 6 |
      ( color & 0x000000f8) >> 3);
}

inline unsigned int A8R8G8B8toA1R5G5B5(unsigned int color)
{
   return (unsigned short)(
      ( color & 0x80000000) >> 16|
      ( color & 0x00f80000) >> 9 |
      ( color & 0x0000f800) >> 6 |
      ( color & 0x000000f8) >> 3);
}

inline unsigned short A8R8G8B8toR5G6B5(unsigned int color)
{
   return (unsigned short)(
      ( color & 0x00f80000) >> 8 |
      ( color & 0x0000fc00) >> 5 |
      ( color & 0x000000f8) >> 3);
}

inline unsigned int A1R5G5B5toA8R8G8B8(unsigned short color)
{
   return ((( -( (int) color & 0x00008000 ) >> (int) 31 ) & 0xff000000 ) |
      (( color & 0x00007c00 ) << 9) | (( color & 0x00007000 ) << 4) |
      (( color & 0x000003e0 ) << 6) | (( color & 0x00000380 ) << 1) |
      (( color & 0x0000001f ) << 3) | (( color & 0x0000001c ) >> 2)
      );
}

inline unsigned int R5G6B5toA8R8G8B8(unsigned short color)
{
   return 0xff000000 |
      ((color & 0xf800) << 8)|
      ((color & 0x07e0) << 5)|
      ((color & 0x001f) << 3);
}

inline unsigned short R5G6B5toA1R5G5B5(unsigned short color)
{
   return 0x8000 | (((color & 0xffc0) >> 1) | (color & 0x1f));
}

inline unsigned short A1R5G5B5toR5G6B5(unsigned short color)
{
   return (((color & 0x7fe0) << 1) | (color & 0x1f));
}

inline unsigned int GetAlphaFromA1R5G5B5(unsigned short color)
{
   return ((color >> 15)&0x1);
}

inline unsigned int GetRedFromA1R5G5B5(unsigned short color)
{
   return ((color >> 10)&0x1f);
}

inline unsigned int GetGreenFromA1R5G5B5(unsigned short color)
{
   return ((color >> 5)&0x1f);
}

inline unsigned int GetBlueFromA1R5G5B5(unsigned short color)
{
   return (color & 0x1f);
}

inline int GetAverageFromA1R5G5B5(short color)
{
   return (
      (GetRedFromA1R5G5B5(color)<<3) +
      (GetGreenFromA1R5G5B5(color)<<3) +
      (GetBlueFromA1R5G5B5(color)<<3)) / 3;
}

typedef int RGBA_t;
typedef int ARGB_t;
typedef int BGRA_t;
typedef int ABGR_t;

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

   explicit Colorf( float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f )
      : r(red), g(green), b(blue), a(alpha) {}

   bool operator==( const Colorf &other ) const;
   bool operator!=( const Colorf &other ) const;
   float operator[]( const int index ) const;
   float &operator[]( const int index );
   Colorf operator+( const Colorf &color ) const;
   Colorf &operator+=( const Colorf &other );
   Colorf operator-( const Colorf &other ) const;
   Colorf &operator-=( const Colorf &other );
   Colorf operator*( const float scalar ) const;
   Colorf &operator*=( const float scalar );
   Colorf operator*( const Colorf &other ) const;
   Colorf operator/( const float scalar ) const;
   Colorf &operator/=( const float scalar );
   Colorf operator/( const Colorf &other ) const;

   RGBA_t GetAsRGBA() const;
   ARGB_t GetAsARGB() const;
   BGRA_t GetAsBGRA() const;
   ABGR_t GetAsABGR() const;
   
   void SetAsRGBA( const RGBA_t color );
   void SetAsARGB( const ARGB_t color );
   void SetAsBGRA( const BGRA_t color );
   void SetAsABGR( const ABGR_t color );

   void SetHSB( const float hue, const float saturation, const float brightness );
   void GetHSB( float* hue, float* saturation, float* brightness ) const;
};

inline float Colorf::operator[]( const int index ) const
{
   assert( index < 4 );
   return *(&r + index);
}

inline float &Colorf::operator[]( const int index )
{
   assert( index < 4 );
   return *(&r + index);
}

inline Colorf Colorf::operator+( const Colorf &other ) const
{
   return Colorf( r + other.r, g + other.g, b + other.b, a + other.a );
}

inline Colorf &Colorf::operator+=( const Colorf &other )
{
   r += other.r;
   g += other.g;
   b += other.b;
   a += other.a;
   return *this;
}

inline Colorf Colorf::operator-( const Colorf &other ) const
{
   return Colorf( r - other.r, g - other.g, b - other.b, a - other.a );
}

inline Colorf &Colorf::operator-=( const Colorf &other )
{
   r -= other.r;
   g -= other.g;
   b -= other.b;
   a -= other.a;
   return *this;
}

inline Colorf Colorf::operator*( const float scalar ) const
{
   return Colorf( scalar * r, scalar * g, scalar * b, scalar * a );
}

inline Colorf &Colorf::operator*=( const float scalar )
{
   r *= scalar;
   g *= scalar;
   b *= scalar;
   a *= scalar;
   return *this;
}

inline Colorf Colorf::operator*( const Colorf &other ) const
{
   return Colorf( r * other.r, g * other.g, b * other.b, a * other.a );
}

inline Colorf Colorf::operator/( const float scalar ) const
{
   assert( scalar != 0.0 );

   float inv = 1.0f / scalar;
   return Colorf( r * inv, g * inv, b * inv, a * inv );
}

inline Colorf &Colorf::operator/=( const float scalar )
{
   assert( scalar != 0.0 );

   float inv = 1.0f / scalar;
   r *= inv;
   g *= inv;
   b *= inv;
   a *= inv;
   return *this;
}

inline Colorf Colorf::operator/( const Colorf &other ) const
{
   return Colorf( r / other.r, g / other.g, b / other.b, a / other.a );
}

class ColorARGB16i
{
   unsigned int color;

   static const ColorARGB16i ZERO;
   static const ColorARGB16i ONE;
   static const ColorARGB16i BLACK;
   static const ColorARGB16i WHITE;
   static const ColorARGB16i RED;
   static const ColorARGB16i GREEN;
   static const ColorARGB16i BLUE;

   ColorARGB16i(
      const unsigned int r,
      const unsigned int g,
      const unsigned int b );
   ColorARGB16i(
      const unsigned int a,
      const unsigned int r,
      const unsigned int g,
      const unsigned int b );
	ColorARGB16i( const unsigned int other );
   unsigned int GetAlpha() const;
   unsigned int GetRed() const;
   unsigned int GetGreen() const;
   unsigned int GetBlue() const;
   void SetAlpha( const unsigned int b );
   void SetRed( const unsigned int r );
   void SetGreen( const unsigned int g );
   void SetBlue( const unsigned int b );
   float GetLightness() const;
   float GetLuminance() const;
   unsigned int GetAverage() const;
   unsigned short ToA1R5G5B5() const;
   void ToOpenGLColor( unsigned char* dest ) const;
   void Set( const unsigned int color );
   void Set( const unsigned int a, const unsigned int r, const unsigned int g, const unsigned int b );
};

inline ColorARGB16i::ColorARGB16i(
   const unsigned int r,
   const unsigned int g,
   const unsigned int b )
   : color((0xff<<24) | ((r & 0xff)<<16) | ((g & 0xff)<<8) | (b & 0xff))
{
}

inline ColorARGB16i::ColorARGB16i(
   const unsigned int a,
   const unsigned int r,
   const unsigned int g,
   const unsigned int b ) 
   : color(((a & 0xff)<<24) | ((r & 0xff)<<16) | ((g & 0xff)<<8) | (b & 0xff))
{
}

inline ColorARGB16i::ColorARGB16i( const unsigned int other ) : color(other)
{
}

inline unsigned int ColorARGB16i::GetAlpha() const
{ 
   return color>>24;
}

inline unsigned int ColorARGB16i::GetRed() const
{ 
   return (color>>16) & 0xff;
}

inline unsigned int ColorARGB16i::GetGreen() const
{ 
   return (color>>8) & 0xff;
}

inline unsigned int ColorARGB16i::GetBlue() const
{ 
   return color & 0xff;
}

inline void ColorARGB16i::SetAlpha( const unsigned int a )
{ 
   color = ((a & 0xff)<<24) | (color & 0x00ffffff);
}

inline void ColorARGB16i::SetRed( const unsigned int r )
{ 
   color = ((r & 0xff)<<16) | (color & 0xff00ffff);
}

inline void ColorARGB16i::SetGreen( const unsigned int g )
{ 
   color = ((g & 0xff)<<8) | (color & 0xffff00ff);
}

inline  void ColorARGB16i::SetBlue( const unsigned int b )
{ 
   color = (b & 0xff) | (color & 0xffffff00);
}

inline float ColorARGB16i::GetLightness() const
{
   return 0.5f*(
      core::math::Max(core::math::Max(GetRed(),GetGreen()),GetBlue()) +
      core::math::Min(core::math::Min(GetRed(),GetGreen()),GetBlue()));
}

inline float ColorARGB16i::GetLuminance() const
{
   return 0.3f*GetRed() + 0.59f*GetGreen() + 0.11f*GetBlue();
}

inline unsigned int ColorARGB16i::GetAverage() const
{
   return ( GetRed() + GetGreen() + GetBlue() ) / 3;
}

inline unsigned short ColorARGB16i::ToA1R5G5B5() const
{
   return A8R8G8B8toA1R5G5B5(color);
}

inline void ColorARGB16i::ToOpenGLColor( unsigned char* dest ) const
{
   *dest = (unsigned char)GetRed();
   *++dest = (unsigned char)GetGreen();
   *++dest = (unsigned char)GetBlue();
   *++dest = (unsigned char)GetAlpha();
}

inline void ColorARGB16i::Set( const unsigned int color )
{ 
   this->color = color;
}

inline void ColorARGB16i::Set( 
   const unsigned int a,
   const unsigned int r,
   const unsigned int g,
   const unsigned int b )
{
   color = (((a & 0xff)<<24) | ((r & 0xff)<<16) | ((g & 0xff)<<8) | (b & 0xff));
}

#endif