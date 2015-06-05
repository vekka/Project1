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

/** @file material.h
*  @brief Defines the material system of the library
*/

#ifndef _MATERIAL_HPP_INCLUDED_
#define _MATERIAL_HPP_INCLUDED_

#include "../core/math/vector3.hpp"
#include "../core/math/vector2.hpp"
using core::math::Vector2f;
using core::math::Vector3f;

#include <string>

#include "../core/math/matrix3.hpp"
#include "../core/math/matrix4.hpp"

#include "../gfx/color3f.hpp"
using gfx::color3f::Color3f;

#include "../gfx/color4f.hpp"
using gfx::color4f::Color4f;

namespace material
{
   //uint32 GetMaterialTextureCount(const Material* pMat, material::eTextureType type);
   //int32 GetMaterialFloatArray(const Material* pMat, const char *pKey, uint32 type, uint32 index, float* pOut, uint32* pMax);

   // Name for default materials (2nd is used if meshes have UV coords)
   extern const char *DEFAULT_MATERIAL_NAME;

   // Defines how the Nth m_texture of a specific type is combined with
   //  the result of all previous layers.
   //
   //  Example (left: key, right: value): <br>
   //  @code
   //  DiffColor0     - gray
   //  DiffTextureOp0 - aiTextureOpMultiply
   //  DiffTexture0   - tex1.png
   //  DiffTextureOp0 - aiTextureOpAdd
   //  DiffTexture1   - tex2.png
   //  @endcode
   //  Written as equation, the final diffuse term for a specific pixel would be:
   //  @code
   //  diffFinal = DiffColor0 * sampleTex(DiffTexture0,UV0) +
   //     sampleTex(DiffTexture1,UV0) * diffContrib;
   //  @endcode
   //  where 'diffContrib' is the intensity of the incoming light for that pixel.

   enum eTextureOperation
   {
      TEXTURE_OPER_MULT, // T = T1 * T2
      TEXTURE_OPER_ADD, // T = T1 + T2
      TEXTURE_OPER_SUB, // T = T1 - T2
      TEXTURE_OPER_DIV, // T = T1 / T2
      TEXTURE_OPER_SMOOTH_ADD, // T = (T1 + T2) - (T1 * T2)
      TEXTURE_OPER_SIGNED_ADD // T = T1 + (T2-0.5)
   };

   // Defines how UV coordinates outside the [0...1] range are handled.
   //  Commonly refered to as 'wrapping mode'.

   enum eTextureMapMode
   {
      TEXTURE_MAPMODE_WRAP, // A m_texture coordinate u|v is translated to u%1|v%1
      TEXTURE_MAPMODE_CLAMP, // Texture coordinates outside [0...1] are clamped to the nearest valid value.
      TEXTURE_MAPMODE_DECAL, // If the m_texture pixel coords are outside [0...1] the m_texture is not applied to that pixel
      TEXTURE_MAPMODE_MIRROR // A m_texture coord u|v becomes u%1|v%1 if (u-(u%1))%2 is 0 and 1-(u%1)|1-(v%1) otherwise
   };

   /** @brief Defines how the mapping coords for a m_texture are generated.
   *
   *  Real-time applications typically require full UV coordinates, so the use of
   *  the aiProcess_GenUVCoords step is highly recommended. It generates proper
   *  UV channels for non-UV mapped objects, as long as an accurate description
   *  how the mapping should look like (e.g spherical) is given.
   *  See the #AI_MATKEY_MAPPING property for more details.
   */
   enum eTextureMapping
   {
      // The mapping coordinates are taken from an UV channel.
      //
      //  The #AI_MATKEY_UVWSRC key specifies from which UV channel
      //  the m_texture coordinates are to be taken from (remember,
      //  meshes can have more than one UV channel).

      TEXTURE_MAP_UV,
      TEXTURE_MAP_SPHERE, // Spherical mapping
      TEXTURE_MAP_CYLINDER, // Cylindrical mapping
      TEXTURE_MAP_BOX, // Cubic mapping
      TEXTURE_MAP_PLANE, // Planar mapping
      TEXTURE_MAP_OTHER // Undefined mapping. Have fun.
   };

   /** Defines the purpose of a m_texture
   *
   *  This is a very difficult topic. Different 3D packages support different
   *  kinds of textures. For very common m_texture types, such as bumpmaps, the
   *  rendering results depend on implementation details in the rendering
   *  pipelines of these applications. Assimp loads all m_texture references from
   *  the model file and tries to determine which of the predefined m_texture
   *  types below is the best choice to match the original use of the m_texture
   *  as closely as possible.<br>
   *
   *  In content pipelines you'll usually define how textures have to be handled,
   *  and the artists working on models have to conform to this specification,
   *  regardless which 3D tool they're using.
   */
   enum eTextureType
   {
      /** Dummy value.
      *
      *  No m_texture, but the value to be used as 'm_texture semantic'
      *  (#MaterialProperty::m_textureSemantic) for all material properties
      *  *not* related to textures.
      */
      TEXTURE_TYPE_NONE,
      TEXTURE_TYPE_DIFFUSE, // m_texture is combined with the result of the diffuse lighting equation
      TEXTURE_TYPE_SPECULAR, // m_texture is combined with the result of the specular lighting equation
      TEXTURE_TYPE_AMBIENT, // m_texture is combined with the result of the ambient lighting equation
      TEXTURE_TYPE_EMISSIVE, // m_texture is added to the result of the lighting calculation. no influence by incoming light
      TEXTURE_TYPE_HEIGHTMAP, // m_texture is a height map. higher gray-scale values stand for higher elevations from the base height
      TEXTURE_TYPE_NORMALS, // m_texture is a (tangent space) normal-map. intentionally no dinstinction between the different conventions
      TEXTURE_TYPE_SHININESS, // m_texture defines the glossiness of the material. exp of (phong) lighting equation
      TEXTURE_TYPE_OPACITY, // m_texture defines per-pixel opacity. Usually 'white' means opaque and 'black' means 'transparency'
      TEXTURE_TYPE_DISPLACEMENT, // purpose and format is application-dependent. higher col values = higher vert displacements

