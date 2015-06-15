
#include "oglbuffer.hpp"

namespace oglbuffer
{
   void generateBufferFromScene(const scene::Scene *sc, GLSLShader &shader, uint32 &vaoID, uint32 &vboIndicesID)
   {
      //vertex array and vertex buffer object IDs
      vaoID = 0;
      uint32 vboVerticesID = 0;
      uint32 iboOffset = 0, vboOffset = 0;
      uint32 vertsSize, idxSize;
      sc->GetSceneByteSize(vertsSize, idxSize);

      glGenVertexArrays(1, &vaoID);
      glBindVertexArray(vaoID);
      glGenBuffers(1, &vboVerticesID);
      glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
      glBufferData(GL_ARRAY_BUFFER, vertsSize, NULL, GL_STATIC_DRAW);

      glGenBuffers(1, &vboIndicesID);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxSize, NULL, GL_STATIC_DRAW);
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

         // buffer for faces
        
         glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, iboOffset, sizeof(uint32) * mesh->m_numFaces * 3, faceArray);
         iboOffset += sizeof(uint32) * mesh->m_numFaces * 3;
         // buffer for vertex positions

         if (mesh->HasPositions())
         {
            glEnableVertexAttribArray(shader["vVertex"]);
            glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
            glBufferSubData(GL_ARRAY_BUFFER, vboOffset, sizeof(float) * 3 * mesh->m_numVertices, mesh->m_pVertices);
            vboOffset += sizeof(Vector3f) * mesh->m_numVertices ;
         }

         // buffer for vertex normals
         //if (mesh->HasNormals())
         //{       
         //   glEnableVertexAttribArray(shader["vNormal"]);
         //   glVertexAttribPointer(shader["vNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,(GLvoid*)12);
         //   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->m_numFaces, mesh->m_pNormals, GL_STATIC_DRAW);

         //}
         // unbind buffers
         
         free(faceArray);
      }
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
   }
}