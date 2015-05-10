#ifndef _D3DDRIVER_HPP_INCLUDED_
#define _D3DDRIVER_HPP_INCLUDED_

#include <windows.h>

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

#include "../core/BasicTypes.hpp"

namespace d3ddriver
{

   class D3DDriver
   {
   private:
      //HGLRC hRC;
      HDC m_hDC;
      HWND m_hWnd;
      IDXGISwapChain *m_swapChain; // the pointer to the swap chain interface
      DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
      ID3D11Device *m_device;  // the pointer to our Direct3D device interface
      ID3D11DeviceContext *m_deviceContext;
      float m_viewportWidth;
      float m_viewportHeight;
      float m_bitsPerPixel;
      bool m_fullscreen;

      static const D3D_FEATURE_LEVEL featureLevels[];

   public:
      D3DDriver(HWND hWnd, float viewportWidth, float viewportHeight, float screenWidth, float screenHeight, bool fullscreen = false);
      bool CreateRenderTargetView();
      bool Init();
      void SetViewportSize(float viewportWidth, float viewportHeight);
      void GetViewportSize(float &viewportWidth, float &viewportHeight);

      ~D3DDriver();
   };

} // namespace d3ddriver

#endif