      /*
      *  Both 'Lightmaps' and dedicated 'ambient occlusion maps' are
      *  covered by this material property. The m_texture contains a
      *  scaling value for the final color value of a pixel. Its
      *  intensity is not affected by incoming light.
      */
      TEXTURE_TYPE_LIGHTMAP, // aka Ambient Occlusion
      TEXTURE_TYPE_REFLECTION, // contains the color of a perfect mirror reflection. rarely used, almost never for real-time applications.

      /** Unknown m_texture
      *
      *  A m_texture reference that does not match any of the definitions
      *  above is considered to be 'unknown'. It is still imported,
      *  but is excluded from any further postprocessing.
      */
      TEXTURE_TYPE_UNKNOWN,
      TEXTURE_TYPE_MAX = TEXTURE_TYPE_UNKNOWN
   };

   /**Defines all shading models supported by the library
   *
   *  The list of shading modes has been taken from Blender.
   *  See Blender documentation for more information. The API does
   *  not distinguish between "specular" and "diffuse" shaders (thus the
   *  specular term for diffuse shading models like Oren-Nayar remains
   *  undefined). <br>
   *  Again, this value is just a hint. Assimp tries to select the shader whose
   *  most common implementation matches the original rendering results of the
   *  3D modeller which wrote a particular model as closely as possible.
   */
   enum eShadingMode
   {
      SHADING_MODE_FLAT = 1, // flat shading. Shading is done on per-face base, diffuse only. Also known as 'faceted shading'.
      SHADING_MODE_GOURAUD, // simple Gouraud shading.
      SHADING_MODE_PHONG,
      SHADING_MODE_BLINN, // Phong-Blinn-Shading
      SHADING_MODE_TOON, // Toon-Shading per pixel
      SHADING_MODE_ORENNAYAR, // OrenNayar-Shading per pixel
      SHADING_MODE_MINNAERT, // Minnaert-Shading per pixel, extension to standard Lambertian shading, taking the "darkness" of the material into account
      SHADING_MODE_COOKTORRANCE, // CookTorrance-Shading per pixel, special shader for metallic surfaces.
      SHADING_MODE_NOSHADING, // no shading at all. Constant light influence of 1.0
      SHADING_MODE_FRESNEL
   };

   /* Defines some mixed flags for a particular m_texture.
   *
   *  Usually you'll instruct your cg artists how textures have to look like ...
   *  and how they will be processed in your application. However, if you use
   *  Assimp for completely generic loading purposes you might also need to
   *  process these flags in order to display as many 'unknown' 3D models as
   *  possible correctly.
   *
   *  This corresponds to the #AI_MATKEY_TEXFLAGS property.
   */
   enum eTextureFlags
   {
      TEXTURE_FLAGS_INVERT = 1, // the m_texture's color values have to be inverted (componentwise 1-n)

      /* Explicit request to the application to process the alpha channel
      *  of the m_texture.
      *
      *  Mutually exclusive with #aiTextureFlags_IgnoreAlpha. These
      *  flags are set if the library can say for sure that the alpha
      *  channel is used/is not used. If the model format does not
      *  define this, it is left to the application to decide whether
      *  the m_texture alpha channel - if any - is evaluated or not.
      */
      TEXTURE_FLAGS_USEALPHA = 2,

      /** Explicit request to the application to ignore the alpha channel
      *  of the m_texture.
      *
      *  Mutually exclusive with #aiTextureFlags_UseAlpha.
      */
      TEXTURE_FLAGS_IGNOREALPHA = 4
   };

   /*Defines alpha-blend flags.
   *
   *  If you're familiar with OpenGL or D3D, these flags aren't new to you.
   *  They define *how* the final color value of a pixel is computed, basing
   *  on the previous color at that pixel and the new color value from the
   *  material.
   *  The blend formula is:
   *  @code
   *    SourceColor * SourceBlend + DestColor * DestBlend
   *  @endcode
   *  where DestColor is the previous color in the framebuffer at this
   *  position and SourceColor is the material colro before the transparency
   *  calculation.<br>
   *  This corresponds to the #AI_MATKEY_BLEND_FUNC property.
   */
   enum eBlendMode
   {
      BLEND_MODE_DEFAULT, // Formula: SourceColor*SourceAlpha + DestColor*(1-SourceAlpha)
      BLEND_MODE_ADDITIVE // Formula: SourceColor*1 + DestColor*1

      // we don't need more for the moment, but we might need them in future versions ...
   };

