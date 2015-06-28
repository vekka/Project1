#include <Windows.h>

//#include "core/string/string.hpp"
//using core::string::String_c;

#include <string>

#include "win32event.hpp"
using win32event::Win32EventQueue;

namespace win32keyboard
{

   enum eVirtualKey
   {
      VKEY_LBUTTON = VK_LBUTTON,
      VKEY_RBUTTON = VK_RBUTTON,
      VKEY_CANCEL = VK_CANCEL,
      VKEY_MBUTTON = VK_MBUTTON,
      VKEY_BACK = VK_BACK,
      VKEY_TAB = VK_TAB,
      VKEY_CLEAR = VK_CLEAR,
      VKEY_RETURN = VK_RETURN,
      VKEY_SHIFT = VK_SHIFT,
      VKEY_CONTROL = VK_CONTROL,
      VKEY_MENU = VK_MENU,
      VKEY_PAUSE = VK_PAUSE,
      VKEY_CAPITAL = VK_CAPITAL,
      VKEY_ESCAPE = VK_ESCAPE,
      VKEY_SPACE = VK_SPACE,
      VKEY_PRIOR = VK_PRIOR,
      VKEY_NEXT = VK_NEXT,
      VKEY_END = VK_END,
      VKEY_HOME = VK_HOME,
      VKEY_LEFT = VK_LEFT,
      VKEY_UP = VK_UP,
      VKEY_RIGHT = VK_RIGHT,
      VKEY_DOWN = VK_DOWN,
      VKEY_SELECT = VK_SELECT,
      VKEY_EXECUTE = VK_EXECUTE,
      VKEY_SNAPSHOT = VK_SNAPSHOT,
      VKEY_INSERT = VK_INSERT,
      VKEY_DELETE = VK_DELETE,
      VKEY_HELP = VK_HELP,
      VKEY_0 = '0',        // 0x30  VK_0   VK_0 thru VK_9 are the same as ASCII '0' thru '9' (// 0x30 - // 0x39)
      VKEY_1,               // 0x31  VK_1
      VKEY_2,               // 0x32  VK_2
      VKEY_3,               // 0x33  VK_3
      VKEY_4,               // 0x34  VK_4
      VKEY_5,               // 0x35  VK_5
      VKEY_6,               // 0x36  VK_6
      VKEY_7,               // 0x37  VK_7
      VKEY_8,               // 0x38  VK_8
      VKEY_9,               // 0x39  VK_9
      VKEY_A = 'A',         // 0x41  VK_A      VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (// 0x41 - // 0x5A)
      VKEY_B,               // 0x42  VK_B
      VKEY_C,               // 0x43  VK_C
      VKEY_D,               // 0x44  VK_D
      VKEY_E,               // 0x45  VK_E
      VKEY_F,               // 0x46  VK_F
      VKEY_G,               // 0x47  VK_G
      VKEY_H,               // 0x48  VK_H
      VKEY_I,               // 0x49  VK_I
      VKEY_J,               // 0x4A  VK_J
      VKEY_K,               // 0x4B  VK_K
      VKEY_L,               // 0x4C  VK_L
      VKEY_M,               // 0x4D  VK_M
      VKEY_N,               // 0x4E  VK_N
      VKEY_O,               // 0x4F  VK_O
      VKEY_P,               // 0x50  VK_P
      VKEY_Q,               // 0x51  VK_Q
      VKEY_R,               // 0x52  VK_R
      VKEY_S,               // 0x53  VK_S
      VKEY_T,               // 0x54  VK_T
      VKEY_U,               // 0x55  VK_U
      VKEY_V,               // 0x56  VK_V
      VKEY_W,               // 0x57  VK_W
      VKEY_X,               // 0x58  VK_X
      VKEY_Y,               // 0x59  VK_Y
      VKEY_Z,               // 0x5A  VK_Z
      VKEY_WIN_LWINDOW = VK_LWIN,
      VKEY_WIN_RWINDOW = VK_RWIN,
      VKEY_WIN_APPS = VK_APPS,
      VKEY_NUMPAD0 = VK_NUMPAD0,
      VKEY_NUMPAD1 = VK_NUMPAD1,
      VKEY_NUMPAD2 = VK_NUMPAD2,
      VKEY_NUMPAD3 = VK_NUMPAD3,
      VKEY_NUMPAD4 = VK_NUMPAD4,
      VKEY_NUMPAD5 = VK_NUMPAD5,
      VKEY_NUMPAD6 = VK_NUMPAD6,
      VKEY_NUMPAD7 = VK_NUMPAD7,
      VKEY_NUMPAD8 = VK_NUMPAD8,
      VKEY_NUMPAD9 = VK_NUMPAD9,
      VKEY_MULTIPLY = VK_MULTIPLY,
      VKEY_ADD = VK_ADD,
      VKEY_SEPARATOR = VK_SEPARATOR,
      VKEY_SUBTRACT = VK_SUBTRACT,
      VKEY_DECIMAL = VK_DECIMAL,
      VKEY_DIVIDE = VK_DIVIDE,
      VKEY_F1 = VK_F1,
      VKEY_F2 = VK_F2,
      VKEY_F3 = VK_F3,
      VKEY_F4 = VK_F4,
      VKEY_F5 = VK_F5,
      VKEY_F6 = VK_F6,
      VKEY_F7 = VK_F7,
      VKEY_F8 = VK_F8,
      VKEY_F9 = VK_F9,
      VKEY_F10 = VK_F10,
      VKEY_F11 = VK_F11,
      VKEY_F12 = VK_F12,
      VKEY_F13 = VK_F13,
      VKEY_F14 = VK_F14,
      VKEY_F15 = VK_F15,
      VKEY_F16 = VK_F16,
      VKEY_F17 = VK_F17,
      VKEY_F18 = VK_F18,
      VKEY_F19 = VK_F19,
      VKEY_F20 = VK_F20,
      VKEY_F21 = VK_F21,
      VKEY_F22 = VK_F22,
      VKEY_F23 = VK_F23,
      VKEY_F24 = VK_F24,
      VKEY_NUMLOCK = VK_NUMLOCK,
      VKEY_SCROLL = VK_SCROLL,
      VKEY_SEMICOLON = VK_OEM_1,
      VKEY_EQUALS = VK_OEM_PLUS,
      VKEY_COMMA = VK_OEM_COMMA,
      VKEY_MINUS = VK_OEM_MINUS,
      VKEY_PERIOD = VK_OEM_PERIOD,
      VKEY_SLASH = VK_OEM_2,
      VKEY_TILDE = VK_OEM_3,
      VKEY_LBRACKET = VK_OEM_4,
      VKEY_BACKSLASH = VK_OEM_5,
      VKEY_RBRACKET = VK_OEM_6,
      VKEY_APOSTROPHE = VK_OEM_7,
      VKEY_OEM_102 = VK_OEM_102
   };

