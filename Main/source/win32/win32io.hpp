#ifndef _WIN32IO_HPP_INCLUDED_
#define _WIN32IO_HPP_INCLUDED_

#include "win32main.hpp"
using win32window::Win32Window;

namespace win32io
{
    class Win32IO
    {
    private:
        static Win32Window::Win32Mouse win32Mouse;
    public:
        void Frame();
    };
} //namespace win32io

#endif
