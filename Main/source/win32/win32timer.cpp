#include "win32timer.hpp"

#include <Windows.h>

//Win32Timer::Win32Timer()
//{
//   perfCountRemainderCurrent = 0.0;
//   
//   usingPerfCounter = QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
//   if (usingPerfCounter) 
//      usingPerfCounter = QueryPerformanceCounter((LARGE_INTEGER*)&perfCountCurrent);
//   else
//      tickCountCurrent = GetTickCount();
//}
//
//const uint32 Win32Timer::GetMilliSecs() 
//{ 
//   if(usingPerfCounter)
//   {
//      // Use QPC, update remainders so we don't leak time, and return the elapsed time
//      QueryPerformanceCounter((LARGE_INTEGER*)&perfCountNext);
//      double elapsed64 = (1000.0 * double(perfCountNext - perfCountCurrent) / double(frequency));
//      elapsed64 += perfCountRemainderCurrent;
//      uint32 elapsed = (uint32)mFloor(elapsedF64);
//
// 64
//         mPerfCountRemainderNext = elapsedF64 - F64(elapsed); 
//
// 65
//
// 
//
// 66
//         return elapsed; 
//
// 67
//      } 
//
// 68
//      else 
//
// 69
//      { 
//
// 70
//         // Do something naive with GTC. 
//
// 71
//         mTickCountNext = GetTickCount(); 
//
// 72
//         return mTickCountNext - mTickCountCurrent; 
//
// 73
//      } 
//
// 74
//   } 
