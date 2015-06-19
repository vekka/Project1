#include "oaldriver.hpp"

namespace oaldriver
{
   bool OALDriver::CreateContext()
   {
      m_pDevice = alcOpenDevice(NULL);
      if (!m_pDevice)
         return false;
      m_pContext = alcCreateContext(m_pDevice, NULL);
      alcMakeContextCurrent(m_pContext);
      if (!m_pContext)
         return false;
   }
} // namespace oaldriver
