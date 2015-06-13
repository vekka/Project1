#ifndef _WIN32CONSOLE_HPP_INCLUDED_
#define _WIN32CONSOLE_HPP_INCLUDED_

#include <Windows.h>

//#include "core/string/string.hpp"
//using core::string::String_c;

#include <string>

#include "core/BasicTypes.hpp"

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

   enum eRedirection
   {
      REDIR_STDOUT = STD_OUTPUT_HANDLE,
      REDIR_STDIN = STD_INPUT_HANDLE,
      REDIR_STDERR = STD_ERROR_HANDLE,
      REDIR_STDIO
   };

   class Win32Console
   {
   private:
      HANDLE stdIn, stdOut, stdErr;
      char inputBuffer[512];
      char rgCmds[MAX_CMDS][512];
      int32 iCmdIndex;
      int32 inpos;
      bool m_created;

      uint32 m_width, m_height;
      //uint32 m_bufferWidth, m_bufferHeight;
      CHAR_INFO *m_pScreenBuffer;
      CONSOLE_SCREEN_BUFFER_INFO m_screenBufferInfo;
   public:
      Win32Console::Win32Console() { };
      // width and height and buffer parameters are for the number of the characters
      Win32Console::Win32Console(
         const uint32 width, const uint32 height,
         const uint32 bufferWidth, const uint32 bufferHeight );
      ~Win32Console( void );

      bool Create();
      bool SetRedirection(eRedirection redir);
      void Destroy( void );
      bool IsEnabled( void ) const;
      bool SetCaption( const std::string &caption );
      void GetCaption( std::string & ) const;
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
