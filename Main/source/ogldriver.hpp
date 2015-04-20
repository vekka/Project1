#ifndef _OGLDRIVER_HPP_INCLUDED_
#define _OGLDRIVER_HPP_INCLUDED_

#define GLEW_STATIC
#include <glew.h>
#include <wglew.h>
#include <wglext.h>

#include "win32/win32main.hpp"
using win32window::Win32Window;

namespace ogldriver
{

   enum eZBuffer
   {
      ZBUF_DISABLE,
      ZBUF_LESSEQUAL,
      ZBUF_EQUAL,
      ZBUF_LESS,
      ZBUF_NOTEQUAL,
      ZBUF_GREATEREQUAL,
      ZBUF_GREATER,
      ZBUF_ALWAYS
   };

   class OGLDriver
   {
   private:
      HGLRC hRC;
      HDC hDC;
      HWND hWnd;
      //Win32Window *window;
   public:
      OGLDriver();
      OGLDriver(const HWND hWnd);
      ~OGLDriver();
      bool CreateContext();
      void SetClearColor();
      void SetViewportSize();
      void ClearBuffers() const;
      bool SwapFrontAndBackBuffer();
      void EnableCulling() const;
      void SetDepthTest(const eZBuffer zBuffer, const double zNnear, const double zFar, const double depth) const;
      bool ChangeRenderContext( /*const SExposedVideoData& videoData,*/ Win32Window* win);
   };

} // namespace ogldriver

#endif