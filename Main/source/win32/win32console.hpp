#ifndef _WIN32CONSOLE_HPP_INCLUDED_
#define _WIN32CONSOLE_HPP_INCLUDED_

#include <Windows.h>

#include "core/string/string.hpp"
using core::string::String_c;

namespace win32console
{
   
   const int32 MAX_CMDS = 10;

   enum eBkgConsoleColor
   {
      BKG_CONSOLE_BLACK,
      BKG_CONSOLE_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
      BKG_CONSOLE_RED = BACKGROUND_RED,
      BKG_CONSOLE_GREEN = BACKGROUND_GREEN,
      BKG_CONSOLE_BLUE = BACKGROUND_BLUE
   };
   
   class Win32Console
   {
   private:
      HANDLE stdIn, stdOut, stdErr;
      char inputBuffer[512];
      char rgCmds[MAX_CMDS][512];
      int32 iCmdIndex;
      int32 inpos;
      bool created;
   
      uint32 width, height;
      uint32 bufferWidth, bufferHeight;
      CHAR_INFO *screenBuffer;
   public:
      Win32Console::Win32Console() { };
      Win32Console::Win32Console( 
         const uint32 width, const uint32 height,
         const uint32 screenBufferWidth, const uint32 screenBufferHeight );
      ~Win32Console( void );
   
      // width and height and buffer parameters are for the number of the characters
      bool Create( 
         const uint32 width, const uint32 height,
         const uint32 bufferWidth, const uint32 bufferHeight, const bool hasInputHandle = true, const bool hasErrorHandle = true );
      void Destroy( void );
      bool IsEnabled( void ) const;
      bool SetCaption( const String_c &caption );
      void GetCaption( String_c & ) const;
      bool UpdateDimension( const uint32 width, const uint32 height );
      void GetDimension( uint32 &width, uint32 &height ) const;
      void UpdatePosition( const uint32 newXPos, const uint32 newYPos );
      void GetPosition( uint32 &posX, uint32 &posY ) const;
      void UpdateScreenBufferDimension( const uint32 width, const uint32 height );
   
      void ClearScreenBuffer( const eBkgConsoleColor bkgColor = BKG_CONSOLE_BLACK ) const;
      void printf( const char *string, ... ) const;
      void ProcessLine( const char *consoleLine ) const;
      void Process( void );
   };

} // namespace win32console

#endif
