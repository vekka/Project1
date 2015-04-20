#ifndef _BMP_HPP_INCLUDED_
#define _BMP_HPP_INCLUDED_

#include "core/fileio/file.hpp"
#include "raw.hpp"

struct Header
{
   char id[2];
	uint32 fileSize;
	uint32 reserved0;
	uint32 dataOffset;
	uint32 headerSize;
	uint32 width;
	uint32 height;
	uint16 planes;
	uint16 bitsPerPixel;
	uint32 compression;
	uint32 dataSize;
	uint32 hRes;
	uint32 vRes;
	uint32 colors;
	uint32 importantColors;
	byte palette[256][4];
};

class BMPFile: public File
{
private:
	 // use rawimage class for storing pixeldata
	/*int	row, column;*/
	//byte	*bufferPtr;
	int32	length;
	Header header;
	//byte	*bmpRGBA;

   //RawImage<T> rawImage;

   bool ReadHeader( void );
public:
   bool ReadBMP( void );
};

#endif