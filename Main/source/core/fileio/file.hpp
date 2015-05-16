#ifndef _FILE_HPP_INCLUDED_
#define _FILE_HPP_INCLUDED_

#include "core/string/string.hpp"

using core::string::String_c;

#include <vector>
using std::vector;

namespace core
{

   namespace fileio
   {

      enum eFileMode
      {
         FMODE_READ,
         FMODE_WRITE,
         FMODE_READWRITE, // To be implemented
         FMODE_APPEND
      };

      class File
      {
      protected:
         FILE *stream;
         String_c path;
         eFileMode mode;
         bool readAsBinary;
         bool isOpen;
         int32 fileSize;
      public:
         File();
         virtual ~File(void);
         bool Open(const String_c &path, const bool readAsBinary = false, const eFileMode mode = FMODE_READ, const bool update = false);
         bool IsOpen() const { return isOpen; }
         String_c GetFilePath() const { return path; }
         void Close();
         //String_c &GetFileNameExtension();

         // copy whole file content into a buffer and rewind the file pointer, the buffer need to be preallocated
         template <typename TCharType> bool CopyToBuffer(TCharType *bufferOut) const;
         template <typename TCharType> bool CopyToBuffer( vector<TCharType> &bufferOut) const;
         
         int32 GetPosition() const;
         int32 GetSize() const;

         bool Seek(const uint32 finalPos, const bool relative) const;

         // read a line from file and the file pointer advances, or read line from an offset, -1 means only advance ptr
         bool ReadLine(String_c &lineOut, const bool includeNewLine = true, const uint32 offset = -1, const int32 length = 256, const bool relative = false) const;
         //bool WriteLine(

         File &operator<<(const String_c &str);
         File &operator<<(const char &c);
         File &operator<<(const int32 &i);

         byte GetByte() const;
      };

      template <class TCharType>
      bool File::CopyToBuffer(TCharType *bufferOut) const
      {
         assert(isOpen);

         if (bufferOut == NULL)
            return false;

         // this will add zero-termination character of ascii files, since the number of characters is less than fileSize for ascii
         if (!readAsBinary)
            memset(bufferOut, 0, fileSize);

         // copy the file into the buffer:
         fread(bufferOut, 1, fileSize, stream);
         rewind(stream);

         //if (result != fileSize) // this test does not work for ascii mode, since it does not count carriage returns
         //   return -1;
         return true;
      }

      template <class TCharType>
      bool File::CopyToBuffer(vector<TCharType> &bufferOut) const
      {
         assert(isOpen);

         //if (bufferOut[0] == NULL)
         //   return false;

         if (!fileSize)
            return false;

         bufferOut.reserve(fileSize + 1);
         bufferOut.resize(fileSize);

         if (fileSize != fread(&bufferOut[0], sizeof(TCharType), fileSize, stream))
         {
         //   throw DeadlyImportError("File read error");
         }

         //ConvertToUTF8(data);

         // append a binary zero to simplify string parsing
         bufferOut.push_back(0);
         return true;
      }

   } // namespace fileio

} // namespace core

#endif