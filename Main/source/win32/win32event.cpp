#include "win32event.hpp"

namespace win32event
{
    void Win32EventQueue::Enqueue(int32 time, eType type, int32 value1, int32 value2, int32 ptrLen, void *ptr)
    {
        Event *event;

        event = &m_queue[m_head & MASK_QUEUED_EVENTS];

        if ( m_head - m_tail >= MAX_QUEUED_EVENTS ) // overflow
        {
            if (event->m_ptr) // discarding an event
            {
                // free event->m_ptr here
            }
            m_tail++;
        }

        m_head++;

        if (time == 0)
        {
            //time = Sys_Milliseconds();
        }

        event->m_time = time;
        event->m_type = type;
        event->m_value1 = value1;
        event->m_value2 = value2;
        event->m_ptrLen = ptrLen;
        event->m_ptr = ptr;
    }

} // namespace win32event
