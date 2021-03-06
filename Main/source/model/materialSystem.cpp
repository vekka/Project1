/*
Open Asset Import Library (assimp)
----------------------------------------------------------------------

Copyright (c) 2006-2012, assimp team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

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

----------------------------------------------------------------------
*/

#include "../core/hash/Hash.hpp"
#include "../core/fast_atof.hpp"
#include "../core/chartypes.hpp"

#include "materialSystem.hpp"

//#include "../include/assimp/DefaultLogger.hpp"
#include "../core/macros.hpp"

#include "../core/fast_atof.hpp"

#include <algorithm>

namespace material
{
   const char *Material::DEFAULT_MATERIAL_NAME = "default";

   const char *Material::KEY_NAME = "?mat.name";
   const char *Material::KEY_TWOSIDED = "$mat.twosided";
   const char *Material::KEY_SHADING_MODEL = "$mat.shadingm";
   const char *Material::KEY_ENABLE_WIREFRAME = "$mat.wireframe";
   const char *Material::KEY_BLEND_FUNC = "$mat.blend";
   const char *Material::KEY_OPACITY = "$mat.opacity";
   const char *Material::KEY_BUMPSCALING = "$mat.bumpscaling";
   const char *Material::KEY_SHININESS = "$mat.shininess";
   const char *Material::KEY_REFLECTIVITY = "$mat.reflectivity";
   const char *Material::KEY_SHININESS_STRENGTH = "$mat.shinpercent";
   const char *Material::KEY_REFRACTI = "$mat.refracti";
   const char *Material::KEY_COLOR_DIFFUSE = "$clr.diffuse";
   const char *Material::KEY_COLOR_AMBIENT = "$clr.ambient";
   const char *Material::KEY_COLOR_SPECULAR = "$clr.specular";
   const char *Material::KEY_COLOR_EMISSIVE = "$clr.emissive";
   const char *Material::KEY_COLOR_TRANSPARENT = "$clr.transparent";
   const char *Material::KEY_COLOR_REFLECTIVE = "$clr.reflective";
   const char *Material::KEY_GLOBAL_BACKGROUND_IMAGE = "?bg.global";

   const char *Material::KEYNAME_TEXTURE_BASE = "$tex.file";
   const char *Material::KEYNAME_UVWSRC_BASE = "$tex.uvwsrc";
   const char *Material::KEYNAME_TEXOP_BASE = "$tex.op";
   const char *Material::KEYNAME_MAPPING_BASE = "$tex.mapping";
   const char *Material::KEYNAME_TEXBLEND_BASE = "$tex.blend";
   const char *Material::KEYNAME_MAPPINGMODE_U_BASE = "$tex.mapmodeu";
   const char *Material::KEYNAME_MAPPINGMODE_V_BASE = "$tex.mapmodev";
   const char *Material::KEYNAME_TEXMAP_AXIS_BASE = "$tex.mapaxis";
   const char *Material::KEYNAME_UVTRANSFORM_BASE = "$tex.uvtrafo";
   const char *Material::KEYNAME_TEXFLAGS_BASE = "$tex.flags";

   // atually the one and only way to get an Material instance
   Material::Material()
   {
      // Allocate 5 entries by default
      m_NumProperties = 0;
      m_NumAllocated = 5;
      m_ppProperties = new MaterialProperty*[5];
   }

   Material::~Material()
   {
      Clear();

      delete[] m_ppProperties;
   }

   int32 GetMaterialProperty(const Material* pMat, const char* pKey, uint32 type, uint32 index, const MaterialProperty **pPropOut)
   {
      assert(pMat != NULL);
      assert(pKey != NULL);
      assert(pPropOut != NULL);

      /*  Just search for a property with exactly this name ..
      *  could be improved by hashing, but it's possibly
      *  no worth the effort (we're bound to C structures,
      *  thus std::map or derivates are not applicable. */
      for (uint32 i = 0; i < pMat->GetNumProperties(); ++i) {
         MaterialProperty* prop = pMat->GetProperty(i);

         if (prop /* just for safety ... */
            && 0 == strcmp(prop->m_key.data(), pKey)
            && (UINT_MAX == type || prop->m_textureSemantic == type) /* UINT_MAX is a wildcard, but this is undocumented :-) */
            && (UINT_MAX == index || prop->m_textureIndex == index))
         {
            *pPropOut = pMat->GetProperty(i);
            return 0;
         }
      }
      *pPropOut = NULL;
      return -1;
   }

