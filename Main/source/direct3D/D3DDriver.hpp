#ifndef _D3DDRIVER_HPP_INCLUDED_
#define _D3DDRIVER_HPP_INCLUDED_

#include <Windows.h>
#include <dxgi.h>
class D3DDriver
{
private:
   //HGLRC hRC;
   HDC hDC;
   HWND hWnd;

   float viewportWidth;
   float viewportHeight;
   float bitsPerPixel;

public:
   D3DDriver(float viewportWidth, float viewportHeight);
   ~D3DDriver();



};


#endif