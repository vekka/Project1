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

//#include "../core/BasicTypes.hpp"
#include "../core/math/vector3.hpp"
#include "../core/math/vector2.hpp"
using core::math::Vector2f;
using core::math::Vector3f;

#include <string>

#include "../core/math/matrix3.hpp"
#include "../core/math/matrix4.hpp"

#include "../gfx/color.hpp"
using gfx::color::Colorf;

namespace material
{
   // Name for default materials (2nd is used if meshes have UV coords)
   const char *DEFAULT_MATERIAL_NAME = "DefaultMaterial";

   // Defines how the Nth texture of a specific type is combined with
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
      TEXTURE_OPER_SIGNED_ADD, // T = T1 + (T2-0.5)
#ifndef SWIG
      TEXTURE_OPER_FORCE32BIT = INT32_MAX
#endif
   };

   // Defines how UV coordinates outside the [0...1] range are handled.
   //  Commonly refered to as 'wrapping mode'.

   enum eTextureMapMode
   {
      TEXTURE_MAPMODE_WRAP, // A texture coordinate u|v is translated to u%1|v%1
      TEXTURE_MAPMODE_CLAMP, // Texture coordinates outside [0...1] are clamped to the nearest valid value.
      TEXTURE_MAPMODE_DECAL, // If the texture pixel coords are outside [0...1] the texture is not applied to that pixel
      TEXTURE_MAPMODE_MIRROR, // A texture coord u|v becomes u%1|v%1 if (u-(u%1))%2 is 0 and 1-(u%1)|1-(v%1) otherwise
   };

   /** @brief Defines how the mapping coords for a texture are generated.
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
      //  the texture coordinates are to be taken from (remember,
      //  meshes can have more than one UV channel).
      
      TEXTURE_MAP_UV,
      TEXTURE_MAP_SPHERE, // Spherical mapping
      TEXTURE_MAP_CYLINDER, // Cylindrical mapping
      TEXTURE_MAP_BOX, // Cubic mapping
      TEXTURE_MAP_PLANE, // Planar mapping
      TEXTURE_MAP_OTHER, // Undefined mapping. Have fun.
   };

   // ---------------------------------------------------------------------------
   /** @brief Defines the purpose of a texture
   *
   *  This is a very difficult topic. Different 3D packages support different
   *  kinds of textures. For very common texture types, such as bumpmaps, the
   *  rendering results depend on implementation details in the rendering
   *  pipelines of these applications. Assimp loads all texture references from
   *  the model file and tries to determine which of the predefined texture
   *  types below is the best choice to match the original use of the texture
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
      *  No texture, but the value to be used as 'texture semantic'
      *  (#MaterialProperty::mSemantic) for all material properties
      *  *not* related to textures.
      */
      TEXTURE_TYPE_NONE,
      TEXTURE_TYPE_DIFFUSE, // texture is combined with the result of the diffuse lighting equation
      TEXTURE_TYPE_SPECULAR, // texture is combined with the result of the specular lighting equation
      TEXTURE_TYPE_AMBIENT, // texture is combined with the result of the ambient lighting equation
      TEXTURE_TYPE_EMISSIVE, // texture is added to the result of the lighting calculation. no influence by incoming light
      TEXTURE_TYPE_HEIGHTMAP, // texture is a height map. higher gray-scale values stand for higher elevations from the base height
      TEXTURE_TYPE_NORMALS, // texture is a (tangent space) normal-map. intentionally no dinstinction between the different conventions
      TEXTURE_TYPE_SHININESS, // texture defines the glossiness of the material. exp of (phong) lighting equation
      TEXTURE_TYPE_OPACITY, // texture defines per-pixel opacity. Usually 'white' means opaque and 'black' means 'transparency'
      TEXTURE_TYPE_DISPLACEMENT, // purpose and format is application-dependent. higher col values = higher vert displacements

      /*
      *  Both 'Lightmaps' and dedicated 'ambient occlusion maps' are
      *  covered by this material property. The texture contains a
      *  scaling value for the final color value of a pixel. Its
      *  intensity is not affected by incoming light.
      */
      TEXTURE_TYPE_LIGHTMAP, // aka Ambient Occlusion
      TEXTURE_TYPE_REFLECTION, // contains the color of a perfect mirror reflection. rarely used, almost never for real-time applications.

      /** Unknown texture
      *
      *  A texture reference that does not match any of the definitions
      *  above is considered to be 'unknown'. It is still imported,
      *  but is excluded from any further postprocessing.
      */
      TEXTURE_TYPE_UNKNOWN,
   };

#define AI_TEXTURE_TYPE_MAX  aiTextureType_UNKNOWN

   // ---------------------------------------------------------------------------
   /** @brief Defines all shading models supported by the library
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
      /** Flat shading. Shading is done on per-face base,
      *  diffuse only. Also known as 'faceted shading'.
      */
      SHADING_MODE_FLAT,

      /** Simple Gouraud shading.
      */
      SHADING_MODE_GOURAUD,

      /** Phong-Shading -
      */
      SHADING_MODE_PHONG,

      /** Phong-Blinn-Shading
      */
      SHADING_MODE_BLINN,

      /** Toon-Shading per pixel
      *
      *  Also known as 'comic' shader.
      */
      SHADING_MODE_TOON,

      /** OrenNayar-Shading per pixel
      *
      *  Extension to standard Lambertian shading, taking the
      *  roughness of the material into account
      */
      SHADING_MODE_ORENNAYAR,

      /** Minnaert-Shading per pixel
      *
      *  Extension to standard Lambertian shading, taking the
      *  "darkness" of the material into account
      */
      SHADING_MODE_MINNAERT,

      /** CookTorrance-Shading per pixel
      *
      *  Special shader for metallic surfaces.
      */
      SHADING_MODE_COOKTORRANCE,

      /** No shading at all. Constant light influence of 1.0.
      */
      SHADING_MODE_NOSHADING,

      /** Fresnel shading
      */
      SHADING_MODE_FRESNEL,


#ifndef SWIG
      SHADING_MODE_FORCE32BIT = INT32_MAX
