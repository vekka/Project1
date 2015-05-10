/*
---------------------------------------------------------------------------
Open Asset Import Library (assimp)
---------------------------------------------------------------------------

Copyright (c) 2006-2012, assimp team

All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the following
conditions are met:

* Redistributions of source code must retain the above
copyright notice, this list of conditions and the
following disclaimer.

* Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the
following disclaimer in the documentation and/or other
materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
contributors may be used to endorse or promote products
derived from this software without specific prior
written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------
*/

//#include "AssimpPCH.h"
#ifndef ASSIMP_BUILD_NO_OBJ_IMPORTER

#include "OBJParser.hpp"

using mesh2::aiPrimitiveType_LINE;
using mesh2::aiPrimitiveType_POINT;

using core::SkipToken;

//#include "ObjFileMtlImporter.h"
//#include "ObjTools.h"

#include "OBJTools.hpp"
using objtools::SkipLine;
using objtools::GetNextWord;

#include "core/fast_atof.hpp"
using Assimp::fast_atof;

#include "OBJFile.hpp"

#include "OBJMTLImporter.hpp"
using model::objmtlimporter::ObjMtlImporter;

#include "OBJTools.hpp"
using objtools::GetName;
using objtools::IsEndOfBuffer;
using objtools::CopyNextWord;

//#include "ParsingUtils.h"
//#include "../include/assimp/types.h"
//#include "DefaultIOSystem.h"


#include <algorithm>
using std::fill_n;

namespace model
{

   namespace objparser
   {

      const String_c ObjFileParser::DEFAULT_MATERIAL_NAME = "default_material_name";

      ObjFileParser::ObjFileParser(std::vector<char> &inData, const String_c &strModelName, File *file) :
         m_dataIterator(inData.begin()),
         m_dataIteratorEndOfBuffer(inData.end()),
         m_pModelInstance(NULL),
         m_currentLine(0),
         m_file(file)
      {
         fill_n(m_buffer, BUFFERSIZE, 0);

         // Create the model instance to store all the data
         m_pModelInstance = new objfile::Model();
         m_pModelInstance->m_ModelName = strModelName;
         
         // create default material and store it
         m_pModelInstance->m_pDefaultMaterial = new objfile::Material();
         m_pModelInstance->m_pDefaultMaterial->MaterialName = DEFAULT_MATERIAL_NAME;
         m_pModelInstance->m_MaterialLib.push_back(DEFAULT_MATERIAL_NAME);
         m_pModelInstance->m_MaterialMap[DEFAULT_MATERIAL_NAME] = m_pModelInstance->m_pDefaultMaterial;

         ParseFile();
      }

      ObjFileParser::~ObjFileParser()
      {
         delete m_pModelInstance;
         m_pModelInstance = NULL;
      }

      // -------------------------------------------------------------------
      //	Returns a pointer to the model instance.
      objfile::Model *ObjFileParser::GetModel() const
      {
         return m_pModelInstance;
      }

      void ObjFileParser::ParseFile()
      {
         if (m_dataIterator == m_dataIteratorEndOfBuffer)
            return;

         while (m_dataIterator != m_dataIteratorEndOfBuffer)
         {
            switch (*m_dataIterator)
            {
            case 'v': // Parse a vertex texture coordinate
            {
               ++m_dataIterator;
               if (*m_dataIterator == ' ' || *m_dataIterator == '\t') {
                  // read in vertex definition
                  GetVector3(m_pModelInstance->m_Vertices);
               }
               else if (*m_dataIterator == 't') {
                  // read in texture coordinate ( 2D or 3D )
                  m_dataIterator++;
                  GetVector(m_pModelInstance->m_TextureCoord);
               }
               else if (*m_dataIterator == 'n') {
                  // Read in normal vector definition
                  m_dataIterator++;
                  GetVector3(m_pModelInstance->m_Normals);
               }
            }
            break;

            case 'p': // Parse a face, line or point statement
            case 'l':
            case 'f':
            {
               GetFace(*m_dataIterator == 'f' ? aiPrimitiveType_POLYGON : (*m_dataIterator == 'l'
                  ? aiPrimitiveType_LINE : aiPrimitiveType_POINT));
            }
            break;

            case '#': // Parse a comment
            {
               GetComment();
            }
            break;

            case 'u': // Parse a material desc. setter
            {
               GetMaterialDesc();
            }
            break;

            case 'm': // Parse a material library or merging group ('mg')
            {
               if (*(m_dataIterator + 1) == 'g')
                  GetGroupNumberAndResolution();
               else
                  GetMaterialLib();
            }
            break;

            case 'g': // Parse group name
            {
               GetGroupName();
            }
            break;

            case 's': // Parse group number
            {
               GetGroupNumber();
            }
            break;

            case 'o': // Parse object name
            {
               GetObjectName();
            }
            break;

            default:
            {
               m_dataIterator = SkipLine<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_currentLine);
            }
            break;
            }
         }
      }