   // Get an array of floating-point values from the material.
   int32 GetMaterialFloatArray(const Material *pMat, const char* pKey, uint32 type, uint32 index, float* pOut, uint32* pMax)
   {
      assert(pOut != NULL);
      assert(pMat != NULL);

      const MaterialProperty* prop;
      GetMaterialProperty(pMat, pKey, type, index, (const MaterialProperty**)&prop);
      if (!prop) {
         return -1;
      }

      // data is given in floats, simply copy it
      uint32 iWrite = 0;
      if (material::PROPERTY_TYPE_INFO_FLOAT == prop->m_propertyTypeInfo || material::PROPERTY_TYPE_INFO_BINARY_BUFFER == prop->m_propertyTypeInfo)	{
         iWrite = prop->m_dataNumBytes / sizeof(float);
         if (pMax) {
            iWrite = std::min(*pMax, iWrite);;
         }
         for (uint32 a = 0; a < iWrite; ++a)	{
            pOut[a] = static_cast<float> (reinterpret_cast<float*>(prop->m_data)[a]);
         }
         if (pMax) {
            *pMax = iWrite;
         }
      }
      // data is given in ints, convert to float
      else if (material::PROPERTY_TYPE_INFO_INTEGER == prop->m_propertyTypeInfo)	{
         iWrite = prop->m_dataNumBytes / sizeof(int32_t);
         if (pMax) {
            iWrite = std::min(*pMax, iWrite);;
         }
         for (uint32 a = 0; a < iWrite; ++a)	{
            pOut[a] = static_cast<float> (reinterpret_cast<int32_t*>(prop->m_data)[a]);
         }
         if (pMax) {
            *pMax = iWrite;
         }
      }
      // a string ... read floats separated by spaces
      else {
         if (pMax) {
            iWrite = *pMax;
         }
         // strings are zero-terminated with a 32 bit length prefix, so this is safe
         const char* cur = prop->m_data + 4;
         assert(prop->m_dataNumBytes >= 5 && !prop->m_data[prop->m_dataNumBytes - 1]);
         for (uint32 a = 0;; a++) {
            cur = core::fast_atoreal_move<float>(cur, pOut[a]);
            if (a == iWrite - 1) {
               break;
            }
            //if (!IsSpace(*cur)) {
            //   DefaultLogger::get()->error("Material property" + std::string(pKey) +
            //      " is a string; failed to parse a float array out of it.");
            //   return -1;
            //}
         }

         if (pMax) {
            *pMax = iWrite;
         }
      }
      return 0;
   }

   // Get an array if integers from the material
   int32 GetMaterialIntegerArray(const Material *pMat, const char* pKey, uint32 type, uint32 index, int32* pOut, uint32* pMax)
   {
      assert(pOut != NULL);
      assert(pMat != NULL);

      const MaterialProperty* prop;
      GetMaterialProperty(pMat, pKey, type, index, (const MaterialProperty**)&prop);
      if (!prop) {
         return -1;
      }

      // data is given in ints, simply copy it
      uint32 iWrite = 0;
      if (material::PROPERTY_TYPE_INFO_INTEGER == prop->m_propertyTypeInfo || material::PROPERTY_TYPE_INFO_BINARY_BUFFER == prop->m_propertyTypeInfo)	{
         iWrite = prop->m_dataNumBytes / sizeof(int32_t);
         if (pMax) {
            iWrite = std::min(*pMax, iWrite);;
         }
         for (uint32 a = 0; a < iWrite; ++a) {
            pOut[a] = static_cast<int32>(reinterpret_cast<int32_t*>(prop->m_data)[a]);
         }
         if (pMax) {
            *pMax = iWrite;
         }
      }
      // data is given in floats convert to int32 
      else if (material::PROPERTY_TYPE_INFO_FLOAT == prop->m_propertyTypeInfo)	{
         iWrite = prop->m_dataNumBytes / sizeof(float);
         if (pMax) {
            iWrite = std::min(*pMax, iWrite);;
         }
         for (uint32 a = 0; a < iWrite; ++a) {
            pOut[a] = static_cast<int32>(reinterpret_cast<float*>(prop->m_data)[a]);
         }
         if (pMax) {
            *pMax = iWrite;
         }
      }
      // it is a string ... no way to read something out of this
      else	{
         if (pMax) {
            iWrite = *pMax;
         }
         // strings are zero-terminated with a 32 bit length prefix, so this is safe
         const char* cur = prop->m_data + 4;
         assert(prop->m_dataNumBytes >= 5 && !prop->m_data[prop->m_dataNumBytes - 1]);
         for (uint32 a = 0;; a++) {
            pOut[a] = core::strtol10(cur, &cur);
            if (a == iWrite - 1) {
               break;
            }
            //if (!IsSpace(*cur)) {
            //   DefaultLogger::get()->error("Material property" + std::string(pKey) +
            //      " is a string; failed to parse an integer array out of it.");
            //   return -1;
            //}
         }

         if (pMax) {
            *pMax = iWrite;
         }
      }
      return 0;
   }

