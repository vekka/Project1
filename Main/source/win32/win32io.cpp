#include "win32io.hpp"

//Win32Window::Win32Keyboard Win32Window::keyboard;


namespace win32io
{
     Win32Window::Win32Mouse Win32IO::win32Mouse;

    void Win32IO::Frame()
    {
        //IN_JoyMove();

        // ...

        //IN_ActivateMouse();

        // post events to the system que
        //IN_MouseMove();
    }
} // namespace win32io