   // class for key states

} // namespace win32keyboard

namespace win32window
{

   class Win32Window
   {
   private:
      static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
      static int32 m_centerX, m_centerY;

      static Win32EventQueue win32EventQueue;

   public:
      class Win32Keyboard
      {
         friend LRESULT CALLBACK win32window::Win32Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
         //friend bool win32window::Win32Window::SwitchToFullScreen();
         //public:
      private:
         static int32 Dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
         {
            switch (msg)
            {
            case WM_SYSKEYDOWN:
            case WM_KEYDOWN:

               keys[wParam] = TRUE;
               //*      _event.eventType = KEY_INPUT_EVENT;
               //      _event.keyInput.key = (KEY_CODE)wParam;*/
               return 0;

            case WM_SYSKEYUP:
            case WM_KEYUP:
               keys[wParam] = FALSE;
               //*      _event.eventType = KEY_INPUT_EVENT;
               //      _event.keyInput.key = (KEY_CODE)wParam;*/
               return 0;
            }
            return 0;
         }
         static bool keys[256];
      public:
         bool KeyIsUp(win32keyboard::eVirtualKey keyCode) { return !keys[keyCode]; }
         bool KeyIsDown(win32keyboard::eVirtualKey keyCode) { return keys[keyCode]; }
      };

      static Win32Keyboard keyboard;

