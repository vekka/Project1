#ifndef _OALDRIVER_HPP_INCLUDED_
#define _OALDRIVER_HPP_INCLUDED_

#include <al.h>
#include <alc.h>

#include <cstdlib>

#include "core/BasicTypes.hpp"

namespace sound
{
   namespace oaldriver
   {
      class OALDriver
      {
      public:
         static const int32 BUFFER_SIZE = 4096;
      private:
          ALCdevice *m_pDevice;
          ALCcontext *m_pContext;
          uint32 buffers[NUM_BUFFERS];
          uint32 source;
      public:   
         OALDriver() : m_pDevice(NULL), m_pContext(NULL) {}
          ~OALDriver() {}
          bool Init();
      };
   } // namespace oaldriver
} // namespace sound

#endif
