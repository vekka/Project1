#include "bmp.hpp"

bool BMPFile::ReadBMP( void )
{
	//if (!ReadHeader())
	//{
	//	return false;
	//}

  // for (int row = header.height - 1; row >= 0; row--)
  // {
  //    //pixelBuffer = data + row*header.width*4;

  //    for (int column = 0; column < header.width; column++)
		//{
  //       byte red, green, blue, alpha;
		//	int palIndex;
		//	unsigned short shortPixel;
		//	switch ( header.bitsPerPixel )
		//	{
		//	case 8:
		//		//palIndex = *buf_p++;
  //          palIndex = GetByte();
  //          *pixelBuffer++ = header.palette[palIndex][2];
  //          *pixelBuffer++ = header.palette[palIndex][1];
  //          *pixelBuffer++ = header.palette[palIndex][0];
  //          *pixelBuffer++ = 0xff;
		//		break;
		//	case 16:
		//		shortPixel = * ( unsigned short * ) pixelBuffer;
		//		pixelBuffer += 2;
		//		*pixelBuffer++ = ( shortPixel & ( 31 << 10 ) ) >> 7;
		//		*pixelBuffer++ = ( shortPixel & ( 31 << 5 ) ) >> 2;
		//		*pixelBuffer++ = ( shortPixel & ( 31 ) ) << 3;
		//		*pixelBuffer++ = 0xff;
		//		break;
		//	case 24:
		//		blue = *pixelBuffer++;
		//		green = *pixelBuffer++;
		//		red = *pixelBuffer++;
		//		*pixelBuffer++ = red;
		//		*pixelBuffer++ = green;
		//		*pixelBuffer++ = blue;
		//		*pixelBuffer++ = 0xff;
		//		break;
		//	case 32:
		//		blue = *pixelBuffer++;
		//		green = *pixelBuffer++;
		//		red = *pixelBuffer++;
		//		alpha = *pixelBuffer++;
		//		*pixelBuffer++ = red;
		//		*pixelBuffer++ = green;
		//		*pixelBuffer++ = blue;
		//		*pixelBuffer++ = alpha;
		//		break;
		//	default:
  //          return false;
		//		//ri.Error( ERR_DROP, "LoadBMP: illegal pixel_size '%d' in file '%s'\n", bmpHeader.bitsPerPixel, name );
		//		break;
		//	}
  //    }
  // }

   return true;
}
//
//bool BMPFile::ReadHeader( void )
//{
//   header.id[0] = GetByte();
//	header.id[1] = GetByte();
//	if ( header.id[0] != 'B' && header.id[1] != 'M' ) 
//	{
//      return false;
//		//ri.Error( ERR_DROP, "LoadBMP: only Windows-style BMP files supported (%s)\n", name );
//	}
//
//	header.fileSize = GetLittleEndianLong();
//   if ( header.fileSize != fileSize )
//   {
//      return false;
//   }
//
//   header.reserved0 = GetLittleEndianLong();
//   header.dataOffset = GetLittleEndianLong();
//   header.headerSize = GetLittleEndianLong();
//   header.width = GetLittleEndianLong();
//   header.height = GetLittleEndianLong();
//   header.planes = GetLittleEndianShort();
//   header.bitsPerPixel = GetLittleEndianShort();
//	//if ( bmpHeader.bitsPerPixel == 8 )
//	//	buf_p += 1024;
//
//	if ( header.bitsPerPixel < 8 )
//	{
//      return false;
//		//ri.Error( ERR_DROP, "LoadBMP: monochrome and 4-bit BMP files not supported (%s)\n", name );
//	}
//   header.compression = GetLittleEndianLong();
//   if ( header.compression > 2 ) // only handles RLE compression
//	{
//      return false;
//		//ri.Error( ERR_DROP, "LoadBMP: only uncompressed BMP files supported (%s)\n", name );
//	}
//   header.dataSize = GetLittleEndianLong();
//   header.hRes = GetLittleEndianLong();
//   header.vRes = GetLittleEndianLong();
//   header.colors = GetLittleEndianLong();
//   header.importantColors = GetLittleEndianLong();
//
//   //uint32 readSize = Read( (void*)header.palette, sizeof( header.palette ) );
//   if ( readSize != sizeof( header.palette ) )
//   {
//      return false;
//   }
//
//	//if ( header.height < 0 )
//	//	header.height = -header.height;
//
//   //rawImage.SetDimensions(header.width, header.height);
//
//	//rawImage.numPixels = header.width * header.height; // calculate in raw
//   int rawImgSize = header.width * header.height * 4;
//
//   //byte	*pixelBuffer;
//   //data = new byte[rawImgSize];
//
//	// decompress data if needed
//	switch(header.compression)
//	{
//	case 1: // 8 bit rle
//		//decompress8BitRLE(bmpData, header.BitmapDataSize, header.Width, header.Height, pitch);
//		break;
//	case 2: // 4 bit rle
//		//decompress4BitRLE(bmpData, header.BitmapDataSize, header.Width, header.Height, pitch);
//		break;
//	}
//
//   return true;
//}