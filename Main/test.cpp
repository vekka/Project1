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

void generateBufferFromScene( scene::Scene *sc);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   LPSTR lpCmdLine, int32 nCmdShow)
{
   FreeCamera camera( FRUSTUM_ORTHOGRAPHIC, -1.0f, 1.0f, -1.0f, 1.0f, 0.3f, 1000.0f );
   File file;
   const scene::Scene *sc;
   importer::Importer importer;

   sc = importer.ReadFile("assets/testObjects/monkey.obj" );


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

   float triangleData[] =
   { 0.0f, 0.0f, 0.0f,
   1.0f, 0.0f, 0.0f,
   0.0f, 1.0f, 0.0f };
   uint32 triangleIndices[] =
   { 0,1,2 };
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
  
   glGenBuffers(1, &vboVerticesID);
   glGenBuffers(1, &vboIndicesID);
   /////////////////////////////////////
   glGenVertexArrays(1, &vaoID);
   glBindVertexArray(vaoID);
   
   glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
   glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), (void*)triangleData, GL_STATIC_DRAW);
 
  
   glEnableVertexAttribArray(shader["vVertex"]);
   glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (const GLvoid*)0);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(uint32), (const void*)triangleIndices, GL_STATIC_DRAW);

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
    
      shader.Use();
      //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      shader.Unuse();
      
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


// http://www.lighthouse3d.com/cg-topics/code-samples/importing-3d-models-with-assimp/
//how to fill opengl vbo with assimp scene

void generateBufferFromScene( scene *sc)
{
   //vertex array and vertex buffer object IDs
   GLuint vaoID = 0;
   GLuint vboVerticesID;
   GLuint vboIndicesID;
   uint32 buffer;

   // For each mesh
   for (uint32 n = 0; n < sc->mNumMeshes; ++n)
   {
      const Mesh* mesh = sc->mMeshes[n];

      // create array with faces
      // have to convert from Assimp format to array
      unsigned int *faceArray;
      faceArray = (uint32 *)malloc(sizeof(uint32) * mesh->mNumFaces * 3);
      unsigned int faceIndex = 0;

      for (unsigned int t = 0; t < mesh->mNumFaces; ++t)
      {
         const aiFace* face = &mesh->mFaces[t];

         memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
         faceIndex += 3;
      }
      aMesh.numFaces = sc->mMeshes[n]->mNumFaces;

      // generate Vertex Array for mesh
      glGenVertexArrays(1, &(aMesh.vao));
      glBindVertexArray(aMesh.vao);

      // buffer for faces
      glGenBuffers(1, &buffer);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

      // buffer for vertex positions
      if (mesh->HasPositions()) {
         glGenBuffers(1, &buffer);
         glBindBuffer(GL_ARRAY_BUFFER, buffer);
         glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
         glEnableVertexAttribArray(vertexLoc);
         glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);
      }
      // buffer for vertex normals
      if (mesh->HasNormals()) {
         glGenBuffers(1, &buffer);
         glBindBuffer(GL_ARRAY_BUFFER, buffer);
         glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
         glEnableVertexAttribArray(normalLoc);
         glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0);
      }


      // unbind buffers
      glBindVertexArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      // create material uniform buffer
      aiMaterial *mtl = sc->mMaterials[mesh->mMaterialIndex];

      aiString texPath;   //contains filename of texture
      if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)){
         //bind texture
         unsigned int texId = textureIdMap[texPath.data];
         aMesh.texIndex = texId;
         aMat.texCount = 1;
      }
      else
         aMat.texCount = 0;

       
      myMeshes.push_back(aMesh);
   }
}