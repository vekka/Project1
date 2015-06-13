#include <stdarg.h>
#include <assert.h>

#include <ios>
#include <io.h>
#include <fcntl.h>

#include "win32console.hpp"

namespace win32console
{

   Win32Console::Win32Console(
      const uint32 width, const uint32 height,
      const uint32 bufferWidth, const uint32 bufferHeight)
   {
      m_created = false;
      m_width = width;
      m_height = height;
      m_screenBufferInfo.dwSize.X = bufferWidth;
      m_screenBufferInfo.dwSize.Y = bufferHeight;

      // /screenBuffer = new CHAR_INFO[screenBufferWidth*screenBufferHeight];
   }

   Win32Console::~Win32Console(void)
   {
      delete[] m_pScreenBuffer;
   }

   bool Win32Console::Create()
   {
      if (!AllocConsole())
         return false;

      //if (useSTDIO)
      //{
      //  if (!std::ios_base::sync_with_stdio())
      //      return false;
      //}

      m_created = true;

      //if (!UpdateDimension(width, height))
      //   printf("Warning: Console Window is not set to requested dimension.\n");

      //UpdateScreenBufferDimension(m_bufferWidth, m_bufferHeight);

      return true;
   }

   bool Win32Console::SetRedirection(eRedirection redir, bool useSTDIO)
   {
      GetConsoleScreenBufferInfo(GetStdHandle(redir), &m_screenBufferInfo);
      if (!SetConsoleScreenBufferSize(GetStdHandle(redir), m_screenBufferInfo.dwSize))
         return false;
      int32 stdHandle = (int32)GetStdHandle(redir);
      int32 hConHandle = _open_osfhandle(stdHandle, _O_TEXT);

      if (redir == REDIR_STDOUT)
      {
         FILE *fp = _fdopen(hConHandle, "w");
         *stdout = *fp;
         setvbuf(stdout, NULL, _IONBF, 0);
      }
      else if (redir == REDIR_STDIN)
      {
         FILE *fp = _fdopen(hConHandle, "r");
         *stdin = *fp;
         setvbuf(stdin, NULL, _IONBF, 0);
      }
      else if (redir == REDIR_STDERR)
      {
         FILE *fp = _fdopen(hConHandle, "w");
         *stderr = *fp;
         setvbuf(stderr, NULL, _IONBF, 0);
      }
      
      if (useSTDIO)
         std::ios::sync_with_stdio();

      return true;
   }

   void Win32Console::Destroy(void)
   {
   }

   bool Win32Console::IsEnabled(void) const
   {
      return true;
   }

   bool Win32Console::SetCaption(const std::string &caption)
   {
      assert(m_created);
      return (bool)(SetConsoleTitle(caption.c_str()) != 0);
   }

   void Win32Console::GetCaption(std::string &caption) const
   {
      assert(m_created);

      char buffer[512];
      /*int32 strLen = */GetConsoleTitle(buffer, 512);

      //if (strLen == 0)
      //   caption = NULL;

      caption = buffer;
   }

   void SetPosition(const uint32 newXPos, const uint32 newYPos)
   {

   }

   bool Win32Console::UpdateDimension(const uint32 width, const uint32 height)
   {
      assert(m_created);

      m_width = width;
      m_height = height;
      SMALL_RECT windowSize = { 0, 0, width - 1, height - 1 };

      return SetConsoleWindowInfo(stdOut, TRUE, &windowSize) != 0;
   }

   void Win32Console::GetDimension(uint32 &width, uint32 &height) const
   {
      width = m_width;
      height = m_height;
   }

   void Win32Console::UpdateScreenBufferDimension(const uint32 width, const uint32 height)
   {
      assert(m_created);

      //m_bufferWidth = width;
      //m_bufferHeight = height;
      //COORD bufferSize = { width, height };
      //screenBuffer = new CHAR_INFO[width*height];
      //SetConsoleScreenBufferSize(stdOut, bufferSize);
   }