   /*  This is just a helper structure for the #AI_MATKEY_UVTRANSFORM key.
   *  See its documentation for more details.
   *
   *  Typically you'll want to build a matrix of this information. However,
   *  we keep separate scaling/translation/rotation values to make it
   *  easier to process and optimize UV transformations internally.
   */
#include "../core/pushpack1.hpp"
   struct UVTransform
   {
      Vector2f m_translation; // translation on the u and v axes. the default value is (0|0).
      Vector2f m_scaling; // Scaling on the u and v axes. The default value is (1|1).
      float m_rotation; // CCW rotation in radians. rotation center is 0.5f|0.5f.

      UVTransform() : m_scaling(1.0f, 1.0f), m_rotation(0.0f) { }
   } PACK_STRUCT;
#include "../core/poppack1.hpp"

   /** A very primitive RTTI system for the contents of material
   *  properties.
   */
   enum ePropertyTypeInfo
   {
      /** Array of single-precision (32 Bit) floats
      *
      *  It is possible to use aiGetMaterialInteger[Array]() (or the C++-API
      *  Material::Get()) to query properties stored in floating-point format.
      *  The material system performs the type conversion automatically.
      */
      PROPERTY_TYPE_INFO_FLOAT = 0x1,

      /** The material property is an aiString.
      *
      *  Arrays of strings aren't possible, aiGetMaterialString() (or the
      *  C++-API Material::Get()) *must* be used to query a string property.
      */
      PROPERTY_TYPE_INFO_STRING = 0x3,

      /** Array of (32 Bit) integers
      *
      *  It is possible to use aiGetMaterialFloat[Array]() (or the C++-API
      *  Material::Get()) to query properties stored in integer format.
      *  The material system performs the type conversion automatically.
      */
      PROPERTY_TYPE_INFO_INTEGER = 0x4,


      /** Simple binary buffer, content undefined. Not convertible to anything.
      */
      PROPERTY_TYPE_INFO_BINARY_BUFFER = 0x5,
   };

   /** Data structure for a single material property
   *
   *  As an user, you'll probably never need to deal with this data structure.
   *  Just use the provided aiGetMaterialXXX() or Material::Get() family
   *  of functions to query material properties easily. Processing them
   *  manually is faster, but it is not the recommended way. It isn't worth
   *  the effort. <br>
   *  Material property names follow a simple scheme:
   *  @code
   *    $<name>
   *    ?<name>
   *       A public property, there must be corresponding AI_MATKEY_XXX define
   *       2nd: Public, but ignored by the #aiProcess_RemoveRedundantMaterials
   *       post-processing step.
   *    ~<name>
   *       A temporary property for internal use.
   *  @endcode
   *  @see Material
   */
   struct MaterialProperty
   {
      std::string m_key; // name of the property (key), generally case insensitive
      uint32 m_textureSemantic; // exact usage semantic of a m_texture, for non-m_texture properties, this is always 0
      uint32 m_textureIndex;  // for non-m_texture properties, this member is always 0
      uint32 m_dataNumBytes; // this value may not be 0

      /* Defines the data layout inside the data buffer. This is used
      * by the library internally to perform debug checks and to
      * utilize proper type conversions.
      * (It's probably a hacky solution, but it works.)
      */
      //I don't understand this one... help
      ePropertyTypeInfo m_propertyTypeInfo;

      /**	Binary buffer to hold the property's value.
      * The size of the buffer is always m_dataNumBytes.
      */
      char *m_data;

      MaterialProperty()
         : m_textureSemantic(0),
         m_textureIndex(0),
         m_dataNumBytes(0),
         m_propertyTypeInfo(PROPERTY_TYPE_INFO_FLOAT),
         m_data(NULL)
      {
      }

      ~MaterialProperty()
      {
         delete[] m_data;
      }
   };


   class Material;

   int32 GetMaterialTexture(const Material* mat,
      eTextureType type,
      uint32  index,
      std::string &path,
      eTextureMapping* mapping = NULL,
      uint32* uvindex = NULL,
      float* blend = NULL,
      eTextureOperation* op = NULL,
      eTextureMapMode* mapmode = NULL,
      uint32* flags = NULL);

   uint32 GetMaterialTextureCount(const Material* pMat, eTextureType type);

/* Data structure for a material
*
*  Material data is stored using a key-value structure. A single key-value
*  pair is called a 'material property'. C++ users should use the provided
*  member functions of Material to process material properties, C users
*  have to stick with the aiMaterialGetXXX family of unbound functions.
*  The library defines a set of standard keys (AI_MATKEY_XXX).
*/

   class Material
   {
   private:
      // all loaded material properties loaded
      MaterialProperty** m_ppProperties;

      uint32 m_NumProperties;
      uint32 m_NumAllocated;

   public:
      Material();
      ~Material();

      uint32 GetNumProperties() const { return m_NumProperties; }
      uint32 GetNumAllocated() const { return m_NumAllocated; }

      // NOTE: indirect access now, instead of direct access of m_ppProperties
      MaterialProperty *GetProperty(int32 idx) const { return m_ppProperties[idx]; }

      /* Retrieve an array of Type values with a specific key
      *  from the material
      *
      * @param pKey Key to search for. One of the MATERIAL_KEYNAME_XXX constants.
      * @param type .. set by MATERIAL_KEYNAME_XXX
      * @param idx .. set by MATERIAL_KEYNAME_XXX
      * @param pOut Pointer to a buffer to receive the result.
      * @param pMax Specifies the size of the given buffer, in Type's.
      * Receives the number of values (not bytes!) read.
      * NULL is a valid value for this parameter.
      */
      template <typename Type>
      int32 Get(const char* pKey, uint32 type, uint32 idx, Type* pOut, uint32* pMax) const;

