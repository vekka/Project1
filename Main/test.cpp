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
using mesh2::Face;

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

using core::math::Matrix4f;

void generateBufferFromScene(const scene::Scene *sc, GLSLShader &shader, uint32 &vaoID, uint32 &vboIndicesID);
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   LPSTR lpCmdLine, int32 nCmdShow)
{
   FreeCamera camera(FRUSTUM_ORTHOGRAPHIC, -1.0f, 1.0f, -1.0f, 1.0f, 0.3f, 1000.0f);
   uint32 vaoID = 0;
   uint32 vboIndicesID = 0;
   const scene::Scene *sc;
   importer::Importer importer;

   sc = importer.ReadFile("assets/testObjects/cow.obj");

   Win32Console debugConsole(100, 100, 3, 3);
   bool t = debugConsole.Create();

   t = debugConsole.SetRedirection(win32console::REDIR_STDOUT);

   Matrix4f mat = Matrix4f::IDENTITY * 3;
   Vector4f v(4, 5, 6, 7);
   printf("hello\n");
   std::cout << mat << std::endl;
   std::cout << v << std::endl;

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
   Vector4f clearColor(-1.0f, 0.0f, 0.0f, -5.0f);
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
   shader.AddUniform("light.position");
   shader.AddUniform("light.color");
   shader.Unuse();

   camera.SetupProjection(1.1693706f, 800.0f / 600.0f);
   Matrix4f modelMatrix = Matrix4f::IDENTITY;
   modelMatrix.SetTranslation(-1.0f, 0.0f, -7.0f);
   modelMatrix = modelMatrix.Transpose();
   modelMatrix.SetRotationRadians(0.24f);

   //you must activate shader program to give uniform variables data
   Vector3f pos(0.333f, 0.0f, 0.3333f);
   Vector3f color(1.0f, 1.0f, 1.0f);

   shader.Use();
   shader.AddUniformData("P", &camera.GetProjectionMatrix(), TYPE_FMAT4, 1);
   shader.AddUniformData("M", modelMatrix.Ptr(), TYPE_FMAT4, 1, false);
   shader.AddUniformData("light.position", pos.Ptr(), TYPE_FVEC3, 1);
   shader.AddUniformData("light.color", color.Ptr(), TYPE_FVEC3, 1);

   shader.Unuse();
 
   generateBufferFromScene(sc, shader, vaoID, vboIndicesID);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // Process the messages
   int32 numIndicesInScene = 0;
   for (int32 i = 0; i < sc->m_numMeshes; i++)
      numIndicesInScene +=( sc->m_ppMeshes[i]->m_numFaces * 3);
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
      //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
      shader.Use();
      //glDrawElements(GL_TRIANGLES, sc->m_ppMeshes[0]->m_numFaces * 3, GL_UNSIGNED_INT, 0);

      glDrawArrays(GL_TRIANGLES, 0, sc->m_ppMeshes[0]->m_numFaces*3);
      shader.Unuse();
      
      glBindVertexArray(0);
     
      oglContext.SwapFrontAndBackBuffer();
   }

   importer.FreeScene(sc);
   shader.DeleteProgram();
   return msg.wParam;
}


// http://www.lighthouse3d.com/cg-topics/code-samples/importing-3d-models-with-assimp/
//how to fill opengl vbo with assimp scene

void generateBufferFromScene(const scene::Scene *sc, GLSLShader &shader, uint32 &vaoID, uint32 &vboIndicesID )
{
   //vertex array and vertex buffer object IDs
   vaoID = 0;
   uint32 vboVerticesID = 0; 
   
   glGenVertexArrays(1, &vaoID);
   glGenBuffers(1, &vboVerticesID);
   glGenBuffers(1, &vboIndicesID);
   // For each mesh
   for (uint32 n = 0; n < sc->m_numMeshes; ++n)
   {
      const Mesh* mesh = sc->m_ppMeshes[n];

      // create array with faces
      // have to convert from Assimp format to array
      uint32 *faceArray;
      faceArray = (uint32 *)malloc(sizeof(uint32) * mesh->m_numFaces * 3);
      uint32 faceIndex = 0;

      for (uint32 t = 0; t < mesh->m_numFaces; ++t)
      {
         const Face* face = &mesh->m_pFaces[t];

         memcpy(&faceArray[faceIndex], face->m_pIndexArray, 3 * sizeof(uint32));
         faceIndex += 3;
      }
   
      // generate Vertex Array for mesh
     
      glBindVertexArray(vaoID);
      // buffer for faces
      
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * mesh->m_numFaces * 3, faceArray, GL_STATIC_DRAW);

      // buffer for vertex positions
      
      if (mesh->HasPositions())
      {
         
         glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
         glEnableVertexAttribArray(shader["vVertex"]);
         glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
         glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->m_numVertices, mesh->m_pVertices, GL_STATIC_DRAW);
        
      }
  
      // buffer for vertex normals
      if (mesh->HasNormals())
      {       
         glEnableVertexAttribArray(shader["vNormal"]);
         glVertexAttribPointer(shader["vNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,(GLvoid*)12);
         glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->m_numFaces, mesh->m_pNormals, GL_STATIC_DRAW);

      }
      // unbind buffers
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
      free(faceArray);
   }
}