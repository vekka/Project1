#include "color.hpp"

namespace gfx
{

   namespace color
   {
      const Colorf Colorf::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
      const Colorf Colorf::ONE(1.0f, 1.0f, 1.0f, 1.0f);
      const Colorf Colorf::BLACK(0.0f, 0.0f, 0.0f);
      const Colorf Colorf::WHITE(1.0f, 1.0f, 1.0f);
      const Colorf Colorf::RED(1.0f, 0.0f, 0.0f);
      const Colorf Colorf::GREEN(0.0f, 1.0f, 0.0f);
      const Colorf Colorf::BLUE(0.0f, 0.0f, 1.0f);

      //#define BIG_ENDIAN

#ifdef BIG_ENDIAN
      ABGR_t Colorf::GetAsABGR() const
#else
      RGBA_t Colorf::GetAsRGBA() const
#endif
      {
         uint8 val8;
         int val32 = 0;

         // Convert to 32bit pattern
         // (RGBA = 8.8.8.8)

         // Red
         val8 = static_cast<uint8>(r * 255);
         val32 = val8 << 24;

         // Green
         val8 = static_cast<uint8>(g * 255);
         val32 += val8 << 16;

         // Blue
         val8 = static_cast<uint8>(b * 255);
         val32 += val8 << 8;

         // Alpha
         val8 = static_cast<uint8>(a * 255);
         val32 += val8;

         return val32;
      }

#ifdef BIG_ENDIAN
      BGRA_t Colorf::GetAsBGRA(void) const
#else
      ARGB_t Colorf::GetAsARGB(void) const
#endif
      {
         uint8 val8;
         int val32 = 0;

         // Convert to 32bit pattern
         // (ARGB = 8.8.8.8)

         // Alpha
         val8 = static_cast<uint8>(a * 255);
         val32 = val8 << 24;

         // Red
         val8 = static_cast<uint8>(r * 255);
         val32 += val8 << 16;

         // Green
         val8 = static_cast<uint8>(g * 255);
         val32 += val8 << 8;

         // Blue
         val8 = static_cast<uint8>(b * 255);
         val32 += val8;

         return val32;
      }

#ifdef BIG_ENDIAN
      ARGB_t Colorf::GetAsARGB(void) const
#else
      BGRA_t Colorf::GetAsBGRA(void) const
#endif
      {
         uint8 val8;
         int val32 = 0;

         // Convert to 32bit pattern
         // (ARGB = 8.8.8.8)

         val8 = static_cast<uint8>(b * 255);
         val32 = val8 << 24;
         val8 = static_cast<uint8>(g * 255);
         val32 += val8 << 16;
         val8 = static_cast<uint8>(r * 255);
         val32 += val8 << 8;
         val8 = static_cast<uint8>(a * 255);
         val32 += val8;

         return val32;
      }

#ifdef BIG_ENDIAN
      RGBA_t Colorf::GetAsRGBA() const
#else
      ABGR_t Colorf::GetAsABGR() const
#endif
      {
         uint8 val8;
         int val32 = 0;

         // Convert to 32bit pattern
         // (ABRG = 8.8.8.8)

         val8 = static_cast<uint8>(a * 255);
         val32 = val8 << 24;
         val8 = static_cast<uint8>(b * 255);
         val32 += val8 << 16;
         val8 = static_cast<uint8>(g * 255);
         val32 += val8 << 8;
         val8 = static_cast<uint8>(r * 255);
         val32 += val8;

         return val32;
      }

#ifdef BIG_ENDIAN
      void Colorf::SetAsABGR( const ABGR_t val )
#else
      void Colorf::SetAsRGBA(const RGBA_t val)
#endif
      {
         int val32 = val;

         // Convert from 32bit pattern
         // (RGBA = 8.8.8.8)

         r = ((val32 >> 24) & 0xff) / 255.0f;
         g = ((val32 >> 16) & 0xff) / 255.0f;
         b = ((val32 >> 8) & 0xff) / 255.0f;
         a = (val32 & 0xff) / 255.0f;
      }

#ifdef BIG_ENDIAN
      void Colorf::SetAsBGRA( const BGRA_t val )
#else
      void Colorf::SetAsARGB(const ARGB_t val)
#endif
      {
         int val32 = val;

         // Convert from 32bit pattern
         // (ARGB = 8.8.8.8)

         a = ((val32 >> 24) & 0xff) / 255.0f;
         r = ((val32 >> 16) & 0xff) / 255.0f;
         g = ((val32 >> 8) & 0xff) / 255.0f;
         b = (val32 & 0xff) / 255.0f;
      }

#ifdef BIG_ENDIAN
      void Colorf::SetAsARGB( const ARGB_t val )
#else
      void Colorf::SetAsBGRA(const BGRA_t val)
#endif
      {
         int val32 = val;

         // Convert from 32bit pattern
         // (ARGB = 8.8.8.8)

         b = ((val32 >> 24) & 0xff) / 255.0f;
         g = ((val32 >> 16) & 0xff) / 255.0f;
         r = ((val32 >> 8) & 0xff) / 255.0f;
         a = (val32 & 0xff) / 255.0f;
      }

#ifdef BIG_ENDIAN
      void Colorf::SetAsRGBA( const RGBA_t val )
#else
      void Colorf::SetAsABGR(const ABGR_t val)
#endif
      {
         int val32 = val;

         // Convert from 32bit pattern
         // (ABGR = 8.8.8.8)

         a = ((val32 >> 24) & 0xff) / 255.0f;
         b = ((val32 >> 16) & 0xff) / 255.0f;
         g = ((val32 >> 8) & 0xff) / 255.0f;
         r = (val32 & 0xff) / 255.0f;
      }

