#ifndef _RAW_HPP_INCLUDED_
#define _RAW_HPP_INCLUDED_

#include "core/memory/allocator.hpp"

#include "core/BasicTypes.hpp"

enum PixelFormat
{
   PF_MONOCHROME,
   PF_R5G6B5,
   PF_A1R5G5B5,
   PF_R8G8B8,
   PF_B8G8R8,
   PF_R8G8B8A8,
   PF_B8G8R8A8,
   PF_8G8R8A8,
   PF_A8R8G8B8,
   PF_A8B8G8R8
};

//template <class T>
class RawImage
{
private:
   byte *data;
   uint32 width, height;
   uint32 bitsPerPixel;
   uint32 rawImgSize;
   uint32 numPixels;
   Allocator<byte> allocator;
   // int palette ?
   // int numMipMaps;
   PixelFormat pixelFormat;
public:
   RawImage( void );
   //RawImage( int width, int height, int bitsPerPixel ) 
   //   : width(width), height(height), bitsPerPixel(bitsPerPixel) {}
   RawImage( const RawImage &other );
   RawImage &operator=( const RawImage &other );
   void SetDimensions( const uint32 width, const uint32 height );
   RawImage &FlipAroundX( void );
   RawImage &FlipAroundY( void );
   void Allocate( const uint32 numBytes );
   void Fill( const byte *arr );

   // convert to the actual Pixel Format
   void Convert1BitToThis( const byte *in, const uint32 linepad, const bool flip = false );
   void Convert4BitToThis( const byte *in, const int *palette, const uint32 linepad, const bool flip = false );
};

inline void RawImage::SetDimensions( const uint32 width, const uint32 height )
{
   this->width = width;
   this->height = height;
   this->pixelFormat = pixelFormat;
   rawImgSize = width * height * 4;
}

#endif