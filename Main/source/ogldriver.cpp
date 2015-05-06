#include "ogldriver.hpp"

#include "shader/glmaterialrenderer.hpp"
#include "shader/glshadermaterialrenderer.hpp"
//#include "gltexturing.hpp"

namespace ogldriver
{

   OGLDriver::OGLDriver()
   {
   }

   OGLDriver::OGLDriver(HWND hWnd, float viewportWidth, float viewportHeight, bool fullscreen)
   {
      m_hWnd = hWnd;
      m_viewPortWidth = viewportWidth;
      m_viewPortHeight = viewportHeight;
      m_fullscreen = fullscreen;

      CreateContext();
   }

   OGLDriver::~OGLDriver()
   {
      wglMakeCurrent(hDC, 0);
      wglDeleteContext(hRC);
      ReleaseDC(m_hWnd, hDC);
   }

   bool OGLDriver::CreateContext()
   {
      hDC = GetDC(m_hWnd);

      int32 nPixelFormat;
      PIXELFORMATDESCRIPTOR pfd;
      memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
      pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
      pfd.nVersion = 1;
      pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
      pfd.iPixelType = PFD_TYPE_RGBA;
      pfd.cColorBits = 32;
      pfd.cDepthBits = 24;
      pfd.cStencilBits = 8;
      pfd.iLayerType = PFD_MAIN_PLANE;

      nPixelFormat = ChoosePixelFormat(this->hDC, &pfd);
      if (nPixelFormat == 0)
         return false;

      if (!SetPixelFormat(this->hDC, nPixelFormat, &pfd))
         return false;

      HGLRC tempOpenglContext = wglCreateContext(hDC);
      wglMakeCurrent(hDC, tempOpenglContext);
      GLenum error = glewInit();
      if (error != GLEW_OK)
         return false;

      int attributes[] = {
         WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
         WGL_CONTEXT_MINOR_VERSION_ARB, 4,
         WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
         0
      };

      if (glewIsSupported("WGL_ARB_CREATE_CONTEXT") == 1)
      {
         hRC = __wglewCreateContextAttribsARB(hDC, NULL, attributes);
         wglMakeCurrent(NULL, NULL);
         wglDeleteContext(tempOpenglContext);
         wglMakeCurrent(hDC, hRC);
      }
      else
         hRC = tempOpenglContext;

      int glVersion[2] = { -1, -1 };

      glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
      glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

      return true;
   }

   void OGLDriver::SetClearColor()
   {
      glClearColor(0.0, 0.0f, 0.0f, 0.0f);
   }

   void OGLDriver::SetViewportSize(float viewportWidth, float viewportHeight)
   {
      RECT rect;
      int width, height;

      GetClientRect(m_hWnd, &rect);
      width = rect.right;
      height = rect.bottom;

      if (height == 0) {
         height = 1;
      }
      glViewport(0, 0, width, height);
   }

   void OGLDriver::GetViewportSize(float &viewportWidth, float &viewportHeight)
   {
      viewportWidth = m_viewPortWidth;
      viewportHeight = m_viewPortHeight;
   }

   void OGLDriver::EnableCulling() const
   {
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
      glFrontFace(GL_CCW);
   }

   void OGLDriver::SetDepthTest(const eZBuffer zBuffer, const double zNear, const double zFar, const double depth) const
   {
      switch (zBuffer)
      {
      case ZBUF_DISABLE:
         glDisable(GL_DEPTH_TEST);
         break;
      case ZBUF_LESSEQUAL:
         glEnable(GL_DEPTH_TEST);
         glDepthFunc(GL_LEQUAL);
         break;
      case ZBUF_EQUAL:
         glEnable(GL_DEPTH_TEST);
         glDepthFunc(GL_EQUAL);
         break;
      case ZBUF_NOTEQUAL:
         glEnable(GL_DEPTH_TEST);
         glDepthFunc(GL_NOTEQUAL);
         break;
      case ZBUF_GREATEREQUAL:
         glEnable(GL_DEPTH_TEST);
         glDepthFunc(GL_GEQUAL);
         break;
      case ZBUF_GREATER:
         glEnable(GL_DEPTH_TEST);
         glDepthFunc(GL_GREATER);
         break;
      case ZBUF_ALWAYS:
         glEnable(GL_DEPTH_TEST);
         glDepthFunc(GL_ALWAYS);
         break;
      }

      glDepthMask(GL_TRUE);

      glDepthRange(zNear, zFar);
      glClearDepth(depth);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   }

   void OGLDriver::ClearBuffers() const
   {
      glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   }

   bool OGLDriver::SwapFrontAndBackBuffer()
   {
      return SwapBuffers(hDC) != 0;
   }

} // namespace ogldriver