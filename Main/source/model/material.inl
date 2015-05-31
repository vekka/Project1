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

/** @file material.inl
*  @brief Defines the C++ getters for the material system
*/

#ifndef AI_MATERIAL_INL_INC
#define AI_MATERIAL_INL_INC

//! @cond never

// ---------------------------------------------------------------------------
inline int32 Material::GetTexture(aiTextureType type,
   uint32  index,
   C_STRUCT aiString* path,
   aiTextureMapping* mapping	/*= NULL*/,
   uint32* uvindex		/*= NULL*/,
   float* blend				   /*= NULL*/,
   aiTextureOp* op				/*= NULL*/,
   aiTextureMapMode* mapmode	/*= NULL*/) const
{
   return ::aiGetMaterialTexture(this, type, index, path, mapping, uvindex, blend, op, mapmode);
}

// ---------------------------------------------------------------------------
inline uint32 aiMaterial::GetTextureCount(aiTextureType type) const
{
   return ::aiGetMaterialTextureCount(this, type);
}

// ---------------------------------------------------------------------------
template <typename Type>
inline int32 aiMaterial::Get(const char* pKey, uint32 type,
   uint32 idx, Type* pOut,
   uint32* pMax) const
{
   uint32 iNum = pMax ? *pMax : 1;

   const MaterialProperty* prop;
   const int32 ret = ::aiGetMaterialProperty(this, pKey, type, idx,
      (const MaterialProperty**)&prop);
   if (AI_SUCCESS == ret)	{

      if (prop->mDataLength < sizeof(Type)*iNum) {
         return AI_FAILURE;
      }

      if (prop->mType != aiPTI_Buffer) {
         return AI_FAILURE;
      }

      iNum = std::min((size_t)iNum, prop->mDataLength / sizeof(Type));
      ::memcpy(pOut, prop->mData, iNum * sizeof(Type));
      if (pMax) {
         *pMax = iNum;
      }
   }
   return ret;
}

// ---------------------------------------------------------------------------
template <typename Type>
inline int32 aiMaterial::Get(const char* pKey, uint32 type,
   uint32 idx, Type& pOut) const
{
   const MaterialProperty* prop;
   const int32 ret = ::aiGetMaterialProperty(this, pKey, type, idx,
      (const MaterialProperty**)&prop);
   if (AI_SUCCESS == ret)	{

      if (prop->mDataLength < sizeof(Type)) {
         return AI_FAILURE;
      }

      if (prop->mType != aiPTI_Buffer) {
         return AI_FAILURE;
      }

      ::memcpy(&pOut, prop->mData, sizeof(Type));
   }
   return ret;
}

// ---------------------------------------------------------------------------
inline int32 aiMaterial::Get(const char* pKey, uint32 type,
   uint32 idx, float* pOut,
   uint32* pMax) const
{
   return ::aiGetMaterialFloatArray(this, pKey, type, idx, pOut, pMax);
}
// ---------------------------------------------------------------------------
inline int32 aiMaterial::Get(const char* pKey, uint32 type,
   uint32 idx, int* pOut,
   uint32* pMax) const
{
   return ::aiGetMaterialIntegerArray(this, pKey, type, idx, pOut, pMax);
}
// ---------------------------------------------------------------------------
inline int32 aiMaterial::Get(const char* pKey, uint32 type,
   uint32 idx, float& pOut) const
{
   return aiGetMaterialFloat(this, pKey, type, idx, &pOut);
}
// ---------------------------------------------------------------------------
inline int32 aiMaterial::Get(const char* pKey, uint32 type,
   uint32 idx, int& pOut) const
{
   return aiGetMaterialInteger(this, pKey, type, idx, &pOut);
}
// ---------------------------------------------------------------------------
inline int32 aiMaterial::Get(const char* pKey, uint32 type,
   uint32 idx, aiColor4D& pOut) const
{
   return aiGetMaterialColor(this, pKey, type, idx, &pOut);
}
// ---------------------------------------------------------------------------
inline int32 aiMaterial::Get(const char* pKey, uint32 type,
   uint32 idx, aiColor3D& pOut) const
{
   aiColor4D c;
   const int32 ret = aiGetMaterialColor(this, pKey, type, idx, &c);
   pOut = aiColor3D(c.r, c.g, c.b);
   return ret;
}
// ---------------------------------------------------------------------------
inline int32 aiMaterial::Get(const char* pKey, uint32 type,
   uint32 idx, aiString& pOut) const
{
   return aiGetMaterialString(this, pKey, type, idx, &pOut);
}
// ---------------------------------------------------------------------------
inline int32 aiMaterial::Get(const char* pKey, uint32 type,
   uint32 idx, aiUVTransform& pOut) const
{
   return aiGetMaterialUVTransform(this, pKey, type, idx, &pOut);
}


