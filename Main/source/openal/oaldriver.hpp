#ifndef _OALDRIVER_HPP_INCLUDED_
#define _OALDRIVER_HPP_INCLUDED_

#include <al.h>
#include <alc.h>

#include <cstdlib>
namespace oaldriver
{
   class OALDriver
   {
   private:
       ALCdevice *m_pDevice;
       ALCcontext *m_pContext;
   public:
      OALDriver() : m_pDevice(NULL), m_pContext(NULL) {}
       ~OALDriver() {}
       bool CreateContext();
   };
} // namespace oaldriver

#endif
