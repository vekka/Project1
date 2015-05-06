#include "D3DDriver.hpp"

#include <D3DCommon.h>
#include <D3D11.h>

namespace d3ddriver
{

   D3DDriver::D3DDriver(HWND hWnd, float viewportWidth, float viewportHeight)
   {
      m_viewportWidth = viewportWidth;
      m_viewportHeight = viewportHeight;
      m_hWnd = hWnd;

      // Create a swap chain structure
      DXGI_SWAP_CHAIN_DESC sd;
      ZeroMemory(&sd, sizeof(sd));
      sd.BufferCount = 1;
      sd.BufferDesc.Width = 640;
      sd.BufferDesc.Height = 480;
      sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      sd.BufferDesc.RefreshRate.Numerator = 60;
      sd.BufferDesc.RefreshRate.Denominator = 1;
      sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      sd.OutputWindow = m_hWnd;
      sd.SampleDesc.Count = 1;
      sd.SampleDesc.Quality = 0;
      sd.Windowed = TRUE;

      // needs to move out from the ctor
      if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, m_featureLevels, numFeatureLevels,
         D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, NULL, &g_pImmediateContext)))
      {
         return FALSE;
      }

      // Initialize the viewport
      D3D11_VIEWPORT vp;
      vp.Width = (FLOAT)m_viewportWidth;
      vp.Height = (FLOAT)m_viewportHeight;
      vp.MinDepth = 0.0f;
      vp.MaxDepth = 1.0f;
      vp.TopLeftX = 0;
      vp.TopLeftY = 0;
      g_pImmediateContext->RSSetViewports(1, &vp);
   }

   D3DDriver::~D3DDriver()
   {


   }

   bool D3DDriver::CreateRenderTargetView()
   {
      ID3D11Texture2D *pBackBuffer;
      if (FAILED(g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
         return false;
      hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
      pBackBuffer->Release();
      if (FAILED(hr))
         return false;
      g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
   }

   //test the thing by clearing buffer with a fancy color
   void TestRenderFunc()
   {
      //
      // Clear the backbuffer
      //
      float ClearColor[4] = { 0.0f, 0.125f, 0.6f, 1.0f }; // RGBA

      g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
      g_pSwapChain->Present(0, 0);
   }

} // namespace d3ddriver