#endif
   };


   // ---------------------------------------------------------------------------
   /** @brief Defines some mixed flags for a particular texture.
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
      /** The texture's color values have to be inverted (componentwise 1-n)
      */
      TEXTURE_FLAGS_INVERT = 1,

      /** Explicit request to the application to process the alpha channel
      *  of the texture.
      *
      *  Mutually exclusive with #aiTextureFlags_IgnoreAlpha. These
      *  flags are set if the library can say for sure that the alpha
      *  channel is used/is not used. If the model format does not
      *  define this, it is left to the application to decide whether
      *  the texture alpha channel - if any - is evaluated or not.
      */
      TEXTURE_FLAGS_USEALPHA = 2,

      /** Explicit request to the application to ignore the alpha channel
      *  of the texture.
      *
      *  Mutually exclusive with #aiTextureFlags_UseAlpha.
      */
      TEXTURE_FLAGS_IGNOREALPHA = 4,
   };

   /** @brief Defines alpha-blend flags.
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
      /**
      *  Formula:
      *  @code
      *  SourceColor*SourceAlpha + DestColor*(1-SourceAlpha)
      *  @endcode
      */
      BLEND_MODE_DEFAULT,

      /** Additive blending
      *
      *  Formula:
      *  @code
      *  SourceColor*1 + DestColor*1
      *  @endcode
      */
      BLEND_MODE_ADDITIVE,

      // we don't need more for the moment, but we might need them
      // in future versions ...
   };


#include "../core/pushpack1.hpp"

   // ---------------------------------------------------------------------------
   /** @brief Defines how an UV channel is transformed.
   *
   *  This is just a helper structure for the #AI_MATKEY_UVTRANSFORM key.
   *  See its documentation for more details.
   *
   *  Typically you'll want to build a matrix of this information. However,
   *  we keep separate scaling/translation/rotation values to make it
   *  easier to process and optimize UV transformations internally.
   */
   struct UVTransform
   {
      /** Translation on the u and v axes.
      *
      *  The default value is (0|0).
      */
      Vector2f m_translation;

      /** Scaling on the u and v axes.
      *
      *  The default value is (1|1).
      */
     Vector2f m_scaling;

      /** Rotation - in counter-clockwise direction.
      *
      *  The rotation angle is specified in radians. The
      *  rotation center is 0.5f|0.5f. The default value
      *  0.f.
      */
      float m_rotation;

      UVTransform() : m_scaling(1.f, 1.f), m_rotation(0.f)
      {
      	// nothing to be done here
      }

   } PACK_STRUCT;

#include "../core/poppack1.hpp"

   //! @cond AI_DOX_INCLUDE_INTERNAL
   // ---------------------------------------------------------------------------
   /** @brief A very primitive RTTI system for the contents of material
   *  properties.
   */
   enum ePropertyTypeInfo
   {
      /** Array of single-precision (32 Bit) floats
      *
      *  It is possible to use aiGetMaterialInteger[Array]() (or the C++-API
      *  aiMaterial::Get()) to query properties stored in floating-point format.
      *  The material system performs the type conversion automatically.
      */
      PROPERTY_TYPE_INFO_FLOAT = 0x1,

      /** The material property is an aiString.
      *
      *  Arrays of strings aren't possible, aiGetMaterialString() (or the
      *  C++-API aiMaterial::Get()) *must* be used to query a string property.
      */
      PROPERTY_TYPE_INFO_STRING = 0x3,

      /** Array of (32 Bit) integers
      *
      *  It is possible to use aiGetMaterialFloat[Array]() (or the C++-API
      *  aiMaterial::Get()) to query properties stored in integer format.
      *  The material system performs the type conversion automatically.
      */
      PROPERTY_TYPE_INFO_INTEGER = 0x4,


      /** Simple binary buffer, content undefined. Not convertible to anything.
      */
      PROPERTY_TYPE_INFO_BINARY_BUFFER = 0x5,
   };

   // ---------------------------------------------------------------------------
   /** @brief Data structure for a single material property
   *
   *  As an user, you'll probably never need to deal with this data structure.
   *  Just use the provided aiGetMaterialXXX() or aiMaterial::Get() family
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
   *  @see aiMaterial
   */
   struct MaterialProperty
   {
      /** Specifies the name of the property (key)
      *  Keys are generally case insensitive.
      */
      std::string m_key;

      /** Textures: Specifies their exact usage semantic.
      * For non-texture properties, this member is always 0
      * (or, better-said, #aiTextureType_NONE).
      */
      uint32 m_semantic;

      /** Textures: Specifies the index of the texture.
      *  For non-texture properties, this member is always 0.
      */
      uint32 m_index;

      /**	Size of the buffer mData is pointing to, in bytes.
      *  This value may not be 0.
      */
      uint32 m_dataLength;

      /** Type information for the property.
      *
      * Defines the data layout inside the data buffer. This is used
      * by the library internally to perform debug checks and to
      * utilize proper type conversions.
      * (It's probably a hacky solution, but it works.)
      */

      //I don't understand this one... help
      ePropertyTypeInfo m_type;

      /**	Binary buffer to hold the property's value.
      * The size of the buffer is always mDataLength.
      */
      char* m_data;

      MaterialProperty()
         : mSemantic(0),
         mIndex(0),
         mDataLength(0),
         mType(aiPTI_Float),
         mData(NULL)
      {
      }

      ~MaterialProperty()
      {
         delete[] m_data;
      }
   };
   //! @endcond

/** @brief Data structure for a material
*
*  Material data is stored using a key-value structure. A single key-value
*  pair is called a 'material property'. C++ users should use the provided
*  member functions of aiMaterial to process material properties, C users
*  have to stick with the aiMaterialGetXXX family of unbound functions.
*  The library defines a set of standard keys (AI_MATKEY_XXX).
*/

struct Material
{
public:
   Material();

   // -------------------------------------------------------------------
   /** @brief Retrieve an array of Type values with a specific key
   *  from the material
   *
   * @param pKey Key to search for. One of the AI_MATKEY_XXX constants.
   * @param type .. set by AI_MATKEY_XXX
   * @param idx .. set by AI_MATKEY_XXX
   * @param pOut Pointer to a buffer to receive the result.
   * @param pMax Specifies the size of the given buffer, in Type's.
   * Receives the number of values (not bytes!) read.
   * NULL is a valid value for this parameter.
   */
   template <typename Type>
   int32 Get(const char* pKey, uint32 type,
      uint32 idx, Type* pOut, uint32* pMax) const;

   int32 Get(const char* pKey, uint32 type,
      uint32 idx, int* pOut, uint32* pMax) const;

   int32 Get(const char* pKey, uint32 type,
      uint32 idx, float* pOut, uint32* pMax) const;

