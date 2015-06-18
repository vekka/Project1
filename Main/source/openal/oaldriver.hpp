#ifndef _OALDRIVER_HPP_INCLUDED_
#define _OALDRIVER_HPP_INCLUDED_

#include <AL/al.h>
#include <AL/alc.h>

namespace oaldriver
{
   class OALDriver
   {
   private:
       ALCdevice *m_device;
       ALCcontext *m_context;
   public:
       OALDriver() : m_device(NULL), m_context(NULL) {}
       ~OALDriver() {}
       bool CreateContext();
   };
} // namespace oaldriver

#endif