      int32 Get(const char* pKey, uint32 type, uint32 idx, int32* pOut, uint32* pMax) const;
      int32 Get(const char* pKey, uint32 type, uint32 idx, float* pOut, uint32* pMax) const;

      /* Retrieve a Type value with a specific key from the material
      *
      * @param pKey Key to search for. One of the MATERIAL_KEYNAME_XXX constants.
      * @param type Specifies the type of the m_texture to be retrieved (
      *    e.g. diffuse, specular, height map ...)
      * @param idx Index of the m_texture to be retrieved.
      * @param pOut Reference to receive the output value
      */
      template <typename Type>
      int32 Get(const char* pKey, uint32 type, uint32 textureIdx, Type &pOut) const;
      int32 Get(const char* pKey, uint32 type, uint32 textureIdx, int32 &pOut) const;
      int32 Get(const char* pKey, uint32 type, uint32 textureIdx, float &pOut) const;
      int32 Get(const char* pKey, uint32 type, uint32 textureIdx, std::string &pOut) const;
      int32 Get(const char* pKey, uint32 type, uint32 textureIdx, Color3f &pOut) const;
      int32 Get(const char* pKey, uint32 type, uint32 textureIdx, Color4f &pOut) const;
      int32 Get(const char* pKey, uint32 type, uint32 textureIdx, UVTransform &pOut) const;

      /** Get the number of textures for a particular m_texture type.
      *  @param type Texture type to check for
      *  @return Number of textures for this type.
      *  @note A m_texture can be easily queried using #GetTexture() */
      uint32 GetTextureCount(eTextureType type) const
      {
         return GetMaterialTextureCount(this, type);
      }

      /** Helper function to get all parameters pertaining to a
      *  particular m_texture slot from a material.
      *
      *  This function is provided just for convenience, you could also
      *  read the single material properties manually.
*/
      int32 GetTexture(eTextureType type,
         uint32 textureIndex,
         std::string &out_Path,
         eTextureMapping *out_Mapping = NULL,
         uint32 *out_UVIndex = NULL,
         float *out_Blend = NULL,
         eTextureOperation *out_Operation = NULL, // operation to be performed between this and the previous m_texture.
         eTextureMapMode *out_MapMode = NULL) const // if valid pre, it MUST point to an array of 3 (UVW) TEXTUREMAP_MODE
      {
         return GetMaterialTexture(this, type, textureIndex, out_Path, out_Mapping, out_UVIndex, out_Blend, out_Operation, out_MapMode);
      }

      /* Add a property with a given key and type info to the material
      *  structure
      *  @param pKey Key/Usage of the property (AI_MATKEY_XXX)
      *  @param type Set by the AI_MATKEY_XXX macro
      *  @param index Set by the AI_MATKEY_XXX macro
      *  @param pType Type information hint */
      int32 AddBinaryProperty(const void *pInputData, uint32 pSizeInBytes, const char *pKey, uint32 type, uint32 index, ePropertyTypeInfo pType);

      /** @brief Add a string property with a given key and type info to the
      *  material structure
      *  @param pKey Key/Usage of the property (MATERIAL_KEYNAME_XXX)
      *  @param type Set by the MATERIAL_KEYNAME_XXX macro
      *  @param index Set by the MATERIAL_KEYNAME_XXX macro */
      int32 AddProperty(const std::string &pInput, const char *pKey, uint32 type = 0, uint32 index = 0);

      /** @brief Add a property with a given key to the material structure
      *  @param pNumValues Number of values in the array
      *  @param pKey Key/Usage of the property (MATERIAL_KEYNAME_XXX)
      *  @param type Set by the MATERIAL_KEYNAME_XXX macro
      *  @param index Set by the MATERIAL_KEYNAME_XXX macro  */
      template<class TYPE>
      int32 AddProperty(const TYPE *pInputData, uint32 pNumValues, const char* pKey, uint32 type = 0, uint32 index = 0);

      int32 AddProperty(const Vector3f *pInputData, uint32 pNumValues, const char* pKey, uint32 type = 0, uint32 index = 0);

      int32 AddProperty(const Color3f *pInputData, uint32 pNumValues, const char* pKey, uint32 type = 0, uint32 index = 0);

      int32 AddProperty(const Color4f *pInputData, uint32 pNumValues, const char* pKey, uint32 type = 0, uint32 index = 0);

      int32 AddProperty(const int32 *pInputData, uint32 pNumValues, const char* pKey, uint32 type = 0, uint32 index = 0);

      int32 AddProperty(const float *pInputData, uint32 pNumValues, const char* pKey, uint32 type = 0, uint32 index = 0);

      int32 AddProperty(const UVTransform* pInputData, uint32 pNumValues, const char* pKey, uint32 type = 0, uint32 index = 0);

      /** @brief Remove a given key from the list.
      *
      *  The function fails if the key isn't found
      *  @param type Set by the AI_MATKEY_XXX macro
      *  @param index Set by the AI_MATKEY_XXX macro  */
      int32 RemoveProperty(const char* pKey, uint32 type = 0, uint32 index = 0);

      /** @brief Removes all properties from the material.
      *
      *  The data array remains allocated so adding new properties is quite fast.  */
      void Clear();