   // -------------------------------------------------------------------
   /** @brief Retrieve a Type value with a specific key
   *  from the material
   *
   * @param pKey Key to search for. One of the AI_MATKEY_XXX constants.
   * @param type Specifies the type of the texture to be retrieved (
   *    e.g. diffuse, specular, height map ...)
   * @param idx Index of the texture to be retrieved.
   * @param pOut Reference to receive the output value
   */
   template <typename Type>
   int32 Get(const char* pKey, uint32 type, uint32 idx, Type& pOut) const;
   int32 Get(const char* pKey, uint32 type, uint32 idx, int32 &pOut) const;
   int32 Get(const char* pKey, uint32 type, uint32 idx, float& pOut) const;
   int32 Get(const char* pKey, uint32 type, uint32 idx, aiString& pOut) const;
   int32 Get(const char* pKey, uint32 type, uint32 idx, aiColor3D& pOut) const;
   int32 Get(const char* pKey, uint32 type, uint32 idx, Colorf& pOut) const;
   int32 Get(const char* pKey, uint32 type, uint32 idx, UVTransform& pOut) const;

   // -------------------------------------------------------------------
   /** Get the number of textures for a particular texture type.
   *  @param type Texture type to check for
   *  @return Number of textures for this type.
   *  @note A texture can be easily queried using #GetTexture() */
   uint32 GetTextureCount(aiTextureType type) const;

   // -------------------------------------------------------------------
   /** Helper function to get all parameters pertaining to a
   *  particular texture slot from a material.
   *
   *  This function is provided just for convenience, you could also
   *  read the single material properties manually.
   *  @param type Specifies the type of the texture to be retrieved (
   *    e.g. diffuse, specular, height map ...)
   *  @param index Index of the texture to be retrieved. The function fails
   *    if there is no texture of that type with this index.
   *    #GetTextureCount() can be used to determine the number of textures
   *    per texture type.
   *  @param path Receives the path to the texture.
   *	 NULL is a valid value.
   *  @param mapping The texture mapping.
   *		NULL is allowed as value.
   *  @param uvindex Receives the UV index of the texture.
   *    NULL is a valid value.
   *  @param blend Receives the blend factor for the texture
   *	 NULL is a valid value.
   *  @param op Receives the texture operation to be performed between
   *	 this texture and the previous texture. NULL is allowed as value.
   *  @param mapmode Receives the mapping modes to be used for the texture.
   *    The parameter may be NULL but if it is a valid pointer it MUST
   *    point to an array of 3 aiTextureMapMode's (one for each
   *    axis: UVW order (=XYZ)).
   */
   // -------------------------------------------------------------------
   int32 GetTexture(aiTextureType type,
      uint32  index,
      std::string aiString* path,
      eTextureMapping* mapping = NULL,
      uint32* uvindex = NULL,
      float* blend = NULL,
      eTextureOperation* op = NULL,
      eTextureMapMode* mapmode = NULL) const;


   // Setters


   // ------------------------------------------------------------------------------
   /** @brief Add a property with a given key and type info to the material
   *  structure
   *
   *  @param pInput Pointer to input data
   *  @param pSizeInBytes Size of input data
   *  @param pKey Key/Usage of the property (AI_MATKEY_XXX)
   *  @param type Set by the AI_MATKEY_XXX macro
   *  @param index Set by the AI_MATKEY_XXX macro
   *  @param pType Type information hint */
   int32 AddBinaryProperty(const void* pInput,
      uint32 pSizeInBytes,
      const char* pKey,
      uint32 type,
      uint32 index,
      ePropertyTypeInfo pType);

   // ------------------------------------------------------------------------------
   /** @brief Add a string property with a given key and type info to the
   *  material structure
   *
   *  @param pInput Input string
   *  @param pKey Key/Usage of the property (AI_MATKEY_XXX)
   *  @param type Set by the AI_MATKEY_XXX macro
   *  @param index Set by the AI_MATKEY_XXX macro */
   int32 AddProperty(const std::string* pInput,
      const char* pKey,
      uint32 type = 0,
      uint32 index = 0);

   // ------------------------------------------------------------------------------
   /** @brief Add a property with a given key to the material structure
   *  @param pInput Pointer to the input data
   *  @param pNumValues Number of values in the array
   *  @param pKey Key/Usage of the property (AI_MATKEY_XXX)
   *  @param type Set by the AI_MATKEY_XXX macro
   *  @param index Set by the AI_MATKEY_XXX macro  */
   template<class TYPE>
   int32 AddProperty(const TYPE* pInput,
      uint32 pNumValues,
      const char* pKey,
      uint32 type = 0,
      uint32 index = 0);

   int32 AddProperty(const Vector3f* pInput,
      uint32 pNumValues,
      const char* pKey,
      uint32 type = 0,
      uint32 index = 0);

   int32 AddProperty(const aiColor3D* pInput,
      uint32 pNumValues,
      const char* pKey,
      uint32 type = 0,
      uint32 index = 0);

   int32 AddProperty(const aiColor4D* pInput,
      uint32 pNumValues,
      const char* pKey,
      uint32 type = 0,
      uint32 index = 0);

   int32 AddProperty(const int32 *pInput,
      uint32 pNumValues,
      const char* pKey,
      uint32 type = 0,
      uint32 index = 0);

   int32 AddProperty(const float *pInput,
      uint32 pNumValues,
      const char* pKey,
      uint32 type = 0,
      uint32 index = 0);

   int32 AddProperty(const UVTransform* pInput,
      uint32 pNumValues,
      const char* pKey,
      uint32 type = 0,
      uint32 index = 0);

   // ------------------------------------------------------------------------------
   /** @brief Remove a given key from the list.
   *
   *  The function fails if the key isn't found
   *  @param pKey Key to be deleted
   *  @param type Set by the AI_MATKEY_XXX macro
   *  @param index Set by the AI_MATKEY_XXX macro  */
   int32 RemoveProperty(const char* pKey,
      uint32 type = 0,
      uint32 index = 0);

   // ------------------------------------------------------------------------------
   /** @brief Removes all properties from the material.
   *
   *  The data array remains allocated so adding new properties is quite fast.  */
   void Clear();

   // ------------------------------------------------------------------------------
   /** Copy the property list of a material
   *  @param pcDest Destination material
   *  @param pcSrc Source material
   */
   static void CopyPropertyList(Material* pcDest, const aiMaterial* pcSrc);

   /** List of all material properties loaded. */
   MaterialProperty** m_ppProperties;