   // Get a color (3 or 4 floats) from the material
   int32 GetMaterialColor(const Material* pMat,
      const char* pKey,
      uint32 type,
      uint32 index,
      Color4f* pOut)
   {
      uint32 iMax = 4;
      const int32 eRet = GetMaterialFloatArray(pMat, pKey, type, index, (float*)pOut, &iMax);

      // if no alpha channel is defined: set it to 1.0
      if (3 == iMax) {
         pOut->a = 1.0f;
      }

      return eRet;
   }

   // Get a UVTransform (4 floats) from the material
   int32 GetMaterialUVTransform(const Material* pMat, const char* pKey, uint32 type, uint32 index, material::UVTransform* pOut)
   {
      uint32 iMax = 4;
      return  GetMaterialFloatArray(pMat, pKey, type, index, (float*)pOut, &iMax);
   }

   int32 GetMaterialString(const Material* pMat, const char* pKey, uint32 type, uint32 index, std::string &pOut)
   {
      assert(!pOut.empty());

      const MaterialProperty* prop;
      GetMaterialProperty(pMat, pKey, type, index, (const MaterialProperty**)&prop);
      if (!prop) {
         return -1;
      }

      if (material::PROPERTY_TYPE_INFO_STRING == prop->m_propertyTypeInfo) {
         assert(prop->m_dataNumBytes >= 5);

         // The string is stored as 32 but length prefix followed by zero-terminated UTF8 data
         //pOut.length = static_cast<uint32>(*reinterpret_cast<uint32 *>(prop->m_data));
         pOut.resize(static_cast<uint32>(*reinterpret_cast<uint32 *>(prop->m_data)));
      
         assert(pOut.size() + 1 + 4 == prop->m_dataNumBytes && !prop->m_data[prop->m_dataNumBytes - 1]);
         //memcpy(pOut.data, prop->m_data + 4, pOut.length + 1);
         pOut = prop->m_data + 4;
      }
      else {
         // TODO - implement lexical cast as well
         //DefaultLogger::get()->error("Material property" + std::string(pKey) +
         //   " was found, but is no string");
         return -1;
      }
      return 0;
   }

   // get the number of textures on a particular m_texture stack
   uint32 GetMaterialTextureCount(const Material* pMat, material::eTextureType type)
   {
      assert(pMat != NULL);

      /* Textures are always stored with ascending indices (ValidateDS provides a check, so we don't need to do it again) */
      uint32 max = 0;
      for (uint32 i = 0; i < pMat->GetNumProperties(); i++) {
         MaterialProperty* prop = pMat->GetProperty(i);

         if (prop /* just a sanity check ... */
            && 0 == strcmp(prop->m_key.data(), material::Material::KEYNAME_TEXTURE_BASE)
            && prop->m_textureSemantic == type) {

            max = std::max(max, prop->m_textureIndex + 1);
         }
      }
      return max;
   }

