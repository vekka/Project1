#ifndef _WIN32EVENT_HPP_INCLUDED_HPP_
#define _WIN32EVENT_HPP_INCLUDED_HPP_

#include "core/BasicTypes.hpp"

namespace win32event
{
    enum eType
    {
        TYPE_NONE = 0,
        TYPE_KEY,
        TYPE_CHAR,
        TYPE_MOUSE,
        TYPE_JOYSTICK_AXIS,
        TYPE_CONSOLE,
        TYPE_PACKET
    };

    class Win32EventQueue
    {
    private:
        static const int32 MAX_QUEUED_EVENTS = 256;
        static const int32 MASK_QUEUED_EVENTS = MAX_QUEUED_EVENTS - 1;

        struct Event
        {
          int32 m_time;
          eType m_type;
          int32 m_value1;
          int32 m_value2;
          int32 m_ptrLen;
          void *m_ptr;
        } m_queue[MAX_QUEUED_EVENTS];

        int32 m_head;
        int32 m_tail;
    public:
        Win32EventQueue() : m_head(0), m_tail(0) {}
        void Enqueue(int32 time, eType type, int32 value1, int32 value2, int32 ptrLen, void *ptr);
    };

} // namespace win32event

#endif