   /** Number of properties in the data base */
   uint32 m_NumProperties;

   /** Storage allocated */
   uint32 m_NumAllocated;
};

#define AI_MATKEY_NAME "?mat.name",0,0
#define AI_MATKEY_TWOSIDED "$mat.twosided",0,0
#define AI_MATKEY_SHADING_MODEL "$mat.shadingm",0,0
#define AI_MATKEY_ENABLE_WIREFRAME "$mat.wireframe",0,0
#define AI_MATKEY_BLEND_FUNC "$mat.blend",0,0
#define AI_MATKEY_OPACITY "$mat.opacity",0,0
#define AI_MATKEY_BUMPSCALING "$mat.bumpscaling",0,0
#define AI_MATKEY_SHININESS "$mat.shininess",0,0
#define AI_MATKEY_REFLECTIVITY "$mat.reflectivity",0,0
#define AI_MATKEY_SHININESS_STRENGTH "$mat.shinpercent",0,0
#define AI_MATKEY_REFRACTI "$mat.refracti",0,0
#define AI_MATKEY_COLOR_DIFFUSE "$clr.diffuse",0,0
#define AI_MATKEY_COLOR_AMBIENT "$clr.ambient",0,0
#define AI_MATKEY_COLOR_SPECULAR "$clr.specular",0,0
#define AI_MATKEY_COLOR_EMISSIVE "$clr.emissive",0,0
#define AI_MATKEY_COLOR_TRANSPARENT "$clr.transparent",0,0
#define AI_MATKEY_COLOR_REFLECTIVE "$clr.reflective",0,0
#define AI_MATKEY_GLOBAL_BACKGROUND_IMAGE "?bg.global",0,0

   // ---------------------------------------------------------------------------
   // Pure key names for all texture-related properties
   //! @cond MATS_DOC_FULL
#define _AI_MATKEY_TEXTURE_BASE			"$tex.file"
#define _AI_MATKEY_UVWSRC_BASE			"$tex.uvwsrc"
#define _AI_MATKEY_TEXOP_BASE			"$tex.op"
#define _AI_MATKEY_MAPPING_BASE			"$tex.mapping"
#define _AI_MATKEY_TEXBLEND_BASE		"$tex.blend"
#define _AI_MATKEY_MAPPINGMODE_U_BASE	"$tex.mapmodeu"
#define _AI_MATKEY_MAPPINGMODE_V_BASE	"$tex.mapmodev"
#define _AI_MATKEY_TEXMAP_AXIS_BASE		"$tex.mapaxis"
#define _AI_MATKEY_UVTRANSFORM_BASE		"$tex.uvtrafo"
#define _AI_MATKEY_TEXFLAGS_BASE		"$tex.flags"
   //! @endcond

   // ---------------------------------------------------------------------------
#define AI_MATKEY_TEXTURE(type, N) _AI_MATKEY_TEXTURE_BASE,type,N

   // For backward compatibility and simplicity
   //! @cond MATS_DOC_FULL
#define AI_MATKEY_TEXTURE_DIFFUSE(N)	\
	AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE,N)

#define AI_MATKEY_TEXTURE_SPECULAR(N)	\
	AI_MATKEY_TEXTURE(aiTextureType_SPECULAR,N)

#define AI_MATKEY_TEXTURE_AMBIENT(N)	\
	AI_MATKEY_TEXTURE(aiTextureType_AMBIENT,N)

#define AI_MATKEY_TEXTURE_EMISSIVE(N)	\
	AI_MATKEY_TEXTURE(aiTextureType_EMISSIVE,N)

#define AI_MATKEY_TEXTURE_NORMALS(N)	\
	AI_MATKEY_TEXTURE(aiTextureType_NORMALS,N)

#define AI_MATKEY_TEXTURE_HEIGHT(N)	\
	AI_MATKEY_TEXTURE(aiTextureType_HEIGHT,N)

#define AI_MATKEY_TEXTURE_SHININESS(N)	\
	AI_MATKEY_TEXTURE(aiTextureType_SHININESS,N)

#define AI_MATKEY_TEXTURE_OPACITY(N)	\
	AI_MATKEY_TEXTURE(aiTextureType_OPACITY,N)

#define AI_MATKEY_TEXTURE_DISPLACEMENT(N)	\
	AI_MATKEY_TEXTURE(aiTextureType_DISPLACEMENT,N)

#define AI_MATKEY_TEXTURE_LIGHTMAP(N)	\
	AI_MATKEY_TEXTURE(aiTextureType_LIGHTMAP,N)

#define AI_MATKEY_TEXTURE_REFLECTION(N)	\
	AI_MATKEY_TEXTURE(aiTextureType_REFLECTION,N)

   //! @endcond

   // ---------------------------------------------------------------------------
#define AI_MATKEY_UVWSRC(type, N) _AI_MATKEY_UVWSRC_BASE,type,N

   // For backward compatibility and simplicity
   //! @cond MATS_DOC_FULL
#define AI_MATKEY_UVWSRC_DIFFUSE(N)	\
	AI_MATKEY_UVWSRC(aiTextureType_DIFFUSE,N)

#define AI_MATKEY_UVWSRC_SPECULAR(N)	\
	AI_MATKEY_UVWSRC(aiTextureType_SPECULAR,N)

#define AI_MATKEY_UVWSRC_AMBIENT(N)	\
	AI_MATKEY_UVWSRC(aiTextureType_AMBIENT,N)

#define AI_MATKEY_UVWSRC_EMISSIVE(N)	\
	AI_MATKEY_UVWSRC(aiTextureType_EMISSIVE,N)

#define AI_MATKEY_UVWSRC_NORMALS(N)	\
	AI_MATKEY_UVWSRC(aiTextureType_NORMALS,N)

#define AI_MATKEY_UVWSRC_HEIGHT(N)	\
	AI_MATKEY_UVWSRC(aiTextureType_HEIGHT,N)

#define AI_MATKEY_UVWSRC_SHININESS(N)	\
	AI_MATKEY_UVWSRC(aiTextureType_SHININESS,N)

#define AI_MATKEY_UVWSRC_OPACITY(N)	\
	AI_MATKEY_UVWSRC(aiTextureType_OPACITY,N)

#define AI_MATKEY_UVWSRC_DISPLACEMENT(N)	\
	AI_MATKEY_UVWSRC(aiTextureType_DISPLACEMENT,N)

