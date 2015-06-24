#include "win32main.hpp"

//using core::string::String_c;

#include <cassert>

using win32window::Win32Window;

namespace win32window
{
   bool Win32Window::Win32Keyboard::keys[256];
   Win32Window::Win32Keyboard Win32Window::keyboard;
   Win32Window::Win32Mouse Win32Window::mouse;
   int32 Win32Window::Win32Mouse::xPos = 0;
   int32 Win32Window::Win32Mouse::yPos = 0;
   bool Win32Window::Win32Mouse::doMouseMove = false;

   Win32Window::funcptr_t Win32Window::customCallback = NULL;

   bool Win32Window::isResized = false;

   Win32Window::Win32Window()
      : hWnd(NULL), fullscreen(false), externalWindow(false), close(false)
   {
   }

   Win32Window::Win32Window(const HINSTANCE hinstance,
      const int32 x, const int32 y,
      const uint32 width, const uint32 height,
      const uint32 bitsPerPel, const DWORD dStyle,
      const HWND parentWnd)
      : hWnd(NULL), fullscreen(false), externalWindow(false), close(false)
   {
      Create(hInstance, x, y, width, height, bitsPerPel, dStyle, parentWnd);
      
   }

   Win32Window::~Win32Window()
   {
   }

   HWND Win32Window::GetWindowHandle() const
   {
      return hWnd;
   }

   HMENU Win32Window::GetMenuHandle() const
   {
      return hMenu;
   }

   LPCTSTR className = "BasicApp";
   LPCTSTR wndName = "A Simple Window";

   HWND Win32Window::Create(const HINSTANCE hinstance,
      const int32 x, const int32 y,
      const uint32 width, const uint32 height,
      const uint32 bitsPerPel,
      const DWORD dStyle,
      const HWND parentWnd)
   {
      RegisterWindowClass();
      this->width = width;
      this->height = height;
      this->bitsPerPel = bitsPerPel;

      hWnd = CreateWindowEx(NULL,
         className,
         wndName,
         dStyle,
         x,
         y,
         width,
         height,
         parentWnd,
         NULL,
         hinstance,
         NULL);

      return hWnd;
   }

   void Win32Window::RegisterWindowClass() const
   {
      WNDCLASSEX wcex;

      // get handle to exe
      HMODULE hInstance = GetModuleHandle(NULL);

      // create the application window
      wcex.cbSize = sizeof(WNDCLASSEX);
      wcex.style = CS_HREDRAW | CS_VREDRAW;
      wcex.lpfnWndProc = (WNDPROC)WndProc;
      wcex.cbClsExtra = 0;
      wcex.cbWndExtra = 0;
      wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
      wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
      wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
      wcex.lpszMenuName = NULL;
      wcex.lpszClassName = className;
      wcex.hInstance = hInstance;
      wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

      // Register the application
      RegisterClassEx(&wcex);
   }

   void Win32Window::UnregisterWindowClass() const
   {
      WNDCLASSEX wcex;
      if (GetClassInfoEx(GetModuleHandle(NULL), className, &wcex))
         UnregisterClass(className, GetModuleHandle(NULL));
   }

   bool Win32Window::SwitchToFullScreen()
   {
      if (fullscreen)
         return true;

      //if (changedtofullscreen)
      //   return (changedisplaysettings(&desktopmode, 0) == disp_change_successful);
      //else
      //   return true;

      DEVMODE dm;
      memset(&dm, 0, sizeof(dm));
      dm.dmSize = sizeof(dm);

      EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
      dm.dmPelsWidth = width;
      dm.dmPelsHeight = height;
      dm.dmBitsPerPel = bitsPerPel;
      dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

      long result = ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
      if (result != DISP_CHANGE_SUCCESSFUL)
      { // try again without forcing display frequency
         dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
         result = ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
      }

      switch (result)
      {
      case DISP_CHANGE_SUCCESSFUL:
         //changedtofullscreen = true;
         return true;
      case DISP_CHANGE_RESTART:
         //os::printer::log("switch to fullscreen: the computer must be restarted in order for the graphics mode to work.", ell_error);
         return false;
      case DISP_CHANGE_BADFLAGS:
         //os::printer::log("switch to fullscreen: an invalid set of flags was passed in.", ell_error);
         return false;
      case DISP_CHANGE_BADPARAM:
         //os::printer::log("switch to fullscreen: an invalid parameter was passed in. this can include an invalid flag or combination of flags.", ell_error);
         return false;
      case DISP_CHANGE_FAILED:
         //os::printer::log("switch to fullscreen: the display driver failed the specified graphics mode.", ell_error);
         return false;
      case DISP_CHANGE_BADMODE:
         //os::printer::log("switch to fullscreen: the graphics mode is not supported.", ell_error);
         return false;
      default:
         //os::printer::log("an unknown error occured while changing to fullscreen.", ell_error);
         return false;
      }

      // Clear the menu bar from the window for full screen
      HMENU menu = GetMenu(hWnd);
      if (menu)
      {
         SetMenu(hWnd, NULL);
      }

      fullscreen = true;

      return true;
   }

   HINSTANCE Win32Window::GetInstance() const
   {
      return hInstance;
   }

   bool Win32Window::IsFullScreen() const
   {
      return fullscreen;
   }

   void Win32Window::OnResize() const
   {
      isResized = false;
   }

   bool Win32Window::SetCaption(const std::string &caption)
   {
      return (bool)(SetWindowTextA(hWnd, caption.c_str()) != 0);
   }

   bool Win32Window::GetResizeFlag() const
   {
      return isResized;
   }

   void Win32Window::GetCaption(std::string &caption) const
   {
      char buffer[512];
      /*int32 strLen = */GetWindowTextA(hWnd, buffer, 512);

      //if (strLen == 0)
      //   caption = NULL;

      caption = buffer;
   }


   void Win32Window::SetPosition(const uint32 newXPos, const uint32 newYPos)
   {
      SetWindowPos(hWnd, HWND_TOP, newXPos, newYPos, 0, 0, SWP_NOSIZE);
   }

   bool Win32Window::Show() const
   {
      return ShowWindow(hWnd, SW_SHOWNORMAL) != 0;
   }

   bool Win32Window::Update() const
   {
      return UpdateWindow(hWnd) != 0;
   }

   bool Win32Window::Minimize() const
   {
      return ShowWindow(hWnd, SW_HIDE) != 0;
   }

   bool Win32Window::Maximize() const
   {
      return ShowWindow(hWnd, SW_MAXIMIZE) != 0;
   }

