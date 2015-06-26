#ifndef _WIN32TIMER_HPP_INCLUDED_HPP_
#define _WIN32TIMER_HPP_INCLUDED_HPP_

#include "core/BasicTypes.hpp"

namespace win32timer
{

   class Win32Timer
   {
   private:
      static bool m_initialized;
      uint32 m_timeBase;
      
   public:
       Win32Timer();
   
       // get the current ticks or time relative to the initial time
       int64 GetTicks();
   
       const int32 GetMilliSecs();
   
       const double GetSeconds();
   
       // reset so that initial ticks is the current time
       void Reset();
   };
   
   // high-resolution timer (64-bit)
   class Win32Timer64
   {
   private:
      int64 m_perfCountCurrent;
      int64 m_perfCountNext;
      int64 m_frequency;
      double m_perfCountRemainderCurrent;
      double m_perfCountRemainderNext;
      bool m_usingPerfCounter;
      
   public:
      // get the time for the specified number of ticks
      const double GetSeconds( const int64 numTicks ) const;
   
      // get the number of ticks for the specified time
      int64 GetTicks( const double seconds ) const;
   };

} // namespace win32timer

#endif
