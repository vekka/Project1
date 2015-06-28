#include <iostream>

//#include "bmp.hpp"
//#include "color.hpp"
//#include "dimension.hpp"
//#include "matrix3.hpp"
//#include "string.hpp"
//#include "win32main.hpp"

#include "model/importer.hpp"

#include "openal/oaldriver.hpp"
#include "gfx/oglbuffer.hpp"

#include "win32/win32console.hpp"
using win32console::Win32Console;

#include "model/ObjParser.hpp"
using namespace model::objparser;

#include "core/math/frustum.hpp"

#include "shader/TransPipeline.hpp"
using pipeline::Pipeline;

#include "core/math/camera.hpp"

using camera::FreeCamera;
using camera::FRUSTUM_ORTHOGRAPHIC;
using camera::FRUSTUM_PERSPECTIVE;

using namespace ogldriver;

#include "model\mesh2.hpp"
using mesh2::Face;
using mesh2::Mesh;

using core::math::Matrix4f;
//using win32window::funcptr_t;


HINSTANCE hInst;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   LPSTR lpCmdLine, int32 nCmdShow)
{
   uint32 vaoID = 0;
   uint32 vboIndicesID = 0;
   const scene::Scene *sc;
   importer::Importer importer;

   sc = importer.ReadFile("assets/testObjects/randomScene.obj");

   Win32Console debugConsole(100, 100, 3, 3);
   bool t = debugConsole.Create();

   t = debugConsole.SetRedirection(win32console::REDIR_STDOUT);

   Matrix4f mat = Matrix4f::IDENTITY * 3;
   Vector4f v(4, 5, 6, 7);
   printf("hello\n");

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

   int32 winHeight, winWidth;
   win.GetDimension(winWidth, winHeight);

   OGLDriver oglContext(hWnd, 800, 600, false);
   Vector4f clearColor(-1.0f, 0.0f, 0.0f, -1.0f);
   /*D3DDriver d3dDriver(hWnd, 30, 30, 800, 600, false);*/
   win.Show();
   win.Update();
   oglContext.SetClearColor(clearColor);
   oglContext.SetDepthTest(ZBUF_LESSEQUAL, 0.0f, 1.0f, 1.0f);
   oglContext.EnableCulling();

   GLSLShader shader;
   shader.Load(GL_VERTEX_SHADER, "source/shader/glsl/vertex/triangle.vert");
   shader.Load(GL_FRAGMENT_SHADER, "source/shader/glsl/fragment/triangle.frag");
   shader.CreateAndLink(std::cerr);

   shader.Use();
   //shader.AddAttribute("vColor");
   shader.AddAttribute("vVertex");
   shader.AddAttribute("vNormal");
   shader.AddUniform("P");
   shader.AddUniform("M");
   shader.AddUniform("V");
   shader.AddUniform("light.position");
   shader.AddUniform("light.color");
   shader.Unuse();

   Vector3f cameraPosition(1.0f, 2.0f, 3.0f);
   Vector3f cameraTarget(0.0f, 0.0f, -1.0f);
   Vector3f cameraUp(0.0f, 1.0f, 0.0f);

   FreeCamera camera( winWidth, winHeight,cameraPosition, cameraTarget,cameraUp );
   camera.InitProjection(FRUSTUM_PERSPECTIVE, -1, 1, 1, -1, 0.3f, 1000.0f);
   camera.SetupProjection(1.1693706f, 800.0f / 600.0f);

   Pipeline pipeline;
   pipeline.Scale(1, 1, 1);
   pipeline.SetWorldPos(0.0f, 3.0f, -10.0f);
   Matrix4f viewMatrix = Matrix4f::IDENTITY;
   pipeline.SetCamera(camera.GetPosition(), camera.GetTarget(), camera.GetUp());

   // light stuff, not in use yet
   Vector3f pos(0.333f, 0.0f, 0.3333f);
   Vector3f color(1.0f, 1.0f, 1.0f);

   shader.Use();
   shader.AddUniformData("P", &camera.GetProjectionMatrix(), oglshader::TYPE_FMAT4, 1, true);
   shader.AddUniformData("M", &pipeline.GetWorldTrans(), oglshader::TYPE_FMAT4, 1, true);
   shader.AddUniformData("V", &pipeline.GetViewTrans(), oglshader::TYPE_FMAT4, 1,true);

   shader.AddUniformData("light.position", pos.Ptr(), oglshader::TYPE_FVEC3, 1);
   shader.AddUniformData("light.color", color.Ptr(), oglshader::TYPE_FVEC3, 1);

   shader.Unuse();

   oglbuffer::generateBufferFromScene(sc, shader, vaoID, vboIndicesID);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 
   int32 numIndicesInScene = 0;
   for (uint32 i = 0; i < sc->m_numMeshes; i++)
      numIndicesInScene +=( sc->m_ppMeshes[i]->m_numFaces * 3);

   win.mouse.SetVisible(true);
   win.mouse.SetPosition(800/2, 600/2);
   while (1)
   {
      oglContext.ClearBuffers();
      if (win.GetResizeFlag())
      {
         //oglContext.SetViewportSize(win.GetDimension[0], win.GetDimension[1]);
         win.OnResize();
      }
      win.HandleSystemMessages(&msg);

      resized = win.GetResizeFlag();   
      if (msg.message == WM_QUIT)
         break;

      if (win.keyboard.KeyIsDown(win32keyboard::VKEY_ESCAPE))
         msg.message = WM_QUIT;
     
      glBindVertexArray(vaoID);    
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
      shader.Use();

      if (win.keyboard.KeyIsDown(win32keyboard::VKEY_W))
         camera.OnKeyboard(win32keyboard::VKEY_W, 0.1f);
      if (win.keyboard.KeyIsDown(win32keyboard::VKEY_S))
         camera.OnKeyboard(win32keyboard::VKEY_S, 0.1f);
      if (win.keyboard.KeyIsDown(win32keyboard::VKEY_A))
         camera.OnKeyboard(win32keyboard::VKEY_A, 0.1f);
      if (win.keyboard.KeyIsDown(win32keyboard::VKEY_D))
         camera.OnKeyboard(win32keyboard::VKEY_D, 0.1f);

      Point2i posClient;
      win.mouse.GetClientPosition(posClient.x, posClient.y);
      //camera.OnMouse(  posClient.x, posClient.y);

      pipeline.SetCamera(camera.GetPosition(), camera.GetTarget(), camera.GetUp());

      shader.AddUniformData("V", &pipeline.GetViewTrans(), oglshader::TYPE_FMAT4, 1, true);
      
      glDrawArrays(GL_TRIANGLES, 0, numIndicesInScene);
      shader.Unuse();   
      glBindVertexArray(0);  
      oglContext.SwapFrontAndBackBuffer();
   }

   importer.FreeScene(sc);
   shader.DeleteProgram();
   return msg.wParam;
}