      class Win32Mouse
      {
         friend LRESULT CALLBACK win32window::Win32Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
      private:
         static int32 m_xPos, m_yPos;

         int32 m_borderX, m_borderY;
         HWND m_hWnd;
         bool m_isVisible;
         HCURSOR m_hCursor;
         static bool m_doMouseMove;

         int32 m_oldButtonState;
         bool m_isActive;
         bool m_isInitialized;

      public:
         Win32Mouse();
         ~Win32Mouse();

         static int32 Dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
         {
            switch (msg)
            {
            case WM_MOUSEMOVE:
               //mouse.SetPosition();
               m_xPos = LOWORD(lParam);
               m_yPos = HIWORD(lParam);

               return 0;
            }
            return 0;
         }

         void Activate();

         void SetVisible(bool visible);
         bool IsVisible() const;
         void SetPosition(int32 x, int32 y);
         void SetCursor(const HWND hWnd, const uint32 winWidth, const uint32 winHeight, const bool isFullScreen);
         void WarpTo(int32 *newX, int32 *newY);

         void GetClientPosition(int32 &m_xPos, int32 &m_yPos) const;
         void GetScreenPosition(int32 &m_xPos, int32 &m_yPos) const;

         void GetRelativePosition(int32 &m_xPos, int32 &m_yPos) const;
         void OnResize(const int32 &sizeX, const int32 &sizeY);
         void UpdateBorderSize(const bool isFullScreen, const bool resizable);
      };

      static Win32Mouse mouse;

   protected:
      HWND m_hWnd;
      HMENU m_hMenu;
      HINSTANCE m_hInstance;
      bool m_fullscreen; // ogldriver or d3ddriver should psbly hav this option alone ? fullscreen need psbly anyway nne set in here. maybe also same for height/width/bpp.
      bool m_externalWindow;
      bool m_close;
      uint32 m_height;
      uint32 m_width;
      uint32 m_bitsPerPel;

      void RegisterWindowClass() const;
      void UnregisterWindowClass() const;

   public:
      Win32Window();
      Win32Window(const HINSTANCE hinstance,
         const int32 x = CW_USEDEFAULT, const int32 y = CW_USEDEFAULT,
         const uint32 width = 800, const uint32 height = 600,
         const uint32 bitsPerPel = 32, const DWORD dStyle = WS_OVERLAPPEDWINDOW,
         const HWND parentWnd = NULL);

      HWND Create(const HINSTANCE hinstance,
         const int32 x = CW_USEDEFAULT, const int32 y = CW_USEDEFAULT,
         const uint32 width = 800, const uint32 height = 600,
         const uint32 bitsPerPel = 32, const DWORD dStyle = WS_OVERLAPPEDWINDOW,
         const HWND parentWnd = NULL);

      virtual ~Win32Window();

      bool SwitchToFullScreen();

      HWND GetWindowHandle() const;
      HMENU GetMenuHandle() const;
      const std::string &GetCaption() const;

      bool IsFullScreen() const;

      HINSTANCE GetInstance() const;

      bool SetCaption(const std::string &);
      void GetCaption(std::string &) const;
      void SetPosition(const uint32 m_xPos, const uint32 m_yPos);
      void GetPosition(uint32 &) const;

