#ifndef _WIN32TIMER_HPP_INCLUDED_HPP_
#define _WIN32TIMER_HPP_INCLUDED_HPP_

#include "core/BasicTypes.hpp"

namespace win32timer
{
   
// high-resolution timer (64-bit)
class Win32Timer
{
private:
   uint32 tickCountCurrent;
   uint32 tickCountNext;
   int64 perfCountCurrent;
   int64 perfCountNext;
   int64 frequency;
   double perfCountRemainderCurrent;
   double perfCountRemainderNext;
   bool usingPerfCounter;
   
public:
    Win32Timer();

    // get the current ticks or time relative to the initial time
    int64 GetTicks();

    const int32 GetMilliSecs();

    const double GetSeconds();

    // get the time for the specified number of ticks
    const double GetSeconds( const int64 numTicks ) const;

    // get the number of ticks for the specified time
    int64 GetTicks( const double seconds ) const;

    // reset so that initial ticks is the current time
    void Reset();
};

} // namespace win32timer

#endif
