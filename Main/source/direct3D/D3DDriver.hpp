#ifndef _D3DDRIVER_HPP_INCLUDED_
#define _D3DDRIVER_HPP_INCLUDED_

#include <Windows.h>
#include <dxgi.h>

namespace d3ddriver
{

   class D3DDriver
   {
   private:
      //HGLRC hRC;
      HDC m_hDC;
      HWND m_hWnd;

      float m_viewportWidth;
      float m_viewportHeight;
      float m_bitsPerPixel;

      D3D_FEATURE_LEVEL featureLevels[] =
      {
         D3D_FEATURE_LEVEL_11_1,
         D3D_FEATURE_LEVEL_11_0,
         D3D_FEATURE_LEVEL_10_1,
         D3D_FEATURE_LEVEL_10_0,
         D3D_FEATURE_LEVEL_9_3,
         D3D_FEATURE_LEVEL_9_2,
         D3D_FEATURE_LEVEL_9_1
      };

   public:
      D3DDriver(HWND hWnd, float viewportWidth, float viewportHeight, bool fullscreen = false);
      bool CreateRenderTargetView();

      void SetViewportSize(float viewportWidth, float viewportHeight);
      void GetViewportSize(float &viewportWidth, float &viewportHeight);

      ~D3DDriver();
   };

} // namespace d3ddriver

#endif
