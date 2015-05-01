#include <iostream>

// test123

// test456

//#include "bmp.hpp"
//#include "color.hpp"
//#include "dimension.hpp"
//#include "matrix3.hpp"
//#include "string.hpp"
//#include "win32main.hpp"
#include "shader/oglshader.hpp"
#include "gfx/vertexbuffer.hpp"
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
using camera::FreeCamera;
using camera::AbstractCamera;
using camera::FRUSTUM_ORTHOGRAPHIC;
using camera::FRUSTUM_PERSPECTIVE;
using vbo::VertexBuffer;

using namespace ogldriver;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   LPSTR lpCmdLine, int nCmdShow)
{

   FreeCamera camera( FRUSTUM_ORTHOGRAPHIC, -1.0f, 1.0f, -1.0f, 1.0f, 0.3f, 1000.0f );

   //FreeCamera camera(FRUSTUM_PERSPECTIVE, -1.0f, 1.0f, 1.0f, -1.0f, 0.3f, 1000.0f);

   ObjFile cube;
   cube.Open("assets/testObjects/cubePNT.obj");

   cube.Read();
   cube.Close();
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
  
   VertexBuffer<float> buffer(cube.mesh.GetVertexFormat(), 3, USAGE_STATIC_READ, ACCESS_READ_ONLY,BBTARGET_ARRAY_BUFFER );

   GLSLShader shader;
   shader.Load(GL_VERTEX_SHADER, "source/shader/glsl/vertex/triangle.vert");
   shader.Load(GL_FRAGMENT_SHADER, "source/shader/glsl/fragment/triangle.frag");
   shader.CreateAndLink();

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   shader.Use();
   shader.AddAttribute("vColor");
   shader.AddAttribute("vVertex");   
   shader.AddUniform("P");
   shader.AddUniform("M");
   shader.Unuse();

   //vertex array and vertex buffer object IDs
   GLuint vaoID = 0;
   GLuint vboVerticesID;
   GLuint vboIndicesID;

   camera.SetupProjection(1.1693706f, 800.0f / 600.0f );
   //for positioning cube in 3-space
   float modelMatrix[] = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f,  0.0f, -5.0f, 1.0f
   };
   //you must activate shader program to give uniform variables data
   shader.Use();
   shader.AddUniformData("P", &camera.GetProjectionMatrix(), TYPE_FMAT4, 1);
   shader.AddUniformData("M", modelMatrix, TYPE_FMAT4, 1);
   shader.Unuse();
  
   glGenBuffers(1, &vboVerticesID);
   glGenBuffers(1, &vboIndicesID);
   
   ///////////////////////////////////////
   //glGenVertexArrays(1, &vaoID);
   //glBindVertexArray(vaoID);

   //glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
   //glBufferData(GL_ARRAY_BUFFER,sizeof(NULL), NULL, GL_STATIC_DRAW);
 
   ////enable vertex attribute array for position
   //glEnableVertexAttribArray(shader["vVertex"]);
   //glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC<float>), (const GLvoid*)offsetof(VertexPC<float>, position));

   ////enable vertex attribute array for colour
   //glEnableVertexAttribArray(shader["vColor"]);
   //glVertexAttribPointer(shader["vColor"], 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC<float>), (const GLvoid*)offsetof(VertexPC<float>, color));

   //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
   //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(NULL), NULL, GL_STATIC_DRAW);

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

      if (win.keyboard.KeyIsDown(win32keyboard::VKEY_ESCAPE) )
         msg.message = WM_QUIT;
      /*if (GetAsyncKeyState('K') & 0x8000)
         msg.message = WM_QUIT;*/

      oglContext.ClearBuffers();
      shader.Use();
      //glBindVertexArray(vaoID);
      glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);

      glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, (const GLvoid*)0);
      //glDrawArrays(GL_TRIANGLES, 0, 8);
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
