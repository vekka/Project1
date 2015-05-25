#include "raw.hpp"

RawImage::RawImage( void ) : data(0), width(0), height(0), pixelFormat(PF_R8G8B8)
{
   switch( pixelFormat )
   {
   //case GRAYSCALE: // 8-bit
   //   break;
   case PF_MONOCHROME:
      bitsPerPixel = 1;
      break;
   case PF_R5G6B5:
   case PF_A1R5G5B5:
      bitsPerPixel = 16;
      break;
   case PF_R8G8B8:
   case PF_B8G8R8:
      bitsPerPixel = 24;
      break;
   case PF_R8G8B8A8:
   case PF_B8G8R8A8:
   case PF_A8R8G8B8:
   case PF_A8B8G8R8:
      bitsPerPixel = 32;
      break;
   default:
      pixelFormat = PF_R8G8B8;
      bitsPerPixel = 32;
      break;
   }
}

RawImage::RawImage( const RawImage &other )
{
}

void RawImage::Allocate( const uint32 numBytes )
{
   rawImgSize = numBytes;
   data = allocator.Allocate(numBytes);
}

void RawImage::Fill( const byte *arr )
{
   memcpy(data, arr, rawImgSize);
}

// monochrome image
void RawImage::Convert1BitToThis( const byte *in, const uint32 linepad, const bool flip )
{
   if (!in || !data )
		return;

   int16 black, white;

   switch( pixelFormat )
   {
   case PF_R5G6B5:
      black = (int16)0xffff;
      white = (int16)0x0000;
      break;
   case PF_A1R5G5B5:
      black = (int16)0xffff;
      white = (int16)0x8000; // A=1, R=G=B=0
      break;
   }

	if (flip)
		data += width * height;

   for (uint32 y = 0; y < height; y++)
   {
      int32 shift = 7;
      if (flip)
         data -= width;

      for (uint32 x = 0; x < width; x++)
      {
         data[x] = (byte)(*in>>shift & 0x01 ? (int16)black : (int16)white);

         if ((--shift) < 0) // 8 pixels done
         {
            shift=7;
            in++;
         }
      }
		if (shift != 7) // width did not fill last byte
			in++;

		if (!flip)
			data += width;
		in += linepad;
   }
}

//void RawImage::Convert1BitToA1R5G5B5( const unsigned char* in, const int32 linepad, bool const flip )
//{
//	if (!in || !data)
//		return;
//
//	if (flip)
//		data += width * height;
//
//	for (int32 y = 0; y < height; y++)
//	{
//		int32 shift = 7;
//		if (flip)
//			data -= width;
//
//		for (int32 x=0; x<width; ++x)
//		{
//			data[x] = *in>>shift & 0x01 ? (short)0xffff : (short)0x8000;
//
//			if ((--shift)<0) // 8 pixel done
//			{
//				shift=7;
//				++in;
//			}
//		}
//
//		if (shift != 7) // width did not fill last byte
//			++in;
//
//		if (!flip)
//			data += width;
//		in += linepad;
//	}
//}