#define AI_MATKEY_UVWSRC_LIGHTMAP(N)	\
	AI_MATKEY_UVWSRC(aiTextureType_LIGHTMAP,N)

#define AI_MATKEY_UVWSRC_REFLECTION(N)	\
	AI_MATKEY_UVWSRC(aiTextureType_REFLECTION,N)

   //! @endcond
   // ---------------------------------------------------------------------------
#define AI_MATKEY_TEXOP(type, N) _AI_MATKEY_TEXOP_BASE,type,N

   // For backward compatibility and simplicity
   //! @cond MATS_DOC_FULL
#define AI_MATKEY_TEXOP_DIFFUSE(N)	\
	AI_MATKEY_TEXOP(aiTextureType_DIFFUSE,N)

#define AI_MATKEY_TEXOP_SPECULAR(N)	\
	AI_MATKEY_TEXOP(aiTextureType_SPECULAR,N)

#define AI_MATKEY_TEXOP_AMBIENT(N)	\
	AI_MATKEY_TEXOP(aiTextureType_AMBIENT,N)

#define AI_MATKEY_TEXOP_EMISSIVE(N)	\
	AI_MATKEY_TEXOP(aiTextureType_EMISSIVE,N)

#define AI_MATKEY_TEXOP_NORMALS(N)	\
	AI_MATKEY_TEXOP(aiTextureType_NORMALS,N)

#define AI_MATKEY_TEXOP_HEIGHT(N)	\
	AI_MATKEY_TEXOP(aiTextureType_HEIGHT,N)

#define AI_MATKEY_TEXOP_SHININESS(N)	\
	AI_MATKEY_TEXOP(aiTextureType_SHININESS,N)

#define AI_MATKEY_TEXOP_OPACITY(N)	\
	AI_MATKEY_TEXOP(aiTextureType_OPACITY,N)

#define AI_MATKEY_TEXOP_DISPLACEMENT(N)	\
	AI_MATKEY_TEXOP(aiTextureType_DISPLACEMENT,N)

#define AI_MATKEY_TEXOP_LIGHTMAP(N)	\
	AI_MATKEY_TEXOP(aiTextureType_LIGHTMAP,N)

#define AI_MATKEY_TEXOP_REFLECTION(N)	\
	AI_MATKEY_TEXOP(aiTextureType_REFLECTION,N)

   //! @endcond
   // ---------------------------------------------------------------------------
#define AI_MATKEY_MAPPING(type, N) _AI_MATKEY_MAPPING_BASE,type,N

   // For backward compatibility and simplicity
   //! @cond MATS_DOC_FULL
#define AI_MATKEY_MAPPING_DIFFUSE(N)	\
	AI_MATKEY_MAPPING(aiTextureType_DIFFUSE,N)

#define AI_MATKEY_MAPPING_SPECULAR(N)	\
	AI_MATKEY_MAPPING(aiTextureType_SPECULAR,N)

#define AI_MATKEY_MAPPING_AMBIENT(N)	\
	AI_MATKEY_MAPPING(aiTextureType_AMBIENT,N)

#define AI_MATKEY_MAPPING_EMISSIVE(N)	\
	AI_MATKEY_MAPPING(aiTextureType_EMISSIVE,N)

#define AI_MATKEY_MAPPING_NORMALS(N)	\
	AI_MATKEY_MAPPING(aiTextureType_NORMALS,N)

#define AI_MATKEY_MAPPING_HEIGHT(N)	\
	AI_MATKEY_MAPPING(aiTextureType_HEIGHT,N)

#define AI_MATKEY_MAPPING_SHININESS(N)	\
	AI_MATKEY_MAPPING(aiTextureType_SHININESS,N)

#define AI_MATKEY_MAPPING_OPACITY(N)	\
	AI_MATKEY_MAPPING(aiTextureType_OPACITY,N)

#define AI_MATKEY_MAPPING_DISPLACEMENT(N)	\
	AI_MATKEY_MAPPING(aiTextureType_DISPLACEMENT,N)

#define AI_MATKEY_MAPPING_LIGHTMAP(N)	\
	AI_MATKEY_MAPPING(aiTextureType_LIGHTMAP,N)

#define AI_MATKEY_MAPPING_REFLECTION(N)	\
	AI_MATKEY_MAPPING(aiTextureType_REFLECTION,N)

   //! @endcond
   // ---------------------------------------------------------------------------
#define AI_MATKEY_TEXBLEND(type, N) _AI_MATKEY_TEXBLEND_BASE,type,N

   // For backward compatibility and simplicity
   //! @cond MATS_DOC_FULL
#define AI_MATKEY_TEXBLEND_DIFFUSE(N)	\
	AI_MATKEY_TEXBLEND(aiTextureType_DIFFUSE,N)

#define AI_MATKEY_TEXBLEND_SPECULAR(N)	\
	AI_MATKEY_TEXBLEND(aiTextureType_SPECULAR,N)

#define AI_MATKEY_TEXBLEND_AMBIENT(N)	\
	AI_MATKEY_TEXBLEND(aiTextureType_AMBIENT,N)

#define AI_MATKEY_TEXBLEND_EMISSIVE(N)	\
	AI_MATKEY_TEXBLEND(aiTextureType_EMISSIVE,N)

#define AI_MATKEY_TEXBLEND_NORMALS(N)	\
	AI_MATKEY_TEXBLEND(aiTextureType_NORMALS,N)

#define AI_MATKEY_TEXBLEND_HEIGHT(N)	\
	AI_MATKEY_TEXBLEND(aiTextureType_HEIGHT,N)

#define AI_MATKEY_TEXBLEND_SHININESS(N)	\
	AI_MATKEY_TEXBLEND(aiTextureType_SHININESS,N)

#define AI_MATKEY_TEXBLEND_OPACITY(N)	\
	AI_MATKEY_TEXBLEND(aiTextureType_OPACITY,N)

#define AI_MATKEY_TEXBLEND_DISPLACEMENT(N)	\
	AI_MATKEY_TEXBLEND(aiTextureType_DISPLACEMENT,N)

#define AI_MATKEY_TEXBLEND_LIGHTMAP(N)	\
	AI_MATKEY_TEXBLEND(aiTextureType_LIGHTMAP,N)

#define AI_MATKEY_TEXBLEND_REFLECTION(N)	\
	AI_MATKEY_TEXBLEND(aiTextureType_REFLECTION,N)

   //! @endcond
   // ---------------------------------------------------------------------------
