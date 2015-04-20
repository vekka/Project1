#include <iostream>

//#include "bmp.hpp"
//#include "color.hpp"
//#include "dimension.hpp"
//#include "matrix3.hpp"
//#include "string.hpp"
//#include "win32main.hpp"
#include "shader/oglshader.hpp"

#include "win32/win32console.hpp"

using namespace shader;

#include "win32/win32console.hpp"
#include "gfx/vertexbuffer.hpp"

#include "gfx/vertexstructs.hpp"
using vertexstructs::VertexPC;

using win32window::Win32Window;

#include "model/objloader.hpp"

#include "win32/win32console.hpp"
#include "core/math/frustum.hpp"
#include "core/math/camera.hpp"
using namespace std;

HINSTANCE hInst;

using namespace core::math;
using namespace camera;
using namespace ogldriver;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   LPSTR lpCmdLine, int nCmdShow)
{

  FreeCamera camera( FRUSTUM_ORTHOGRAPHIC, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f );

   Win32Console debugConsole;
   debugConsole.Create(100, 50, 100, 50);
   bool resized = false;
   //The message class of the application
   MSG msg;
   // Initialize the instance of this application
   hInst = hInstance;
   // create and register the application

   // Create the window object
   Win32Window win;

   //create window
   win.Create(hInst, 20, 20, 800, 600);
   //Get window handle
   HWND hWnd = win.GetWindowHandle();
   //create rendering context for window
   OGLDriver oglContext(hWnd);
   win.Show();
   win.Update();
   oglContext.SetClearColor();
   oglContext.SetDepthTest(ZBUF_LESSEQUAL, 0.0f, 1.0f, 1.0f);
   //oglContext.EnableCulling();
  
   GLSLShader shader;
   shader.Load(GL_VERTEX_SHADER, "source/shader/glsl/vertex/triangle.vert");
   shader.Load(GL_FRAGMENT_SHADER, "source/shader/glsl/fragment/triangle.frag");
   shader.CreateAndLink();

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   shader.Use();
   shader.AddAttribute("vColor");
   shader.AddAttribute("vVertex");   
   shader.AddUniform("MVP");
   shader.Unuse();

   //vertex array and vertex buffer object IDs
   GLuint vaoID = 0;
   GLuint vboVerticesID;
   GLuint vboIndicesID;

   //VertexPC<float> vertices[3];
   uint32 indices[3];
   //Vertex vertices[3];
   VertexPC<float> vertices[3];

   //camera.SetupProjection(67.0f, 800.0f / 600.0f);

   vertices[0].position = Vector3f(0, 0, 0);
   vertices[0].color = Vector3f(1, 0, 0);

   vertices[1].position = Vector3f(1, 1, 0 );
   vertices[1].color = Vector3f(0, 1, 0);

   vertices[2].position = Vector3f(0, 1, 0);
   vertices[2].color = Vector3f(0, 0, 1);
   float mvpData[] = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      -0.3f, -0.5f, 0.0f, 1.0f
   };

   //you must activate shader program to give uniform variables data
   shader.Use();
   //shader.AddUniformData("MVP", &camera.GetProjectionMatrix(), TYPE_FMAT4, 1);
   shader.Unuse();

   indices[0] = 0;
   indices[1] = 1;
   indices[2] = 2;
  
   glGenBuffers(1, &vboVerticesID);
   glGenBuffers(1, &vboIndicesID);
   
   ///////////////////////////////////////
   //glGenVertexArrays(1, &vaoID);
   //glBindVertexArray(vaoID);

   glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
   glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);
 
   //enable vertex attribute array for position
   glEnableVertexAttribArray(shader["vVertex"]);
   glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC<float>), (const GLvoid*)offsetof(VertexPC<float>, position));

   //enable vertex attribute array for colour
   glEnableVertexAttribArray(shader["vColor"]);
   glVertexAttribPointer(shader["vColor"], 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC<float>), (const GLvoid*)offsetof(VertexPC<float>, color));

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   //glBindVertexArray(0);
  // Process the messages
   while( 1 )
   {
      if( win.GetResizeFlag() )
      {
         oglContext.SetViewportSize();
         win.OnResize();
      }
      win.HandleSystemMessages(&msg);
      //resized = win.GetResizeFlag();
      if( msg.message == WM_QUIT )
         break;

      if (win.keyboard.KeyIsDown(win32keyboard::VKEY_K) && win.keyboard.KeyIsDown(win32keyboard::VKEY_N) && win.keyboard.KeyIsDown(win32keyboard::VKEY_3))
         msg.message = WM_QUIT;
      /*if (GetAsyncKeyState('K') & 0x8000)
         msg.message = WM_QUIT;*/

      oglContext.ClearBuffers();
      shader.Use();
      //glBindVertexArray(vaoID);
      glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);

      glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (const GLvoid*)0);
      //glDrawArrays(GL_TRIANGLES, 0, 3);
      shader.Unuse();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    
      oglContext.SwapFrontAndBackBuffer();
   }

   //f.CopyToBuffer( buf );
	//while( !win.HandleSystemMessages(&msg) )
	//{
     
     /* TranslateMessage(&Msg);
      DispatchMessage(&Msg);*/
	//}
   return msg.wParam;
}