#include "core/BasicTypes.hpp"

#include "win32/Win32Console.hpp"
using win32console::Win32Console;

#include "win32/Win32Timer.hpp"
using win32timer::Win32Timer;

int32 WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow)
{
   Win32Console sysConsole;
      
   // for error output
   sysConsole.Create();
      
   Win32Timer win32Timer;
   win32Timer.GetMilliSecs();
   
   while(1)
   {
      int32 startTime, endTime;
      int32 totalMS, countMS;
      
      startTime = Sys_Milliseconds();
      
      //InFrame();
      
      //ComFrame();
      
   	totalMS += endTime - startTime;
		countMS++;   
   }
}