      void ObjFileParser::CopyNextWord(char *pBuffer, size_t length)
      {
         size_t index = 0;
         m_dataIterator = GetNextWord<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer);
         while (m_dataIterator != m_dataIteratorEndOfBuffer && !IsSpaceOrNewLine(*m_dataIterator)) {
            pBuffer[index] = *m_dataIterator;
            index++;
            if (index == length - 1) {
               break;
            }
            m_dataIterator++;
         }

         assert(index < length);
         pBuffer[index] = '\0';
      }

      void ObjFileParser::CopyNextLine(char *pBuffer, size_t length)
      {
         size_t index = 0u;

         // some OBJ files have line continuations using \ (such as in C++ et al)
         bool continuation = false;
         for (; m_dataIterator != m_dataIteratorEndOfBuffer && index < length - 1; m_dataIterator++)
         {
            const char c = *m_dataIterator;
            if (c == '\\') {
               continuation = true;
               continue;
            }

            if (c == '\n' || c == '\r') {
               if (continuation) {
                  pBuffer[index++] = ' ';
                  continue;
               }
               break;
            }

            continuation = false;
            pBuffer[index++] = c;
         }
         assert(index < length);
         pBuffer[index] = '\0';
      }

      void ObjFileParser::GetVector(std::vector<Vector3f> &point3d_array) {
         size_t numComponents(0);
         const char* tmp(&m_dataIterator[0]);
         while (!IsLineEnd(*tmp)) {
            if (!SkipSpaces(&tmp)) {
               break;
            }
            SkipToken(tmp);
            numComponents++;
         }
         float x, y, z;
         if (2 == numComponents) {
            CopyNextWord(m_buffer, BUFFERSIZE);
            x = (float)fast_atof(m_buffer);

            CopyNextWord(m_buffer, BUFFERSIZE);
            y = (float)fast_atof(m_buffer);
            z = 0.0;
         }
         else if (3 == numComponents) {
            CopyNextWord(m_buffer, BUFFERSIZE);
            x = (float)fast_atof(m_buffer);

            CopyNextWord(m_buffer, BUFFERSIZE);
            y = (float)fast_atof(m_buffer);

            CopyNextWord(m_buffer, BUFFERSIZE);
            z = (float)fast_atof(m_buffer);
         }
         else {
            assert(!"Invalid number of components");
         }
         point3d_array.push_back(Vector3f(x, y, z));
         m_dataIterator = SkipLine<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_currentLine);
      }

      void ObjFileParser::GetVector3(std::vector<Vector3f> &point3d_array) {
         float x, y, z;
         CopyNextWord(m_buffer, BUFFERSIZE);
         x = (float)fast_atof(m_buffer);

         CopyNextWord(m_buffer, BUFFERSIZE);
         y = (float)fast_atof(m_buffer);

         CopyNextWord(m_buffer, BUFFERSIZE);
         z = (float)fast_atof(m_buffer);

         point3d_array.push_back(Vector3f(x, y, z));
         m_dataIterator = SkipLine<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_currentLine);
      }

      void ObjFileParser::GetVector2(std::vector<Vector2f> &point2d_array) {
         float x, y;
         CopyNextWord(m_buffer, BUFFERSIZE);
         x = (float)fast_atof(m_buffer);

         CopyNextWord(m_buffer, BUFFERSIZE);
         y = (float)fast_atof(m_buffer);

         point2d_array.push_back(Vector2f(x, y));

         m_dataIterator = SkipLine<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_currentLine);
      }

      void ObjFileParser::GetFace(aiPrimitiveType type)
      {
         CopyNextLine(m_buffer, BUFFERSIZE);
         if (m_dataIterator == m_dataIteratorEndOfBuffer)
            return;

         char *pPtr = m_buffer;
         char *pEnd = &pPtr[BUFFERSIZE];
         pPtr = GetNextToken<char*>(pPtr, pEnd);
         if (pPtr == pEnd || *pPtr == '\0')
            return;

         std::vector<uint32> *pIndices = new std::vector < uint32 > ;
         std::vector<uint32> *pTexID = new std::vector < uint32 > ;
         std::vector<uint32> *pNormalID = new std::vector < uint32 > ;
         bool hasNormal = false;

         const int32 vSize = m_pModelInstance->m_Vertices.size();
         const int32 vtSize = m_pModelInstance->m_TextureCoord.size();
         const int32 vnSize = m_pModelInstance->m_Normals.size();

         const bool vt = (!m_pModelInstance->m_TextureCoord.empty());
         const bool vn = (!m_pModelInstance->m_Normals.empty());
         int32 iStep = 0, iPos = 0;
         while (pPtr != pEnd)
         {
            iStep = 1;

            if (IsLineEnd(*pPtr))
               break;

            if (*pPtr == '/')
            {
               if (type == aiPrimitiveType_POINT) {
                  //DefaultLogger::get()->error("Obj: Separator unexpected in point statement");
               }
               if (iPos == 0)
               {
                  //if there are no texture coordinates in the file, but normals
                  if (!vt && vn) {
                     iPos = 1;
                     iStep++;
                  }
               }
               iPos++;
            }
            else if (IsSpaceOrNewLine(*pPtr))
            {
               iPos = 0;
            }
            else
            {
               //OBJ USES 1 Base ARRAYS!!!!
               const int32 iVal = atoi(pPtr);

               // increment iStep position based off of the sign and # of digits
               int32 tmp = iVal;
               if (iVal < 0)
                  iStep++;
               while ((tmp = tmp / 10) != 0)
                  iStep++;

               if (iVal > 0)
               {
                  // Store parsed index
                  if (0 == iPos)
                  {
                     pIndices->push_back(iVal - 1);
                  }
                  else if (1 == iPos)
                  {
                     pTexID->push_back(iVal - 1);
                  }
                  else if (2 == iPos)
                  {
                     pNormalID->push_back(iVal - 1);
                     hasNormal = true;
                  }
                  else
                  {
                     ReportErrorTokenInFace();
                  }
               }
               else if (iVal < 0)
               {
                  // Store relatively index
                  if (0 == iPos)
                  {
                     pIndices->push_back(vSize + iVal);
                  }
                  else if (1 == iPos)
                  {
                     pTexID->push_back(vtSize + iVal);
                  }
                  else if (2 == iPos)
                  {
                     pNormalID->push_back(vnSize + iVal);
                     hasNormal = true;
                  }
                  else
                  {
                     ReportErrorTokenInFace();
                  }
               }
            }
            pPtr += iStep;
         }

         if (pIndices->empty())
         {
            //DefaultLogger::Get()->error("Obj: Ignoring empty face");
            m_dataIterator = SkipLine<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_currentLine);
            return;
         }

         objfile::Face *face = new objfile::Face(pIndices, pNormalID, pTexID, type);

         // Set active material, if one set
         if (NULL != m_pModelInstance->m_pCurrentMaterial)
            face->m_pMaterial = m_pModelInstance->m_pCurrentMaterial;
         else
            face->m_pMaterial = m_pModelInstance->m_pDefaultMaterial;

         // Create a default object, if nothing is there
         if (NULL == m_pModelInstance->m_pCurrent)
            CreateObject("defaultobject");

         // Assign face to mesh
         if (NULL == m_pModelInstance->m_pCurrentMesh)
         {
            CreateMesh();
         }

         // Store the face
         m_pModelInstance->m_pCurrentMesh->m_Faces.push_back(face);
         m_pModelInstance->m_pCurrentMesh->m_uiNumIndices += (uint32)face->m_pVertices->size();
         m_pModelInstance->m_pCurrentMesh->m_uiUVCoordinates[0] += (uint32)face->m_pTexturCoords[0].size();
         if (!m_pModelInstance->m_pCurrentMesh->m_hasNormals && hasNormal)
         {
            m_pModelInstance->m_pCurrentMesh->m_hasNormals = true;
         }
         // Skip the rest of the line
         m_dataIterator = SkipLine<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_currentLine);
      }

      //	Get values for a new material description
      void ObjFileParser::GetMaterialDesc()
      {
         // Each material request a new object.
         // Sometimes the object is already created (see 'o' tag by example), but it is not initialized !
         // So, we create a new object only if the current on is already initialized !
         if (m_pModelInstance->m_pCurrent != NULL &&
            (m_pModelInstance->m_pCurrent->m_Meshes.size() > 1 ||
            (m_pModelInstance->m_pCurrent->m_Meshes.size() == 1 && m_pModelInstance->m_Meshes[m_pModelInstance->m_pCurrent->m_Meshes[0]]->m_Faces.size() != 0))
            )
            m_pModelInstance->m_pCurrent = NULL;

         // Get next data for material data
         m_dataIterator = GetNextToken<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer);
         if (m_dataIterator == m_dataIteratorEndOfBuffer)
            return;

         char *pStart = &(*m_dataIterator);
         while (m_dataIterator != m_dataIteratorEndOfBuffer && !IsSpaceOrNewLine(*m_dataIterator)) {
            m_dataIterator++;
         }

         // Get name
         String_c strName(pStart, &(*m_dataIterator));
         if (strName.IsEmpty())
            return;

         // Search for material
         std::map<String_c, objfile::Material*>::iterator it = m_pModelInstance->m_MaterialMap.find(strName);
         if (it == m_pModelInstance->m_MaterialMap.end())
         {
            // Not found, use default material
            m_pModelInstance->m_pCurrentMaterial = m_pModelInstance->m_pDefaultMaterial;
            //DefaultLogger::get()->error("OBJ: failed to locate material " + strName + ", skipping");
         }
         else
         {
            // Found, using detected material
            m_pModelInstance->m_pCurrentMaterial = (*it).second;
            if (NeedsNewMesh(strName))
            {
               CreateMesh();
            }
            m_pModelInstance->m_pCurrentMesh->m_uiMaterialIndex = GetMaterialIndex(strName);
         }

         // Skip rest of line
         m_dataIterator = SkipLine<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_currentLine);
      }

      //	Get a comment, values will be skipped
      void ObjFileParser::GetComment()
      {
         while (m_dataIterator != m_dataIteratorEndOfBuffer)
         {
            if ('\n' == (*m_dataIterator))
            {
               ++m_dataIterator;
               break;
            }
            else
            {
               ++m_dataIterator;
            }
         }
      }

      void ObjFileParser::GetMaterialLib()
      {
         // Translate tuple
         m_dataIterator = GetNextToken<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer);
         if (m_dataIterator == m_dataIteratorEndOfBuffer) {
            return;
         }

         char *pStart = &(*m_dataIterator);
         while (m_dataIterator != m_dataIteratorEndOfBuffer && !IsLineEnd(*m_dataIterator)) {
            ++m_dataIterator;
         }

         // Check for existence
         const String_c strMatName(pStart, &(*m_dataIterator));
         //std::string strMatName(pStart, &(*m_dataIterator));

         //IOStream *pFile = m_pIO->Open(strMatName);
         File file;

         if (!file.Open(strMatName))
         {
            //DefaultLogger::get()->error("OBJ: Unable to locate material file " + strMatName);
            m_dataIterator = SkipLine<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_currentLine);
            return;
         }

         // Import material library data from file
         std::vector<char> buffer;

         file.CopyToBuffer(buffer);
         //BaseImporter::TextFileToBuffer(pFile, buffer);
         //m_pIO->Close(pFile);
         file.Close();

         // Importing the material library

         //ObjMtlImporter mtlImporter(buffer, strMatName, m_pModelInstance);
      }

      //	Set a new material definition as the current material.
      void ObjFileParser::GetNewMaterial()
      {
         m_dataIterator = GetNextToken<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer);
         m_dataIterator = GetNextToken<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer);
         if (m_dataIterator == m_dataIteratorEndOfBuffer) {
            return;
         }

         char *pStart = &(*m_dataIterator);
         String_c strMat(pStart, *m_dataIterator);
         while (m_dataIterator != m_dataIterator && IsSpaceOrNewLine(*m_dataIterator)) {
            ++m_dataIterator;
         }
         std::map<String_c, objfile::Material*>::iterator it = m_pModelInstance->m_MaterialMap.find(strMat);
         if (it == m_pModelInstance->m_MaterialMap.end())
         {
            // Show a warning, if material was not found
            //DefaultLogger::get()->warn("OBJ: Unsupported material requested: " + strMat);
            m_pModelInstance->m_pCurrentMaterial = m_pModelInstance->m_pDefaultMaterial;
         }
         else
         {
            // Set new material
            if (NeedsNewMesh(strMat))
            {
               CreateMesh();
            }
            m_pModelInstance->m_pCurrentMesh->m_uiMaterialIndex = GetMaterialIndex(strMat);
         }

         m_dataIterator = SkipLine<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_currentLine);
      }

      int32 ObjFileParser::GetMaterialIndex(const String_c &strMaterialName)
      {
         int32 mat_index = -1;
         if (strMaterialName.IsEmpty()) {
            return mat_index;
         }
         for (size_t index = 0; index < m_pModelInstance->m_MaterialLib.size(); ++index)
         {
            if (strMaterialName == m_pModelInstance->m_MaterialLib[index])
            {
               mat_index = (int32)index;
               break;
            }
         }
         return mat_index;
      }

      //	Getter for a group name.  
      void ObjFileParser::GetGroupName()
      {
         String_c strGroupName;

         m_dataIterator = GetName<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, strGroupName);
         if (IsEndOfBuffer(m_dataIterator, m_dataIteratorEndOfBuffer)) {
            return;
         }

         // Change active group, if necessary
         if (m_pModelInstance->m_strActiveGroup != strGroupName)
         {
            // Search for already existing entry
            objfile::Model::ConstGroupMapIt it = m_pModelInstance->m_Groups.find(strGroupName);

            // We are mapping groups into the object structure
            CreateObject(strGroupName);

            // New group name, creating a new entry
            if (it == m_pModelInstance->m_Groups.end())
            {
               std::vector<uint32> *pFaceIDArray = new std::vector < uint32 > ;
               m_pModelInstance->m_Groups[strGroupName] = pFaceIDArray;
               m_pModelInstance->m_pGroupFaceIDs = (pFaceIDArray);
            }
            else
            {
               m_pModelInstance->m_pGroupFaceIDs = (*it).second;
            }
            m_pModelInstance->m_strActiveGroup = strGroupName;
         }
         m_dataIterator = SkipLine<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_currentLine);
      }

      //	Not supported
      void ObjFileParser::GetGroupNumber()
      {
         // Not used

         m_dataIterator = SkipLine<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_currentLine);
      }

      //	Not supported
      void ObjFileParser::GetGroupNumberAndResolution()
      {
         // Not used

         m_dataIterator = SkipLine<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_currentLine);
      }

      //	Stores values for a new object instance, name will be used to 
      //	identify it.
      void ObjFileParser::GetObjectName()
      {
         m_dataIterator = GetNextToken<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer);
         if (m_dataIterator == m_dataIteratorEndOfBuffer) {
            return;
         }
         char *pStart = &(*m_dataIterator);
         while (m_dataIterator != m_dataIteratorEndOfBuffer && !IsSpaceOrNewLine(*m_dataIterator)) {
            ++m_dataIterator;
         }

         String_c strObjectName(pStart, &(*m_dataIterator));
         if (!strObjectName.IsEmpty())
         {
            // Reset current object
            m_pModelInstance->m_pCurrent = NULL;

            // Search for actual object
            for (std::vector<objfile::Object*>::const_iterator it = m_pModelInstance->m_Objects.begin();
               it != m_pModelInstance->m_Objects.end();
               ++it)
            {
               if ((*it)->m_strObjName == strObjectName)
               {
                  m_pModelInstance->m_pCurrent = *it;
                  break;
               }
            }

            // Allocate a new object, if current one was not found before
            if (NULL == m_pModelInstance->m_pCurrent) {
               CreateObject(strObjectName);
            }
         }
         m_dataIterator = SkipLine<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_currentLine);
      }

      //	Creates a new object instance
      void ObjFileParser::CreateObject(const String_c &strObjectName)
      {
         assert(NULL != m_pModelInstance);
         //assert( !strObjectName.empty() );

         m_pModelInstance->m_pCurrent = new objfile::Object;
         m_pModelInstance->m_pCurrent->m_strObjName = strObjectName;
         m_pModelInstance->m_Objects.push_back(m_pModelInstance->m_pCurrent);

         CreateMesh();

         if (m_pModelInstance->m_pCurrentMaterial)
         {
            m_pModelInstance->m_pCurrentMesh->m_uiMaterialIndex =
               GetMaterialIndex(m_pModelInstance->m_pCurrentMaterial->MaterialName.begin());
            m_pModelInstance->m_pCurrentMesh->m_pMaterial = m_pModelInstance->m_pCurrentMaterial;
         }
      }

      //	Creates a new mesh
      void ObjFileParser::CreateMesh()
      {
         assert(NULL != m_pModelInstance);
         m_pModelInstance->m_pCurrentMesh = new objfile::Mesh;
         m_pModelInstance->m_Meshes.push_back(m_pModelInstance->m_pCurrentMesh);
         uint32 meshId = m_pModelInstance->m_Meshes.size() - 1;
         if (NULL != m_pModelInstance->m_pCurrent)
         {
            m_pModelInstance->m_pCurrent->m_Meshes.push_back(meshId);
         }
         else
         {
            //DefaultLogger::get()->error("OBJ: No object detected to attach a new mesh instance.");
         }
      }

      //	Returns true, if a new mesh must be created.
      bool ObjFileParser::NeedsNewMesh(const String_c &rMaterialName)
      {
         if (m_pModelInstance->m_pCurrentMesh == 0)
         {
            // No mesh data yet
            return true;
         }
         bool newMat = false;
         int32 matIdx = GetMaterialIndex(rMaterialName);
         int32 curMatIdx = m_pModelInstance->m_pCurrentMesh->m_uiMaterialIndex;
         if (curMatIdx != int32(objfile::Mesh::NoMaterial) || curMatIdx != matIdx)
         {
            // New material -> only one material per mesh, so we need to create a new 
            // material
            newMat = true;
         }
         return newMat;
      }

      void ObjFileParser::ReportErrorTokenInFace()
      {
         m_dataIterator = SkipLine<DataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_currentLine);
         //DefaultLogger::get()->error("OBJ: Not supported token in face description detected");
      }

   } // namespace objparser

}	// namespace model

#endif