   bool Win32Window::Restore() const
   {
      return ShowWindow(hWnd, SW_RESTORE) != 0;
   }

   bool Win32Window::Hide() const
   {
      return ShowWindow(hWnd, SW_HIDE) != 0;
   }

   void Win32Window::GetDimension(uint32 &width, uint32 &height) const
   {
      width = this->width;
      height = this->height;
   }

   void Win32Window::HandleSystemMessages(MSG *msg)
   {
      //bool quit = false;
      //while ( PeekMessage(msg, NULL, 0, 0, PM_REMOVE) )

      while (PeekMessage(msg, NULL, 0, 0, PM_REMOVE))
      {
         // No message translation because we don't use WM_CHAR and it would conflict with our
         // deadkey handling.32

         TranslateMessage(msg);
         DispatchMessage(msg);
         //if (externalWindow && msg.hwnd == hWnd)
         //	WndProc(hWnd, msg.message, msg.wParam, msg.lParam);
         //else
         //	DispatchMessage(&msg);
      }
   }

   void Win32Window::ClearSystemMessages() const
   {
      MSG msg;
      while (PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
      {
      }
      while (PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE))
      {
      }
   }

   void Win32Window::WmMouseMove(HWND hwnd, WPARAM wp, LPARAM lp)
   {
      //POINTS p;
      //HDC hdc;
      //if (!do_mouse_move) 
      //   return;

      //p = MAKEPOINTS(lp);

      //MessageBox(hwnd, "hello", "hellod", NULL);
   }

   LRESULT CALLBACK Win32Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
   {
      switch (msg)
      {
      case WM_CREATE:
         return 0;

      case WM_PAINT:
         PAINTSTRUCT ps;
         BeginPaint(hWnd, &ps);
         EndPaint(hWnd, &ps);
         return 0;
      case WM_ACTIVATE:
         return 0;
      case WM_SIZE:
         isResized = true;
         return 0;
         //case WM_SYSCOMMAND: // maybe use only in fullscreen
         //   // prevent screensaver or monitor powersave mode from starting
         //   switch (wParam)
         //   {
         //   case SC_SCREENSAVE:
         //   case SC_MONITORPOWER: 
         //   //case SC_KEYMENU:
         //      return 0;
         //   }
         //   return 0;
      case WM_DESTROY:
         PostQuitMessage(WM_QUIT);
         return 0;
      default:
         keyboard.Dispatch(msg, wParam, lParam);
         mouse.Dispatch(msg, wParam, lParam);
         return DefWindowProc(hWnd, msg, wParam, lParam);
      }
      return 0;
   }

   bool Win32Window::Resize(const int32 w, const int32 h)
   {
      assert(w != 0 && h != 0);
      return true;
   }



   // Win32Mouse is nested into class Win32Window
   Win32Window::Win32Mouse::Win32Mouse()
   {
   }

   Win32Window::Win32Mouse::~Win32Mouse()
   {
   }

   void Win32Window::Win32Mouse::SetVisible(const bool visible)
   {
      ShowCursor(visible);
   }

   bool Win32Window::Win32Mouse::IsVisible() const
   {
      return true;
   }

   void Win32Window::Win32Mouse::SetPosition(int32 x, int32 y)
   {
      POINT pt;
      //ClientToScreen(hWnd, &pt);
      SetCursorPos(x, y);

      xPos = x;
      yPos = y;
   }

   void Win32Window::Win32Mouse::SetCursor(const HWND hWnd, const uint32 winWidth, const uint32 winHeight, const bool isFullScreen)
   {
   }

   void Win32Window::Win32Mouse::GetPosition(int32 &xPos, int32 &yPos) const
   {
      xPos = this->xPos;
      yPos = this->yPos;
   }

   void Win32Window::Win32Mouse::GetRelativePosition(int32 &xPos, int32 &yPos) const
   {
   }

   void Win32Window::Win32Mouse::OnResize(const int32 &sizeX, const int32 &sizeY)
   {
   }

