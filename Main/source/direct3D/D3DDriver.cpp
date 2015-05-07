#include "D3DDriver.hpp"



namespace d3ddriver
{

   const D3D_FEATURE_LEVEL D3DDriver::featureLevels[7] = {
      D3D_FEATURE_LEVEL_11_1,
         D3D_FEATURE_LEVEL_11_0,
         D3D_FEATURE_LEVEL_10_1,
         D3D_FEATURE_LEVEL_10_0,
         D3D_FEATURE_LEVEL_9_3,
         D3D_FEATURE_LEVEL_9_2,
         D3D_FEATURE_LEVEL_9_1
   };

   D3DDriver::D3DDriver(HWND hWnd, float viewportWidth, float viewportHeight, float screenWidth, float screenHeight, bool fullscreen)
   {
      m_viewportWidth = viewportWidth;
      m_viewportHeight = viewportHeight;
      m_hWnd = hWnd;
      m_fullscreen = fullscreen;

      // Create a swap chain structure
      ZeroMemory(&m_swapChainDesc, sizeof(m_swapChainDesc));
      m_swapChainDesc.BufferCount = 1;
      m_swapChainDesc.BufferDesc.Width = (UINT)screenWidth;
      m_swapChainDesc.BufferDesc.Height = (UINT)screenHeight;
      m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
      m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
      m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      m_swapChainDesc.OutputWindow = m_hWnd;
      m_swapChainDesc.SampleDesc.Count = 4;
      m_swapChainDesc.SampleDesc.Quality = 0;
      m_swapChainDesc.Windowed = m_fullscreen;
   }

   D3DDriver::~D3DDriver()
   {

   }

   bool D3DDriver::CreateRenderTargetView()
   {
      //ID3D11Texture2D *pBackBuffer;
      //if (FAILED(g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
      //   return false;
      //hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
      //pBackBuffer->Release();
      //if (FAILED(hr))
      //   return false;
      //g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
      return false;
   }


   bool D3DDriver::Init()
   {

      // needs to move out from the ctor
      if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, NULL,
         D3D11_SDK_VERSION, &m_swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext)))
      {
         return false;
      }

      // Initialize the viewport
      D3D11_VIEWPORT viewport;
      viewport.Width = (FLOAT)m_viewportWidth;
      viewport.Height = (FLOAT)m_viewportHeight;
      viewport.MinDepth = 0.0f;
      viewport.MaxDepth = 1.0f;
      viewport.TopLeftX = 0;
      viewport.TopLeftY = 0;
      m_deviceContext->RSSetViewports(1, &viewport);

      return true;
   }

   //test the thing by clearing buffer with a fancy color
   void TestRenderFunc()
   {
      ////
      //// Clear the backbuffer
      ////
      //float ClearColor[4] = { 0.0f, 0.125f, 0.6f, 1.0f }; // RGBA

      //g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
      //g_pSwapChain->Present(0, 0);
   }

} // namespace d3ddriver