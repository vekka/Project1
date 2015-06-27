#include "core/BasicTypes.hpp"

#include "win32/Win32Console.hpp"
using win32console::Win32Console;

#include "win32/Win32Timer.hpp"
using win32timer::Win32Timer;

//int32 WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow)
void WinM() // temp since WinMain is defined in test
{
   Win32Console sysConsole;
      
   // for error output
   sysConsole.Create();
      
   Win32Timer win32Timer;
   win32Timer.GetMilliSecs();
   
   while(1)
   {
      int32 startTime, endTime = 0;
      int32 totalMS = 0, countMS = 0;
      
      startTime = win32Timer.GetMilliSecs();
      
      //InFrame();
      
      //ComFrame();
      
   	totalMS += endTime - startTime;
		countMS++;   
   }
}
