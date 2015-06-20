#include "oaldriver.hpp"

namespace oaldriver
{
   bool OALDriver::Init()
   {
      m_pDevice = alcOpenDevice(NULL);
      if (!m_pDevice)
         return false;
      m_pContext = alcCreateContext(m_pDevice, NULL);
      alcMakeContextCurrent(m_pContext);
      if (!m_pContext)
         return false;
      alGenBuffers(NUM_BUFFERS, buffers);
      alGenSources(1, &source);
      if (alGetError() != AL_NO_ERROR)
         return false;
   }
} // namespace oaldriver