      void Colorf::SetHSB(float hue, float saturation, float brightness)
      {
         // wrap hue
         if (hue > 1.0f)
         {
            hue -= (int)hue;
         }
         else if (hue < 0.0f)
         {
            hue += (int)hue + 1;
         }
         // clamp saturation / brightness
         //saturation = core::Min(saturation, (float)1.0);
         //saturation = core::Max(saturation, (float)0.0);
         saturation = core::math::Clamp(saturation, 0.0f, 1.0f);
         //brightness = core::Min(brightness, (float)1.0);
         //brightness = core::Max(brightness, (float)0.0);
         brightness = core::math::Clamp(brightness, 0.0f, 1.0f);

         if (brightness == 0.0f)
         {
            // early exit, this has to be black
            r = g = b = 0.0f;
            return;
         }

         if (saturation == 0.0f)
         {
            // early exit, this has to be grey
            r = g = b = brightness;
            return;
         }


         float hueDomain = hue * 6.0f;
         if (hueDomain >= 6.0f)
         {
            // wrap around, and allow mathematical errors
            hueDomain = 0.0f;
         }
         uint16 domain = (uint16)hueDomain;
         float f1 = brightness * (1 - saturation);
         float f2 = brightness * (1 - saturation * (hueDomain - domain));
         float f3 = brightness * (1 - saturation * (1 - (hueDomain - domain)));

         switch (domain)
         {
         case 0:
            // red domain; green ascends
            r = brightness;
            g = f3;
            b = f1;
            break;
         case 1:
            // yellow domain; red descends
            r = f2;
            g = brightness;
            b = f1;
            break;
         case 2:
            // green domain; blue ascends
            r = f1;
            g = brightness;
            b = f3;
            break;
         case 3:
            // cyan domain; green descends
            r = f1;
            g = f2;
            b = brightness;
            break;
         case 4:
            // blue domain; red ascends
            r = f3;
            g = f1;
            b = brightness;
            break;
         case 5:
            // magenta domain; blue descends
            r = brightness;
            g = f1;
            b = f2;
            break;
         }
      }

      void Colorf::GetHSB(float *hue, float *saturation, float *brightness) const
      {
         using core::math::Min;
         using core::math::Max;
         using core::math::Equals;

         float vMin = Min(r, Min(g, b));
         float vMax = Max(r, Max(g, b));
         float delta = vMax - vMin;

         *brightness = vMax;

         if (Equals(delta, 0.0f))
         {
            // grey
            *hue = 0;
            *saturation = 0;
         }
         else
         {
            // a colour
            *saturation = delta / vMax;

            float deltaR = (((vMax - r) / 6.0f) + (delta / 2.0f)) / delta;
            float deltaG = (((vMax - g) / 6.0f) + (delta / 2.0f)) / delta;
            float deltaB = (((vMax - b) / 6.0f) + (delta / 2.0f)) / delta;

            if (Equals(r, vMax))
               *hue = deltaB - deltaG;
            else if (Equals(g, vMax))
               *hue = 0.3333333f + deltaR - deltaB;
            else if (Equals(b, vMax))
               *hue = 0.6666667f + deltaG - deltaR;

            if (*hue < 0.0f)
               *hue += 1.0f;
            if (*hue > 1.0f)
               *hue -= 1.0f;
         }
      }

      const ColorARGB16i ColorARGB16i::ZERO(0, 0, 0, 0);
      const ColorARGB16i ColorARGB16i::ONE(255, 255, 255, 255);
      const ColorARGB16i ColorARGB16i::WHITE(255, 255, 255);
      const ColorARGB16i ColorARGB16i::BLACK(0, 0, 0);
      const ColorARGB16i ColorARGB16i::RED(255, 0, 0);
      const ColorARGB16i ColorARGB16i::GREEN(0, 255, 0);
      const ColorARGB16i ColorARGB16i::BLUE(0, 0, 255);

   } // namespace color

} // namespace gfx