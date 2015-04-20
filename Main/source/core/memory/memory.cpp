
#include "memory.hpp"

void RefCounterBase::Grab() const
{
   refCount++;
}

void RefCounterBase::Release() const
{
   assert( refCount > 0 );
   refCount--;
   if( refCount == 0 )
   {
      delete (RefCounterBase*)this;
   }
}