#define AI_MATKEY_MAPPINGMODE_U(type, N) _AI_MATKEY_MAPPINGMODE_U_BASE,type,N

   // For backward compatibility and simplicity
   //! @cond MATS_DOC_FULL
#define AI_MATKEY_MAPPINGMODE_U_DIFFUSE(N)	\
	AI_MATKEY_MAPPINGMODE_U(aiTextureType_DIFFUSE,N)

#define AI_MATKEY_MAPPINGMODE_U_SPECULAR(N)	\
	AI_MATKEY_MAPPINGMODE_U(aiTextureType_SPECULAR,N)

#define AI_MATKEY_MAPPINGMODE_U_AMBIENT(N)	\
	AI_MATKEY_MAPPINGMODE_U(aiTextureType_AMBIENT,N)

#define AI_MATKEY_MAPPINGMODE_U_EMISSIVE(N)	\
	AI_MATKEY_MAPPINGMODE_U(aiTextureType_EMISSIVE,N)

#define AI_MATKEY_MAPPINGMODE_U_NORMALS(N)	\
	AI_MATKEY_MAPPINGMODE_U(aiTextureType_NORMALS,N)

#define AI_MATKEY_MAPPINGMODE_U_HEIGHT(N)	\
	AI_MATKEY_MAPPINGMODE_U(aiTextureType_HEIGHT,N)

#define AI_MATKEY_MAPPINGMODE_U_SHININESS(N)	\
	AI_MATKEY_MAPPINGMODE_U(aiTextureType_SHININESS,N)

#define AI_MATKEY_MAPPINGMODE_U_OPACITY(N)	\
	AI_MATKEY_MAPPINGMODE_U(aiTextureType_OPACITY,N)

#define AI_MATKEY_MAPPINGMODE_U_DISPLACEMENT(N)	\
	AI_MATKEY_MAPPINGMODE_U(aiTextureType_DISPLACEMENT,N)

#define AI_MATKEY_MAPPINGMODE_U_LIGHTMAP(N)	\
	AI_MATKEY_MAPPINGMODE_U(aiTextureType_LIGHTMAP,N)

#define AI_MATKEY_MAPPINGMODE_U_REFLECTION(N)	\
	AI_MATKEY_MAPPINGMODE_U(aiTextureType_REFLECTION,N)

   //! @endcond
   // ---------------------------------------------------------------------------
#define AI_MATKEY_MAPPINGMODE_V(type, N) _AI_MATKEY_MAPPINGMODE_V_BASE,type,N

   // For backward compatibility and simplicity
   //! @cond MATS_DOC_FULL
#define AI_MATKEY_MAPPINGMODE_V_DIFFUSE(N)	\
	AI_MATKEY_MAPPINGMODE_V(aiTextureType_DIFFUSE,N)

#define AI_MATKEY_MAPPINGMODE_V_SPECULAR(N)	\
	AI_MATKEY_MAPPINGMODE_V(aiTextureType_SPECULAR,N)

#define AI_MATKEY_MAPPINGMODE_V_AMBIENT(N)	\
	AI_MATKEY_MAPPINGMODE_V(aiTextureType_AMBIENT,N)

#define AI_MATKEY_MAPPINGMODE_V_EMISSIVE(N)	\
	AI_MATKEY_MAPPINGMODE_V(aiTextureType_EMISSIVE,N)

#define AI_MATKEY_MAPPINGMODE_V_NORMALS(N)	\
	AI_MATKEY_MAPPINGMODE_V(aiTextureType_NORMALS,N)

#define AI_MATKEY_MAPPINGMODE_V_HEIGHT(N)	\
	AI_MATKEY_MAPPINGMODE_V(aiTextureType_HEIGHT,N)

#define AI_MATKEY_MAPPINGMODE_V_SHININESS(N)	\
	AI_MATKEY_MAPPINGMODE_V(aiTextureType_SHININESS,N)

#define AI_MATKEY_MAPPINGMODE_V_OPACITY(N)	\
	AI_MATKEY_MAPPINGMODE_V(aiTextureType_OPACITY,N)

#define AI_MATKEY_MAPPINGMODE_V_DISPLACEMENT(N)	\
	AI_MATKEY_MAPPINGMODE_V(aiTextureType_DISPLACEMENT,N)

#define AI_MATKEY_MAPPINGMODE_V_LIGHTMAP(N)	\
	AI_MATKEY_MAPPINGMODE_V(aiTextureType_LIGHTMAP,N)

#define AI_MATKEY_MAPPINGMODE_V_REFLECTION(N)	\
	AI_MATKEY_MAPPINGMODE_V(aiTextureType_REFLECTION,N)

   //! @endcond
   // ---------------------------------------------------------------------------
#define AI_MATKEY_TEXMAP_AXIS(type, N) _AI_MATKEY_TEXMAP_AXIS_BASE,type,N

   // For backward compatibility and simplicity
   //! @cond MATS_DOC_FULL
#define AI_MATKEY_TEXMAP_AXIS_DIFFUSE(N)	\
	AI_MATKEY_TEXMAP_AXIS(aiTextureType_DIFFUSE,N)

#define AI_MATKEY_TEXMAP_AXIS_SPECULAR(N)	\
	AI_MATKEY_TEXMAP_AXIS(aiTextureType_SPECULAR,N)

#define AI_MATKEY_TEXMAP_AXIS_AMBIENT(N)	\
	AI_MATKEY_TEXMAP_AXIS(aiTextureType_AMBIENT,N)

#define AI_MATKEY_TEXMAP_AXIS_EMISSIVE(N)	\
	AI_MATKEY_TEXMAP_AXIS(aiTextureType_EMISSIVE,N)

#define AI_MATKEY_TEXMAP_AXIS_NORMALS(N)	\
	AI_MATKEY_TEXMAP_AXIS(aiTextureType_NORMALS,N)

#define AI_MATKEY_TEXMAP_AXIS_HEIGHT(N)	\
	AI_MATKEY_TEXMAP_AXIS(aiTextureType_HEIGHT,N)

#define AI_MATKEY_TEXMAP_AXIS_SHININESS(N)	\
	AI_MATKEY_TEXMAP_AXIS(aiTextureType_SHININESS,N)

#define AI_MATKEY_TEXMAP_AXIS_OPACITY(N)	\
	AI_MATKEY_TEXMAP_AXIS(aiTextureType_OPACITY,N)