      /** Copy the property list of a material
      *  @param pcDest Destination material
      *  @param pcSrc Source material
      */
      static void CopyPropertyList(Material* pcDest, const Material* pcSrc);
   };


//#define AI_MATKEY_NAME "?mat.name",0,0
//#define AI_MATKEY_TWOSIDED "$mat.twosided",0,0
//#define AI_MATKEY_SHADING_MODEL "$mat.shadingm",0,0
//#define AI_MATKEY_ENABLE_WIREFRAME "$mat.wireframe",0,0
//#define AI_MATKEY_BLEND_FUNC "$mat.blend",0,0
//#define AI_MATKEY_OPACITY "$mat.opacity",0,0
//#define AI_MATKEY_BUMPSCALING "$mat.bumpscaling",0,0
//#define AI_MATKEY_SHININESS "$mat.shininess",0,0
//#define AI_MATKEY_REFLECTIVITY "$mat.reflectivity",0,0
//#define AI_MATKEY_SHININESS_STRENGTH "$mat.shinpercent",0,0
//#define AI_MATKEY_REFRACTI "$mat.refracti",0,0
//#define AI_MATKEY_COLOR_DIFFUSE "$clr.diffuse",0,0
//#define AI_MATKEY_COLOR_AMBIENT "$clr.ambient",0,0
//#define AI_MATKEY_COLOR_SPECULAR "$clr.specular",0,0
//#define AI_MATKEY_COLOR_EMISSIVE "$clr.emissive",0,0
//#define AI_MATKEY_COLOR_TRANSPARENT "$clr.transparent",0,0
//#define AI_MATKEY_COLOR_REFLECTIVE "$clr.reflective",0,0
//#define AI_MATKEY_GLOBAL_BACKGROUND_IMAGE "?bg.global",0,0

    //extern const char *MATERIAL_KEY_NAME;
    //extern const char *MATERIAL_KEY_TWOSIDED;
    //extern const char *MATERIAL_KEY_SHADING_MODEL;
    //extern const char *MATERIAL_KEY_ENABLE_WIREFRAME;
    //extern const char *MATERIAL_KEY_BLEND_FUNC;
    //extern const char *MATERIAL_KEY_OPACITY;
    //extern const char *MATERIAL_KEY_BUMPSCALING;
    //extern const char *MATERIAL_KEY_SHININESS;
    //extern const char *MATERIAL_KEY_REFLECTIVITY;
    //extern const char *MATERIAL_KEY_SHININESS_STRENGTH;
    //extern const char *MATERIAL_KEY_REFRACTI;
    //extern const char *MATERIAL_KEY_COLOR_DIFFUSE;
    //extern const char *MATERIAL_KEY_COLOR_AMBIENT;
    //extern const char *MATERIAL_KEY_COLOR_SPECULAR;
    //extern const char *MATERIAL_KEY_COLOR_EMISSIVE;
    //extern const char *MATERIAL_KEY_COLOR_TRANSPARENT;
    //extern const char *MATERIAL_KEY_COLOR_REFLECTIVE;
    //extern const char *MATERIAL_KEY_GLOBAL_BACKGROUND_IMAGE;
   const char *MATERIAL_KEY_NAME = "?mat.name";
   const char *MATERIAL_KEY_TWOSIDED = "$mat.twosided";
   const char *MATERIAL_KEY_SHADING_MODEL = "$mat.shadingm";
   const char *MATERIAL_KEY_ENABLE_WIREFRAME = "$mat.wireframe";
   const char *MATERIAL_KEY_BLEND_FUNC = "$mat.blend";
   const char *MATERIAL_KEY_OPACITY = "$mat.opacity";
   const char *MATERIAL_KEY_BUMPSCALING = "$mat.bumpscaling";
   const char *MATERIAL_KEY_SHININESS = "$mat.shininess";
   const char *MATERIAL_KEY_REFLECTIVITY = "$mat.reflectivity";
   const char *MATERIAL_KEY_SHININESS_STRENGTH = "$mat.shinpercent";
   const char *MATERIAL_KEY_REFRACTI = "$mat.refracti";
   const char *MATERIAL_KEY_COLOR_DIFFUSE = "$clr.diffuse";
   const char *MATERIAL_KEY_COLOR_AMBIENT = "$clr.ambient";
   const char *MATERIAL_KEY_COLOR_SPECULAR = "$clr.specular";
   const char *MATERIAL_KEY_COLOR_EMISSIVE = "$clr.emissive";
   const char *MATERIAL_KEY_COLOR_TRANSPARENT = "$clr.transparent";
   const char *MATERIAL_KEY_COLOR_REFLECTIVE = "$clr.reflective";
   const char *MATERIAL_KEY_GLOBAL_BACKGROUND_IMAGE = "?bg.global";



   const char *MATERIAL_KEYNAME_TEXTURE_BASE = "$tex.file";
   const char *MATERIAL_KEYNAME_UVWSRC_BASE = "$tex.uvwsrc";
   const char *MATERIAL_KEYNAME_TEXOP_BASE = "$tex.op";
   const char *MATERIAL_KEYNAME_MAPPING_BASE = "$tex.mapping";
   const char *MATERIAL_KEYNAME_TEXBLEND_BASE = "$tex.blend";
   const char *MATERIAL_KEYNAME_MAPPINGMODE_U_BASE = "$tex.mapmodeu";
   const char *MATERIAL_KEYNAME_MAPPINGMODE_V_BASE = "$tex.mapmodev";
   const char *MATERIAL_KEYNAME_TEXMAP_AXIS_BASE = "$tex.mapaxis";
   const char *MATERIAL_KEYNAME_UVTRANSFORM_BASE = "$tex.uvtrafo";
   const char *MATERIAL_KEYNAME_TEXFLAGS_BASE = "$tex.flags";