   // clear the screen buffer with given background color
   void Win32Console::ClearScreenBuffer(const eBkgConsoleColor bkgColor) const
   {
      //assert(created);

      //for (uint32 i = 0; i < m_bufferWidth * m_bufferHeight; i++)
      //{
      //   screenBuffer[i].Char.AsciiChar = ' ';
      //   screenBuffer[i].Attributes = bkgColor | BACKGROUND_INTENSITY;
      //}

      //COORD charBufSize = { m_bufferWidth, m_bufferHeight };
      //COORD characterPos = { 0, 0 };
      //SMALL_RECT writeArea = { 0, 0, m_bufferWidth - 1, m_bufferHeight - 1 };

      //WriteConsoleOutputA(stdOut, screenBuffer, charBufSize, characterPos, &writeArea);
   }

   void Win32Console::printf(const char *string, ...) const
   {
      assert(m_created);

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
      FlushFileBuffers(stdOut);
   }

   void Win32Console::ProcessLine(const char *consoleLine) const
   {
   }

   void Win32Console::Process(void)
   {
      assert(m_created);

      DWORD numEvents;
      GetNumberOfConsoleInputEvents(stdIn, &numEvents);
      if (numEvents)
      {
         INPUT_RECORD rec[20];
         char outbuf[512];
         int32 outpos = 0;
         ReadConsoleInput(stdIn, rec, 20, &numEvents);
         DWORD i;
         for (i = 0; i < numEvents; i++)
         {
            if (rec[i].EventType == KEY_EVENT)
            {
               KEY_EVENT_RECORD *ke = &(rec[i].Event.KeyEvent);
               if (ke->bKeyDown)
               {
                  switch (ke->uChar.AsciiChar)
                  {
                     // If no ASCII char, check if it's a handled virtual key
                  case 0:
                     switch (ke->wVirtualKeyCode)
                     {
                        // UP ARROW
                     case 0x26:
                        // Go to the previous command in the cyclic array
                        if ((--iCmdIndex) < 0)
                           iCmdIndex = MAX_CMDS - 1;
                        // If this command isn't empty ...
                        if (rgCmds[iCmdIndex][0] != '\0')
                        {
                           // Obliterate current displayed text
                           for (int32 i = outpos = 0; i < inpos; i++)
                           {
                              outbuf[outpos++] = '\b';
                              outbuf[outpos++] = ' ';
                              outbuf[outpos++] = '\b';
                           }
                           // Copy command into command and display buffers
                           for (inpos = 0; inpos < (int32)strlen(rgCmds[iCmdIndex]); inpos++, outpos++)
                           {
                              outbuf[outpos] = rgCmds[iCmdIndex][inpos];
                              inputBuffer[inpos] = rgCmds[iCmdIndex][inpos];
                           }
                        }
                        // If previous is empty, stay on current command
                        else if ((++iCmdIndex) >= MAX_CMDS)
                        {
                           iCmdIndex = 0;
                        }
                        break;
                        // DOWN ARROW
                     case 0x28: {
                        // Go to the next command in the command array, if
                        // it isn't empty
                        if (rgCmds[iCmdIndex][0] != '\0' && (++iCmdIndex) >= MAX_CMDS)
                           iCmdIndex = 0;
                        // Obliterate current displayed text
                        for (int32 i = outpos = 0; i < inpos; i++)
                        {
                           outbuf[outpos++] = '\b';
                           outbuf[outpos++] = ' ';
                           outbuf[outpos++] = '\b';
                        }
                        // Copy command into command and display buffers
                        for (inpos = 0; inpos < (int32)strlen(rgCmds[iCmdIndex]); inpos++, outpos++)
                        {
                           outbuf[outpos] = rgCmds[iCmdIndex][inpos];
                           inputBuffer[inpos] = rgCmds[iCmdIndex][inpos];
                        }
                     }
                                break;
                                // LEFT ARROW
                     case 0x25:
                        break;
                        // RIGHT ARROW
                     case 0x27:
                        break;
                     default:
                        break;
                     }
                     break;
                  case '\b':
                     if (inpos > 0)
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
                     strcpy(rgCmds[iCmdIndex++], inputBuffer);
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
         if (outpos)
         {
            outbuf[outpos] = 0;
            printf("%s", outbuf);
         }
      }
   }

} // namespace win32console
