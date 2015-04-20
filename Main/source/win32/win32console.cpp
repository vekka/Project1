#include <stdarg.h>

#include "win32console.hpp"

Win32Console::Win32Console( 
   const uint32 width, const uint32 height,
   const uint32 screenBufferWidth, const uint32 screenBufferHeight )
{
   created = false;
   this->width = width;
   this->height = height;
   bufferWidth = screenBufferWidth;
   bufferHeight = screenBufferHeight;

   screenBuffer = new CHAR_INFO[screenBufferWidth*screenBufferHeight];
}

Win32Console::~Win32Console( void )
{
   delete[] screenBuffer;
}

bool Win32Console::Create( 
   const uint32 width, const uint32 height,
   const uint32 bufferWidth, const uint32 bufferHeight, 
   const bool hasInputHandle, const bool hasErrorHandle )
{
   if (!AllocConsole()) // fails
      return false;

   stdOut = GetStdHandle( STD_OUTPUT_HANDLE );
   stdIn = hasInputHandle ? GetStdHandle( STD_INPUT_HANDLE ) : NULL;
   stdErr = hasErrorHandle ? GetStdHandle( STD_ERROR_HANDLE ) : NULL;

   if ( stdOut == INVALID_HANDLE_VALUE
      || stdIn == INVALID_HANDLE_VALUE
      || stdErr == INVALID_HANDLE_VALUE )
   {
      return false;
   }

   created = true;

   if (!UpdateDimension(width, height))
      printf( "Warning: Console Window is not set to requested dimension.\n" );

   UpdateScreenBufferDimension(bufferWidth, bufferHeight);
   
   return true;
}

void Win32Console::Destroy( void )
{
}

bool Win32Console::IsEnabled( void ) const
{
   return true;
}

bool Win32Console::SetCaption( const String_c &caption )
{
   assert( created );
   return (bool)(SetConsoleTitle(caption.CString()) != 0);
}

void Win32Console::GetCaption( String_c &caption ) const
{
   assert( created );

	char buffer[512];
	int32 strLen = GetConsoleTitle(buffer, 512);

	if(strLen==0)
		caption = NULL;

	caption = buffer;
}

void SetPosition( const uint32 newXPos, const uint32 newYPos )
{

}

bool Win32Console::UpdateDimension( const uint32 width, const uint32 height )
{
   assert( created );

   this->width = width;
   this->height = height;
   SMALL_RECT windowSize = {0, 0, width-1, height-1};

   return SetConsoleWindowInfo(stdOut, TRUE, &windowSize) != 0;
}

void Win32Console::GetDimension( uint32 &width, uint32 &height ) const
{
   width = this->width;
   height = this->height;
}

void Win32Console::UpdateScreenBufferDimension( const uint32 width, const uint32 height )
{
   assert( created );

   bufferWidth = width;
   bufferHeight = height;
   COORD bufferSize = {width, height};
   screenBuffer = new CHAR_INFO[width*height];
   SetConsoleScreenBufferSize(stdOut, bufferSize);
}

// clear the screen buffer with given background color
void Win32Console::ClearScreenBuffer( const eBkgConsoleColor bkgColor ) const
{
   assert( created );

   for ( uint32 i = 0; i < bufferWidth * bufferHeight; i++ )
   {
      screenBuffer[i].Char.AsciiChar = ' ';
      screenBuffer[i].Attributes = bkgColor | BACKGROUND_INTENSITY;
   }
   
   COORD charBufSize = {bufferWidth, bufferHeight};
   COORD characterPos = {0, 0};
   SMALL_RECT writeArea = {0, 0,bufferWidth-1, bufferHeight-1};

   WriteConsoleOutputA(stdOut, screenBuffer, charBufSize, characterPos, &writeArea);
}

void Win32Console::printf( const char *string, ... ) const
{
   assert( created );

   static const int32 LINE_BUFFER_SIZE = 4096;
   static char buffer[LINE_BUFFER_SIZE];
   DWORD bytes;

   va_list args;
   va_start(args, string);
   _vsnprintf(buffer, LINE_BUFFER_SIZE, string, args);

   char *pos = buffer;
   while (*pos)
   {
      if (*pos == '\t')
      {
         *pos = '^';
      }
      pos++;
   }
   
   WriteFile(stdOut, buffer, strlen(buffer), &bytes, NULL);
   FlushFileBuffers( stdOut );
}

void Win32Console::ProcessLine( const char *consoleLine ) const
{
}