   int32 GetMaterialTexture(const Material* mat,
      material::eTextureType type,
      uint32 index,
      std::string &path,
      material::eTextureMapping* _mapping	/*= NULL*/,
      uint32* uvindex		/*= NULL*/,
      float* blend				/*= NULL*/,
      material::eTextureOperation* op				/*= NULL*/,
      material::eTextureMapMode* mapmode	/*= NULL*/,
      uint32* flags         /*= NULL*/
      )
   {
      assert(NULL != mat && !path.empty());

      // Get the path to the m_texture
      if (0 != GetMaterialString(mat, material::Material::KEYNAME_TEXTURE_BASE, type, index, path))	{
         return -1;
      }
      // Determine mapping type 
      material::eTextureMapping mapping = material::TEXTURE_MAP_UV;
      GetMaterialInteger(mat, material::Material::KEYNAME_MAPPING_BASE, type, index, (int32*)&mapping);
      if (_mapping)
         *_mapping = mapping;

      // Get UV index 
      if (material::TEXTURE_MAP_UV == mapping && uvindex)	{
         GetMaterialInteger(mat, material::Material::KEYNAME_UVWSRC_BASE, type, index, (int32*)uvindex);
      }
      // Get blend factor 
      if (blend)	{
         GetMaterialFloat(mat, material::Material::KEYNAME_TEXBLEND_BASE, type, index, blend);
      }
      // Get m_texture operation 
      if (op){
         GetMaterialInteger(mat, material::Material::KEYNAME_TEXOP_BASE, type, index, (int32*)op);
      }
      // Get m_texture mapping modes
      if (mapmode)	{
         GetMaterialInteger(mat, material::Material::KEYNAME_MAPPINGMODE_U_BASE, type, index, (int32*)&mapmode[0]);
         GetMaterialInteger(mat, material::Material::KEYNAME_MAPPINGMODE_V_BASE, type, index, (int32*)&mapmode[1]);
      }
      // Get m_texture flags
      if (flags){
         GetMaterialInteger(mat, material::Material::KEYNAME_TEXFLAGS_BASE, type, index, (int32*)flags);
      }
      return 0;
   }

   void Material::Clear()
   {
      for (uint32 i = 0; i < m_NumProperties; ++i)	{
         // delete this entry
         delete m_ppProperties[i];
         AI_DEBUG_INVALIDATE_PTR(m_ppProperties[i]);
      }
      m_NumProperties = 0;

      // The array remains allocated, we just invalidated its contents
   }

   int32 Material::RemoveProperty(const char* pKey, uint32 type, uint32 index)
   {
      assert(NULL != pKey);

      for (uint32 i = 0; i < m_NumProperties; ++i) {
         MaterialProperty* prop = m_ppProperties[i];

         if (prop && !strcmp(prop->m_key.data(), pKey) &&
            prop->m_textureSemantic == type && prop->m_textureIndex == index)
         {
            // Delete this entry
            delete m_ppProperties[i];

            // collapse the array behind --.
            --m_NumProperties;
            for (uint32 a = i; a < m_NumProperties; a++)	{
               m_ppProperties[a] = m_ppProperties[a + 1];
            }
            return 0;
         }
      }

      return -1;
   }

   int32 Material::AddBinaryProperty(const void* pInput, uint32 pSizeInBytes, const char* pKey,
      uint32 type, uint32 index, ePropertyTypeInfo pType)
   {
      assert(pInput != NULL);
      assert(pKey != NULL);
      assert(0 != pSizeInBytes);

      // first search the list whether there is already an entry with this key
      uint32 iOutIndex = UINT_MAX;
      for (uint32 i = 0; i < m_NumProperties; i++)
      {
         MaterialProperty* prop = m_ppProperties[i];

         if (prop /* just for safety */ && !strcmp(prop->m_key.data(), pKey) &&
            prop->m_textureSemantic == type && prop->m_textureIndex == index){

            delete m_ppProperties[i];
            iOutIndex = i;
         }
      }

      // Allocate a new material property
      MaterialProperty* pcNew = new MaterialProperty();

      // .. and fill it
      pcNew->m_propertyTypeInfo = pType;
      pcNew->m_textureSemantic = type;
      pcNew->m_textureIndex = index;

      pcNew->m_dataNumBytes = pSizeInBytes;
      pcNew->m_data = new char[pSizeInBytes];
      memcpy(pcNew->m_data, pInput, pSizeInBytes);

      //pcNew->m_key.length = strlen(pKey);
      //pcNew->m_key.resize(strlen(pKey));
      //assert(MAXLEN > pcNew->m_key.length); // MAXLEN = ?
      //strcpy(pcNew->m_key.data, pKey);
      pcNew->m_key = pKey;

      if (UINT_MAX != iOutIndex)	{
         m_ppProperties[iOutIndex] = pcNew;
         return 0;
      }

      // resize the array ... double the storage allocated
      if (m_NumProperties == m_NumAllocated)	{
         const uint32 iOld = m_NumAllocated;
         m_NumAllocated *= 2;

         MaterialProperty** ppTemp;
         try {
            ppTemp = new MaterialProperty*[m_NumAllocated];
         }
         catch (std::bad_alloc&) {
            return -3; // AI_OUTOFMEMORY;
         }

         // just copy all items over; then replace the old array
         memcpy(ppTemp, m_ppProperties, iOld * sizeof(void*));

         delete[] m_ppProperties;
         m_ppProperties = ppTemp;
      }
      // push back ...
      m_ppProperties[m_NumProperties++] = pcNew;
      return 0;
   }