   void Win32Window::Win32Mouse::UpdateBorderSize(const bool isFullScreen, const bool resizable)
   {
   }



 } // namespace win32window

   //#include "win32main.hpp"
   //
   //const char* CLASS_NAME = __TEXT("CWin32");
   //
   //struct CreationParameters
   //{
   //   CreationParameters()
   //      : windowSize(800, 600),
   //      bitsPerPel(16),
   //      zBufferBits(16),
   //      isFullScreen(false),
   //      stencilBuffer(false),
   //      vSync(false),
   //      antiAlias(0),
   //      handleSRGB(false),
   //      withAlphaChannel(false),
   //      doubleBuffer(true),
   //      ignoreInputEvents(false),
   //      stereoBuffer(false),
   //      highPrecisionFPU(false),
   //      windowId(0),
   //      displayAdapter(0),
   //      driverMultithreaded(false),
   //      usePerformanceTimer(true) {}
   //
   //   //E_DEVICE_TYPE deviceType;
   //   //video::E_DRIVER_TYPE driverType;
   //   Dimension2u windowSize;
   //   uint8 bitsPerPel;
   //   uint8 zBufferBits;
   //   bool isFullScreen;
   //   bool stencilBuffer;
   //   bool vSync;
   //   uint8 antiAlias;
   //   bool handleSRGB;
   //   bool withAlphaChannel;
   //   bool doubleBuffer;
   //   bool ignoreInputEvents;
   //   bool stereoBuffer;
   //   bool highPrecisionFPU;
   //   //IEventReceiver* EventReceiver;
   //   void* windowId;
   //   //ELOG_LEVEL LoggingLevel;
   //   uint32 displayAdapter;
   //   bool driverMultithreaded;
   //   bool usePerformanceTimer;
   //};
   //
   //class Win32Window;
   //
   ////IVideoDriver* CreateOpenGLDriver(const CreationParameters &params,
   ////			io::IFileSystem* io, Win32Window* device);
   //
   //void CreateOpenGLDriver(const CreationParameters &params,
   //			/*io::IFileSystem* io,*/ Win32Window* device)
   //{
   //}
   //
   //enum EKEY_CODE
   //{
   //   KEY_LBUTTON          = 0x01,  // Left mouse button
   //   KEY_RBUTTON          = 0x02,  // Right mouse button
   //   KEY_CANCEL           = 0x03,  // Control-break processing
   //   KEY_MBUTTON          = 0x04,  // Middle mouse button (three-button mouse)
   //   KEY_XBUTTON1         = 0x05,  // Windows 2000/XP: X1 mouse button
   //   KEY_XBUTTON2         = 0x06,  // Windows 2000/XP: X2 mouse button
   //   KEY_BACK             = 0x08,  // BACKSPACE key
   //   KEY_TAB              = 0x09,  // TAB key
   //   KEY_CLEAR            = 0x0C,  // CLEAR key
   //   KEY_RETURN           = 0x0D,  // ENTER key
   //   KEY_SHIFT            = 0x10,  // SHIFT key
   //   KEY_CONTROL          = 0x11,  // CTRL key
   //   KEY_MENU             = 0x12,  // ALT key
   //   KEY_PAUSE            = 0x13,  // PAUSE key
   //   KEY_CAPITAL          = 0x14,  // CAPS LOCK key
   //   KEY_KANA             = 0x15,  // IME Kana mode
   //   KEY_HANGUEL          = 0x15,  // IME Hanguel mode (maintained for compatibility use KEY_HANGUL)
   //   KEY_HANGUL           = 0x15,  // IME Hangul mode
   //   KEY_JUNJA            = 0x17,  // IME Junja mode
   //   KEY_FINAL            = 0x18,  // IME final mode
   //   KEY_HANJA            = 0x19,  // IME Hanja mode
   //   KEY_KANJI            = 0x19,  // IME Kanji mode
   //   KEY_ESCAPE           = 0x1B,  // ESC key
   //   KEY_CONVERT          = 0x1C,  // IME convert
   //   KEY_NONCONVERT       = 0x1D,  // IME nonconvert
   //   KEY_ACCEPT           = 0x1E,  // IME accept
   //   KEY_MODECHANGE       = 0x1F,  // IME mode change request
   //   KEY_SPACE            = 0x20,  // SPACEBAR
   //   KEY_PRIOR            = 0x21,  // PAGE UP key
   //   KEY_NEXT             = 0x22,  // PAGE DOWN key
   //   KEY_END              = 0x23,  // END key
   //   KEY_HOME             = 0x24,  // HOME key
   //   KEY_LEFT             = 0x25,  // LEFT ARROW key
   //   KEY_UP               = 0x26,  // UP ARROW key
   //   KEY_RIGHT            = 0x27,  // RIGHT ARROW key
   //   KEY_DOWN             = 0x28,  // DOWN ARROW key
   //   KEY_SELECT           = 0x29,  // SELECT key
   //   KEY_PRINT            = 0x2A,  // PRINT key
   //   KEY_EXECUT           = 0x2B,  // EXECUTE key
   //   KEY_SNAPSHOT         = 0x2C,  // PRINT SCREEN key
   //   KEY_INSERT           = 0x2D,  // INS key
   //   KEY_DELETE           = 0x2E,  // DEL key
   //   KEY_HELP             = 0x2F,  // HELP key
   //   KEY_KEY_0            = 0x30,  // 0 key
   //   KEY_KEY_1            = 0x31,  // 1 key
   //   KEY_KEY_2            = 0x32,  // 2 key
   //   KEY_KEY_3            = 0x33,  // 3 key
   //   KEY_KEY_4            = 0x34,  // 4 key
   //   KEY_KEY_5            = 0x35,  // 5 key
   //   KEY_KEY_6            = 0x36,  // 6 key
   //   KEY_KEY_7            = 0x37,  // 7 key
   //   KEY_KEY_8            = 0x38,  // 8 key
   //   KEY_KEY_9            = 0x39,  // 9 key
   //   KEY_KEY_A            = 0x41,  // A key
   //   KEY_KEY_B            = 0x42,  // B key
   //   KEY_KEY_C            = 0x43,  // C key
   //   KEY_KEY_D            = 0x44,  // D key
   //   KEY_KEY_E            = 0x45,  // E key
   //   KEY_KEY_F            = 0x46,  // F key
   //   KEY_KEY_G            = 0x47,  // G key
   //   KEY_KEY_H            = 0x48,  // H key
   //   KEY_KEY_I            = 0x49,  // I key
   //   KEY_KEY_J            = 0x4A,  // J key
   //   KEY_KEY_K            = 0x4B,  // K key
   //   KEY_KEY_L            = 0x4C,  // L key
   //   KEY_KEY_M            = 0x4D,  // M key
   //   KEY_KEY_N            = 0x4E,  // N key
   //   KEY_KEY_O            = 0x4F,  // O key
   //   KEY_KEY_P            = 0x50,  // P key
   //   KEY_KEY_Q            = 0x51,  // Q key
   //   KEY_KEY_R            = 0x52,  // R key
   //   KEY_KEY_S            = 0x53,  // S key
   //   KEY_KEY_T            = 0x54,  // T key
   //   KEY_KEY_U            = 0x55,  // U key
   //   KEY_KEY_V            = 0x56,  // V key
   //   KEY_KEY_W            = 0x57,  // W key
   //   KEY_KEY_X            = 0x58,  // X key
   //   KEY_KEY_Y            = 0x59,  // Y key
   //   KEY_KEY_Z            = 0x5A,  // Z key
   //   KEY_LWIN             = 0x5B,  // Left Windows key (Microsoft® Natural® keyboard)
   //   KEY_RWIN             = 0x5C,  // Right Windows key (Natural keyboard)
   //   KEY_APPS             = 0x5D,  // Applications key (Natural keyboard)
   //   KEY_SLEEP            = 0x5F,  // Computer Sleep key
   //   KEY_NUMPAD0          = 0x60,  // Numeric keypad 0 key
   //   KEY_NUMPAD1          = 0x61,  // Numeric keypad 1 key
   //   KEY_NUMPAD2          = 0x62,  // Numeric keypad 2 key
   //   KEY_NUMPAD3          = 0x63,  // Numeric keypad 3 key
   //   KEY_NUMPAD4          = 0x64,  // Numeric keypad 4 key
   //   KEY_NUMPAD5          = 0x65,  // Numeric keypad 5 key
   //   KEY_NUMPAD6          = 0x66,  // Numeric keypad 6 key
   //   KEY_NUMPAD7          = 0x67,  // Numeric keypad 7 key
   //   KEY_NUMPAD8          = 0x68,  // Numeric keypad 8 key
   //   KEY_NUMPAD9          = 0x69,  // Numeric keypad 9 key
   //   KEY_MULTIPLY         = 0x6A,  // Multiply key
   //   KEY_ADD              = 0x6B,  // Add key
   //   KEY_SEPARATOR        = 0x6C,  // Separator key
   //   KEY_SUBTRACT         = 0x6D,  // Subtract key
   //   KEY_DECIMAL          = 0x6E,  // Decimal key
   //   KEY_DIVIDE           = 0x6F,  // Divide key
   //   KEY_F1               = 0x70,  // F1 key
   //   KEY_F2               = 0x71,  // F2 key
   //   KEY_F3               = 0x72,  // F3 key
   //   KEY_F4               = 0x73,  // F4 key
   //   KEY_F5               = 0x74,  // F5 key
   //   KEY_F6               = 0x75,  // F6 key
   //   KEY_F7               = 0x76,  // F7 key
   //   KEY_F8               = 0x77,  // F8 key
   //   KEY_F9               = 0x78,  // F9 key
   //   KEY_F10              = 0x79,  // F10 key
   //   KEY_F11              = 0x7A,  // F11 key
   //   KEY_F12              = 0x7B,  // F12 key
   //   KEY_F13              = 0x7C,  // F13 key
   //   KEY_F14              = 0x7D,  // F14 key
   //   KEY_F15              = 0x7E,  // F15 key
   //   KEY_F16              = 0x7F,  // F16 key
   //   KEY_F17              = 0x80,  // F17 key
   //   KEY_F18              = 0x81,  // F18 key
   //   KEY_F19              = 0x82,  // F19 key
   //   KEY_F20              = 0x83,  // F20 key
   //   KEY_F21              = 0x84,  // F21 key
   //   KEY_F22              = 0x85,  // F22 key
   //   KEY_F23              = 0x86,  // F23 key
   //   KEY_F24              = 0x87,  // F24 key
   //   KEY_NUMLOCK          = 0x90,  // NUM LOCK key
   //   KEY_SCROLL           = 0x91,  // SCROLL LOCK key
   //   KEY_LSHIFT           = 0xA0,  // Left SHIFT key
   //   KEY_RSHIFT           = 0xA1,  // Right SHIFT key
   //   KEY_LCONTROL         = 0xA2,  // Left CONTROL key
   //   KEY_RCONTROL         = 0xA3,  // Right CONTROL key
   //   KEY_LMENU            = 0xA4,  // Left MENU key
   //   KEY_RMENU            = 0xA5,  // Right MENU key
   //   KEY_OEM_1            = 0xBA,  // for US    ";:"
   //   KEY_PLUS             = 0xBB,  // Plus Key   "+"
   //   KEY_COMMA            = 0xBC,  // Comma Key  ","
   //   KEY_MINUS            = 0xBD,  // Minus Key  "-"
   //   KEY_PERIOD           = 0xBE,  // Period Key "."
   //   KEY_OEM_2            = 0xBF,  // for US    "/?"
   //   KEY_OEM_3            = 0xC0,  // for US    "`~"
   //   KEY_OEM_4            = 0xDB,  // for US    "[{"
   //   KEY_OEM_5            = 0xDC,  // for US    "\|"
   //   KEY_OEM_6            = 0xDD,  // for US    "]}"
   //   KEY_OEM_7            = 0xDE,  // for US    "'""
   //   KEY_OEM_8            = 0xDF,  // None
   //   KEY_OEM_AX           = 0xE1,  // for Japan "AX"
   //   KEY_OEM_102          = 0xE2,  // "<>" or "\|"
   //   KEY_ATTN             = 0xF6,  // Attn key
   //   KEY_CRSEL            = 0xF7,  // CrSel key
   //   KEY_EXSEL            = 0xF8,  // ExSel key
   //   KEY_EREOF            = 0xF9,  // Erase EOF key
   //   KEY_PLAY             = 0xFA,  // Play key
   //   KEY_ZOOM             = 0xFB,  // Zoom key
   //   KEY_PA1              = 0xFD,  // PA1 key
   //   KEY_OEM_CLEAR        = 0xFE,   // Clear key
   //
   //   KEY_KEY_CODES_COUNT  = 0xFF // this is not a key, but the amount of keycodes there are.
   //};
   //
   //enum EMOUSE_INPUT_EVENT
   //{
   //   //! Left mouse button was pressed down.
   //   EMIE_LMOUSE_PRESSED_DOWN = 0,
   //
   //   //! Right mouse button was pressed down.
   //   EMIE_RMOUSE_PRESSED_DOWN,
   //
   //   //! Middle mouse button was pressed down.
   //   EMIE_MMOUSE_PRESSED_DOWN,
   //
   //   //! Left mouse button was left up.
   //   EMIE_LMOUSE_LEFT_UP,
   //
   //   //! Right mouse button was left up.
   //   EMIE_RMOUSE_LEFT_UP,
   //
   //   //! Middle mouse button was left up.
   //   EMIE_MMOUSE_LEFT_UP,
   //
   //   //! The mouse cursor changed its position.
   //   EMIE_MOUSE_MOVED,
   //
   //   //! The mouse wheel was moved. Use Wheel value in event data to find out
   //   //! in what direction and how fast.
   //   EMIE_MOUSE_WHEEL,
   //
   //   //! Left mouse button double click.
   //   //! This event is generated after the second EMIE_LMOUSE_PRESSED_DOWN event.
   //   EMIE_LMOUSE_DOUBLE_CLICK,
   //
   //   //! Right mouse button double click.
   //   //! This event is generated after the second EMIE_RMOUSE_PRESSED_DOWN event.
   //   EMIE_RMOUSE_DOUBLE_CLICK,
   //
   //   //! Middle mouse button double click.
   //   //! This event is generated after the second EMIE_MMOUSE_PRESSED_DOWN event.
   //   EMIE_MMOUSE_DOUBLE_CLICK,
   //
   //   //! Left mouse button triple click.
   //   //! This event is generated after the third EMIE_LMOUSE_PRESSED_DOWN event.
   //   EMIE_LMOUSE_TRIPLE_CLICK,
   //
   //   //! Right mouse button triple click.
   //   //! This event is generated after the third EMIE_RMOUSE_PRESSED_DOWN event.
   //   EMIE_RMOUSE_TRIPLE_CLICK,
   //
   //   //! Middle mouse button triple click.
   //   //! This event is generated after the third EMIE_MMOUSE_PRESSED_DOWN event.
   //   EMIE_MMOUSE_TRIPLE_CLICK,
   //
   //   //! No real event. Just for convenience to get number of events
   //   EMIE_COUNT
   //};
   //
   ////! Masks for mouse button states
   //enum E_MOUSE_BUTTON_STATE_MASK
   //{
   //   EMBSM_LEFT    = 0x01,
   //   EMBSM_RIGHT   = 0x02,
   //   EMBSM_MIDDLE  = 0x04,
   //
   //   //! currently only on windows
   //   EMBSM_EXTRA1  = 0x08,
   //
   //   //! currently only on windows
   //   EMBSM_EXTRA2  = 0x10,
   //
   //   EMBSM_FORCE_32_BIT = 0x7fffffff
   //};
   //
   //enum EEVENT_TYPE
   //{
   //   //! An event of the graphical user interface.
   //   /** GUI events are created by the GUI environment or the GUI elements in response
   //   to mouse or keyboard events. When a GUI element receives an event it will either
   //   process it and return true, or pass the event to its parent. If an event is not absorbed
   //   before it reaches the root element then it will then be passed to the user receiver. */
   //   EET_GUI_EVENT = 0,
   //
   //   //! A mouse input event.
   //   /** Mouse events are created by the device and passed to IrrlichtDevice::postEventFromUser
   //   in response to mouse input received from the operating system.
   //   Mouse events are first passed to the user receiver, then to the GUI environment and its elements,
   //   then finally the input receiving scene manager where it is passed to the active camera.
   //   */
   //   EET_MOUSE_INPUT_EVENT,
   //
   //   //! A key input event.
   //   /** Like mouse events, keyboard events are created by the device and passed to
   //   IrrlichtDevice::postEventFromUser. They take the same path as mouse events. */
   //   EET_KEY_INPUT_EVENT,
   //
   //   //! A joystick (joypad, gamepad) input event.
   //   /** Joystick events are created by polling all connected joysticks once per
   //   device run() and then passing the events to IrrlichtDevice::postEventFromUser.
   //   They take the same path as mouse events.
   //   Windows, SDL: Implemented.
   //   Linux: Implemented, with POV hat issues.
   //   MacOS / Other: Not yet implemented.
   //   */
   //   EET_JOYSTICK_INPUT_EVENT,
   //
   //   //! A log event
   //   /** Log events are only passed to the user receiver if there is one. If they are absorbed by the
   //   user receiver then no text will be sent to the console. */
   //   EET_LOG_TEXT_EVENT,
   //
   //   //! A user event with user data.
   //   /** This is not used by Irrlicht and can be used to send user
   //   specific data though the system. The Irrlicht 'window handle'
   //   can be obtained from IrrlichtDevice::getExposedVideoData()
   //   The usage and behavior depends on the operating system:
   //   Windows: send a WM_USER message to the Irrlicht Window; the
   //   wParam and lParam will be used to populate the
   //   UserData1 and UserData2 members of the SUserEvent.
   //   Linux: send a ClientMessage via XSendEvent to the Irrlicht
   //   Window; the data.l[0] and data.l[1] members will be
   //   casted to s32 and used as UserData1 and UserData2.
   //   MacOS: Not yet implemented
   //   */
   //   EET_USER_EVENT,
   //
   //   //! This enum is never used, it only forces the compiler to
   //   //! compile these enumeration values to 32 bit.
   //   EGUIET_FORCE_32_BIT = 0x7fffffff
   //
   //};
   //
   //
   //#include <list>
   //
   //using namespace std;
   //
   //struct SEnvMapper
   //{
   //   HWND hWnd;
   //   Win32Window* irrDev;
   //};
   //
   //struct SEvent
   //{
   //	//! Any kind of GUI event.
   //	//struct SGUIEvent
   //	//{
   //	//	//! IGUIElement who called the event
   //	//	gui::IGUIElement* Caller;
   //
   //	//	//! If the event has something to do with another element, it will be held here.
   //	//	gui::IGUIElement* Element;
   //
   //	//	//! Type of GUI Event
   //	//	gui::EGUI_EVENT_TYPE EventType;
   //
   //	//};
   //
   //	//! Any kind of mouse event.
   //	struct SMouseInput
   //	{
   //		//! X position of mouse cursor
   //		int32 X;
   //
   //		//! Y position of mouse cursor
   //		int32 Y;
   //
   //		//! mouse wheel delta, often 1.0 or -1.0, but can have other values < 0.f or > 0.f;
   //		/** Only valid if event was EMIE_MOUSE_WHEEL */
   //		float Wheel;
   //
   //		//! True if shift was also pressed
   //		bool Shift:1;
   //
   //		//! True if ctrl was also pressed
   //		bool Control:1;
   //
   //		//! A bitmap of button states. You can use isButtonPressed() to determine
   //		//! if a button is pressed or not.
   //		//! Currently only valid if the event was EMIE_MOUSE_MOVED
   //		uint32 ButtonStates;
   //
   //		//! Is the left button pressed down?
   //		bool isLeftPressed() const { return 0 != ( ButtonStates & EMBSM_LEFT ); }
   //
   //		//! Is the right button pressed down?
   //		bool isRightPressed() const { return 0 != ( ButtonStates & EMBSM_RIGHT ); }
   //
   //		//! Is the middle button pressed down?
   //		bool isMiddlePressed() const { return 0 != ( ButtonStates & EMBSM_MIDDLE ); }
   //
   //		//! Type of mouse event
   //		EMOUSE_INPUT_EVENT Event;
   //	};
   //
   //	//! Any kind of keyboard event.
   //	struct SKeyInput
   //	{
   //		//! Character corresponding to the key (0, if not a character)
   //		wchar_t Char;
   //
   //		//! Key which has been pressed or released
   //		EKEY_CODE Key;
   //
   //		//! If not true, then the key was left up
   //		bool PressedDown:1;
   //
   //		//! True if shift was also pressed
   //		bool Shift:1;
   //
   //		//! True if ctrl was also pressed
   //		bool Control:1;
   //	};
   //
   //	//! A joystick event.
   //	/** Unlike other events, joystick events represent the result of polling
   //	 * each connected joystick once per run() of the device. Joystick events will
   //	 * not be generated by default.  If joystick support is available for the
   //	 * active device, _IRR_COMPILE_WITH_JOYSTICK_EVENTS_ is defined, and
   //	 * @ref irr::IrrlichtDevice::activateJoysticks() has been called, an event of
   //	 * this type will be generated once per joystick per @ref IrrlichtDevice::run()
   //	 * regardless of whether the state of the joystick has actually changed. */
   //	struct SJoystickEvent
   //	{
   //		enum
   //		{
   //			NUMBER_OF_BUTTONS = 32,
   //
   //			AXIS_X = 0, // e.g. analog stick 1 left to right
   //			AXIS_Y,		// e.g. analog stick 1 top to bottom
   //			AXIS_Z,		// e.g. throttle, or analog 2 stick 2 left to right
   //			AXIS_R,		// e.g. rudder, or analog 2 stick 2 top to bottom
   //			AXIS_U,
   //			AXIS_V,
   //			NUMBER_OF_AXES
   //		};
   //
   //		/** A bitmap of button states.  You can use IsButtonPressed() to
   //		 ( check the state of each button from 0 to (NUMBER_OF_BUTTONS - 1) */
   //		uint32 ButtonStates;
   //
   //		/** For AXIS_X, AXIS_Y, AXIS_Z, AXIS_R, AXIS_U and AXIS_V
   //		 * Values are in the range -32768 to 32767, with 0 representing
   //		 * the center position.  You will receive the raw value from the
   //		 * joystick, and so will usually want to implement a dead zone around
   //		 * the center of the range. Axes not supported by this joystick will
   //		 * always have a value of 0. On Linux, POV hats are represented as axes,
   //		 * usually the last two active axis.
   //		 */
   //		int16 Axis[NUMBER_OF_AXES];
   //
   //		/** The POV represents the angle of the POV hat in degrees * 100,
   //		 * from 0 to 35,900.  A value of 65535 indicates that the POV hat
   //		 * is centered (or not present).
   //		 * This value is only supported on Windows.  On Linux, the POV hat
   //		 * will be sent as 2 axes instead. */
   //		uint16 POV;
   //
   //		//! The ID of the joystick which generated this event.
   //		/** This is an internal Irrlicht index; it does not map directly
   //		 * to any particular hardware joystick. */
   //		uint8 Joystick;
   //
   //		//! A helper function to check if a button is pressed.
   //		bool IsButtonPressed(uint32 button) const
   //		{
   //			if(button >= (uint32)NUMBER_OF_BUTTONS)
   //				return false;
   //
   //			return (ButtonStates & (1 << button)) ? true : false;
   //		}
   //	};
   //
   //
   //	//! Any kind of log event.
   //	struct SLogEvent
   //	{
   //		//! Pointer to text which has been logged
   //		const int8* Text;
   //
   //		//! Log level in which the text has been logged
   //		//ELOG_LEVEL Level;
   //	};
   //
   //	//! Any kind of user event.
   //	struct SUserEvent
   //	{
   //		int32 UserData1;
   //		int32 UserData2;
   //	};
   //
   //	EEVENT_TYPE EventType;
   //	union
   //	{
   //		//struct SGUIEvent GUIEvent;
   //		struct SMouseInput MouseInput;
   //		struct SKeyInput KeyInput;
   //		struct SJoystickEvent JoystickEvent;
   //		struct SLogEvent LogEvent;
   //		struct SUserEvent UserEvent;
   //	};
   //
   //};
   //
   //list<SEnvMapper> EnvMap;
   //
   //SEnvMapper* GetEnvMapperFromHWnd(HWND hWnd)
   //{
   //	list<SEnvMapper>::iterator it = EnvMap.begin();
   //	for (; it!= EnvMap.end(); ++it)
   //		if ((*it).hWnd == hWnd)
   //			return &(*it);
   //
   //	return 0;
   //}
   //
   //Win32Window* GetDeviceFromHWnd(HWND hWnd)
   //{
   //	list<SEnvMapper>::iterator it = EnvMap.begin();
   //	for (; it!= EnvMap.end(); ++it)
   //		if ((*it).hWnd == hWnd)
   //			return (*it).irrDev;
   //
   //	return 0;
   //}
   //
   ////bool Win32Window::PostEventFromUser(const SEvent& event)
   ////{
   ////	bool absorbed = false;
   //
   //	//if (UserReceiver)
   //	//	absorbed = UserReceiver->OnEvent(event);
   //
   //	//if (!absorbed && GUIEnvironment)
   //	//	absorbed = GUIEnvironment->postEventFromUser(event);
   //
   //	//scene::ISceneManager* inputReceiver = InputReceivingSceneManager;
   //	//if (!inputReceiver)
   //	//	inputReceiver = SceneManager;
   //
   //	//if (!absorbed && inputReceiver)
   //	//	absorbed = inputReceiver->postEventFromUser(event);
   //
   //	//_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
   //	//return absorbed;
   //   //return true;
   ////}
   //
   //LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
   //{
   //   Win32Window* dev = 0;
   //   SEvent _event;
   //
   //   static int32 ClickCount = 0;
   //	if (GetCapture() != hWnd && ClickCount > 0)
   //		ClickCount = 0;
   //
   //	struct messageMap
   //	{
   //		int32 group;
   //		UINT winMessage;
   //		int32 irrMessage;
   //	};
   //
   //	static messageMap mouseMap[] =
   //	{
   //		{0, WM_LBUTTONDOWN, EMIE_LMOUSE_PRESSED_DOWN},
   //		{1, WM_LBUTTONUP,   EMIE_LMOUSE_LEFT_UP},
   //		{0, WM_RBUTTONDOWN, EMIE_RMOUSE_PRESSED_DOWN},
   //		{1, WM_RBUTTONUP,   EMIE_RMOUSE_LEFT_UP},
   //		{0, WM_MBUTTONDOWN, EMIE_MMOUSE_PRESSED_DOWN},
   //		{1, WM_MBUTTONUP,   EMIE_MMOUSE_LEFT_UP},
   //		{2, WM_MOUSEMOVE,   EMIE_MOUSE_MOVED},
   //		{3, WM_MOUSEWHEEL,  EMIE_MOUSE_WHEEL},
   //		{-1, 0, 0}
   //	};
   //
   //	// handle grouped events
   //	messageMap * m = mouseMap;
   //	while ( m->group >=0 && m->winMessage != message )
   //		m += 1;
   //
   //	if ( m->group >= 0 )
   //	{
   //		if ( m->group == 0 )	// down
   //		{
   //			ClickCount++;
   //			SetCapture(hWnd);
   //		}
   //		else
   //		if ( m->group == 1 )	// up
   //		{
   //			ClickCount--;
   //			if (ClickCount<1)
   //			{
   //				ClickCount=0;
   //				ReleaseCapture();
   //			}
   //		}
   //
   //		_event.EventType = EET_MOUSE_INPUT_EVENT;
   //		_event.MouseInput.Event = (EMOUSE_INPUT_EVENT) m->irrMessage;
   //		_event.MouseInput.X = (short)LOWORD(lParam);
   //		_event.MouseInput.Y = (short)HIWORD(lParam);
   //		_event.MouseInput.Shift = ((LOWORD(wParam) & MK_SHIFT) != 0);
   //		_event.MouseInput.Control = ((LOWORD(wParam) & MK_CONTROL) != 0);
   //		// left and right mouse buttons
   //		_event.MouseInput.ButtonStates = wParam & ( MK_LBUTTON | MK_RBUTTON);
   //		// middle and extra buttons
   //		if (wParam & MK_MBUTTON)
   //			_event.MouseInput.ButtonStates |= EMBSM_MIDDLE;
   //#if(_WIN32_WINNT >= 0x0500)
   //		if (wParam & MK_XBUTTON1)
   //			_event.MouseInput.ButtonStates |= EMBSM_EXTRA1;
   //		if (wParam & MK_XBUTTON2)
   //			_event.MouseInput.ButtonStates |= EMBSM_EXTRA2;
   //#endif
   //		_event.MouseInput.Wheel = 0.f;
   //
   //		// wheel
   //		if ( m->group == 3 )
   //		{
   //			POINT p; // fixed by jox
   //			p.x = 0; p.y = 0;
   //			ClientToScreen(hWnd, &p);
   //			_event.MouseInput.X -= p.x;
   //			_event.MouseInput.Y -= p.y;
   //			_event.MouseInput.Wheel = ((float)((short)HIWORD(wParam))) / (float)WHEEL_DELTA;
   //		}
   //
   //		dev = GetDeviceFromHWnd(hWnd);
   //		if (dev)
   //		{
   //			//dev->PostEventFromUser(_event);
   //
   //			if ( _event.MouseInput.Event >= EMIE_LMOUSE_PRESSED_DOWN && _event.MouseInput.Event <= EMIE_MMOUSE_PRESSED_DOWN )
   //			{
   //				//uint32 clicks = dev->CheckSuccessiveClicks(_event.MouseInput.X, _event.MouseInput.Y, _event.MouseInput.Event);
   //				//if ( clicks == 2 )
   //				//{
   //				//	_event.MouseInput.Event = (EMOUSE_INPUT_EVENT)(EMIE_LMOUSE_DOUBLE_CLICK + _event.MouseInput.Event-EMIE_LMOUSE_PRESSED_DOWN);
   //				//	dev->PostEventFromUser(_event);
   //				//}
   //				//else if ( clicks == 3 )
   //				//{
   //				//	_event.MouseInput.Event = (EMOUSE_INPUT_EVENT)(EMIE_LMOUSE_TRIPLE_CLICK + _event.MouseInput.Event-EMIE_LMOUSE_PRESSED_DOWN);
   //				//	dev->PostEventFromUser(_event);
   //				//}
   //			}
   //		}
   //		return 0;
   //	}
   //
   //   switch (message)
   //   {
   //	case WM_PAINT:
   //		{
   //			PAINTSTRUCT ps;
   //			BeginPaint(hWnd, &ps);
   //			EndPaint(hWnd, &ps);
   //		}
   //      return 0;
   //	case WM_DESTROY:
   //		PostQuitMessage(0);
   //		return 0;
   //	case WM_SYSCOMMAND:
   //		// prevent screensaver or monitor powersave mode from starting
   //		if ((wParam & 0xFFF0) == SC_SCREENSAVE ||
   //			(wParam & 0xFFF0) == SC_MONITORPOWER ||
   //			(wParam & 0xFFF0) == SC_KEYMENU)
   //			   return 0;
   //		break;
   //	case WM_ACTIVATE:
   //		// we need to take care for screen changes, e.g. Alt-Tab
   //		dev = GetDeviceFromHWnd(hWnd);
   //		if (dev /*&& dev->bIsFullscreen()*/)
   //		{
   //			if ((wParam&0xFF)==WA_INACTIVE)
   //			{
   //				// If losing focus we minimize the app to show other one
   //				ShowWindow(hWnd,SW_MINIMIZE);
   //				// and switch back to default resolution
   //				//dev->switchToFullScreen(true);
   //			}
   //			else
   //			{
   //				// Otherwise we retore the fullscreen Irrlicht app
   //				SetForegroundWindow(hWnd);
   //				ShowWindow(hWnd, SW_RESTORE);
   //				// and set the fullscreen resolution again
   //				//dev->switchToFullScreen();
   //			}
   //		}
   //		break;
   //   }
   //
   //   return DefWindowProc(hWnd, message, wParam, lParam);
   //}
   //
   //Win32Window::Win32Window() 
   //   : hWnd(NULL),
   //   hParentWnd(NULL),
   //   hMenu(NULL),
   //   isFullScreen(false)
   //{
   //}
   //
   //bool Win32Window::CreateDriver()
   //{
   //   //VideoDriver = video::createOpenGLDriver(CreationParams, FileSystem, this);
   //   return true;
   //}
   //
   //bool Win32Window::RegisterWindowClass()
   //{
   //   return true;
   //}
   //
   //void Win32Window::UnregisterWindowClass()
   //{
   //	WNDCLASSEX wcex;
   //	if (GetClassInfoEx(GetModuleHandle(NULL), CLASS_NAME, &wcex))
   //		UnregisterClass(CLASS_NAME, GetModuleHandle(NULL));
   //}
   //
   //void Win32Window::Create(HINSTANCE hInstance)
   //{
   //   hInstance = GetModuleHandle(NULL);
   //
   //   WNDCLASSEX wcex;
   //   wcex.cbSize			= sizeof(WNDCLASSEX);
   //   wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
   //   wcex.lpfnWndProc	= WndProc;
   //   wcex.cbClsExtra	= 0;
   //   wcex.cbWndExtra	= 0;
   //   wcex.hInstance		= hInstance;
   //   wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);;
   //   wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
   //   wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
   //   wcex.lpszMenuName	= 0;
   //   wcex.lpszClassName = CLASS_NAME;
   //   wcex.hIconSm		= 0;
   //   wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   //      //(HICON)LoadImage(hInstance, __TEXT("irrlicht.ico"), IMAGE_ICON, 0,0, LR_LOADFROMFILE);
   //
   //   if (!RegisterClassEx(&wcex))
   //   {
   //      // assertISV
   //      //return false;
   //   }
   //
   //   RECT clientSize;
   //   clientSize.top = 0;
   //   clientSize.left = 0;
   //   clientSize.right = dimension.x;
   //   clientSize.bottom = dimension.y;
   //
   //   DWORD style = WS_POPUP;
   //
   //   if (!isFullScreen)
   //      style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
   //
   //   AdjustWindowRect(&clientSize, style, FALSE);
   //
   //   const int32 realWidth = clientSize.right - clientSize.left;
   //   const int32 realHeight = clientSize.bottom - clientSize.top;
   //
   //   int32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
   //   int32 windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;
   //
   //   if ( windowLeft < 0 )
   //      windowLeft = 0;
   //   if ( windowTop < 0 )
   //      windowTop = 0;	// make sure window menus are in screen on creation
   //
   //   //todo: CreateWindowEx
   //   hWnd = CreateWindow( CLASS_NAME, __TEXT(""), style, windowLeft, windowTop,
   //					realWidth, realHeight, NULL, NULL, hInstance, NULL);
   //	//CreationParams.WindowId = HWnd;
   //   ShowWindow(hWnd, SW_SHOWNORMAL);
   //   UpdateWindow(hWnd);
   //
   //   resized = true;
   //
   //   //cursor->SetCursor(hWnd, dimension, isFullScreen);
   //
   //   if (!externalWindow)
   //	{
   //		SetActiveWindow(hWnd);
   //		SetForegroundWindow(hWnd);
   //	}
   //
   //   if (!CreateDriver())
   //   {
   //   }
   //}
   //

   //
   //   if(displayWindow)
   //      ShowWindow(GetWindowHandle(), SW_SHOWNORMAL);
   //
   //   // Clear the menu bar from the window for full screen
   //   HMENU menu = GetMenu(GetWindowHandle());
   //   if(menu)
   //   {
   //      SetMenu(GetWindowHandle(), NULL);
   //   }
   //
   //	return false;
   //}
   //
   //bool Win32Window::SetCaption( const std::string &caption )
   //{
   //   return (bool)SetWindowTextA( hWnd, caption.CString() );
   //}
   //
   //void Win32Window::SetFocus()
   //{
   //   ::SetFocus( hWnd );
   //}
   //
   //std::string &Win32Window::GetCaption() const
   //{
   //	char buffer[512];
   //	int32 strLen = GetWindowTextA(hWnd, buffer, 512);
   //
   //	if(strLen==0)
   //		return std::string("");
   //
   //   return std::string(buffer);
   //}
   //
   //bool Win32Window::IsActive() const
   //{
   //   return true;
   //}
   //
   //bool Win32Window::IsFocused() const
   //{
   //   return true;
   //}
   //
   //bool Win32Window::IsMinimized() const
   //{
   //   return true;
   //}
   //
   //void Win32Window::CloseDevice()
   //{
   //}
   //
   //void Win32Window::OnResized()
   //{
   //   resized = true;
   //}
   //
   //void Win32Window::SetResizable( const bool resizable )
   //{
   //   	//if (externalWindow || !GetVideoDriver() || isFullScreen)
   //		return;
   //
   //	LONG_PTR style = WS_POPUP;
   //
   //	if (!resizable)
   //		style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
   //	else
   //		style = WS_THICKFRAME | WS_SYSMENU | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
   //
   //	//if (!SetWindowLongPtr(hWnd, GWL_STYLE, style))
   //		//os::Printer::log("Could not change window style.");
   //
   //	RECT clientSize;
   //	clientSize.top = 0;
   //	clientSize.left = 0;
   //	//clientSize.right = GetVideoDriver()->getScreenSize().Width;
   //	//clientSize.bottom = GetVideoDriver()->getScreenSize().Height;
   //
   //	AdjustWindowRect(&clientSize, style, FALSE);
   //
   //	const int32 realWidth = clientSize.right - clientSize.left;
   //	const int32 realHeight = clientSize.bottom - clientSize.top;
   //
   //	const int32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
   //	const int32 windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;
   //
   //	SetWindowPos(hWnd, HWND_TOP, windowLeft, windowTop, realWidth, realHeight,
   //		SWP_FRAMECHANGED | SWP_NOMOVE | SWP_SHOWWINDOW);
   //
   //	//static_cast<Win32Mouse*>(Win32Mouse)->updateBorderSize(FullScreen, resizable);
   //}
   //
   //void Win32Window::Minimize()
   //{
   //}
   //
   //void Win32Window::Maximize()
   //{
   //}
   //
   //void Win32Window::RestoreWindowSize()
   //{
   //}
   //
   //void Win32Window::ClearPendingSystemMessages()
   //{
   //}
   //
   //void Win32Window::SetClientExtent( const Dimension2u newExtent )
   //{
   //   Dimension2u oldExtent = GetClientExtent();
   //	if (oldExtent == newExtent)
   //		return;   
   //
   //	RECT rtClient;
   //	DWORD style, exStyle;
   //
   //	SetRect( &rtClient, 0, 0, newExtent.x, newExtent.y );
   //	style = GetWindowLong( hWnd, GWL_STYLE);
   //	exStyle = GetWindowLong( hWnd, GWL_EXSTYLE );
   //
   //	AdjustWindowRectEx( &rtClient, style, GetMenuHandle() != NULL, exStyle );
   //	if( style & WS_VSCROLL ) 
   //		rtClient.right += GetSystemMetrics( SM_CXVSCROLL );
   //	if( style & WS_HSCROLL ) 
   //		rtClient.bottom += GetSystemMetrics( SM_CYVSCROLL );
   //
   //	SetWindowPos( hWnd, NULL, 0, 0,
   //      rtClient.right - rtClient.left,
   //      rtClient.bottom - rtClient.top,
   //      SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
   //}
   //
   //Dimension2u Win32Window::GetClientExtent() const
   //{
   //   // Fetch Client Rect from Windows
   //	RECT clientRect;
   //	
   //   ::GetClientRect(hWnd, &clientRect);
   //
   //	// we don't care about origin as it's always 0, 0
   //	return Dimension2u(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
   //}
   //
   //bool Win32Window::ClearFullScreen()
   //{
   //   return true;
   //}
   //
   //bool Win32Window::IsFullScreen()
   //{
   //   return isFullScreen;
   //}
   //
   //Win32Mouse *Win32Window::GetWin32Cursor() const
   //{
   //   return cursor;
   //}
   //
   //void Win32Window::SetMenuHandle( const HMENU menuHandle )
   //{
   //   hMenu = menuHandle;
   //   if(!isFullScreen)
   //      SetMenu(hWnd, hMenu);
   //}
   //
   //bool Win32Window::Run()
   //{
   //   return true;
   //}
   //
   ////void Win32Window::Yield()
   ////{
   ////}
   //
   //void Win32Window::Sleep( const uint32 timeMs, const bool pauseTimer )
   //{
   //}