      //void GetMousePos( int32 &m_xPos, int32 &m_yPos ) { return mousePos; }
      //void SetDimension( const Dimension2u dimension );
      void OnResize() const;
      bool Show() const;
      bool Update() const;
      bool Minimize() const;
      bool Maximize() const;
      bool Hide() const;
      bool Restore() const;
      bool Resize(const int32 width, const int32 height);
      bool GetResizeFlag() const;

      void GetDimension(int32 &width, int32 &height) const;

      static void WmMouseMove(HWND hwnd, WPARAM wp, LPARAM lp);

      static bool isResized;
      void HandleSystemMessages(MSG *);
      void ClearSystemMessages() const;
   };

   //#ifndef _WIN32_HPP_INCLUDED_
   //#define _WIN32_HPP_INCLUDED_
   //
   //#include <windows.h>
   //
   //#include "dimension.hpp"
   //#include "win32cursor.hpp"
   //
   //inline uint16 GetXParam( short lp )
   //{
   //   return uint16(LOWORD(lp));
   //}
   //
   //inline uint16 GetYParam( short lp )
   //{
   //   return uint16(HIWORD(lp));
   //}
   //
   //class Win32Window
   //{
   //private:
   //   HWND hWnd;
   //   HWND hParentWnd;
   //   HMENU hMenu;
   //   bool isFullScreen;
   //   bool changedToFullScreen;
   //   bool resized;
   //   bool externalWindow;
   //   bool displayWindow;

   //   DWORD windowStyle;
   //   DEVMODE desktopMode;
   //   Win32Mouse *cursor;
   //
   //   friend class Win32WindowManager;
   //
   //   bool CreateDriver();
   //   void HandleSystemMessages();
   //   std::string &GetWindowsVersion();
   //   void Resize();
   //public:
   //   Win32Window();
   //   bool RegisterWindowClass();
   //   void UnregisterWindowClass();
   //
   //   //----------- mutators
   //   virtual bool SetCaption( const std::string & );
   //   virtual void SetFocus();
   //   virtual void SetClientExtent( const Dimension2u newExtent );
   //   virtual void SetResizable( const bool resizable );
   //
   //   //----------- queries
   //   virtual bool IsActive() const;
   //   virtual bool IsFocused() const;
   //   virtual bool IsMinimized() const;
   //   virtual std::string &GetCaption() const;
   //   virtual Dimension2u GetClientExtent() const;
   //   void GetVideoModeList(); // TODO: list
   //
   //   virtual void Create( HINSTANCE );
   //   virtual void CloseDevice();
   //   void OnResized();
   //   //virtual void SetResizable(bool resize=false);
   //   virtual void Minimize();
   //   virtual void Maximize();
   //   virtual void RestoreWindowSize();
   //   virtual void ClearPendingSystemMessages();
   //
   //   bool SwitchToFullScreen( const bool reset = false );
   //   virtual bool ClearFullScreen();
   //   virtual bool IsFullScreen();
   //
   //   HWND &GetWindowHandle() { return hWnd; }
   //   HMENU &GetMenuHandle() { return hMenu; }
   //
   //   void SetMenuHandle( const HMENU menuHandle );
   //
   //   static void GetLastWin32Error();
   //
   //   Win32Mouse *GetWin32Cursor() const;
   //
   //   //virtual void setVideoMode( const GFXVideoMode &mode );
   //   //virtual const GFXVideoMode &GetVideoMode();
   //   //virtual ~Win32Window();
   //   virtual bool Run();
   //   //virtual void Yield( vosid );
   //   virtual void Sleep( const uint32 timeMs, const bool pauseTimer );
   //
   //   //bool PostEventFromUser(const SEvent& event);
   //};
   //
   ////HWND &Win32Window::GetHWND()
   ////{
   ////   return m_hWnd;
   ////}
   //
   //class Win32WindowManager
   //{
   //private:
   //   friend class Win32Window;
   //   void AddWindowToList( const Win32Window *window );
   //   void RemoveWindowFromList( const Win32Window *window );
   //};
   //
   //#endif

} // namespace win32window