void Win32Console::Process( void )
{
   assert( created );

   DWORD numEvents;
   GetNumberOfConsoleInputEvents(stdIn, &numEvents);
   if(numEvents)
   {
      INPUT_RECORD rec[20];
      char outbuf[512];
      int32 outpos = 0;
      ReadConsoleInput(stdIn, rec, 20, &numEvents);
      DWORD i;
      for(i = 0; i < numEvents; i++)
      {
         if(rec[i].EventType == KEY_EVENT)
         {
            KEY_EVENT_RECORD *ke = &(rec[i].Event.KeyEvent);
            if(ke->bKeyDown)
            {
               switch (ke->uChar.AsciiChar)
               {
                  // If no ASCII char, check if it's a handled virtual key
               case 0:
                  switch (ke->wVirtualKeyCode)
                  {
                     // UP ARROW
                  case 0x26 :
                     // Go to the previous command in the cyclic array
                     if ((-- iCmdIndex) < 0)
                        iCmdIndex = MAX_CMDS - 1;
                     // If this command isn't empty ...
                     if (rgCmds[iCmdIndex][0] != '\0')
                     {
                        // Obliterate current displayed text
                        for (int32 i = outpos = 0; i < inpos; i ++)
                        {
                           outbuf[outpos ++] = '\b';
                           outbuf[outpos ++] = ' ';
                           outbuf[outpos ++] = '\b';
                        }
                        // Copy command into command and display buffers
                        for (inpos = 0; inpos < (int32)strlen(rgCmds[iCmdIndex]); inpos ++, outpos ++)
                        {
                           outbuf[outpos] = rgCmds[iCmdIndex][inpos];
                           inputBuffer[inpos] = rgCmds[iCmdIndex][inpos];
                        }
                     }
                     // If previous is empty, stay on current command
                     else if ((++ iCmdIndex) >= MAX_CMDS)
                     {
                        iCmdIndex = 0;
                     }
                     break;
                     // DOWN ARROW
                  case 0x28 : {
                     // Go to the next command in the command array, if
                     // it isn't empty
                     if (rgCmds[iCmdIndex][0] != '\0' && (++ iCmdIndex) >= MAX_CMDS)
                        iCmdIndex = 0;
                     // Obliterate current displayed text
                     for (int32 i = outpos = 0; i < inpos; i ++)
                     {
                        outbuf[outpos++] = '\b';
                        outbuf[outpos++] = ' ';
                        outbuf[outpos++] = '\b';
                     }
                     // Copy command into command and display buffers
                     for (inpos = 0; inpos < (int32)strlen(rgCmds[iCmdIndex]); inpos ++, outpos ++)
                     {
                        outbuf[outpos] = rgCmds[iCmdIndex][inpos];
                        inputBuffer[inpos] = rgCmds[iCmdIndex][inpos];
                     }
                              }
                              break;
                              // LEFT ARROW
                  case 0x25 :
                     break;
                     // RIGHT ARROW
                  case 0x27 :
                     break;
                  default :
                     break;
                  }
                  break;
               case '\b':
                  if(inpos > 0)
                  {
                     outbuf[outpos++] = '\b';
                     outbuf[outpos++] = ' ';
                     outbuf[outpos++] = '\b';
                     inpos--;
                  }
                  break;
               case '\t':
                  // In the output buffer, we're going to have to erase the current line (in case
                  // we're cycling through various completions) and write out the whole input
                  // buffer, so (inpos * 3) + complen <= 512.  Should be OK.  The input buffer is
                  // also 512 chars long so that constraint will also be fine for the input buffer.
                  {
                     // Erase the current line.
                     int32 i;
                     for (i = 0; i < inpos; i++) {
                        outbuf[outpos++] = '\b';
                        outbuf[outpos++] = ' ';
                        outbuf[outpos++] = '\b';
                     }
                     // Modify the input buffer with the completion.
                     uint32 maxlen = 512 - (inpos * 3);
                     if (ke->dwControlKeyState & SHIFT_PRESSED) {
                        inpos; // = Con::tabComplete(inbuf, inpos, maxlen, false);
                     }
                     else {
                        inpos; // = Con::tabComplete(inbuf, inpos, maxlen, true);
                     }
                     // Copy the input buffer to the output.
                     for (i = 0; i < inpos; i++) {
                        outbuf[outpos++] = inputBuffer[i];
                     }
                  }
                  break;
               case '\n':
               case '\r':
                  outbuf[outpos++] = '\r';
                  outbuf[outpos++] = '\n';
                  inputBuffer[inpos] = 0;
                  outbuf[outpos] = 0;
                  printf("%s", outbuf);
                  // Pass the line along to the console for execution.
                  {
                     //RawData rd;
                     //rd.size = inpos + 1;
                     //rd.data = ( int8* ) inputBuffer;
                     //Con::smConsoleInput.trigger(rd);
                  }
                  // If we've gone off the end of our array, wrap
                  // back to the beginning
                  if (iCmdIndex >= MAX_CMDS)
                     iCmdIndex %= MAX_CMDS;
                  // Put the new command into the array
                  strcpy(rgCmds[iCmdIndex ++], inputBuffer);
                  //printf("%s", Con::getVariable("Con::Prompt"));
                  inpos = outpos = 0;
                  break;
               default:
                  inputBuffer[inpos++] = ke->uChar.AsciiChar;
                  outbuf[outpos++] = ke->uChar.AsciiChar;
                  break;
               }
            }
         }
      }
      if(outpos)
      {
         outbuf[outpos] = 0;
         printf("%s", outbuf);
      }
   }
}