   // pure key names for all m_texture-related properties
    //extern const char *MATERIAL_KEYNAME_TEXTURE_BASE;
    //extern const char *MATERIAL_KEYNAME_UVWSRC_BASE;
    //extern const char *MATERIAL_KEYNAME_TEXOP_BASE;
    //extern const char *MATERIAL_KEYNAME_MAPPING_BASE;
    //extern const char *MATERIAL_KEYNAME_TEXBLEND_BASE;
    //extern const char *MATERIAL_KEYNAME_MAPPINGMODE_U_BASE;
    //extern const char *MATERIAL_KEYNAME_MAPPINGMODE_V_BASE;
    //extern const char *MATERIAL_KEYNAME_TEXMAP_AXIS_BASE;
    //extern const char *MATERIAL_KEYNAME_UVTRANSFORM_BASE;
    //extern const char *MATERIAL_KEYNAME_TEXFLAGS_BASE;

   /** @brief Retrieve a material property with a specific key from the material
   *
   * @param pMat Pointer to the input material. May not be NULL
   * @param pKey Key to search for. One of the AI_MATKEY_XXX constants.
   * @param type Specifies the type of the m_texture to be retrieved (
   *    e.g. diffuse, specular, height map ...)
   * @param index Index of the m_texture to be retrieved.
   * @param pPropOut Pointer to receive a pointer to a valid MaterialProperty
   *        structure or NULL if the key has not been found. */

   int32 GetMaterialProperty(const Material* pMat, const char* pKey, uint32 type, uint32 index, const MaterialProperty **ppPropOut);

   /** @brief Retrieve an array of float values with a specific key
   *  from the material
   *
   * Pass one of the AI_MATKEY_XXX constants for the last three parameters (the
   * example reads the #AI_MATKEY_UVTRANSFORM property of the first diffuse m_texture)
   * @code
   * UVTransform trafo;
   * uint32 max = sizeof(UVTransform);
   * if (0 != aiGetMaterialFloatArray(mat, AI_MATKEY_UVTRANSFORM(aiTextureType_DIFFUSE,0),
   *    (float*)&trafo, &max) || sizeof(UVTransform) != max)
   * {
   *   // error handling
   * }
   * @endcode
   *
   * @param pMat Pointer to the input material. May not be NULL
   * @param pKey Key to search for. One of the AI_MATKEY_XXX constants.
   * @param pOut Pointer to a buffer to receive the result.
   * @param pMax Specifies the size of the given buffer, in float's.
   *        Receives the number of values (not bytes!) read.
   * @param type (see the code sample above)
   * @param index (see the code sample above)
   * @return Specifies whether the key has been found. If not, the output
   *   arrays remains unmodified and pMax is set to 0.*/

   int32 GetMaterialFloatArray(const Material* pMat, const char* pKey, uint32 type, uint32 index, float* pOut, uint32* pMax);

   /** @brief Retrieve a single float property with a specific key from the material.
   *
   * Pass one of the AI_MATKEY_XXX constants for the last three parameters (the
   * example reads the #AI_MATKEY_SHININESS_STRENGTH property of the first diffuse m_texture)
   * @code
   * float specStrength = 1.f; // default value, remains unmodified if we fail.
   * aiGetMaterialFloat(mat, AI_MATKEY_SHININESS_STRENGTH,
   *    (float*)&specStrength);
   * @endcode
   *
   * @param pMat Pointer to the input material. May not be NULL
   * @param pKey Key to search for. One of the AI_MATKEY_XXX constants.
   * @param pOut Receives the output float.
   * @param type (see the code sample above)
   * @param index (see the code sample above)
   * @return Specifies whether the key has been found. If not, the output
   *   float remains unmodified.*/

   inline int32 GetMaterialFloat(const Material* pMat, const char* pKey, uint32 type, uint32 index, float* pOut)
   {
      return GetMaterialFloatArray(pMat, pKey, type, index, pOut, (uint32*)0x0);
   }

   /** @brief Retrieve an array of integer values with a specific key
   *  from a material
   *
   * See the sample for aiGetMaterialFloatArray for more information.*/
   int32 GetMaterialIntegerArray(const Material* pMat,
      const char* pKey,
      uint32  type,
      uint32  index,
      int32* pOut,
      uint32* pMax);

   /** @brief Retrieve an integer property with a specific key from a material
   *
   * See the sample for aiGetMaterialFloat for more information.*/
   inline int32 GetMaterialInteger(const Material* pMat,
      const char* pKey,
      uint32 type,
      uint32 index,
      int32* pOut)
   {
      return GetMaterialIntegerArray(pMat, pKey, type, index, pOut, (uint32*)0x0);
   }


   /** @brief Retrieve a color value from the material property table
   *
   * See the sample for aiGetMaterialFloat for more information*/

   int32 GetMaterialColor(const Material* pMat, const char* pKey, uint32 type, uint32 index, Color4f* pOut);

   /** @brief Retrieve a UVTransform value from the material property table
   *
   * See the sample for aiGetMaterialFloat for more information*/

   int32 GetMaterialUVTransform(const Material* pMat,
      const char* pKey,
      uint32 type,
      uint32 index,
      UVTransform* pOut);

   /** @brief Retrieve a string from the material property table
   *
   * See the sample for aiGetMaterialFloat for more information.*/