   int32 Material::AddProperty(const std::string &pInput, const char* pKey, uint32 type, uint32 index)
   {
      // We don't want to add the whole buffer .. write a 32 bit length
      // prefix followed by the zero-terminated UTF8 string.
      // (HACK) I don't want to break the ABI now, but we definitely
      // ought to change aiString::mLength to uint32  one day.
      if (sizeof(size_t) == 8) {
         std::string copy = pInput;
         uint32 length = copy.size();
         uint32 * s = reinterpret_cast<uint32 *>(&length);
         s[1] = static_cast<uint32>(pInput.size());

         return AddBinaryProperty(s + 1,
            pInput.size() + 1 + 4,
            pKey,
            type,
            index,
            material::PROPERTY_TYPE_INFO_STRING);
      }
      assert(sizeof(size_t) == 4);
      return AddBinaryProperty(&pInput,
         pInput.size() + 1 + 4,
         pKey,
         type,
         index,
         material::PROPERTY_TYPE_INFO_STRING);
   }

   uint32 ComputeMaterialHash(const Material* mat, bool includeMatName /*= false*/)
   {
      uint32  hash = 1503; // magic start value, chosen to be my birthday :-)
      for (uint32 i = 0; i < mat->GetNumProperties(); ++i)	{
         MaterialProperty* prop;

         // Exclude all properties whose first character is '?' from the hash
         // See doc for MaterialProperty.
         if ((prop = mat->GetProperty(i)) && (includeMatName || prop->m_key[0] != '?'))	{

            hash = core::SuperFastHash(prop->m_key.data(), (uint32)prop->m_key.size(), hash);
            hash = core::SuperFastHash(prop->m_data, prop->m_dataNumBytes, hash);

            // Combine the semantic and the index with the hash
            hash = core::SuperFastHash((const char*)&prop->m_textureSemantic, sizeof(uint32), hash);
            hash = core::SuperFastHash((const char*)&prop->m_textureIndex, sizeof(uint32), hash);
         }
      }
      return hash;
   }

   void Material::CopyPropertyList(Material* pcDest, const Material* pcSrc)
   {
      assert(NULL != pcDest);
      assert(NULL != pcSrc);

      uint32 iOldNum = pcDest->m_NumProperties;
      pcDest->m_NumAllocated += pcSrc->m_NumAllocated;
      pcDest->m_NumProperties += pcSrc->m_NumProperties;

      MaterialProperty** pcOld = pcDest->m_ppProperties;
      pcDest->m_ppProperties = new MaterialProperty*[pcDest->m_NumAllocated];

      if (iOldNum && pcOld)	{
         for (uint32 i = 0; i < iOldNum; ++i) {
            pcDest->m_ppProperties[i] = pcOld[i];
         }

         delete[] pcOld;
      }
      for (uint32 i = iOldNum; i < pcDest->m_NumProperties; ++i)	{
         MaterialProperty* propSrc = pcSrc->m_ppProperties[i];

         // search whether we have already a property with this name -> if yes, overwrite it
         MaterialProperty* prop;
         for (uint32 q = 0; q < iOldNum; ++q) {
            prop = pcDest->m_ppProperties[q];
            if (prop /* just for safety */ && prop->m_key == propSrc->m_key && prop->m_textureSemantic == propSrc->m_textureSemantic
               && prop->m_textureIndex == propSrc->m_textureIndex)	{
               delete prop;

               // collapse the whole array ...
               memmove(&pcDest->m_ppProperties[q], &pcDest->m_ppProperties[q + 1], i - q);
               i--;
               pcDest->m_NumProperties--;
            }
         }

         // Allocate the output property and copy the source property
         prop = pcDest->m_ppProperties[i] = new MaterialProperty();
         prop->m_key = propSrc->m_key;
         prop->m_dataNumBytes = propSrc->m_dataNumBytes;
         prop->m_propertyTypeInfo = propSrc->m_propertyTypeInfo;
         prop->m_textureSemantic = propSrc->m_textureSemantic;
         prop->m_textureIndex = propSrc->m_textureIndex;

         prop->m_data = new char[propSrc->m_dataNumBytes];
         memcpy(prop->m_data, propSrc->m_data, prop->m_dataNumBytes);
      }
      return;
   }

} // namespace material
