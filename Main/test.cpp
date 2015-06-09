#include <iostream>

//#include "bmp.hpp"
//#include "color.hpp"
//#include "dimension.hpp"
//#include "matrix3.hpp"
//#include "string.hpp"
//#include "win32main.hpp"

#include "model/importer.hpp"
#include "scene/scene.hpp"

#include "shader/oglshader.hpp"
#include "gfx/vertexbuffer.hpp"
#include "win32/win32console.hpp"
using win32console::Win32Console;

using namespace oglshader;


#include "win32/win32console.hpp"
#include "gfx/vertexbuffer.hpp"

#include "gfx/vertexstructs.hpp"
using vertexstructs::VertexPC;

#include "model/ObjParser.hpp"
using namespace model::objparser;

using win32window::Win32Window;

#include "win32/win32console.hpp"
#include "core/math/frustum.hpp"
#include "core/math/camera.hpp"
using namespace std;

//#include "direct3D/D3DDriver.hpp"
//using d3ddriver::D3DDriver;
HINSTANCE hInst;

using namespace core::math;
using camera::FreeCamera;
using camera::AbstractCamera;
using camera::FRUSTUM_ORTHOGRAPHIC;
using camera::FRUSTUM_PERSPECTIVE;
using vbo::VertexBuffer;

using namespace ogldriver;

#include "model\mesh2.hpp"
using mesh2::Mesh;

#include "source\model\mesh2.hpp"
//using mesh2;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   LPSTR lpCmdLine, int32 nCmdShow)
{
   FreeCamera camera( FRUSTUM_ORTHOGRAPHIC, -1.0f, 1.0f, -1.0f, 1.0f, 0.3f, 1000.0f );
   File file;
   const scene::Scene *sc;
   importer::Importer importer;

   sc = importer.ReadFile("assets/testObjects/monkey.obj", &file );
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

   //D3DDriver(HWND hWnd, float viewportWidth, float viewportHeight, float screenWidth, float screenHeight, bool fullscreen = false);

   OGLDriver oglContext(hWnd, 800, 600, false);
   Vector4f clearColor(0.0f, 0.0f, 0.0f, 1.0f);
   /*D3DDriver d3dDriver(hWnd, 30, 30, 800, 600, false);*/
   win.Show();
   win.Update();
   oglContext.SetClearColor(clearColor);
   oglContext.SetDepthTest(ZBUF_LESSEQUAL, 0.0f, 1.0f, 1.0f);
   //oglContext.EnableCulling();
 


   GLSLShader shader;
   shader.Load(GL_VERTEX_SHADER, "source/shader/glsl/vertex/triangle.vert");
   shader.Load(GL_FRAGMENT_SHADER, "source/shader/glsl/fragment/triangle.frag");
   shader.CreateAndLink();

   shader.Use();
   //shader.AddAttribute("vColor");
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
      2.4f,  0.0f, -5.0f, 1.0f
   };
   //you must activate shader program to give uniform variables data
   shader.Use();
   shader.AddUniformData("P", &camera.GetProjectionMatrix(), TYPE_FMAT4, 1);
   shader.AddUniformData("M", modelMatrix, TYPE_FMAT4, 1);
   shader.Unuse();
  
   //uint32 cubeIndices[2904], x = 0;

   //uint32 arrayPos = 0;
   //for (int32 face = 0; face < monkey->m_pCurrentMesh->m_faces.size(); face++)
   //{
   //   cubeIndices[arrayPos] = monkey->m_pCurrentMesh->m_faces[face]->m_pVertexIndices->operator[](0);
   //   cubeIndices[arrayPos + 1] = monkey->m_pCurrentMesh->m_faces[face]->m_pVertexIndices->operator[](1);
   //   cubeIndices[arrayPos + 2] = monkey->m_pCurrentMesh->m_faces[face]->m_pVertexIndices->operator[](2);
   //   arrayPos+=3;
   //}

   //float *test = &(monkey->m_pVertices[0][0]);
   //GLsizeiptr size = monkey->m_pVertices.size() * sizeof(Vector3f);

   //glGenBuffers(1, &vboVerticesID);
   //glGenBuffers(1, &vboIndicesID);
   ///////////////////////////////////////
   //glGenVertexArrays(1, &vaoID);
   //glBindVertexArray(vaoID);
   //
   //glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
   //glBufferData(GL_ARRAY_BUFFER, size, (void*)test, GL_STATIC_DRAW);
 
  
   //glEnableVertexAttribArray(shader["vVertex"]);
   //glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (const GLvoid*)0);

   //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
   //glBufferData(GL_ELEMENT_ARRAY_BUFFER, monkey->m_pCurrentMesh->m_faces.size() * 3 * sizeof(uint32), (const void*)monkey->m_pCurrentMesh->, GL_STATIC_DRAW);

  // Process the messages
   while (1)
   {
      oglContext.ClearBuffers();
      if (win.GetResizeFlag())
      {
         //oglContext.SetViewportSize(100.0f, 100.0f);
         //win.OnResize();
      }
      win.HandleSystemMessages(&msg);
      //resized = win.GetResizeFlag();
      if (msg.message == WM_QUIT)
         break;
      if (win.keyboard.KeyIsDown(win32keyboard::VKEY_ESCAPE))
         msg.message = WM_QUIT;
     
      glBindVertexArray(vaoID);
    
      //shader.Use();
      //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
      //glDrawArrays(GL_TRIANGLES, 0, 8);
      //shader.Unuse();
      
      glBindVertexArray(0);
     
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