   int32 GetMaterialString(const Material* pMat,
      const char* pKey,
      uint32 type,
      uint32 index,
      std::string &pOut);

   /** Get the number of textures for a particular m_texture type.
   *  @param[in] pMat Pointer to the input material. May not be NULL
   *  @param type Texture type to check for
   *  @return Number of textures for this type.
   *  @note A m_texture can be easily queried using #aiGetMaterialTexture() */

   /** @brief Helper function to get all values pertaining to a particular
   *  m_texture slot from a material structure.
   *
   *  This function is provided just for convenience. You could also read the
   *  m_texture by parsing all of its properties manually. This function bundles
   *  all of them in a huge function monster.
   *
   *  @param[in] mat Pointer to the input material. May not be NULL
   *  @param[in] type Specifies the m_texture stack to read from (e.g. diffuse,
   *     specular, height map ...).
   *  @param[in] index Index of the m_texture. The function fails if the
   *     requested index is not available for this m_texture type.
   *     #aiGetMaterialTextureCount() can be used to determine the number of
   *     textures in a particular m_texture stack.
   *  @param[out] path Receives the output path
   *      This parameter must be non-null.
   *  @param mapping The m_texture mapping mode to be used.
   *      Pass NULL if you're not interested in this information.
   *  @param[out] uvindex For UV-mapped textures: receives the index of the UV
   *      source channel. Unmodified otherwise.
   *      Pass NULL if you're not interested in this information.
   *  @param[out] blend Receives the blend factor for the m_texture
   *      Pass NULL if you're not interested in this information.
   *  @param[out] op Receives the m_texture blend operation to be perform between
   *		this m_texture and the previous m_texture.
   *      Pass NULL if you're not interested in this information.
   *  @param[out] mapmode Receives the mapping modes to be used for the m_texture.
   *      Pass NULL if you're not interested in this information. Otherwise,
   *      pass a pointer to an array of two aiTextureMapMode's (one for each
   *      axis, UV order).
   *  @param[out] flags Receives the the m_texture flags.
   *  @return 0 on success, otherwise something else. Have fun.*/

   //inline int32 Material::GetTexture(eTextureType type,
   //   uint32  index,
   //   std::string path,
   //   eTextureMapping* mapping	/*= NULL*/,
   //   uint32* uvindex		/*= NULL*/,
   //   float* blend				   /*= NULL*/,
   //   eTextureOperation* op				/*= NULL*/,
   //   eTextureMapMode* mapmode	/*= NULL*/) const
   //{
   //   return ::GetMaterialTexture(this, type, index, path, mapping, uvindex, blend, op, mapmode);
   //}

   //inline uint32 Material::GetTextureCount(aiTextureType type) const
   //{
   //   return ::aiGetMaterialTextureCount(this, type);
   //}

   template <typename Type>
   inline int32 Material::Get(const char* pKey, uint32 type,
      uint32 idx, Type* pOut,
      uint32* pMax) const
   {
      uint32 iNum = pMax ? *pMax : 1;

      const MaterialProperty* prop;
      const int32 ret = ::GetMaterialProperty(this, pKey, type, idx,
         (const MaterialProperty**)&prop);
      if (0 == ret)	{

         if (prop->m_dataNumBytes < sizeof(Type)*iNum) {
            return -1;
         }

         if (prop->m_propertyTypeInfo != PROPERTY_TYPE_INFO_BINARY_BUFFER) {
            return -1;
         }

         iNum = std::min((size_t)iNum, prop->m_dataNumBytes / sizeof(Type));
         ::memcpy(pOut, prop->m_data, iNum * sizeof(Type));
         if (pMax) {
            *pMax = iNum;
         }
      }
      return ret;
   }

   template <typename Type>
   inline int32 Material::Get(const char* pKey, uint32 type,
      uint32 idx, Type& pOut) const
   {
      const MaterialProperty* prop;
      const int32 ret = ::GetMaterialProperty(this, pKey, type, idx,
         (const MaterialProperty**)&prop);
      if (0 == ret)	{

         if (prop->m_dataNumBytes < sizeof(Type)) {
            return -1;
         }

         if (prop->m_propertyTypeInfo != PROPERTY_TYPE_INFO_BINARY_BUFFER) {
            return -1;
         }

         ::memcpy(&pOut, prop->m_data, sizeof(Type));
      }
      return ret;
   }

   inline int32 Material::Get(const char* pKey, uint32 type, uint32 idx, float *pOut, uint32* pMax) const
   {
      return GetMaterialFloatArray(this, pKey, type, idx, pOut, pMax);
   }

   inline int32 Material::Get(const char* pKey, uint32 type, uint32 idx, int32 *pOut, uint32* pMax) const
   {
      return GetMaterialIntegerArray(this, pKey, type, idx, pOut, pMax);
   }

   inline int32 Material::Get(const char* pKey, uint32 type, uint32 idx, float &pOut) const
   {
      return GetMaterialFloat(this, pKey, type, idx, &pOut);
   }

   inline int32 Material::Get(const char* pKey, uint32 type,
      uint32 idx, int32 &pOut) const
   {
      return GetMaterialInteger(this, pKey, type, idx, &pOut);
   }

   inline int32 Material::Get(const char* pKey, uint32 type,
      uint32 idx, Color4f &pOut) const
   {
      return GetMaterialColor(this, pKey, type, idx, &pOut);
   }