// ---------------------------------------------------------------------------
template<class TYPE>
int32 aiMaterial::AddProperty(const TYPE* pInput,
   const uint32 pNumValues,
   const char* pKey,
   uint32 type,
   uint32 index)
{
   return AddBinaryProperty((const void*)pInput,
      pNumValues * sizeof(TYPE),
      pKey, type, index, aiPTI_Buffer);
}

// ---------------------------------------------------------------------------
inline int32 aiMaterial::AddProperty(const float* pInput,
   const uint32 pNumValues,
   const char* pKey,
   uint32 type,
   uint32 index)
{
   return AddBinaryProperty((const void*)pInput,
      pNumValues * sizeof(float),
      pKey, type, index, aiPTI_Float);
}

// ---------------------------------------------------------------------------
inline int32 aiMaterial::AddProperty(const aiUVTransform* pInput,
   const uint32 pNumValues,
   const char* pKey,
   uint32 type,
   uint32 index)
{
   return AddBinaryProperty((const void*)pInput,
      pNumValues * sizeof(aiUVTransform),
      pKey, type, index, aiPTI_Float);
}

// ---------------------------------------------------------------------------
inline int32 aiMaterial::AddProperty(const aiColor4D* pInput,
   const uint32 pNumValues,
   const char* pKey,
   uint32 type,
   uint32 index)
{
   return AddBinaryProperty((const void*)pInput,
      pNumValues * sizeof(aiColor4D),
      pKey, type, index, aiPTI_Float);
}

// ---------------------------------------------------------------------------
inline int32 aiMaterial::AddProperty(const aiColor3D* pInput,
   const uint32 pNumValues,
   const char* pKey,
   uint32 type,
   uint32 index)
{
   return AddBinaryProperty((const void*)pInput,
      pNumValues * sizeof(aiColor3D),
      pKey, type, index, aiPTI_Float);
}

// ---------------------------------------------------------------------------
inline int32 aiMaterial::AddProperty(const aiVector3D* pInput,
   const uint32 pNumValues,
   const char* pKey,
   uint32 type,
   uint32 index)
{
   return AddBinaryProperty((const void*)pInput,
      pNumValues * sizeof(aiVector3D),
      pKey, type, index, aiPTI_Float);
}

// ---------------------------------------------------------------------------
inline int32 aiMaterial::AddProperty(const int* pInput,
   const uint32 pNumValues,
   const char* pKey,
   uint32 type,
   uint32 index)
{
   return AddBinaryProperty((const void*)pInput,
      pNumValues * sizeof(int),
      pKey, type, index, aiPTI_Integer);
}


// ---------------------------------------------------------------------------
// The template specializations below are for backwards compatibility.
// The recommended way to add material properties is using the non-template
// overloads.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
template<>
inline int32 aiMaterial::AddProperty<float>(const float* pInput,
   const uint32 pNumValues,
   const char* pKey,
   uint32 type,
   uint32 index)
{
   return AddBinaryProperty((const void*)pInput,
      pNumValues * sizeof(float),
      pKey, type, index, aiPTI_Float);
}

// ---------------------------------------------------------------------------
template<>
inline int32 aiMaterial::AddProperty<aiUVTransform>(const aiUVTransform* pInput,
   const uint32 pNumValues,
   const char* pKey,
   uint32 type,
   uint32 index)
{
   return AddBinaryProperty((const void*)pInput,
      pNumValues * sizeof(aiUVTransform),
      pKey, type, index, aiPTI_Float);
}

// ---------------------------------------------------------------------------
template<>
inline int32 aiMaterial::AddProperty<aiColor4D>(const aiColor4D* pInput,
   const uint32 pNumValues,
   const char* pKey,
   uint32 type,
   uint32 index)
{
   return AddBinaryProperty((const void*)pInput,
      pNumValues * sizeof(aiColor4D),
      pKey, type, index, aiPTI_Float);
}

// ---------------------------------------------------------------------------
template<>
inline int32 aiMaterial::AddProperty<aiColor3D>(const aiColor3D* pInput,
   const uint32 pNumValues,
   const char* pKey,
   uint32 type,
   uint32 index)
{
   return AddBinaryProperty((const void*)pInput,
      pNumValues * sizeof(aiColor3D),
      pKey, type, index, aiPTI_Float);
}

// ---------------------------------------------------------------------------
template<>
inline int32 aiMaterial::AddProperty<aiVector3D>(const aiVector3D* pInput,
   const uint32 pNumValues,
   const char* pKey,
   uint32 type,
   uint32 index)
{
   return AddBinaryProperty((const void*)pInput,
      pNumValues * sizeof(aiVector3D),
      pKey, type, index, aiPTI_Float);
}

// ---------------------------------------------------------------------------
template<>
inline int32 aiMaterial::AddProperty<int>(const int* pInput,
   const uint32 pNumValues,
   const char* pKey,
   uint32 type,
   uint32 index)
{
   return AddBinaryProperty((const void*)pInput,
      pNumValues * sizeof(int),
      pKey, type, index, aiPTI_Integer);
}

//! @endcond

#endif //! AI_MATERIAL_INL_INC