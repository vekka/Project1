#ifndef _D3DDRIVER_HPP_INCLUDED_
#define _D3DDRIVER_HPP_INCLUDED_

#include <Windows.h>
#include <dxgi.h>

class D3DDriver
{
private:
   //HGLRC hRC;
   HDC m_hDC;
   HWND m_hWnd;

   float m_viewportWidth;
   float m_viewportHeight;
   float m_bitsPerPixel;

public:
   D3DDriver(float viewportWidth, float viewportHeight);
   ~D3DDriver();
};


#endif
