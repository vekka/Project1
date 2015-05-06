#include "core/fileio/file.hpp"

using core::string::String_c;

namespace core
{

   namespace fileio
   {

      File::File()
      {
         isOpen = false;
      }

      bool File::Open(const String_c &path, const bool readAsBinary, const eFileMode mode, const bool update)
      {
         assert(FMODE_READ || FMODE_WRITE || FMODE_APPEND /* || FMODE_READWRITE */);

         this->mode = mode;
         this->path = path;
         this->readAsBinary = readAsBinary;
         String_c strMode;

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
            strMode = strMode.Append('+');
         if (readAsBinary)
            strMode = strMode.Append('b');

         errno_t err = fopen_s(&stream, path.CString(), strMode.CString());
         if (err != 0)
            return false;

         isOpen = true;

         fseek(stream, 0, SEEK_END);
         fileSize = GetPosition();
         fseek(stream, 0, SEEK_SET);

         return true;
      }

      // TODO fix me one day
      //String_c &File::GetFilePath() const
      //{
      //   assert( isOpen );
      //
      //   return path;
      //}

      //! get the filename extension from a file path
      //String_c &File::GetFileNameExtension()
      //{
      // //  String_c dest;
      //	//int endPos = path.find_last_of ( '.' );
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

      bool File::ReadLine(String_c &lineOut, const bool includeNewLine, const uint32 offset, const int32 length, const bool relative) const
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
            lineOut.RemoveChars("\n\r");

         delete[] str;
         return true;
      }

      File &File::operator<<(const String_c &str)
      {
         assert(isOpen/* && mode != FMODE_READ*/);

         fputs(str.CString(), stream);
         return *this;
      }

      File &File::operator<<(const char &c)
      {
         assert(isOpen/* && mode != FMODE_READ*/);

         fputc(c, stream);
         return *this;
      }

      File &File::operator<<(const int32 &i)
      {
         assert(isOpen/* && mode != FMODE_READ*/);

         String_c integerStr(i);
         fputs(integerStr.CString(), stream);
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