   inline int32 Material::Get(const char* pKey, uint32 type,
      uint32 idx, Color3f &pOut) const
   {
      Color4f c;
      const int32 ret = GetMaterialColor(this, pKey, type, idx, &c);
      pOut = Color3f(c.r, c.g, c.b);
      return ret;
   }

   inline int32 Material::Get(const char* pKey, uint32 type, uint32 idx, std::string &pOut) const
   {
      return GetMaterialString(this, pKey, type, idx, pOut);
   }

   inline int32 Material::Get(const char* pKey, uint32 type, uint32 idx, UVTransform &pOut) const
   {
      return GetMaterialUVTransform(this, pKey, type, idx, &pOut);
   }

   template<class TYPE>
   int32 Material::AddProperty(const TYPE* pInput,
      const uint32 pNumValues,
      const char* pKey,
      uint32 type,
      uint32 index)
   {
      return AddBinaryProperty((const void*)pInput,
         pNumValues * sizeof(TYPE),
         pKey, type, index, PROPERTY_TYPE_INFO_BINARY_BUFFER);
   }

   inline int32 Material::AddProperty(const float* pInput,
      const uint32 pNumValues,
      const char* pKey,
      uint32 type,
      uint32 index)
   {
      return AddBinaryProperty((const void*)pInput,
         pNumValues * sizeof(float),
         pKey, type, index, PROPERTY_TYPE_INFO_FLOAT);
   }


   inline int32 Material::AddProperty(const UVTransform* pInput,
      const uint32 pNumValues,
      const char* pKey,
      uint32 type,
      uint32 index)
   {
      return AddBinaryProperty((const void*)pInput,
         pNumValues * sizeof(UVTransform),
         pKey, type, index, PROPERTY_TYPE_INFO_FLOAT);
   }

   inline int32 Material::AddProperty(const Color4f *pInput, uint32 numValues, const char* pKey, uint32 type, uint32 index)
   {
      return AddBinaryProperty((const void*)pInput, numValues * sizeof(Color4f),
         pKey, type, index, PROPERTY_TYPE_INFO_FLOAT);
   }

   inline int32 Material::AddProperty(const Color3f* pInput, uint32 numValues, const char* pKey, uint32 type, uint32 index)
   {
      return AddBinaryProperty((const void*)pInput, numValues * sizeof(Color3f),
         pKey, type, index, PROPERTY_TYPE_INFO_FLOAT);
   }

   inline int32 Material::AddProperty(const Vector3f* pInput, uint32 numValues, const char* pKey, uint32 type, uint32 index)
   {
      return AddBinaryProperty((const void*)pInput, numValues * sizeof(Vector3f),
         pKey, type, index, PROPERTY_TYPE_INFO_FLOAT);
   }

   inline int32 Material::AddProperty(const int32* pInput, uint32 numValues, const char* pKey, uint32 type, uint32 index)
   {
      return AddBinaryProperty((const void*)pInput, numValues * sizeof(int32),
         pKey, type, index, PROPERTY_TYPE_INFO_INTEGER);
   }


   // The template specializations below are for backwards compatibility.
   // The recommended way to add material properties is using the non-template
   // overloads.
   template<>
   inline int32 Material::AddProperty<float>(const float* pInput,
      const uint32 pNumValues,
      const char* pKey,
      uint32 type,
      uint32 index)
   {
      return AddBinaryProperty((const void*)pInput,
         pNumValues * sizeof(float),
         pKey, type, index, PROPERTY_TYPE_INFO_FLOAT);
   }

   template<>
   inline int32 Material::AddProperty<UVTransform>(const UVTransform* pInput,
      const uint32 pNumValues,
      const char* pKey,
      uint32 type,
      uint32 index)
   {
      return AddBinaryProperty((const void*)pInput,
         pNumValues * sizeof(UVTransform),
         pKey, type, index, PROPERTY_TYPE_INFO_FLOAT);
   }

   template<>
   inline int32 Material::AddProperty<Color4f>(const Color4f* pInput,
      const uint32 pNumValues,
      const char* pKey,
      uint32 type,
      uint32 index)
   {
      return AddBinaryProperty((const void*)pInput,
         pNumValues * sizeof(Color4f),
         pKey, type, index, PROPERTY_TYPE_INFO_FLOAT);
   }

   template<>
   inline int32 Material::AddProperty<Color3f>(const Color3f* pInput,
      const uint32 pNumValues,
      const char* pKey,
      uint32 type,
      uint32 index)
   {
      return AddBinaryProperty((const void*)pInput,
         pNumValues * sizeof(Color3f),
         pKey, type, index, PROPERTY_TYPE_INFO_FLOAT);
   }

   template<>
   inline int32 Material::AddProperty<Vector3f>(const Vector3f* pInput,
      const uint32 pNumValues,
      const char* pKey,
      uint32 type,
      uint32 index)
   {
      return AddBinaryProperty((const void*)pInput,
         pNumValues * sizeof(Vector3f),
         pKey, type, index, PROPERTY_TYPE_INFO_FLOAT);
   }

   template<>
   inline int32 Material::AddProperty<int32>(const int32* pInput,
      const uint32 pNumValues,
      const char* pKey,
      uint32 type,
      uint32 index)
   {
      return AddBinaryProperty((const void*)pInput,
         pNumValues * sizeof(int32),
         pKey, type, index, PROPERTY_TYPE_INFO_INTEGER);
   }

} // namespace material

#endif
