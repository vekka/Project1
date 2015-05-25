#include "core/fileio/file.hpp"

//using core::string::String_c;

#include <assert.h>

namespace core
{

   namespace fileio
   {

      File::File()
      {
         isOpen = false;
      }

      bool File::Open(const std::string &path, const bool readAsBinary, const eFileMode mode, const bool update)
      {
         assert(FMODE_READ || FMODE_WRITE || FMODE_APPEND /* || FMODE_READWRITE */);

         this->mode = mode;
         this->path = path;
         this->readAsBinary = readAsBinary;
         std::string strMode;

         switch (mode)
         {
         case FMODE_READ:
            strMode = "r";
            break;
         case FMODE_WRITE:
            strMode = "w";
            break;
         case FMODE_APPEND:
            strMode = "a";
            break;
         }

         if (update)
            strMode += '+';
         if (readAsBinary)
            strMode += 'b';

         errno_t err = fopen_s(&stream, path.c_str(), strMode.c_str());
         if (err != 0)
            return false;

         isOpen = true;

         fseek(stream, 0, SEEK_END);
         fileSize = GetPosition();
         fseek(stream, 0, SEEK_SET);

         return true;
      }

      // TODO fix me one day
      //std::string &File::GetFilePath() const
      //{
      //   assert( isOpen );
      //
      //   return path;
      //}

      //! get the filename extension from a file path
      //std::string &File::GetFileNameExtension()
      //{
      // //  std::string dest;
      //	//int32 endPos = path.find_last_of ( '.' );
      //	//if ( endPos < 0 )
      //	//	dest = "";
      //	//else
      // //     dest = path.substr ( endPos, path.size() );
      //	//return dest;
      //
      //}

      int32 File::GetPosition() const
      {
         assert(isOpen);

         return (int32)ftell(stream);
      }

      int32 File::GetSize() const
      {
         assert(isOpen);

         return fileSize;
      }

      bool File::Seek(const uint32 finalPos, const bool relative) const
      {
         assert(isOpen);

         return (bool)(fseek(stream, finalPos, relative ? SEEK_CUR : SEEK_SET) == 0);
      }

      void File::Close()
      {
         assert(isOpen);

         if (stream)
            fclose(stream);

         isOpen = false;
      }

      bool File::ReadLine(std::string &lineOut, const bool includeNewLine, const uint32 offset, const int32 length, const bool relative) const
      {
         assert(isOpen);

         char *str = new char[length];

         if (offset != -1)
         {
            fseek(stream, offset, relative ? SEEK_CUR : SEEK_SET);
         }

         if (fgets(str, length, stream) == NULL) // false
         {
            delete[] str;
            return false;
         }

         lineOut = str;
         if (!includeNewLine)
         {
            size_t idx = 0;
            while (std::string::npos != (idx = lineOut.find_first_of("/n/r")))
            {
               lineOut.erase(idx);
            }
         }

         delete[] str;
         return true;
      }

      File &File::operator<<(const std::string &str)
      {
         assert(isOpen/* && mode != FMODE_READ*/);

         fputs(str.c_str(), stream);
         return *this;
      }

      File &File::operator<<(const char &c)
      {
         assert(isOpen/* && mode != FMODE_READ*/);

         fputc(c, stream);
         return *this;
      }

      File &File::operator<<(const int32 &i) // string constructor take no integer
      {
         assert(isOpen/* && mode != FMODE_READ*/);

         //std::string integerStr(i);
         //fputs(integerStr.c_str(), stream);
         return *this;
      }

      byte File::GetByte() const
      {
         assert(isOpen);

         return fgetc(stream);
      }

      File::~File()
      {
         if (stream)
            fclose(stream);

         isOpen = false;
      }

   } // namespace fileio

} // namespace core