#define AI_MATKEY_TEXMAP_AXIS_DISPLACEMENT(N)	\
	AI_MATKEY_TEXMAP_AXIS(aiTextureType_DISPLACEMENT,N)

#define AI_MATKEY_TEXMAP_AXIS_LIGHTMAP(N)	\
	AI_MATKEY_TEXMAP_AXIS(aiTextureType_LIGHTMAP,N)

#define AI_MATKEY_TEXMAP_AXIS_REFLECTION(N)	\
	AI_MATKEY_TEXMAP_AXIS(aiTextureType_REFLECTION,N)

   //! @endcond
   // ---------------------------------------------------------------------------
#define AI_MATKEY_UVTRANSFORM(type, N) _AI_MATKEY_UVTRANSFORM_BASE,type,N

   // For backward compatibility and simplicity
   //! @cond MATS_DOC_FULL
#define AI_MATKEY_UVTRANSFORM_DIFFUSE(N)	\
	AI_MATKEY_UVTRANSFORM(aiTextureType_DIFFUSE,N)

#define AI_MATKEY_UVTRANSFORM_SPECULAR(N)	\
	AI_MATKEY_UVTRANSFORM(aiTextureType_SPECULAR,N)

#define AI_MATKEY_UVTRANSFORM_AMBIENT(N)	\
	AI_MATKEY_UVTRANSFORM(aiTextureType_AMBIENT,N)

#define AI_MATKEY_UVTRANSFORM_EMISSIVE(N)	\
	AI_MATKEY_UVTRANSFORM(aiTextureType_EMISSIVE,N)

#define AI_MATKEY_UVTRANSFORM_NORMALS(N)	\
	AI_MATKEY_UVTRANSFORM(aiTextureType_NORMALS,N)

#define AI_MATKEY_UVTRANSFORM_HEIGHT(N)	\
	AI_MATKEY_UVTRANSFORM(aiTextureType_HEIGHT,N)

#define AI_MATKEY_UVTRANSFORM_SHININESS(N)	\
	AI_MATKEY_UVTRANSFORM(aiTextureType_SHININESS,N)

#define AI_MATKEY_UVTRANSFORM_OPACITY(N)	\
	AI_MATKEY_UVTRANSFORM(aiTextureType_OPACITY,N)

#define AI_MATKEY_UVTRANSFORM_DISPLACEMENT(N)	\
	AI_MATKEY_UVTRANSFORM(aiTextureType_DISPLACEMENT,N)

#define AI_MATKEY_UVTRANSFORM_LIGHTMAP(N)	\
	AI_MATKEY_UVTRANSFORM(aiTextureType_LIGHTMAP,N)

#define AI_MATKEY_UVTRANSFORM_REFLECTION(N)	\
	AI_MATKEY_UVTRANSFORM(aiTextureType_REFLECTION,N)

#define AI_MATKEY_UVTRANSFORM_UNKNOWN(N)	\
	AI_MATKEY_UVTRANSFORM(aiTextureType_UNKNOWN,N)

   //! @endcond
   // ---------------------------------------------------------------------------
#define AI_MATKEY_TEXFLAGS(type, N) _AI_MATKEY_TEXFLAGS_BASE,type,N

   // For backward compatibility and simplicity
   //! @cond MATS_DOC_FULL
#define AI_MATKEY_TEXFLAGS_DIFFUSE(N)	\
	AI_MATKEY_TEXFLAGS(aiTextureType_DIFFUSE,N)

#define AI_MATKEY_TEXFLAGS_SPECULAR(N)	\
	AI_MATKEY_TEXFLAGS(aiTextureType_SPECULAR,N)

#define AI_MATKEY_TEXFLAGS_AMBIENT(N)	\
	AI_MATKEY_TEXFLAGS(aiTextureType_AMBIENT,N)

#define AI_MATKEY_TEXFLAGS_EMISSIVE(N)	\
	AI_MATKEY_TEXFLAGS(aiTextureType_EMISSIVE,N)

#define AI_MATKEY_TEXFLAGS_NORMALS(N)	\
	AI_MATKEY_TEXFLAGS(aiTextureType_NORMALS,N)

#define AI_MATKEY_TEXFLAGS_HEIGHT(N)	\
	AI_MATKEY_TEXFLAGS(aiTextureType_HEIGHT,N)

#define AI_MATKEY_TEXFLAGS_SHININESS(N)	\
	AI_MATKEY_TEXFLAGS(aiTextureType_SHININESS,N)

#define AI_MATKEY_TEXFLAGS_OPACITY(N)	\
	AI_MATKEY_TEXFLAGS(aiTextureType_OPACITY,N)

#define AI_MATKEY_TEXFLAGS_DISPLACEMENT(N)	\
	AI_MATKEY_TEXFLAGS(aiTextureType_DISPLACEMENT,N)

#define AI_MATKEY_TEXFLAGS_LIGHTMAP(N)	\
	AI_MATKEY_TEXFLAGS(aiTextureType_LIGHTMAP,N)

#define AI_MATKEY_TEXFLAGS_REFLECTION(N)	\
	AI_MATKEY_TEXFLAGS(aiTextureType_REFLECTION,N)

#define AI_MATKEY_TEXFLAGS_UNKNOWN(N)	\
	AI_MATKEY_TEXFLAGS(aiTextureType_UNKNOWN,N)

   //! @endcond
   //!
   // ---------------------------------------------------------------------------
   /** @brief Retrieve a material property with a specific key from the material
   *
   * @param pMat Pointer to the input material. May not be NULL
   * @param pKey Key to search for. One of the AI_MATKEY_XXX constants.
   * @param type Specifies the type of the texture to be retrieved (
   *    e.g. diffuse, specular, height map ...)
   * @param index Index of the texture to be retrieved.
   * @param pPropOut Pointer to receive a pointer to a valid MaterialProperty
   *        structure or NULL if the key has not been found. */
   // ---------------------------------------------------------------------------
   ASSIMP_API int32 aiGetMaterialProperty(
      const aiMaterial* pMat,
      const char* pKey,
      uint32 type,
      uint32 index,
      const MaterialProperty** pPropOut);

   // ---------------------------------------------------------------------------
   /** @brief Retrieve an array of float values with a specific key
   *  from the material
   *
   * Pass one of the AI_MATKEY_XXX constants for the last three parameters (the
   * example reads the #AI_MATKEY_UVTRANSFORM property of the first diffuse texture)
   * @code
   * aiUVTransform trafo;
   * uint32 max = sizeof(aiUVTransform);
   * if (AI_SUCCESS != aiGetMaterialFloatArray(mat, AI_MATKEY_UVTRANSFORM(aiTextureType_DIFFUSE,0),
   *    (float*)&trafo, &max) || sizeof(aiUVTransform) != max)
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
   // ---------------------------------------------------------------------------
   ASSIMP_API int32 aiGetMaterialFloatArray(
      const aiMaterial* pMat,
      const char* pKey,
      uint32 type,
      uint32 index,
      float* pOut,
      uint32* pMax);

   /** @brief Retrieve a single float property with a specific key from the material.
   *
   * Pass one of the AI_MATKEY_XXX constants for the last three parameters (the
   * example reads the #AI_MATKEY_SHININESS_STRENGTH property of the first diffuse texture)
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
   // ---------------------------------------------------------------------------
   inline int32 aiGetMaterialFloat(const aiMaterial* pMat,
      const char* pKey,
      uint32 type,
      uint32 index,
      float* pOut)
   {
      return aiGetMaterialFloatArray(pMat, pKey, type, index, pOut, (uint32*)0x0);
   }

#else 

   // Use our friend, the C preprocessor
#define aiGetMaterialFloat (pMat, type, index, pKey, pOut) \
    aiGetMaterialFloatArray(pMat, type, index, pKey, pOut, NULL)

#endif //!__cplusplus


   // ---------------------------------------------------------------------------
   /** @brief Retrieve an array of integer values with a specific key
   *  from a material
   *
   * See the sample for aiGetMaterialFloatArray for more information.*/
   ASSIMP_API int32 aiGetMaterialIntegerArray(const aiMaterial* pMat,
      const char* pKey,
      uint32  type,
      uint32  index,
      int* pOut,
      uint32* pMax);

   // ---------------------------------------------------------------------------
   /** @brief Retrieve an integer property with a specific key from a material
   *
   * See the sample for aiGetMaterialFloat for more information.*/
   // ---------------------------------------------------------------------------
   inline int32 aiGetMaterialInteger(const aiMaterial* pMat,
      const char* pKey,
      uint32 type,
      uint32 index,
      int* pOut)
   {
      return aiGetMaterialIntegerArray(pMat, pKey, type, index, pOut, (uint32*)0x0);
   }

#else 

   // use our friend, the C preprocessor
#define aiGetMaterialInteger (pMat, type, index, pKey, pOut) \
    aiGetMaterialIntegerArray(pMat, type, index, pKey, pOut, NULL)

#endif //!__cplusplus



   // ---------------------------------------------------------------------------
   /** @brief Retrieve a color value from the material property table
   *
   * See the sample for aiGetMaterialFloat for more information*/
   // ---------------------------------------------------------------------------
   ASSIMP_API int32 aiGetMaterialColor(const C_STRUCT aiMaterial* pMat,
      const char* pKey,
      uint32 type,
      uint32 index,
      aiColor4D* pOut);


   // ---------------------------------------------------------------------------
   /** @brief Retrieve a aiUVTransform value from the material property table
   *
   * See the sample for aiGetMaterialFloat for more information*/
   // ---------------------------------------------------------------------------
   ASSIMP_API int32 aiGetMaterialUVTransform(const C_STRUCT aiMaterial* pMat,
      const char* pKey,
      uint32 type,
      uint32 index,
      UVTransform* pOut);
   // ---------------------------------------------------------------------------
   /** @brief Retrieve a string from the material property table
   *
   * See the sample for aiGetMaterialFloat for more information.*/
   // ---------------------------------------------------------------------------
   ASSIMP_API int32 aiGetMaterialString(const aiMaterial* pMat,
      const char* pKey,
      uint32 type,
      uint32 index,
      aiString* pOut);

   // ---------------------------------------------------------------------------
   /** Get the number of textures for a particular texture type.
   *  @param[in] pMat Pointer to the input material. May not be NULL
   *  @param type Texture type to check for
   *  @return Number of textures for this type.
   *  @note A texture can be easily queried using #aiGetMaterialTexture() */
   // ---------------------------------------------------------------------------
   ASSIMP_API uint32 aiGetMaterialTextureCount(const aiMaterial* pMat, aiTextureType type);

   // ---------------------------------------------------------------------------
   /** @brief Helper function to get all values pertaining to a particular
   *  texture slot from a material structure.
   *
   *  This function is provided just for convenience. You could also read the
   *  texture by parsing all of its properties manually. This function bundles
   *  all of them in a huge function monster.
   *
   *  @param[in] mat Pointer to the input material. May not be NULL
   *  @param[in] type Specifies the texture stack to read from (e.g. diffuse,
   *     specular, height map ...).
   *  @param[in] index Index of the texture. The function fails if the
   *     requested index is not available for this texture type.
   *     #aiGetMaterialTextureCount() can be used to determine the number of
   *     textures in a particular texture stack.
   *  @param[out] path Receives the output path
   *      This parameter must be non-null.
   *  @param mapping The texture mapping mode to be used.
   *      Pass NULL if you're not interested in this information.
   *  @param[out] uvindex For UV-mapped textures: receives the index of the UV
   *      source channel. Unmodified otherwise.
   *      Pass NULL if you're not interested in this information.
   *  @param[out] blend Receives the blend factor for the texture
   *      Pass NULL if you're not interested in this information.
   *  @param[out] op Receives the texture blend operation to be perform between
   *		this texture and the previous texture.
   *      Pass NULL if you're not interested in this information.
   *  @param[out] mapmode Receives the mapping modes to be used for the texture.
   *      Pass NULL if you're not interested in this information. Otherwise,
   *      pass a pointer to an array of two aiTextureMapMode's (one for each
   *      axis, UV order).
   *  @param[out] flags Receives the the texture flags.
   *  @return AI_SUCCESS on success, otherwise something else. Have fun.*/
   // ---------------------------------------------------------------------------
   ASSIMP_API int32 aiGetMaterialTexture(const C_STRUCT aiMaterial* mat,
      aiTextureType type,
      uint32  index,
      aiString* path,
      aiTextureMapping* mapping = NULL,
      uint32* uvindex = NULL,
      float* blend = NULL,
      aiTextureOp* op = NULL,
      aiTextureMapMode* mapmode = NULL,
      uint32* flags = NULL);
}

} // namespace material

#include "material.inl"

#endif //!__cplusplus
#endif //!!AI_MATERIAL_H_INC
